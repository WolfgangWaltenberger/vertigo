#include "Vertigo/VertexAssociation/interface/VertexAssociatorByTracks.h"
// SimVertexShareTracks
#include "Vertigo/TrackAssociation/interface/TrackAssociator.h"
#include "Environment/MCObjects/interface/SimTrack.h"
#include "Environment/MCObjects/interface/SimVertex.h"
#include <rave/Track.h>
#include <rave/Vertex.h>
#include "Environment/Configuration/interface/SimpleConfigurable.h"

#include <algorithm>
#include <cfloat>
#include <utility>

using namespace std;

VertexAssociatorByTracks::VertexAssociatorByTracks(
  const vector<vertigo::SimVertex > & sim, 
  const vector<rave::Vertex > & rec,
  const vertigo::AssociationManager & ass )
  : theAss(ass), theRecVertices(rec), theSimVertices(sim), 
    upToDate( false )
{
  setPurityCut();
}


VertexAssociatorByTracks::VertexAssociatorByTracks(const VertexAssociatorByTracks & other) 
  : thePurityCut(other.thePurityCut), theAss ( other.theAss ),
    theRecToSimMap(other.theRecToSimMap),
    theSimToRecMap(other.theSimToRecMap), 
    theRecVertices(other.theRecVertices), theSimVertices(other.theSimVertices),
    upToDate(other.upToDate)
{}


void VertexAssociatorByTracks::setPurityCut()
{
  SimpleConfigurable<double>
    purityCutConfigurable(0.50001, "VertexAssociatorByTracks:purityCut");
  // default 50 percent
  thePurityCut = purityCutConfigurable.value();
}


template <class T>
class VertexAssociatorByTracks::MoreTracksInCommon {

public:

  MoreTracksInCommon(const map<T, int > & aMap)
    : theMap(aMap) {}

  bool operator() ( T a, T b) {
    typename map< T, int >::const_iterator ia = theMap.find(a);
    typename map< T, int >::const_iterator ib = theMap.find(b);
    if (ia == theMap.end()) {
      return false;
    }
    else if (ib == theMap.end()) {
      return true;
    }
    else {
      return (*ia).second > (*ib).second;
    }
  }

private:

  const map< T, int > & theMap;
};


vector<vertigo::SimVertex >
VertexAssociatorByTracks::simVertices( const rave::Vertex & rv ) const
{
  const_cast<VertexAssociatorByTracks*>(this)->check();

  vector<vertigo::SimVertex > simV;
  RecToSimMap::const_iterator i = theRecToSimMap.find(rv);
  if (i != theRecToSimMap.end()) {
    for (SimMap::const_iterator j = (*i).second.begin();
	 j != (*i).second.end(); j++) {
      if (((*j).second / (rv.tracks().size()*1.0))
	  >= thePurityCut) { simV.push_back((*j).first); }
    }
  } else {
    cout << "[VertexAssociatorByTracks] empty map?" << endl;
  }
  sort(simV.begin(), simV.end(), MoreTracksInCommon<vertigo::SimVertex>((*i).second));
  return simV;
}


vector<rave::Vertex>
VertexAssociatorByTracks::recVertices( const vertigo::SimVertex & sv ) const
{
  const_cast<VertexAssociatorByTracks*>(this)->check();

  vector<rave::Vertex > recV;

  SimToRecMap::const_iterator i = theSimToRecMap.find(sv);

  if (i != theSimToRecMap.end())
  {
    for (RecMap::const_iterator j = (*i).second.begin(); j != (*i).second.end(); j++)
    {
      // always use the nb of rectracks in the recvertex as purity
      if ((j->second / (j->first.tracks().size()*1.0)) >= thePurityCut)
      { 
        recV.push_back(j->first); 
      }
    }
  }

  sort(recV.begin(), recV.end(), MoreTracksInCommon<rave::Vertex>((*i).second));
  return recV;
}


vector<rave::Vertex > VertexAssociatorByTracks::recVertices() const
{
  return theRecVertices;
}


float VertexAssociatorByTracks::purityCut() const
{
  return thePurityCut;
}


void VertexAssociatorByTracks::check()
{
  if (!upToDate) update();
}


void VertexAssociatorByTracks::update()
{
  // fill maps
  for (vector<rave::Vertex>::const_iterator irv = theRecVertices.begin();
       irv != theRecVertices.end(); irv++)
  {
    theRecToSimMap[*irv] = simMap(*irv);
  }

  for ( vector<vertigo::SimVertex>::const_iterator isv 
        = theSimVertices.begin(); isv != theSimVertices.end(); ++isv )
  {
    RecMap map;
    for ( RecToSimMap::const_iterator i = theRecToSimMap.begin();
          i != theRecToSimMap.end(); i++)
    {
      SimMap::const_iterator j = (*i).second.find(*isv);
      if (j != (*i).second.end())
      {
        pair< rave::Vertex, int> value((*i).first, (*j).second);
        map.insert(value);
      }
    }
    theSimToRecMap[*isv] = map;
  }

  // up to date now
  upToDate = true;

  // printMaps();
}

void VertexAssociatorByTracks::printMaps() const
{
  cout << endl;
  cout << "[VertexAssociatorByTracks] printing maps:" << endl
       << "=========================================" << endl;
  for ( RecToSimMap::const_iterator i=theRecToSimMap.begin(); 
        i!=theRecToSimMap.end() ; ++i )
  {
    cout << " RecVtx " << i->first.id() << ": " << i->second.size() << " associations" << endl;
  }
  cout << endl;

  for ( SimToRecMap::const_iterator i=theSimToRecMap.begin(); 
        i!=theSimToRecMap.end() ; ++i )
  {
    cout << " SimVtx " << i->first.id() << ": " << i->second.size() << " associations" << endl;
  }
  cout << endl;
}


VertexAssociatorByTracks::SimMap
VertexAssociatorByTracks::simMap( const rave::Vertex & vtx )
{
  SimMap map;
  vector< rave::Track > rTracks = vtx.tracks();

  for (vector< rave::Track >::const_iterator ir = rTracks.begin();
       ir != rTracks.end(); ir++)
  {
    vector< vertigo::SimTrack > sTracks = theAss.tracks( *ir );
    if (sTracks.empty()) continue;

    vertigo::SimTrack s = sTracks.front();
    const vertigo::SimVertex * sVtx = s.vertex();
    if ( sVtx )
    {
      map[*sVtx]++;
      // } else {
      // cout << "[VertexAssociatorByTracks] simtrack has no simvertex." << endl;
    }
  }
  return map;
}

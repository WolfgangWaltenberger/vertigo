#include "Vertigo/VertexAssociation/interface/BVertexAssociator.h"
#include "Vertigo/VertexAssociation/interface/VertexDistance.h"
// SimVertexShareTracks
#include "Vertigo/TrackAssociation/interface/TrackAssociator.h"
#include "Environment/MCObjects/interface/SimTrack.h"
#include "Environment/MCObjects/interface/SimVertex.h"
#include <rave/impl/RaveBase/Converters/interface/RaveStreamers.h>
// #include <rave/impl/RecoBTag/SecondaryVertex/interface/DebugTaggingVertex.h>
#include <rave/Track.h>
#include <rave/Vertex.h>
#include "Environment/Configuration/interface/SimpleConfigurable.h"

#include <algorithm>
#include <cfloat>
#include <utility>

using namespace std;
using namespace rave;
using namespace vertigo;

namespace {
  vector < Vertex > sortVertices ( const vector < Vertex  > & orig )
  {
    vector < Vertex > ret;
    for ( vector< Vertex >::const_iterator i=orig.begin(); i!=orig.end() ; ++i )
    {
      ret.push_back ( *i );
    }
    return ret;
  }
}

BVertexAssociator::BVertexAssociator(
  const vector<SimVertex > & sim, 
  const vector<Vertex > & rec )
{
  associate(sim,rec);
}

void BVertexAssociator::associate ( const vector<SimVertex > & sim,
    const vector<Vertex > & rec )
{
  //
  // get the Sim Vertices!
  SimVertex primaryvertex;
  vector < SimVertex > bvertices;
  for ( vector< SimVertex >::const_iterator i=sim.begin(); i!=sim.end() ; ++i )
  {
    if ( i->isPrimary() && (!primaryvertex.isValid() ) )
    {
      // cout << "[BVertexAssociator] ha! primary vertex: " << i->id() << endl;
      primaryvertex = (*i);
    }

    if ( abs((i->pid()%1000)/100) == 5 )
    {
      // cout << "[BVertexAssociator] ha! bvertex: " << i->id() << endl;
      bvertices.push_back(*i);
    }
  }


  // get the primary rec vertex
  if ( rec.size() > 0 && primaryvertex.isValid() )
  {
    associate( primaryvertex, rec[0] );
  }

  if ( rec.size() < 2 || bvertices.size() == 0 )
    // nothing to do for us anymore, we cannot associate a B meson!
    return;

  // and now for the b vertices! we need to reorder!
  // this should be just like in the simple vertex based tagger!
  // that is, we sort by 
  
  // vector < Vertex > sorted = sortVertices ( rec );

  const Vertex & first_secondary_vertex=rec[1]; // FIXME not!!
  // cout << "[BVertexAssociator] tagging vertex at " << DebugTaggingVertex::taggingVertex.x() << endl;
  // cout << "[BVertexAssociator] my rave vertex at " << first_secondary_vertex.position().x() << endl;

  VertexDistance distor(true);
  // we associate with the B that is closest to the rec vertex
  SimVertex & bvertex = *(bvertices.begin());
  float distance_tmp = FLT_MAX;
  for ( vector< SimVertex >::const_iterator i=bvertices.begin(); i!=bvertices.end() ; ++i )
  {
    float newdist=distor.distance ( *i, first_secondary_vertex );
    if ( newdist < distance_tmp )
    {
      distance_tmp=newdist;
      bvertex=(*i);
    }

  }

  // cout << "[BVertexAssociator] simid=" << bvertex.id() << "distance_tmp=" << distance_tmp << endl;
  if ( distance_tmp < 5. )
  {
    // if distance < 10 sigmas, associate!

    // cout << "[BVertexAssociator] associate " << bvertex.id() << " with " << rec[1].id() << endl;
    associate(bvertex, first_secondary_vertex );
  }
}

void BVertexAssociator::associate ( const SimVertex & sim, const Vertex & rec )
{
  vector < SimVertex > sims;
  sims.push_back ( sim );
  theRecAss[rec]=sims;

  vector < Vertex > recs;
  recs.push_back ( rec );
  theSimAss[sim]=recs;
}

vector<SimVertex >
BVertexAssociator::simVertices( const Vertex & rv ) const
{
  return theRecAss[rv];
}

vector<Vertex>
BVertexAssociator::recVertices( const SimVertex & sv ) const
{
  return theSimAss[sv];
}

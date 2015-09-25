#include "LHEEventGenerator.h"
#include "Environment/MCObjects/interface/PDGCodes.h"

using namespace std;

bool LHEEventGenerator::hasBeamSpot() const
{
  return false;
}

void LHEEventGenerator::reset()
{
  if ( theReader ) delete theReader;
  theReader = new LHEF::Reader ( theFilename );
}

rave::Ellipsoid3D LHEEventGenerator::beamSpot() const
{
  return rave::Ellipsoid3D();
}

LHEEventGenerator::LHEEventGenerator( const string & filename ) :
  theFilename ( filename ), theReader ( 0 ), theCtr(0)
{
  if ( filename.size() )
    theReader = new LHEF::Reader ( filename );
}

std::string LHEEventGenerator::describe() const
{
  return "LHEEventGenerator";
}

LHEEventGenerator::~LHEEventGenerator()
{
  // delete theReader;
}

vertigo::Event LHEEventGenerator::next() const
{
  theCtr++;
  // return vertigo::Event();
  bool re = theReader->readEvent();
  if (!re) throw std::string ( "EOF reached");
  vertigo::Event e ( theCtr, theCtr, theReader->eventComments, theFilename );

  map < int, vector < int > > decays;
  map < int, vertigo::SimVertex > vertices;
  map < int, vertigo::SimTrack > tracks;
  map < int, vertigo::SimParticle > particles;

  int n = theReader->hepeup.NUP;
  for ( int i=0; i< n ; i++ )
  {
    int pdgid = theReader->hepeup.IDUP[i];
    int status = theReader->hepeup.ISTUP[i];
    int mother = theReader->hepeup.MOTHUP[i].first-1; // index of mother
    int daughter = theReader->hepeup.MOTHUP[i].second-1; // index of daughter
    float px = theReader->hepeup.PUP[i][0];
    float py = theReader->hepeup.PUP[i][1];
    float pz = theReader->hepeup.PUP[i][2];
    // float E = theReader->hepeup.PUP[i][3];
    float M = theReader->hepeup.PUP[i][4];
    // http://home.thep.lu.se/~leif/LHEF/classLHEF_1_1HEPEUP.html
    if (theDebug)
    {
      cout << "[LHEEGen]" << setw(9) << PDGCodes::name ( pdgid ) 
           << " i=" << setw(2) << i
           << " s=" << setw(2) << status 
           << " m=" << setw(2) <<  mother 
           << " d=" << setw(2) << daughter
           << " M=" << setw(4) << M << endl;
    }
    rave::Vector6D v6 ( 0., 0., 0., px, py, pz );
    rave::Vector7D v7 ( 0., 0., 0., px, py, pz, M );
    int q = 0;
    if ( pdgid > 0 ) q = 1;
    if ( pdgid < 0 ) q = -1;
    if ( status == 1 || status==2 )
    {
      vertigo::SimVertex * prod = 0;
      if ( mother >= 0 )
      {
        if ( decays.count ( mother ) == 0 )
        {
          decays[mother]=vector<int>();
        }
        decays[mother].push_back ( i );
        if ( vertices.count ( mother ) )
        {
          prod = &vertices[mother];
        }
      }

      vertigo::SimTrack s ( v6, q, prod, pdgid, "", 0 );
      tracks[i]=s;
      vertigo::SimParticle s2 ( v7, q, prod, pdgid, "", 0 );
      particles[i]=s2;
    }

    /*
    if ( status == -1 ) 
    {
      cout << "[LHEEventGenerator] production particle ``"
           << PDGCodes::name ( pdgid ) << "'' mother=" 
           << mother << " daughter=" << daughter << endl;
    }*/

    if ( status == 2 )
    {
      if ( daughter >= 0 )
      {
        if ( decays.count ( i ) == 0 )
        {
          decays[i]=vector<int>();
        }
        decays[i].push_back ( daughter );
      }
      vector < vertigo::SimTrack > trks;
      vector < int > trkids = decays[i];
      for ( vector< int >::const_iterator id=trkids.begin(); 
            id!=trkids.end() ; ++id )
      {
        if ( tracks.count ( *id ) )
        {
          trks.push_back ( tracks[*id] );
        }
      }
      bool isprimary=false;
      if ( mother==0 ) isprimary=true;
      vertigo::SimVertex s ( rave::Point3D (), trks,
                             pdgid, "", isprimary );
      vertices[i]=s;
      vertigo::SimVertex * prod = 0;
      vertigo::SimParticle s2 ( v7, q, prod, pdgid, "", 0 );
      particles[i]=s2;
    }
  }
  // FIXME now we should also add all things in decay, a posteriori
  for ( map< int, vector < int > >::const_iterator i=decays.begin(); 
        i!=decays.end() ; ++i )
  {
    if ( i->second.size() == 0 ) continue;
    if (theDebug)
      cout << "[LHEEGen] now connect " << i->first << " with ";
    vector < int > s = i->second;
    for ( vector< int >::const_iterator j=s.begin(); j!=s.end() ; ++j )
    {
      // if ( *j == 1 ) continue; // drop production vertex
      if (theDebug) cout << (*j) << " ";
      vertices[i->first].add ( tracks[*j] ); // add track to vertex
      tracks[*j].setVertex ( &(vertices[i->first] ) ); // define vertex of track
    }

    if (theDebug) cout << endl;
  }

  // now we want to sort by the particle ids, not the position in the lhe
  // file, for consistency

  map < int, vector < vertigo::SimVertex > > sortedVertices;
  map < int, vector < vertigo::SimTrack > > sortedTracks;

  for ( map < int, vertigo::SimVertex >::const_iterator v=vertices.begin(); 
        v!=vertices.end() ; ++v )
  {
    // e.add ( v->second );
    if ( !sortedVertices.count ( v->second.pid() ) )
    {
      sortedVertices[v->second.pid()]=vector < vertigo::SimVertex >();
    }
    sortedVertices[v->second.pid()].push_back ( v->second );
  }

  for ( map < int, vector < vertigo::SimVertex > >::const_iterator i=
        sortedVertices.begin(); i!=sortedVertices.end() ; ++i )
  {
    e.add ( i->second );
  }

  for ( map < int, vertigo::SimTrack >::const_iterator v=tracks.begin(); 
        v!=tracks.end() ; ++v )
  {
    // e.add ( v->second );
    if ( !sortedTracks.count ( v->second.pid() ) )
    {
      sortedTracks[v->second.pid()]=vector < vertigo::SimTrack >();
    }
    sortedTracks[v->second.pid()].push_back ( v->second );
  }

  for ( map < int, vector < vertigo::SimTrack > >::const_iterator 
        i=sortedTracks.begin(); i!=sortedTracks.end() ; ++i )
  {
    e.add ( i->second );
  }
  
  for ( map < int, vertigo::SimParticle >::const_iterator i=
        particles.begin(); i!=particles.end() ; ++i )
  {
    e.add ( i->second );
  }

  return e;
}

LHEEventGenerator::LHEEventGenerator() :
  theFilename(""), theReader ( 0 ), theDebug(false), theCtr(0)
{}

void LHEEventGenerator::source ( const std::string & filename )
{
  theFilename=filename;
  if ( theReader ) delete theReader;
  theReader = new LHEF::Reader ( filename );
}

int LHEEventGenerator::skip ( int n ) const
{
  theCtr+=n;
  for ( int i=0; i< n ; i++ )
    theReader->readEvent();

  // theReader->skip (n );
  return n;
}

#include "Vertigo/EventGeneration/interface/GeneratorBuilder.h"
namespace {
    GeneratorBuilder<LHEEventGenerator> t( "lhe", "LHE file reader" );
}


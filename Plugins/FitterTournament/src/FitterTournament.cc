#include "FitterTournament.h"
#include <rave/impl/RaveBase/Converters/interface/RaveToCmsObjects.h>
#include <rave/impl/RaveBase/Converters/interface/CmsToRaveObjects.h>
#include <rave/impl/RaveBase/RaveEngine/interface/RaveVertexReconstructor.h>
#include "Vertigo/EventLoop/interface/LoopSettings.h"
#include "Vertigo/EventLoop/interface/EventLoop.h"

using namespace std;

namespace {
  string shortName ( string src )
  {
    string::size_type pos=src.find(",");
    if ( pos != string::npos )
    {
      src.replace ( pos, string::npos , "" );
    }
    pos=src.find("file:");
    if ( pos != string::npos )
    {
      src.replace ( 0, 5, "" );
    }
    pos=src.find("gun:");
    if ( pos != string::npos )
    {
      src.replace ( 0, 4, "" );
    }
    pos=src.rfind(".");
    if ( pos != string::npos )
    {
      src.replace ( pos, string::npos, "" );
    }
    pos=src.rfind("/");
    if ( pos != string::npos )
    {
      src.replace ( 0, pos+1, "" );
    }

    if ( src!="" ) src=src+".";
    return src;
  }
}

FitterTournament::FitterTournament() : hasWrittenSource(false)
{
  // setup ("" );
}

void FitterTournament::setup ( const string & prefix )
{
  destroy();
  vector < string > algos;
  algos.push_back ( "kalman" );
  algos.push_back ( "avf" );
  algos.push_back ( "tkvf" );
  algos.push_back ( "mvf" );

  for ( vector< string >::const_iterator i=algos.begin(); i!=algos.end() ; ++i )
  {
    try {
      theRectors[*i]=new rave::VertexReconstructor( *i );
      theHistos[*i]=new VertexHisto ( prefix + (*i) + ".hdf" );
    } catch ( ... ) {
      cout << "[FitterTournament] no tkvf available!";
    }
  }
}

void FitterTournament::destroy()
{
  for ( map < string, VertexReconstructor * >::const_iterator i=theRectors.begin(); 
        i!=theRectors.end() ; ++i )
  {
    delete i->second;
  }
  theRectors.clear();

  for ( map < string, VertexHisto * >::const_iterator i=theHistos.begin(); 
        i!=theHistos.end() ; ++i )
  {
    delete i->second;
  }
  theHistos.clear();
}

FitterTournament::~FitterTournament()
{
  destroy();
}

void FitterTournament::configure ( const string & f )
{
  for ( map < string, VertexHisto * >::const_iterator i=theHistos.begin(); 
        i!=theHistos.end() ; ++i )
  {
    string name=i->second->fileName();
    i->second->configure ( f + name );
  }
}

void FitterTournament::writeSource ( const string & s )
{
  if ( hasWrittenSource ) return;
  setup ( s );
  for ( map < string, VertexHisto * >::const_iterator i=theHistos.begin(); 
        i!=theHistos.end() ; ++i )
  {
    i->second->writeSource(s);
  }
  hasWrittenSource=true;
}

void FitterTournament::setup()
{
  string src = shortName ( LoopSettings::Instance().source() );
  // cout << "[FitterTournament] setting name to " << src << endl;
  writeSource ( src );
}


void FitterTournament::process ( const vertigo::Event & e )
{
  setup();
  int evctr=0;
  vector < vertigo::SimVertex > svtces = e.simVertices();
  if ( svtces.size() == 0 )
  {
    return;
  }

  const vector < rave::Track > & rtrks = e.tracks();
  vector < reco::TransientTrack > trks;
  RaveToCmsObjects rconverter;
  for ( vector< rave::Track >::const_iterator i=rtrks.begin(); i!=rtrks.end() ; ++i )
  {
    trks.push_back ( rconverter.tTrack ( *i ) );
  }

  for ( map < string, VertexReconstructor * >::const_iterator i=theRectors.begin(); 
        i!=theRectors.end() ; ++i )
  {
    try {
      vector < TransientVertex > tvtces = i->second->vertices ( trks );
      if ( tvtces.size() == 0 )
      {
        continue;
      }
      vector < rave::Vertex > vtces;
      for ( vector< TransientVertex >::const_iterator j=tvtces.begin(); 
            j!=tvtces.end() ; ++j )
      {
        vtces.push_back ( CmsToRaveObjects().convert ( *j, rtrks ) );
      }
      theHistos[i->first]->fill ( svtces[0], vtces[0], 0., evctr );
    } catch ( exception & e ) {
      theHistos[i->first]->except ( e.what(), "std" );
    } catch ( ... ) {
      theHistos[i->first]->except ( "???", "..." );
    }
  }
}

void FitterTournament::process ( const exception & e, const string & type )
{
  for ( map < string, VertexHisto * >::const_iterator i=theHistos.begin(); 
        i!=theHistos.end() ; ++i )
  {
    i->second->except ( e.what(), type );
  }
}

FitterTournament * FitterTournament::clone() const
{
  return new FitterTournament ( *this );
}

#include "Analysis/Observer/interface/ObserverBuilder.h"
namespace {
  ObserverBuilder<FitterTournament> t( "FitterTournament", "Tournament observer that compares different fitters", false );
}


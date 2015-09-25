#include "EventHistogrammer.h"
#include "Vertigo/Event/interface/Event.h"
#include "Vertigo/EventLoop/interface/LoopSettings.h"
#include "Environment/Configuration/interface/SimpleConfigurable.h"
#include <dataharvester/Writer.h>

// #include "Vertex/DataHarvesting/interface/SystemHarvester.h"
// #include "Vertex/RobustVertexFitTest/interface/CmsInfoHarvester.h"

using namespace std;
using namespace dataharvester;

namespace {
  inline float square ( float a )
  {
    return a*a;
  }

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

  template <class T > float ptTot ( const vector < T > & tracks )
  {
    float ret=0.;
    for ( typename vector< T >::const_iterator i=tracks.begin(); i!=tracks.end() ; ++i )
    {
      float x=i->state().px();
      float y=i->state().py();
      ret+=sqrt(x*x+y*y);
    }
    return ret;
  }
}


EventHistogrammer::EventHistogrammer() :
    theFileName ("events.hdf"), theFixFileName ( true )
{
}

EventHistogrammer::~EventHistogrammer()
{
}

void EventHistogrammer::configure ( const string & s )
{
  theFileName=s;
  theFixFileName=false;
}

EventHistogrammer * EventHistogrammer::clone() const
{
  return new EventHistogrammer ( *this );
}

void EventHistogrammer::process ( const vertigo::Event & e )
{
  analysis ( e.simTracks(), e.simVertices(), e.tracks(), e.id(),
      e.time() );
}
void EventHistogrammer::process ( const exception & e, const string & type )
{
  Tuple data ( "Exceptions" );
  data["what"]=e.what();
  data["type"]=type;
  dataharvester::Writer::file ( theFileName ) << data;
}

void EventHistogrammer::analysis (
                         const vector < vertigo::SimTrack > & simtracks,
                         const vector < vertigo::SimVertex > & simvtces,
                         const vector < rave::Track > & rectracks, int evnr, float time )
{
  if ( theFixFileName )
  {
    theFileName=shortName ( LoopSettings::Instance().source() ) + "event.hdf";
    theFixFileName=false;
  }

  Tuple event ( "event" );
  event["time"]=time;
  event["n_simvtces"]=simvtces.size();
  event["n_simtracks"]=simtracks.size();
  event["n_rectracks"]=rectracks.size();
  event["rec_pt_tot"]= ptTot<rave::Track > ( rectracks );
  event["sim_pt_tot"]= ptTot<vertigo::SimTrack > ( simtracks );
  // event.describe ( "sum of pts of all reconstructed tracks" , "rec_pt_tot" );
  // event.describe ( "sum of pts of all simulated tracks", "sim_pt_tot" );

  event["event"]=evnr;
  dataharvester::Writer::file ( theFileName ) << event;

  bool isprimary=false;
  float primx=0.;
  float primy=0.;
  float primz=0.;

  for ( vector< vertigo::SimVertex >::const_iterator i=simvtces.begin(); i!=simvtces.end() ; ++i )
  {
    Tuple mp("vtx");
    if ( i==simvtces.begin() )
    {
      isprimary=true;
      primx=(*i).position().x();
      primy=(*i).position().y();
      primz=(*i).position().z();
    }
    mp["id"]=(*i).id();
    // mp["pointer"]=&(*i);
    float x=(*i).position().x();
    float y=(*i).position().y();
    float z=(*i).position().z();
    mp["posx"]=x;
    mp["posy"]=y;
    mp["posz"]=z;
    mp["isprimary"]=isprimary;
    mp["dprim"]=sqrt ( square(x-primx)+square(y-primy)+square(z-primz) );
    // mp.describe ( "distance to primary", "dprim" );
    // mp.describe ( "is this a primary vertex", "isprimary" );
    mp["event"]=evnr;
    dataharvester::Writer::file ( theFileName ) << mp;
  };
}

#include "Analysis/Observer/interface/ObserverBuilder.h"
namespace {
  ObserverBuilder<EventHistogrammer> t( "EventHistogrammer", 
      "Writes down some stats about the events", false );
}


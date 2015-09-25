#include "Vertigo/EventLoop/interface/EventLoop.h"
#include "Vertigo/EventLoop/interface/LoopThread.h"
#include "Vertigo/EventLoop/interface/LoopSettings.h"
#include "Vertigo/EventLoop/interface/VertexMetaFactory.h"
#include "Environment/Utilities/interface/Console.h"
#include "VertigoHarvest/interface/VertigoHarvest.h"
#include "Vertigo/main/Version.h"

#include <rave/Ellipsoid3D.h>
#include <rave/Version.h>
#include <rave/impl/RaveBase/Converters/interface/RaveStreamers.h>
#include <rave/impl/FWCore/MessageLogger/interface/MessageLogger.h>

#ifdef WITH_THREADS
#include <boost/thread/thread.hpp>
#endif
#include <sstream>
#include <vector>
// #include <limits>
#include <climits>

using namespace std;

namespace {
  int timer( int n )
  {
    if (n<1) return 0;
    static int cum = 0;
    static time_t last = time ( 0 );
    time_t cur = time ( 0 );
    cum += cur;
    cum -= last;
    // int spEv = int ( floor ( (float) cum / (float) (n-1) * 10000 ) / 10. );
    int spEv = cum * 1000 / n;
    last = cur;
    return spEv;
  }

  string pretty_print_ms ( int ms )
  {
    // pretty print milliseconds
    string unit="ms";
    int n=ms;
    if ( n > 5000 ) // > 5 seconds?
    {
      n=ms/1000;
      unit="s";
      // seconds
    }

    if ( n > 5*60*1000 ) // > 5 minutes?
    {
      n=ms/60000;
      unit="m";
    }

    if ( n > 5*60*60*1000 ) // > 5 hours?
    {
      n=ms/(60*60*1000);
      unit="h";
    }

    ostringstream o;
    o << n << " " << unit;
    return o.str();
  }
}

void EventLoop::configureObserver()
{
  return;
  /*
  map < string, Observer * > obs=ObserverManager::Instance().get();
  for ( map < string, Observer * >::iterator i=obs.begin(); i!=obs.end() ; ++i )
  {
    string name = i->first + "." + LoopSettings::source() + "." + LoopSettings::Instance().method() + ".txt";
    i->second->setFileName ( name );
  }*/
}

string EventLoop::method() const
{
  return LoopSettings::Instance().method();
}

string EventLoop::source() const
{
  return LoopSettings::Instance().source();
}

int EventLoop::verbosity() const
{
  return LoopSettings::Instance().verbosity();
}

EventLoop::EventLoop ( const string & source, const string & vertexmethod,
                       const string & flavortagmethod, const string & skin, 
                       int min, signed max, int verbosity,
   bool usebeamspot, int nthreads, int expect )
{
  LoopSettings::Instance().set ( source, vertexmethod, flavortagmethod, min, max, verbosity, usebeamspot, nthreads, expect );
  const rave::VertexFactory * tmp = VertexMetaFactory::Instance().get ( skin, vertexmethod, verbosity );
  if (!tmp)
  {
    edm::LogWarning ("EventLoop") << "Couldnt get a VertexFactory? skin=" << skin
                                  << ", vertexmethod=" << vertexmethod;
  }
  LoopSettings::Instance().setVertexFactory ( tmp );
  LoopSettings::Instance().setEventFactory ( EventFactory(source) );

  // LogDebug("") << "EventFactory: " << source;
  // LogDebug("") << "has beamspot? " << LoopSettings::Instance().eventFactory().hasBeamSpot();
  if ( LoopSettings::Instance().eventFactory().hasBeamSpot() )
  {
    // propagate beamspot information from event factory to Vertex Factory!
    LogDebug("") << "Propagating beamspot information from EventFactory to VertexFactory!";
    if ( tmp!=0 ) LoopSettings::Instance().nonConstVertexFactory()->setBeamSpot( LoopSettings::Instance().eventFactory().beamSpot() );
  }
  // cout << "[EventLoop] method=" << method << " theVtxFactory=" << theVtxFactory << endl;
  if ( tmp != 0 ) LoopSettings::Instance().setFlavorTagFactory ( new rave::FlavorTagFactory ( *tmp, flavortagmethod ) );
  configureObserver ();
  if ( (verbosity > 9) && tmp )
  {
    rave::Point3D zero (0.,0.,0.);
    rave::Point3D one (0.,0.,200.);
    edm::LogInfo("EventLoop") 
         << "Magnetic Field: "
         << LoopSettings::Instance().vertexFactory()->getMagneticField().inTesla(zero)
         << " [Tesla] at " << zero << " [cm]" << endl
         << "                            " 
         << LoopSettings::Instance().vertexFactory()->getMagneticField().inTesla(one)
         << " [Tesla] at " << one  << " [cm]" << endl;
    /*
    rave::Point3D two (10.,0.,0.);
    cout << "                            " 
         << LoopSettings::Instance().vertexFactory()->getMagneticField().inTesla(two)
         << " [Tesla] at " << two  << " [cm]" << endl;
         */
  }

}

void EventLoop::loop()
{
  int n=1;
  if ( LoopSettings::Instance().min() > 0)
  {
    n=LoopSettings::Instance().min();
    LoopSettings::Instance().eventFactory().skip( LoopSettings::Instance().min() );
  }

  edm::LogInfo ("EventLoop")  << "start looping.";

  if ( LoopSettings::Instance().fancyCounter() &&
       LoopSettings::Instance().verbosity() == 1 )
  {
    cout << "\033[0;32m[";
    for ( int i=0; i< LoopSettings::Instance().barLength() ; i++ )
      cout << ".";
    cout << "]\033[;0m\033[2G" << flush;
  }

  bool method_is_none=false;
  if ( LoopSettings::Instance().method() == "none" ) method_is_none = true;

  // total number of events is max - min
  int max = LoopSettings::Instance().max();
  int ntot=max-LoopSettings::Instance().min()+1;
  if ( max < 0 ) ntot=INT_MAX; // FIXME numeric_limits<int>::infinity();

  // cout << "[EventLoop] max=" << max << " ntot=" << ntot << endl;
  bool with_ghost = true;

  #ifdef WITH_THREADS
  if ( LoopSettings::Instance().nThreads() == 1 ) {
    LoopThread l ( 0, ntot, !method_is_none );
    l();
  } else {
    int nthread = -1; // number of events per thread
    if ( ntot > 0 )
    {
      nthread = floor ( ntot / LoopSettings::Instance().nThreads() );
    }

    vector < boost::thread * > ts;
    for ( int i=0; i< LoopSettings::Instance().nThreads() ; i++ )
    {
      int n = nthread;
      if ( i == LoopSettings::Instance().nThreads() -1 ) // last thread gets the extras.
      {
        n=nthread+ntot-nthread*LoopSettings::Instance().nThreads();
      }
      boost::thread * t = new boost::thread ( LoopThread ( i*nthread, n, !method_is_none, with_ghost ) );
      ts.push_back ( t );
    }
  
    for ( vector< boost::thread * >::const_iterator i=ts.begin(); i!=ts.end() ; ++i )
    {
      (**i).join();
    }

    for ( vector< boost::thread * >::const_iterator i=ts.begin(); i!=ts.end() ; ++i )
    {
      delete (*i);
    }
    ts.clear();
  } 
  #else
  LoopThread l ( 0, ntot, !method_is_none );
  l();
  #endif

  if ( LoopSettings::Instance().verbosity() == 1 ) cout << endl;

  if ( LoopSettings::Instance().verbosity() > 2 )
  {
    vertigo::Console c;
    cout << c.reset() << endl 
         << "[EventLoop] ends. Took about " << pretty_print_ms ( timer( 1 ) ) << endl;
  }
  ObserverManager::Instance().destroy();
  VertigoHarvest::close();
}

const rave::VertexFactory & EventLoop::vertexFactory() const
{
  return *(LoopSettings::Instance().vertexFactory());
}


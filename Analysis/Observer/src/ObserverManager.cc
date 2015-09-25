#include "Analysis/Observer/interface/ObserverManager.h"
#include "Vertigo/Event/interface/Event.h"
#include "Vertigo/EventLoop/interface/LoopSettings.h"
#include "VertigoHarvest/interface/VertigoHarvest.h"
#include <rave/impl/FWCore/MessageLogger/interface/MessageLogger.h>
#include <vector>
#include <iostream>
#include <dlfcn.h>
#include <cstdlib>

#ifdef WITH_THREADS
#include <boost/thread/mutex.hpp>
#endif

using namespace std;

namespace {
  #ifdef WITH_THREADS 
  boost::mutex mutex;
  #endif

  static const char * PlatformExtension = ".so";

  void load ( const string & observer, bool verbose=true )
  {
    const string dh_plugin_dir = getenv("VERTIGO_PLUGIN_DIR") ? getenv("VERTIGO_PLUGIN_DIR") : VERTIGO_PLUGIN_DIR;

    string lib=dh_plugin_dir+"/obs_"+observer+PlatformExtension;
    void * r = dlopen ( lib.c_str(), RTLD_LAZY );
    if ( verbose && (!r) )
    {
      edm::LogError("ObserverManager" ) << "could not load " << lib << ":" << endl
                                        << "                  " << dlerror();
      /*
      cout << "[ObserverManager] error: could not load " << lib << ":" << endl
           << "                  " << dlerror() << endl;
           */
    } else {
      if ( verbose )
        edm::LogInfo("ObserverManager") << "loaded: " << lib;
      /*
      if ( verbose && ( LoopSettings::Instance().verbosity() > 9 ) )
        cout << "[ObserverManager] loaded: " << lib << endl;
        */
    }

  }
}

void ObserverManager::registerObserver ( const string & name, Observer * o, const string & d,
                                         bool enable )
{
  theObservers[name]=o;
  theIsEnabled[name]=enable;
  theDescription[name]=d;
}

ObserverManager::~ObserverManager()
{
  destroy();
}

void ObserverManager::close()
{
  cout << "close" << endl;
  VertigoHarvest::close();
  cout << "closed" << endl;
}

void ObserverManager::destroy()
{
  for ( map < string, Observer * >::iterator i=theObservers.begin();
        i!=theObservers.end() ; ++i )
  {
    delete i->second;
  }
  theObservers.clear();
}

string ObserverManager::describe ( const string & d )
{
  return theDescription[d];
}

ObserverManager * ObserverManager::clone() const
{
  return new ObserverManager ( * this );
}

ObserverManager::ObserverManager ( const ObserverManager & o )
{
  cout << "[ObserverManager] copy constructor! Error!" << endl;
  exit(0);
  /*
  for ( map < string, Observer * >::const_iterator i=o.theObservers.begin();
        i!=o.theObservers.end() ; ++i )
  {
    theObservers[ i->first ] = i->second->clone();
  }

  theIsEnabled=o.theIsEnabled;
  */
}

ObserverManager & ObserverManager::Instance()
{
  static ObserverManager singleton;
  return singleton;
}

bool ObserverManager::disable ( const string & s )
{
  bool ret=theIsEnabled.count(s);
  theIsEnabled[s]=false;
  return ret;
}

Observer * ObserverManager::get ( const string & s, bool verbose, bool newinstance )
{
  if ( verbose )
    edm::LogInfo("ObserverManager" ) << "trying to obtain " << s;
  if (theObservers[s])
  {
    if (newinstance) return theObservers[s]->clone();
    return theObservers[s];
  }
  // try to load shared object
  load ( s, verbose );
  Observer * ret = theObservers[s];
  if ( ret == 0 )
  {
    theObservers.erase ( s );
  }
  if (newinstance) return ret->clone();
  return ret;
}

map < string, Observer * > ObserverManager::get()
{
  return theObservers;
}

void ObserverManager::configure ( const string & s )
{
  cout << "[ObserverManager] unimplemented" << endl;
}

void ObserverManager::configure ( const string & name, const string & conf )
{
  Observer * tmp = get ( name );
  if (!tmp)
  {
    cout << "[ObserverManager] Error: trying to configure non-existant Observer \""
         << name << "\"" << endl;
    exit(-1);
  }
  tmp->configure( conf );
}

void ObserverManager::disableAll ()
{
  for ( map < string, bool >::iterator i=theIsEnabled.begin();
        i!=theIsEnabled.end() ; ++i )
  {
    i->second=false;
  };
}

bool ObserverManager::enable ( const string & s )
{
  bool ret=theIsEnabled.count(s);
  if (!ret)
  {
    load ( s );
    ret=theIsEnabled.count(s);
  }

  theIsEnabled[s]=true;
  return ret;
}

ObserverManager::ObserverManager()
{}

void ObserverManager::process ( const vertigo::Event & e )
{
  #ifdef WITH_THREADS
  boost::mutex::scoped_lock lock(mutex);
  #endif
  for ( map< string, Observer * >::iterator i=theObservers.begin();
        i!=theObservers.end() ; ++i )
  {
    if ( theIsEnabled[i->first] )
    {
      i->second->process ( e );
    }
  }
}

void ObserverManager::process ( const exception & e, const string & type )
{
  #ifdef WITH_THREADS
  boost::mutex::scoped_lock lock(mutex);
  #endif
  for ( map< string, Observer * >::iterator i=theObservers.begin();
        i!=theObservers.end() ; ++i )
  {
    if ( theIsEnabled[i->first] )
    {
      i->second->process ( e, type );
    }
  }
}

/*
void ObserverManager::process ( const edm::ParameterSet & e )
{
  #ifdef WITH_THREADS
  boost::mutex::scoped_lock lock(mutex);
  #endif
  for ( map< string, Observer * >::iterator i=theObservers.begin();
        i!=theObservers.end() ; ++i )
  {
    if ( theIsEnabled[i->first] )
    {
      i->second->process ( e );
    }
  }
}
*/

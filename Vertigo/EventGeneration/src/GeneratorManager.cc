#include "Vertigo/EventGeneration/interface/GeneratorManager.h"
#include <rave/impl/FWCore/MessageLogger/interface/MessageLogger.h>
#include "Vertigo/Event/interface/Event.h"
#include "Vertigo/EventLoop/interface/LoopSettings.h"
#include <vector>
#include <iostream>
#include <dlfcn.h>
#include <cstdlib>

using namespace std;

namespace {
  void load ( const string & generator, bool verbose )
  {
    const string dh_plugin_dir = getenv("VERTIGO_PLUGIN_DIR") ? getenv("VERTIGO_PLUGIN_DIR") : VERTIGO_PLUGIN_DIR;
    string lib=dh_plugin_dir + "/evtgen_"+generator+".so";
    void * r = dlopen ( lib.c_str(), RTLD_LAZY );
    if ( verbose && (!r) )
    {
      edm::LogError ("GeneratorManager" ) << "could not load " << lib << ": " << endl
                                          << "                   " << dlerror();
      /*
      cout << "[GeneratorManager] error: could not load " << lib << ": " << endl
           << "                   " << dlerror() << endl;
           */
    } else {
      edm::LogInfo("GeneratorManager" ) << "loaded: " << lib;
      /*
      if ( verbose && (LoopSettings::Instance().verbosity() > 9) )
        cout << "[GeneratorManager] loaded: " << lib << endl;
        */
    }
  }
}

void GeneratorManager::registerGenerator ( const string & name, EventGenerator * o, const string & d )
{
  // edm::LogInfo("GeneratorManager" ) << "registered: " << name;
  theGenerators[name]=o;
  theDescription[name]=d;
}

GeneratorManager::~GeneratorManager()
{
  /*
   let the system clean it up ....
  for ( map < string, EventGenerator * >::iterator i=theGenerators.begin();
        i!=theGenerators.end() ; ++i )
  {
    delete i->second;
  }
  */
}

std::string GeneratorManager::describe ( const std::string & d )
{
  return theDescription[d];
}

GeneratorManager::GeneratorManager ( const GeneratorManager & o )
{
  std::cout << "[GeneratorManager] copy constructor! Error!" << std::endl;
  exit(0);
}

GeneratorManager & GeneratorManager::Instance()
{
  static GeneratorManager singleton;
  return singleton;
}

EventGenerator * GeneratorManager::get ( const string & s, bool verbose )
{
  // cout << "[GeneratorManager] trying to obtain " << s << endl;
  if (theGenerators[s]) return theGenerators[s];
  /*
  cout << "[GeneratorManager] could not find " << s
       << " trying to load plugin!" << endl;
       */
  // try to load shared object
  load ( s, verbose );
  EventGenerator * ret = theGenerators[s];
  if ( ret == 0 )
  {
    theGenerators.erase ( s );
  }
  return ret;
}

map < string, EventGenerator * > GeneratorManager::get()
{
  return theGenerators;
}

GeneratorManager::GeneratorManager()
{}


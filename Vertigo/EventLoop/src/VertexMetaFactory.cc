#include "Vertigo/EventLoop/interface/VertexMetaFactory.h"
#include "Vertigo/EventLoop/interface/LoopSettings.h"                                                   
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include <iostream>
#include <dlfcn.h>                                                                                      
#include <algorithm>

using namespace std;

namespace {
  void load ( const string & skin, bool verbose=true )
  {
    const string dh_plugin_dir = getenv("VERTIGO_PLUGIN_DIR") ? getenv("VERTIGO_PLUGIN_DIR") : VERTIGO_PLUGIN_DIR;

    string lib=dh_plugin_dir+"/skin_"+skin+".so";
    void * r = dlopen ( lib.c_str(), RTLD_LAZY );
    if ( verbose && (!r) )
    {
      cout << "[SkinManager] error: could not load " << lib << ":" << endl
           << "                  " << dlerror() << endl;
    } else {
      if ( verbose && ( LoopSettings::Instance().verbosity() > 9 ) )
        cout << "[SkinManager] loaded: " << lib << endl;
    }
  }
}

bool VertexMetaFactory::query ( const string & skin )
{
  if (!theFactories[skin])
  {
    load ( skin, false );
  }
  return theFactories[skin];

}

const rave::VertexFactory * VertexMetaFactory::get ( const string & skin,
      const string & method, int verbosity )
{
  rave::VertexFactory * tmp = theFactories[skin];
  if (!tmp) load ( skin, false );
  tmp=theFactories[skin];
  if (!tmp)
  {
    edm::LogError ( "VertexMetaFactory") <<  "Unknown skin \"" << skin << "\"";
    exit(-1);
  }

  tmp->init();
  tmp->setDefaultMethod ( method );
  
  if ( method=="none" )
  {
    return 0; // WHY no factory if method==none??
  }
  return tmp;
}

VertexMetaFactory & VertexMetaFactory::Instance()
{
  static VertexMetaFactory f;
  return f;
}

VertexMetaFactory::VertexMetaFactory() {}

void VertexMetaFactory::registerFactory ( const string & name, rave::VertexFactory * f,
   const string & description )
{
  theFactories[name]=f;
  theDescriptions[name]=description;
  // edm::LogInfo ("VertexMetaFactory") << name << " registered!";
}

string VertexMetaFactory::getDescription ( const string & skin )
{
  // transform( skin.begin(), skin.end(), skin.begin(), ::tolower );
  return theDescriptions [ skin ];
}

map < string, string > VertexMetaFactory::getDescriptions()
{
  return theDescriptions;
}


/*!
 * @file main.cpp
 * @brief main routine of vertigo
 *
 * This implements the entry point for the operating system when running
 * the Vertigo sandbox to try Rave algorithms
 * RAVE = "Reconstruction Algorithms for Vertices"
 * VERTIGO = "VErtex ReconsTruction and Interfaces to Generic Objects"
 *
 */

#include <iostream>
#include <fstream>
#include <iomanip>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/convenience.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/program_options.hpp>
#include <dlfcn.h>
#include <rave/impl/FWCore/ParameterSet/interface/ParameterSet.h>
#include <rave/impl/RecoVertex/ConfigurableVertexReco/interface/VertexRecoManager.h>
#include <rave/impl/FWCore/MessageLogger/interface/MessageLogger.h>
#include <rave/impl/RaveBase/Utilities/interface/StringTools.h>
#include <rave/Version.h>
#include <rave/Logger.h>
#include "Vertigo/EventLoop/interface/EventLoop.h"
#include "Vertigo/EventGeneration/interface/GeneratorManager.h"
#include "Vertigo/EventLoop/interface/LoopSettings.h"
#include "Vertigo/EventLoop/interface/VertexMetaFactory.h"
#include "Vertigo/main/Version.h"
#include "Environment/Configuration/interface/ConfigurationSetter.h"
#include "Environment/Configuration/interface/SimpleConfiguration.h"
#include "Environment/Utilities/interface/Console.h"
#include "Environment/Configuration/interface/MultiType.h"
#include "Analysis/Observer/interface/ObserverManager.h"

extern "C" int setup_sigsegv();

using namespace std;
namespace fs = boost::filesystem;
namespace po = boost::program_options;

#if !defined(BOOST_FILESYSTEM_VERSION)
#warning "BOOST_FILESYSTEM_VERSION is not defined!"
#endif

namespace {
  // static const char * PlatformExtension = ".so";

  void loadAllPlugins()
  {
    edm::LogInfo( "ObserverManager" ) << "trying to load all plugins";
    /// loads all plugins from VERTIGO_PLUGIN_DIR
    const string dh_plugin_dir = getenv("VERTIGO_PLUGIN_DIR") ? getenv("VERTIGO_PLUGIN_DIR") :
                                 VERTIGO_PLUGIN_DIR;
    // cout << "[ObserverManager] loading all plugins found in " << dh_plugin_dir << endl;
    fs::path pth ( dh_plugin_dir );

    if (!fs::exists ( pth ) )
    {
      edm::LogError( "ObserverManager" ) << dh_plugin_dir << " does not exist!";
      return;
    }

    if (!fs::is_directory ( pth ) )
    {
      edm::LogError( "ObserverManager" ) << dh_plugin_dir << " is not a directory!";
      return;
    }

    fs::directory_iterator end_iter;

    for ( fs::directory_iterator dir_itr( pth ); dir_itr != end_iter; ++dir_itr )
    {
      try {
        string filename = "";
#if defined(BOOST_FILESYSTEM_VERSION) && BOOST_FILESYSTEM_VERSION != 3
        // boost filesystem version 2?
        filename= dir_itr->string();
#else
        // boost filesystem version 3
        filename=dir_itr->path().string();
#endif
        string extension = filename.substr(filename.size()-2);
        if ( extension != "so" ) continue;
        LogDebug( "main" ) << "  trying to load " << filename << ":" << filename.substr(filename.size()-3) << endl;
        void * r = dlopen ( filename.c_str(), RTLD_LAZY ); 
        if (!r)
        {
          cout << "[main] could not load " << filename << ": "
               << dlerror() << endl;
        } else {
          LogDebug ("main" ) << "loaded " << filename;
        }
        // try to open everything that is in that folder.
        /*
        string extension = fs::extension ( dir_itr->native_directory_string() );
        if ( extension == PlatformExtension )
        {
          string plugin = dir_itr->native_directory_string();
          // plugin=plugin.substr(0,plugin.size()-sizeof(PlatformExtension)+1 );
          // cout << " loading: " << plugin << endl;
          dlopen ( plugin.c_str(), RTLD_LAZY );
        }*/
      } catch ( ... ) {
        #if defined(BOOST_FILESYSTEM_VERSION) && BOOST_FILESYSTEM_VERSION != 3
        edm::LogWarning ("main" ) << "cannot dlopen " << dir_itr->string();
        #else
        edm::LogWarning ("main" ) << "cannot dlopen " << dir_itr->path().string();
        #endif
      };
    }
  }

  // typedef ods option_descriptions;

  void usage ( char * name, const po::options_description & ods )
  {
    cout << "Usage: " << name << " [-h] [-l] [-v<verbosity>] [-s<source>] [-m<method>]"
         << " [-S<skin>] [-t<file>] [-L<file>] [-d] [-L<logfile>]"
         << " [-n<min-max>] [-e<max>] [-o<observers>] [-c<name1=value1,name2=value2>]"
         << " [-C]" << endl
         << ods << endl;
  }

  void setMinMax ( int & min, int & max, string t )
  {
    rave::StringTools::strip ( t );
    if ( t=="all" )
    {
      min=0;
      max=-1;
      return;
    }
    string::size_type pos=t.find("-");
    if ( pos == string::npos )
    {
      // no -
      min=atoi(t.c_str() );
      max=min;
    } else if  ( pos == 0 )
    {
      // - at beginning
      min=0;
      max= atoi ( t.substr(1,string::npos).c_str() );
    } else if ( pos == t.size()-1 ) {
      max=-1;
      min= atoi ( t.substr(0,t.size()-1).c_str() );
    } else {
      min= atoi ( t.substr(0,pos).c_str() );
      max= atoi ( t.substr(pos+1,string::npos).c_str() );
    }
    // cout << "setMinMax " << t << " min=" << min << " max=" << max << endl;
  }

  string removeOption ( string text, const string & option, const string & longoption="" )
  {
    string::size_type pos = text.find ( longoption );
    string::size_type sz = longoption.size();
    if ( pos != string::npos )
    {
      text=text.replace ( pos, sz, "" );
      if ( text[0] == '=' )
      {
        text=text.replace(0,1,"");
      }
    }
    pos = text.find ( option );
    sz = option.size();
    if ( pos != string::npos )
    {
      text=text.replace ( pos, sz, "" );
    }
    return text;
  }

  void enableObserver ( string t, int verbosity )
  {
    rave::StringTools::strip ( t );
    if ( t=="none" )
    {
      ObserverManager::Instance().disableAll();
      return;
    }
    bool ret = ObserverManager::Instance().enable ( t );
    if (verbosity>1)
    {
      if (ret)
      {
        edm::LogInfo( "ObserverManager" ) << "enabling Observer " << t;
      } else {
        edm::LogError( "ObserverManager" ) << "no Observer \"" << t << "\" defined.";
        exit(-1);
      };
    }
  }

  void useSpecifiedObservers ( const string & s, int verbosity )
  {
    ObserverManager::Instance().disableAll();
    list < string > obs=rave::StringTools::split ( s, "," );
    for ( list < string >::const_iterator i=obs.begin(); i!=obs.end() ; ++i )
    {
      string name=*i;
      string::size_type pos = i->find("-");
      if ( pos != string::npos )
      {
        name=i->substr(0,pos);
        string conf=i->substr(pos+1,string::npos );
        ObserverManager::Instance().configure ( name, conf );
      }
      enableObserver ( name, verbosity );
    }
  }

  void noArgument ( const string & s )
  {
    edm::LogError( "main" ) << "no argument for " << s << " was given. (Forgot space?)";
    exit(0);
  }

  template <class T> unsigned maxLength ( const map < string, T > & m )
  {
    unsigned ret=0;
    for ( typename map < string, T >::const_iterator i=m.begin();
          i!=m.end() ; ++i )
    {
      if ( i->first.size() > ret ) ret=i->first.size();
    }
    return ret;
  }

  void listAll()
  {
    cout << "Vertigo version    " << vertigo::Version() << endl;
    cout << "Using rave version " << rave::Version() << endl;
    cout << endl
         << "Observers:" << endl
         << "==========" << endl;
    loadAllPlugins();
    map < string, Observer * > obs = ObserverManager::Instance().get();
    unsigned w=maxLength<Observer *> ( obs );

    for ( map < string, Observer * >::const_iterator i=obs.begin(); i!=obs.end() ; ++i )
    {
      cout << " " << setw(w) << i->first << " - " << ObserverManager::Instance().describe(i->first)
           << endl;
    };
    cout << endl
         << "Methods:" << endl
         << "========" << endl;

    map < string, AbstractConfReconstructor * > recs = VertexRecoManager::Instance().get();
    w=maxLength<AbstractConfReconstructor *> ( recs );
    for ( map < string, AbstractConfReconstructor * >::const_iterator i=recs.begin();
          i!=recs.end() ; ++i )
    {
      cout << " " << setw(w) << i->first << " - " << VertexRecoManager::Instance().describe(i->first)
           << endl
           << "           " << i->first;

      edm::ParameterSet def = i->second->defaults();
      if ( !(def.isEmpty() ) )
      {
        cout << "-" << def;
      } else {
        cout << endl;
      }
      cout << endl;
    };

    cout << endl
         << "Event sources:" << endl
         << "==============" << endl;
    map < string, EventGenerator * > guns = GeneratorManager::Instance().get();
    for ( map< string, EventGenerator * >::const_iterator i=guns.begin(); i!=guns.end() ; ++i )
    {
      cout << " " << setw(15) << i->first << " - " << GeneratorManager::Instance().describe(i->first)
           << endl;
    }
    cout << endl;
    
    EventGenerator * g = GeneratorManager::Instance().get("gun",false);
    if ( g ) { cout << g->describe() << endl; }

    cout << endl
         << "Skins:" << endl
         << "======" << endl;
    map < string, string > skins = VertexMetaFactory::Instance().getDescriptions();
    for ( map< string, string >::const_iterator i=skins.begin(); i!=skins.end() ; ++i )
    {
      cout << " " << setw(7) << i->first << " - " << i->second << endl;
    }

    cout << endl;

    exit(0);
  }

  void showConfigurables()
  {
    cout << endl
         << "List of SimpleConfigurables:" << endl
         << "============================" << endl
         << "(Note that this list shows only those" << endl
         << " configurables that have been used in this run)" << endl
         << endl;
    SimpleConfiguration::current()->status();
    exit(0);
  }
}

int main ( int argc, char * argv[] )
{
    string method;
    int verbosity;
    string source;
    string skin;
    string observers;
    string configurables;
    string flavortag_method="";
    string logfile="stdout";
    signed min=0;
    signed max=-1;
    int expect=0;
    int nthreads=1;
    bool show_confs=false;
    bool usebeamspot=false;
    string nevs; /// number of events, as string
    bool sigsegv=false; // print stack on sigsegv?
    string cfgfile="vertigo.cfg";
  
    po::options_description option_descriptions("Options");

    option_descriptions.add_options() 
      ( "help,h", "Show this help." )
      ( "list,l", "List all methods, observers, and guns (then quit)." )
      ( "verbosity,v",po::value<int>(&verbosity)->default_value(10), "Set verbosity." )
      ( "configfile,f",po::value<string>(&cfgfile)->default_value("vertigo.cfg"), "Define configfile." )
      ( "expect,e",po::value<int>(&expect)->default_value(0), "Expect this many events (just to have a decent progress bar)." )
      ( "nthreads,j",po::value<int>(&nthreads)->default_value(1), "Define number of threads to use." )
      ( "flavortagmethod,F",po::value<string>(&flavortag_method)->default_value("none"),"Flavortag method: none, default." )
      ( "logfile,L",po::value<string>(&logfile)->default_value("stdout"),"Where do we log to" )
      ( "source,s",po::value<string>(&source)->default_value(""),
        "Specify data source(s) [Multiple sources are separated via ';']." )
      ( "skin,S",po::value<string>(&skin)->default_value("cms"),"Specify skin." )
      ( "print-stack,d","Print stack when sigsegfaulting." )
      ( "method,m",po::value<string>(&method)->default_value("default"),"Specify reconstruction method." )
      ( "events,n",po::value<string>(&nevs)->default_value("all"),"Specify the event numbers to be considered, specified as min-max. max=-1 means \"until the end\". \"all\" is the same as \"0--1\".")
      ( "beamspot,b","Use beamspot constraint.")
      ( "observer,o",po::value<string>(&observers)->default_value("none"),"Run specified observers (comma-separated list).")
      ( "configurable,c",po::value<string>(&configurables)->default_value(""),"Set configurables (comma-separated list).")
      ( "list-configurables,C","List all configurables at the end of the run.")
    ;

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, option_descriptions), vm);
    po::notify(vm);
    
    if ( logfile != "stdout" && logfile != "" && logfile != "--" )
    {
      rave::Logger::writeToFile ( logfile );
      LoopSettings::Instance().setLogToFile ( true );
    }

    LoopSettings::Instance().setVerbosity( verbosity );
    int barlength=50;
    if ( getenv("COLUMNS" ) )
    {
      barlength=atoi ( getenv("COLUMNS" ) ) - 30;
    }
    LoopSettings::Instance().setBarLength( barlength );

    if (vm.count("help")) { usage (argv[0], option_descriptions ); exit(0); };
    if (vm.count("list")) { listAll(); exit(0); };

    ifstream istr( cfgfile.c_str() );
    if ( istr.is_open() )
    {
      cout << "[main] parsing " << cfgfile << endl;
      po::store(po::parse_config_file(istr,option_descriptions), vm);
      po::notify(vm);
    } else {
      // couldnt open config file? See if there one explicitly asked for!
      if ( cfgfile != "vertigo.cfg" )
      {
        cout << "[main] error: config file \"" << cfgfile << "\" could not be opened." << endl;
        exit(-1);
      }
    }

    if (vm.count("configurable") && configurables.size() ) { ConfigurationSetter::set ( configurables ); };
    if (vm.count("list-configurables" )) { show_confs=true; };
    if (vm.count("beamspot")) { usebeamspot=true; };
    if (vm.count("print-stack")) { sigsegv=true; };
    useSpecifiedObservers ( observers, verbosity );
    setMinMax( min, max, nevs );
    if ( expect == 0 ) { expect=(max-min); };

    edm::LogInfo( "main" ) << "considering events [" << min << ", " << max << "]";

    if ( sigsegv ) setup_sigsegv();

    try {
    if ( source.size() )
    {
      // And now for the event loop!!!
      EventLoop loop( source, method, flavortag_method, skin, min, max, verbosity, usebeamspot, nthreads, expect );
      loop.loop();
    }
    } catch ( std::exception & s )
    {
      cout << "[main] std::exception " << s.what() << endl;
    } catch ( cms::Exception & s )
    {
      cout << "[main] cms::Exception " << s.what() << endl;
    }

    // finally, show configurables, if user wants to.
    cout << vertigo::Console().reset();
    if ( show_confs ) showConfigurables();
}

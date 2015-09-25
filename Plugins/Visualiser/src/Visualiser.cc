#include "Visualiser.h"

#include "Vertigo/Event/interface/Event.h"
#include "Vertigo/EventLoop/interface/LoopSettings.h"
#include "VertigoHarvest/interface/EventToTuple.h"
#include "Vertigo/EventLoop/interface/LoopSettings.h"
#include "Environment/Configuration/interface/SimpleConfigurable.h"
#include "Environment/Configuration/interface/ConfigurationSetter.h"

#include <rave/impl/DataFormats/GeometryVector/interface/GlobalPoint.h>
#include <rave/impl/RaveBase/Utilities/interface/StringTools.h>
#include <dataharvester/Tuple.h>

#include <Visualiser/VisMain.h>
#include <Config/Configurable.h>
#include <Reader/VisEvent.h>
#include <Reader/VisEventBuilder.h>

#include <vector>
#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>

using namespace std;
using namespace dataharvester;

namespace {
  int do_wait()
  {
    return SimpleConfigurable<int>(1,"Visualiser:Wait").value();
  }
}

Visualiser::Visualiser( const string & name ) :
    firstEvent(true), fork_(true)
{
  ConfigurationSetter::set("EventToTuple:AddRecVertices=1");
  VisMain::instance().setDefaults();
  configure ( name );
}

void Visualiser::process ( const vertigo::Event & e )
{
  if ( LoopSettings::Instance().verbosity() > 5 )
  {
    cout << "[Visualiser] About to draw event: " << e.tracks().size() << " tracks, "
         << e.vertices().size() << " vertices, " << e.jets().size() << " jets."
         << endl;
  }
  Tuple t=vertigo::EventToTuple().create ( e, true, true );
  VisEvent ev = VisEventBuilder().create ( t );
  /*
  cout << "[Visualiser] drawing event " << e.id() << ": " << ev.objects().size()
       << " objects." << endl;
       */
  
  // pid_t pid = vfork();
  pid_t pid = 0;
  if ( fork_ ) pid = fork();
  int status;

  if ( pid == 0 )
  {
    VisMain::instance().init ( ev, "direct", 0, 0, false );
    exit(0);
  } else {
    if (do_wait())
    {
      while (wait(&status) != pid)
      {
      };
    };
  };
}

void Visualiser::process ( const exception & e, const string & )
{}

void Visualiser::configure ( const string & f )
{
  try {
    theConfig = f;
    // ConfigurationSetter::set( theConfig );
    if ( f=="nofork" ) fork_=false;
    list < string > tokens = rave::StringTools::split ( f, ";" );
    for ( list < string >::const_iterator i=tokens.begin(); 
          i!=tokens.end() ; ++i )
    {
      if ( (*i).find("=")!=string::npos )
      {
        string name=i->substr(0, (*i).find("=") );
        string value=i->substr( (*i).find("=")+1, string::npos );
        Configurable().set(name,value);
      }
    }
  } catch ( ... ) {
    cout << "[Visualiser] error caught exception when configuring." << endl;
  }

}

Visualiser * Visualiser::clone() const
{
  return new Visualiser ( *this );
}

#include "Analysis/Observer/interface/ObserverBuilder.h"
namespace {
  ObserverBuilder<Visualiser> t( "Visualiser", "Observer that feeds event to RaveVis", false );
}

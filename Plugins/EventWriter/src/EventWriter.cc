#include "EventWriter.h"
#include <vector>
#include <iostream>
#include "Vertigo/Event/interface/Event.h"
#include "Vertigo/EventLoop/interface/LoopSettings.h"
#include "Vertigo/EventLoop/interface/LoopSettings.h"
#include "Environment/Configuration/interface/SimpleConfigurable.h"
#include <rave/impl/DataFormats/GeometryVector/interface/GlobalPoint.h>
#include <rave/impl/FWCore/MessageLogger/interface/MessageLogger.h>


using namespace std;

EventWriter::EventWriter( string name ) :
    theFileName ( name ) , theWriter ( name )
{
}

void EventWriter::process ( const vertigo::Event & e )
{
  edm::LogInfo("EventWriter") 
    << "Writing " << e.tracks().size() << " tracks, " << e.vertices().size() << " vertices, and "
    << e.particles().size() << " particles to \""
    << theFileName << "\"";
  static bool propagate=SimpleConfigurable<bool>(true,"EventWriter:Propagate").value();
  static bool colors=SimpleConfigurable<bool>(true,"EventWriter:Colors").value();
  theWriter.save ( e, propagate, colors );
}

void EventWriter::process ( const exception & e, const string & what )
{
  edm::LogInfo("EventWriter" ) << "Exception " << e.what() << " received.";
}

void EventWriter::configure ( const string & f )
{
  theFileName = f;
  theWriter.configure( theFileName );
}

EventWriter * EventWriter::clone() const
{
  return new EventWriter ( *this );
}

#include "Analysis/Observer/interface/ObserverBuilder.h"
namespace {
  ObserverBuilder<EventWriter> t( "EventWriter",
      "Simple Observer that dumps events into a file", false );
}

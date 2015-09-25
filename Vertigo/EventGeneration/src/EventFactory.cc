#include "Vertigo/EventGeneration/interface/EventFactory.h"
#include "Vertigo/EventLoop/interface/LoopSettings.h"
#include "Vertigo/EventGeneration/interface/GeneratorManager.h"
#include <utility>
#include <iostream>

using namespace std;

namespace {
  pair < string, string > splitGenName ( const string & s )
  {
    pair < string, string > ret;
    string::size_type pos = s.find(":");
    if ( pos == string::npos )
    {
      ret.first="file";
      ret.second=s;
    } else {
      ret.first=s.substr(0,pos);
      ret.second=s.substr(pos+1,string::npos );
    }
    return ret;
  }

  string & lstrip( string & source )
  {
    while( source.size() && isspace(source[0]) )
      { source.replace ( 0, 1, "" ); };
    return source;
  }

  string & rstrip( string & source )
  {
    while( source.size() && isspace(source[source.size()-1]) )
      { source.replace ( source.size()-1, 1, "" ); };
    return source;
  }

  //------------------------------------------------------------
  string & strip( string & source )
  {
    lstrip( rstrip(source) );
    return source;
  }

  vector < string > splitSources ( const string & s )
  {
    string tmp=s;
    vector < string > ret;
    string::size_type pos = tmp.find(";");
    while ( pos != string::npos )
    {
      string t=tmp.substr ( 0, pos ) ;
      string token=strip ( t );
      ret.push_back ( token );
      tmp=tmp.substr(pos+1,string::npos );
      pos = tmp.find(";");
    }

    if ( tmp.size() )
    {
      string token=strip ( tmp );
      ret.push_back ( token );
    }
    return ret;
  }

  vector < pair < string, string > > getGenerators ( const string & g )
  {
    vector < pair < string, string > > ret;
    vector < string > srcs = splitSources ( g );
    for ( vector< string >::const_iterator i=srcs.begin(); i!=srcs.end() ; ++i )
    {
      pair < string, string > name = splitGenName ( *i );
      ret.push_back ( name );
    }
    
    /*
    cout << "[EventFactory] DEBUG: generators " << endl;
    for ( vector< pair < string, string > >::const_iterator i=ret.begin(); i!=ret.end() ; ++i )
      cout << "   - >" << i->first << "=" << i->second << "<" << endl;
      */
    
    return ret;
  }
}

void EventFactory::source ( const string & generator )
{
  // cout << "[EventFactory] source " << generator << endl;
  theGeneratorName=generator;
  if ( generator.size() == 0 ) return;
  theGenerators = getGenerators ( theGeneratorName );

  if ( theGenerator )
  {
    cout << "[EventFactory] WARNING: has a generator before ::source???" << endl;
  }
}

void EventFactory::initialise() const
{
  theGeneratorIter = theGenerators.begin();
  /* cout << "[EventFactory] generator: " << theGeneratorIter->first << " -> "
       << theGeneratorIter->second << endl; */
  theGenerator = GeneratorManager::Instance().get ( theGeneratorIter->first );
  if ( !theGenerator )
  {
    cout << "[EventFactory] Error: no event generator ``" << theGeneratorIter->first
         << "'' registered."
         << endl;
    exit ( 0 );
  }
  // cout << "[EventFactory] now ::source " << name.second << endl;
  theGenerator->source ( theGeneratorIter->second );
}

EventFactory::EventFactory( const string & generator ) :
  theGeneratorIter(0), theGenerator ( 0 ), theDontComplainAboutDoubles(false)
{
  source ( generator );
}

bool EventFactory::hasBeamSpot() const
{
  if ( &(*theGeneratorIter) == 0 ) initialise();
  if ( theGenerator ) return theGenerator->hasBeamSpot();
  return false;
}

rave::Ellipsoid3D EventFactory::beamSpot() const
{
  if ( &(*theGeneratorIter) == 0 ) initialise();
  if ( theGenerator ) return theGenerator->beamSpot();
  return rave::Ellipsoid3D();
}

void EventFactory::nextGenerator() const
{
  theGeneratorIter++;
  if ( theGeneratorIter == theGenerators.end() )
  {
    // cout << "[EventFactory no generator left ... " << endl;
    throw exception();
  }

  // cout << theGeneratorIter->first << endl;

  theGenerator = GeneratorManager::Instance().get ( theGeneratorIter->first );

  if ( !theGenerator )
  {
    cout << "[EventFactory] Error: no event generator ``" << theGeneratorIter->first
         << "'' registered."
         << endl;
    exit ( 0 );
  }
  theGenerator->source ( theGeneratorIter->second );
}

/*
EventFactory EventFactory::operator= ( const EventFactory & )
{
  cout << "[EventFactory] operator=" << endl;
  return EventFactory ( *this );
}

EventFactory::EventFactory ( const EventFactory & o ) :
  theGeneratorName ( o.theGeneratorName ),
  theGenerators ( o.theGenerators ), theGeneratorIter ( 0 ),
  theGenerator ( 0 )

{
  cout << "[EventFactory] copy construction!" << endl;
  source ( theGeneratorName );
}*/

EventFactory::~EventFactory()
{
  // delete theGenerator; deleting is now the GeneratorManager's task
}

int EventFactory::skip ( int n ) const
{
  if ( &(*theGeneratorIter) == 0 ) initialise();
  return theGenerator->skip ( n );
}

std::string EventFactory::describe() const
{
  return "EventFactory";
}

void EventFactory::reset()
{
  // first reset all generators
  for ( vector< pair < string, string > >::const_iterator 
        i=theGenerators.begin(); i!=theGenerators.end() ; ++i )
  {
    theGenerator = GeneratorManager::Instance().get ( i->first );
    theGenerator->reset();
  }
  // then point back to first generator
  initialise();
}

vertigo::Event EventFactory::next() const
{
  // cout << "[EventFactory] next." << endl;
  if ( &(*theGeneratorIter) == 0 ) initialise();
  /*
  cout << "[EventFactory] get next event, theGenerator="
       << (void *) theGenerator << endl;
       */
  try {
    return theGenerator->next();
    /*
    cout << "[EventFactory] yeah, try" << endl;
    vertigo::Event event = theGenerator->next();
    cout << "[EventFactory] got an event." << endl;
    return event;
    */
  } catch ( ... ) { 
  }
  // try next generator
  nextGenerator();
  return next();
}

void EventFactory::dontComplainAboutDoubles()
{
  theDontComplainAboutDoubles=true;
}

const EventFactory & EventFactory::__iter__() const
{
  return (*this);
}

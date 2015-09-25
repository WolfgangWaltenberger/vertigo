#include "Vertigo/Event/interface/Event.h"

using namespace std;
using namespace vertigo;


Event::Event ( int id, int run, const string & tag, const string & algo ) :
    Base ( new BasicEvent ( id, run, tag, algo ) )
{}

Event::Event ( const vector < rave::Track > & t,
                             const vector < vertigo::SimTrack > & s,
                             const vector < vertigo::SimVertex > & v, int id, int run,
                             const string & tag, const string & algo ) :
    Base ( new BasicEvent ( t, s, v, id, run, tag, algo ) )
{}

Event::Event ( const vector < vertigo::Jet > & j,
                             const vector < vertigo::SimTrack > & s,
                             const vector < vertigo::SimVertex > & v, int id, int run,
                             const string & tag, const string & algo ) :
    Base ( new BasicEvent ( j, s, v, id, run, tag, algo ) )
{}

Event::Event (
  const std::vector< rave::KinematicParticle > & p,
  const std::vector< vertigo::SimParticle > & sp,
  const std::vector< vertigo::SimVertex > & sv, int id, int run,
  const std::string & tag, const std::string & algo ) :
    Base( new BasicEvent( p, sp, sv, id, run, tag, algo ) )
{}


void Event::add ( const rave::Track & o )
{
  // cout << "[Event] add " << o.id() << endl;
  sharedData().add ( o );
}

void Event::add ( const std::vector < rave::Track > & o )
{
  addTracks ( o );
}

void Event::addTracks ( const std::vector < rave::Track > & o )
{
  for ( vector< rave::Track >::const_iterator i=o.begin(); 
        i!=o.end() ; ++i )
    add ( *i );
}

void Event::add ( const std::vector < rave::Vertex > & o )
{
  addVertices ( o );
}

void Event::addVertices ( const std::vector < rave::Vertex > & o )
{
  for ( vector< rave::Vertex >::const_iterator i=o.begin(); 
        i!=o.end() ; ++i )
    add ( *i );
}

void Event::add ( const std::vector < rave::KinematicParticle > & o )
{
  addParticles ( o );
}

void Event::addParticles ( const std::vector < rave::KinematicParticle > & o )
{
  for ( vector< rave::KinematicParticle >::const_iterator i=o.begin(); 
        i!=o.end() ; ++i )
    add ( *i );
}

void Event::addSimParticles ( const std::vector < vertigo::SimParticle > & o )
{
  for ( vector< vertigo::SimParticle >::const_iterator i=o.begin(); 
        i!=o.end() ; ++i )
    add ( *i );
}

void Event::add ( const std::vector < vertigo::SimParticle > & o )
{
  addSimParticles ( o );
}

void Event::add ( const std::vector < vertigo::Jet > & o, bool addt )
{
  addJets ( o, addt );
}

void Event::addJets ( const std::vector < vertigo::Jet > & o, bool addt )
{
  for ( vector< vertigo::Jet >::const_iterator i=o.begin(); 
        i!=o.end() ; ++i )
    add ( *i, addt );
}


void Event::clearTracks()
{
  sharedData().clearTracks();
}

void Event::removeParticles()
{
  // FIXME lets see if this clears our memory hole
  sharedData().removeParticles();
}

void Event::add ( const rave::KinematicParticle & p )
{
  sharedData().add ( p );
}

void Event::add ( const rave::Vertex & o )
{
  sharedData().add ( o );
}

void Event::add ( const vertigo::SimTrack & o )
{
  sharedData().add ( o );
}

void Event::add ( const vertigo::SimParticle & sp )
{
  sharedData().add ( sp );
}

void Event::add ( const vertigo::SimVertex & o )
{
  sharedData().add ( o );
}

void Event::add ( const std::vector < vertigo::SimVertex > & o )
{
  addSimVertices ( o );
}

void Event::addSimVertices ( const std::vector < vertigo::SimVertex > & o )
{
  for ( vector< vertigo::SimVertex >::const_iterator i = o.begin(); i != o.end() ; ++i )
    add ( *i );
}

void Event::add ( const std::vector < vertigo::SimTrack > & o )
{
  addSimTracks ( o );
}

void Event::addSimTracks ( const std::vector < vertigo::SimTrack > & o )
{
  for ( vector< vertigo::SimTrack >::const_iterator i = o.begin(); i != o.end() ; ++i )
    add ( *i );
}

void Event::add ( const vertigo::Jet & o, bool addtracks )
{
  sharedData().add ( o, addtracks );
}

Event::~Event()
{
  // remove all associations and pointers, or else 
  // ref counting cannot work.
  // cout << "[Event] destroy!" << endl;
}

vector < rave::Vertex > Event::vertices() const
{
  return data().vertices();
}

vector < rave::Track > Event::tracks() const
{
  return data().tracks();
}

vector < rave::KinematicParticle > Event::particles() const
{
  return data().particles();
}

vector < vertigo::Jet > Event::jets() const
{
  return data().jets();
}

vector < vertigo::Jet > & Event::jetRefs()
{
  return sharedData().jetRefs();
}


void Event::tag ( const vertigo::Jet & j, double value )
{
  sharedData().tag ( j, value );
}

vector < vertigo::SimTrack > Event::simTracks() const
{
  return data().simTracks();
}

vector < vertigo::SimParticle > Event::simParticles() const
{
  return data().simParticles();
}

vector < vertigo::SimVertex > Event::simVertices() const
{
  return data().simVertices();
}

int Event::id() const
{
  return data().id();
}

int Event::run() const
{
  return data().run();
}

string Event::tag() const
{
  return data().tag();
}

void Event::setTag ( const string & t )
{
  sharedData().setTag ( t );
}

string Event::algorithm() const
{
  return data().algorithm();
}

void Event::associateTracks()
{
  sharedData().associateTracks();
}

void Event::associateVertices()
{
  sharedData().associateVertices();
}

void Event::associate ( const vertigo::SimTrack & s, const rave::Track & r )
{
  sharedData().associate ( s, r );
}

void Event::associate ( const vertigo::SimParticle & s, const rave::KinematicParticle & r )
{
  sharedData().associate ( s, r );
}

void Event::associate ( const vertigo::SimVertex & s, const rave::Vertex & r )
{
  sharedData().associate ( s, r );
}

void Event::unReference()
{
  sharedData().unReference();
}

std::vector < vertigo::SimTrack > Event::associatedTracks ( const rave::Track & t ) const
{
  return data().associatedTracks ( t );
}

std::vector < rave::Track > Event::associatedTracks ( const vertigo::SimTrack & t ) const
{
  return data().associatedTracks ( t );
}

std::vector < vertigo::SimParticle > Event::associatedParticles ( const rave::KinematicParticle & p ) const
{
  return data().associatedParticles ( p );
}

std::vector < rave::KinematicParticle > Event::associatedParticles ( const vertigo::SimParticle & p ) const
{
  return data().associatedParticles ( p );
}


bool Event::isReconstructible ( const SimVertex & vtx ) const
{
  return data().isReconstructible ( vtx );
}

int Event::nRecTracks ( const SimVertex & vtx ) const
{
  return data().nRecTracks ( vtx );
}

std::vector < vertigo::SimVertex > Event::associatedVertices ( const rave::Vertex & t ) const
{
  return data().associatedVertices ( t );
}

std::vector < rave::Vertex > Event::associatedVertices ( const vertigo::SimVertex & t ) const
{
  return data().associatedVertices ( t );
}


float Event::time () const
{
  return data().time();
}

void Event::setTime ( float t )
{
  sharedData().setTime ( t );
}

void Event::setAlgorithm ( const string & algo )
{
  sharedData().setAlgorithm ( algo );
}

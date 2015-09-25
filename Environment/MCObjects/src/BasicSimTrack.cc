#include "Environment/MCObjects/interface/BasicSimTrack.h"
#include "Environment/MCObjects/interface/SimVertex.h"
#include <rave/impl/RaveBase/RaveEngine/interface/RaveId.h>
#include <rave/impl/RaveBase/Converters/interface/RaveStreamers.h>

using namespace std;
using namespace vertigo;

BasicSimTrack::BasicSimTrack( const rave::Vector6D & s, rave::Charge q,
   void * p, int pid, const std::string & d, const SimVertex * vtx )
  : theState ( s ), theCharge ( q ), thePointer ( p ), thePid ( pid ), theTag ( d ),
    theId ( RaveId::uniqueId() ), theVertex ( 0 )
{
  if ( vtx )
  {
    theVertex = new vertigo::SimVertex ( *vtx );
  }
}

BasicSimTrack::BasicSimTrack( int id, const rave::Vector6D & s, rave::Charge q,
   void * p, int pid, const std::string & d, const SimVertex * vtx )
  : theState ( s ), theCharge ( q ), thePointer ( p ), thePid ( pid ), theTag ( d ),
    theId ( id ), theVertex ( 0 )
{
  if ( vtx )
  {
    theVertex = new vertigo::SimVertex ( *vtx );
  }
}


BasicSimTrack::BasicSimTrack() : theState ( rave::Vector6D() ), theCharge (0), thePointer(0),
  thePid(0), theTag("invalid"), theId ( 0 ), theVertex ( 0 )
{
}

int BasicSimTrack::pid() const
{
  return thePid;
}

BasicSimTrack::~BasicSimTrack()
{
  if ( theVertex ) delete theVertex;
}

std::string BasicSimTrack::tag() const
{
  return theTag;
}

rave::Charge BasicSimTrack::charge() const
{
  return theCharge;
}

int BasicSimTrack::id() const
{
  return theId;
}

const rave::Vector6D & BasicSimTrack::state() const
{
  return theState;
}

void * BasicSimTrack::originalObject() const
{
  return thePointer;
}

void BasicSimTrack::setId ( int id )
{
  theId=id;
}

void BasicSimTrack::setVertex ( const SimVertex * v )
{
  if ( theVertex ) delete theVertex;
  if ( v==0 )
  {
    theVertex=0;
  } else {
    theVertex= new SimVertex ( *v );
  }
}

const SimVertex * BasicSimTrack::vertex() const
{
  return theVertex;
}

bool BasicSimTrack::operator< ( const BasicSimTrack & o ) const
{
  return ( id() < o.id() );
}

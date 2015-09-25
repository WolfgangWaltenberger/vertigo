#include "Environment/MCObjects/interface/SimTrack.h"
#include <rave/impl/RaveBase/RaveEngine/interface/RaveId.h>
#include <rave/impl/RaveBase/Converters/interface/RaveStreamers.h>

using namespace vertigo;

SimTrack::SimTrack( const rave::Vector6D & s, rave::Charge q,
   void * p, int pid, const std::string & d, const SimVertex * vtx )
  : Base ( new BasicSimTrack ( s, q, p, pid, d, vtx ) )
{}

SimTrack::SimTrack( int id, const rave::Vector6D & s, rave::Charge q,
   void * p, int pid, const std::string & d, const SimVertex * vtx )
  : Base ( new BasicSimTrack ( id, s, q, p, pid, d, vtx ) )
{}

SimTrack::SimTrack() : Base ( new BasicSimTrack () ) {}

void SimTrack::setVertex ( const SimVertex * v )
{
  sharedData().setVertex ( v );
}

void SimTrack::setId ( int id )
{
  sharedData().setId ( id );
}

std::string SimTrack::tag() const
{
  return data().tag();
}

rave::Charge SimTrack::charge() const
{
  return data().charge();
}

int SimTrack::id() const
{
  return data().id();
}

int SimTrack::pid() const
{
  return data().pid();
}

const rave::Vector6D & SimTrack::state() const
{
  return data().state();
}

const rave::Vector3D & SimTrack::momentum() const
{
  return data().state().momentum();
}

const rave::Point3D & SimTrack::position() const
{
  return data().state().position();
}

float SimTrack::pt() const
{
  return data().state().momentum().perp();
}


void * SimTrack::originalObject() const
{
  return data().originalObject();
}

const SimVertex * SimTrack::vertex() const
{
  return data().vertex();
}

bool SimTrack::operator< ( const SimTrack & o ) const
{
  return ( id() < o.id() );
}

bool SimTrack::operator== ( const SimTrack & o ) const
{
  return ( id() == o.id() );
}

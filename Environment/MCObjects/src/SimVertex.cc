#include "Environment/MCObjects/interface/SimVertex.h"
#include <rave/impl/RaveBase/RaveEngine/interface/RaveId.h>

using namespace vertigo;

SimVertex::SimVertex ( const rave::Point3D & p,
                       const std::vector < SimTrack > & tracks,
                       int pid,
                       const std::string & name, bool isprim ) :
    Base ( new BasicSimVertex ( p, tracks, pid, name, isprim ) )
{}

SimVertex::SimVertex ( const rave::Point3D & p,
                       const std::vector < SimParticle > & particles,
                       int pid, const std::string & name, bool isprim ) :
    Base ( new BasicSimVertex ( p, particles, pid, name, isprim ) )
{}


std::string SimVertex::tag() const
{
  return data().tag();
}

bool SimVertex::isPrimary() const
{
  return data().isPrimary();
}

SimVertex::SimVertex() : Base ( new BasicSimVertex() )
{}

const rave::Point3D & SimVertex::position() const
{
  return data().position();
}

void SimVertex::add ( const SimTrack & t )
{
  sharedData().add ( t );
}

void SimVertex::add ( const SimParticle & p )
{
  sharedData().add ( p );
}

const std::vector < SimTrack > & SimVertex::tracks() const
{
  return data().tracks();
}

const std::vector < SimParticle > & SimVertex::particles() const
{
  return data().particles();
}

int SimVertex::id() const
{
  return data().id();
}

int SimVertex::pid() const
{
  return data().pid();
}

bool SimVertex::operator< ( const SimVertex & o ) const
{
  return ( id() < o.id() );
}

bool SimVertex::operator== ( const SimVertex & o ) const
{
  return ( id() == o.id() );
}

void SimVertex::setId( int id )
{
  sharedData().setId(id);
}

bool SimVertex::isValid() const
{
  return data().isValid();
}

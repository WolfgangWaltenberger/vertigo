#include "Environment/MCObjects/interface/SimParticle.h"
#include <rave/impl/RaveBase/RaveEngine/interface/RaveId.h>
#include <rave/impl/RaveBase/Converters/interface/RaveStreamers.h>

using namespace vertigo;

SimParticle::SimParticle (
  const rave::Vector7D & s, rave::Charge q,
  void * p, int pid, const std::string & d, const SimVertex * vtx ) :
    Base ( new BasicSimParticle ( s, q, p, pid, d, vtx ) )
{}

SimParticle::SimParticle() :
    Base ( new BasicSimParticle () )
{}

rave::Vector4D SimParticle::p4() const
{
  return rave::Vector4D ( state().momentum(), state().m() );
}

void SimParticle::setVertex ( const SimVertex * v )
{
  sharedData().setVertex ( v );
}

std::string SimParticle::tag() const
{
  return data().tag();
}

rave::Charge SimParticle::charge() const
{
  return data().charge();
}

int SimParticle::id() const
{
  return data().id();
}

int SimParticle::pid() const
{
  return data().pid();
}

const rave::Vector7D & SimParticle::state() const
{
  return data().state();
}

void * SimParticle::originalObject() const
{
  return data().originalObject();
}

const SimVertex * SimParticle::vertex() const
{
  return data().vertex();
}

bool SimParticle::operator< ( const SimParticle & o ) const
{
  return ( id() < o.id() );
}

bool SimParticle::operator== ( const SimParticle & o ) const
{
  return ( id() == o.id() );
}

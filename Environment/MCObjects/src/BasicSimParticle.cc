#include "Environment/MCObjects/interface/BasicSimParticle.h"

#include "boost/cast.hpp"

namespace vertigo
{

BasicSimParticle::BasicSimParticle (
  const rave::Vector7D & v, rave::Charge q, void * originalobject, int pid,
  const std::string & tag, const SimVertex * vtx ) :
    BasicSimTrack ( v, q, originalobject, pid, tag, vtx ),
    theState ( v )
{}

BasicSimParticle::BasicSimParticle()
    : BasicSimTrack()
{}

const rave::Vector7D & BasicSimParticle::state() const
{
  return theState;
}

}

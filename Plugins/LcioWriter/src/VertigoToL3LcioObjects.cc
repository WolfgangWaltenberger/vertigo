#include "VertigoToL3LcioObjects.h"

#include <boost/assert.hpp>
#include <boost/numeric/ublas/symmetric.hpp>

#include <math.h>

VertigoToL3LcioObjects::VertigoToL3LcioObjects ( double Bz ) :
    theBz ( Bz )
{
}

VertigoToL3LcioObjects::~VertigoToL3LcioObjects()
{
}

IMPL::MCParticleImpl * VertigoToL3LcioObjects::convert(
    const vertigo::SimParticle & vertigoSimParticle ) const
{
  rave::Charge   raveCharge = vertigoSimParticle.charge();
  rave::Vector7D raveState  = vertigoSimParticle.state();

  double lcioMass = raveState.m();

  double lcioVertex[3]   = { raveState.x(),  raveState.y(),  raveState.z() };
  double lcioMomentum[3] = { raveState.px(), raveState.py(), raveState.pz() };

  IMPL::MCParticleImpl* lcioMCParticle = new IMPL::MCParticleImpl();

  lcioMCParticle->setVertex( lcioVertex );
  lcioMCParticle->setMomentum( lcioMomentum );
  lcioMCParticle->setMass( lcioMass );
  lcioMCParticle->setCharge( raveCharge );
  
  return lcioMCParticle;
}

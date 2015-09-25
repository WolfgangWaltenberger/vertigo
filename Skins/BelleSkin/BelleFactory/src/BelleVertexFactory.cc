#include "Skins/BelleSkin/BelleFactory/interface/BelleVertexFactory.h"
#include "Skins/BelleSkin/BelleBeamSpot/interface/BelleBeamSpot.h"    
#include <rave/VacuumPropagator.h>
#include <rave/ConstantMagneticField.h>
#include "Environment/Configuration/interface/SimpleConfiguration.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

using namespace std;

rave::BelleVertexFactory::BelleVertexFactory () :
   rave::VertexFactory ( rave::ConstantMagneticField(0.,0.,1.5), rave::VacuumPropagator(), BelleBeamSpot(), "none" )
{
}

void rave::BelleVertexFactory::init()
{
  edm::LogWarning("BelleVertexFactory") << "no beamspot yet!";
}

#include "Vertigo/EventLoop/interface/VertexFactoryBuilder.h"

namespace {
  VertexFactoryBuilder < rave::BelleVertexFactory > t ( "belle", "Skin for the japanese Belle detector [KEK]" );
}

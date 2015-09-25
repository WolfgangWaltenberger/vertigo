#include "Skins/Belle2Skin/Belle2Factory/interface/Belle2VertexFactory.h"
#include "Skins/Belle2Skin/Belle2BeamSpot/interface/Belle2BeamSpot.h"    
#include <rave/VacuumPropagator.h>
#include <rave/ConstantMagneticField.h>
#include "Environment/Configuration/interface/SimpleConfiguration.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

using namespace std;

rave::Belle2VertexFactory::Belle2VertexFactory () :
   rave::VertexFactory ( rave::ConstantMagneticField(0.,0.,1.5), rave::VacuumPropagator(), Belle2BeamSpot(), "none" )
{
}

void rave::Belle2VertexFactory::init()
{
  edm::LogWarning("Belle2VertexFactory") << "no beamspot yet!";
}

#include "Vertigo/EventLoop/interface/VertexFactoryBuilder.h"

namespace {
  VertexFactoryBuilder < rave::Belle2VertexFactory > t ( "belle2", "Skin for the japanese Belle2 detector [KEK]" );
}

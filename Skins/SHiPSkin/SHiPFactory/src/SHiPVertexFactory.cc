#include "Skins/SHiPSkin/SHiPFactory/interface/SHiPVertexFactory.h"
#include "Skins/SHiPSkin/SHiPBeamSpot/interface/SHiPBeamSpot.h"    
#include "Skins/SHiPSkin/SHiPMagneticField/interface/SHiPMagneticField.h"
#include <rave/VacuumPropagator.h>
#include <rave/ConstantMagneticField.h>
#include "Environment/Configuration/interface/SimpleConfiguration.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

using namespace std;

rave::SHiPVertexFactory::SHiPVertexFactory () :
   rave::VertexFactory ( SHiPMagneticField(), rave::VacuumPropagator(), SHiPBeamSpot(), "none" )
{
}

void rave::SHiPVertexFactory::init()
{
  edm::LogWarning("SHiPVertexFactory") << "no beamspot yet!";
}

#include "Vertigo/EventLoop/interface/VertexFactoryBuilder.h"

namespace {
  VertexFactoryBuilder < rave::SHiPVertexFactory > t ( "ship", "Skin for the SHiP experiment at the SPS" );
}

#include "Skins/SIDSkin/SIDFactory/interface/SIDVertexFactory.h"
#include "Skins/SIDSkin/SIDMagneticField/interface/SIDMagneticField.h"
#include "Skins/SIDSkin/SIDPropagator/interface/SIDPropagator.h"
#include "Skins/SIDSkin/SIDBeamSpot/interface/SIDBeamSpot.h"
#include "Environment/Configuration/interface/SimpleConfiguration.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include <rave/VacuumPropagator.h>

using namespace std;

rave::SIDVertexFactory::SIDVertexFactory () :
   rave::VertexFactory ( SIDMagneticField(), rave::VacuumPropagator(), SIDBeamSpot(), "none" )
{
}

void rave::SIDVertexFactory::init()
{
  edm::LogWarning ("SIDVertexFactory") << "Dont forget to set inner barrel and inner forward region if necessary!";
  SimpleConfiguration::current()->modify ( "BarrelRegion:r", 38.6 );
  SimpleConfiguration::current()->modify ( "ForwardRegion:dz", 125.0 );
}

#include "Vertigo/EventLoop/interface/VertexFactoryBuilder.h"

namespace {
  VertexFactoryBuilder < rave::SIDVertexFactory > t ( "sid", "Skin for SID (SIlicon Detector) setup [ILC]" );
}

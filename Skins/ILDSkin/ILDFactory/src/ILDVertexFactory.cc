#include "Skins/ILDSkin/ILDFactory/interface/ILDVertexFactory.h"
#include "Skins/ILDSkin/ILDMagneticField/interface/ILDMagneticField.h"                                                
#include "Skins/ILDSkin/ILDPropagator/interface/ILDPropagator.h"                                                      
#include <rave/VacuumPropagator.h>
#include "Skins/ILDSkin/ILDBeamSpot/interface/ILDBeamSpot.h"    
#include "Environment/Configuration/interface/SimpleConfiguration.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

using namespace std;

rave::ILDVertexFactory::ILDVertexFactory () :
   rave::VertexFactory ( ILDMagneticField(), rave::VacuumPropagator(), ILDBeamSpot(), "none" )
{
}

void rave::ILDVertexFactory::init()
{
  edm::LogWarning ("ILDVertexFactory") << "Dont forget to set inner barrel and inner forward region if necessary!";
  SimpleConfiguration::current()->modify ( "BarrelRegion:r", 38.6 );                                          
  SimpleConfiguration::current()->modify ( "ForwardRegion:dz", 125.0 );
}

#include "Vertigo/EventLoop/interface/VertexFactoryBuilder.h"

namespace {
  VertexFactoryBuilder < rave::ILDVertexFactory > t ( "ild", "Skin for the ILD (International Large Detector) experiment [ILC]" );
}

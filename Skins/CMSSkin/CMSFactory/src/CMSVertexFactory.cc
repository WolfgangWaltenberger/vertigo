#include "Skins/CMSSkin/CMSFactory/interface/CMSVertexFactory.h"
#include "Skins/CMSSkin/CMSMagneticField/interface/CMSMagneticField.h"                                                
#include "Skins/CMSSkin/CMSPropagator/interface/CMSPropagator.h"                                                      
#include <rave/VacuumPropagator.h>
#include "Skins/CMSSkin/CMSBeamSpot/interface/CMSBeamSpot.h"    

rave::CMSVertexFactory::CMSVertexFactory () :
   rave::VertexFactory ( CMSMagneticField(), rave::VacuumPropagator(), CMSBeamSpot(), "none" )
{}

void rave::CMSVertexFactory::init() {}

#include "Vertigo/EventLoop/interface/VertexFactoryBuilder.h"

namespace {
  VertexFactoryBuilder < rave::CMSVertexFactory > t ( "cms", "Skin for CMS (Compact Muon Solenoid) [LHC]" );
}

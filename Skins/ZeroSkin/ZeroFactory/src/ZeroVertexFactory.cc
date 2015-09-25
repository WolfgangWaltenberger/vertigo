#include "Skins/ZeroSkin/ZeroFactory/interface/ZeroVertexFactory.h"
#include "Skins/ZeroSkin/ConstantMagneticField/interface/ConstantMagneticField.h"

using namespace std;

rave::ZeroVertexFactory::ZeroVertexFactory () :
   rave::VertexFactory ( ConstantMagneticField(0.,0.,0.), rave::VacuumPropagator(), "none" )
{}

void rave::ZeroVertexFactory::init() {}

#include "Vertigo/EventLoop/interface/VertexFactoryBuilder.h"

namespace {
  VertexFactoryBuilder < rave::ZeroVertexFactory > t ( "zero", "Zero Skin, no magnetic field, vacuum propagation" );
}

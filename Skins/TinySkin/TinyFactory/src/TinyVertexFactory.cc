#include "Skins/TinySkin/TinyFactory/interface/TinyVertexFactory.h"
#include "Skins/TinySkin/ConstantMagneticField/interface/ConstantMagneticField.h"

using namespace std;

rave::TinyVertexFactory::TinyVertexFactory () :
   rave::VertexFactory ( ConstantMagneticField(0.,0.,0.1), rave::VacuumPropagator(), "none" )
{}

void rave::TinyVertexFactory::init() {}

#include "Vertigo/EventLoop/interface/VertexFactoryBuilder.h"

namespace {
  VertexFactoryBuilder < rave::TinyVertexFactory > t ( "tiny", "Tiny Skin, constant magnetic field (0.01 Tesla), vacuum propagation" );
}

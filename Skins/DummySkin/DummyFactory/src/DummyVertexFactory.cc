#include "Skins/DummySkin/DummyFactory/interface/DummyVertexFactory.h"
#include "Skins/DummySkin/ConstantMagneticField/interface/ConstantMagneticField.h"

using namespace std;

rave::DummyVertexFactory::DummyVertexFactory () :
   rave::VertexFactory ( ConstantMagneticField(0.,0.,4.), rave::VacuumPropagator(), "none" )
{}

void rave::DummyVertexFactory::init() {}

#include "Vertigo/EventLoop/interface/VertexFactoryBuilder.h"

namespace {
  VertexFactoryBuilder < rave::DummyVertexFactory > t ( "dummy", "Dummy Skin, constant magnetic field (4 Tesla), vacuum propagation" );
}

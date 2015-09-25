#include "Skins/DummySkin/ConstantMagneticField/interface/ConstantMagneticField.h"

ConstantMagneticField::ConstantMagneticField( double v ) : theValue (v ) {}

rave::Vector3D ConstantMagneticField::inTesla ( const rave::Point3D & ) const
{
  return rave::Vector3D ( 0. , 0. , theValue );
}

ConstantMagneticField * ConstantMagneticField::copy() const
{
  return new ConstantMagneticField ( *this );
}

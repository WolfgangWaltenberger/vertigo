#include "Skins/SIDSkin/SIDMagneticField/interface/SIDMagneticField.h"

SIDMagneticField::SIDMagneticField( double v ) : theValue (v ) {}

rave::Vector3D SIDMagneticField::inTesla ( const rave::Point3D & ) const
{
  return rave::Vector3D ( 0. , 0. , theValue );
}

SIDMagneticField * SIDMagneticField::copy() const
{
  return new SIDMagneticField ( *this );
}

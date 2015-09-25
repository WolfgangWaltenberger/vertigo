#include "Skins/ILDSkin/ILDMagneticField/interface/ILDMagneticField.h"

ILDMagneticField::ILDMagneticField( double v ) : theValue (v ) {}

rave::Vector3D ILDMagneticField::inTesla ( const rave::Point3D & ) const
{
  return rave::Vector3D ( 0. , 0. , theValue );
}

ILDMagneticField * ILDMagneticField::copy() const
{
  return new ILDMagneticField ( *this );
}

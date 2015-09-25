#ifndef _VertigoConstantMagneticField_H_
#define _VertigoConstantMagneticField_H_

#include <rave/MagneticField.h>

/** \class ConstantMagneticField makes sure we can
 * access the "exterior" magnetic field from within rave.
 */

class ConstantMagneticField : public rave::MagneticField
{
  public:
    ConstantMagneticField ( double tesla=0.0 );
    rave::Vector3D inTesla ( const rave::Point3D & ) const;
    ConstantMagneticField * copy() const;

  private:
    double theValue;
};

#endif


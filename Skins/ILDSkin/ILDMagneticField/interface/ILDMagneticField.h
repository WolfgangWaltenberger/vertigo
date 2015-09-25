#ifndef _ILDMagneticField_H_
#define _ILDMagneticField_H_

#include <rave/MagneticField.h>

/** \class ILDField makes sure we can
 * access the "exterior" magnetic field from within rave.
 */

class ILDMagneticField : public rave::MagneticField
{
  public:
    ILDMagneticField ( double tesla=3.5 );
    rave::Vector3D inTesla ( const rave::Point3D & ) const;
    ILDMagneticField * copy() const;

  private:
    double theValue;
};

#endif


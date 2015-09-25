#ifndef _SIDMagneticField_H_
#define _SIDMagneticField_H_

#include <rave/MagneticField.h>

/** \class SIDField makes sure we can
 * access the "exterior" magnetic field from within rave.
 */

class SIDMagneticField : public rave::MagneticField
{
  public:
    SIDMagneticField ( double tesla=4.0 );
    rave::Vector3D inTesla ( const rave::Point3D & ) const;
    SIDMagneticField * copy() const;

  private:
    double theValue;
};

#endif


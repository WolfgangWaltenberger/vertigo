#ifndef _SHiPMagneticField_H_
#define _SHiPMagneticField_H_

#include <rave/MagneticField.h>

/** \class ILDField makes sure we can
 * access the "exterior" magnetic field from within rave.
 */

class SHiPMagneticField : public rave::MagneticField
{
  public:
    SHiPMagneticField ();
    rave::Vector3D inTesla ( const rave::Point3D & ) const;
    SHiPMagneticField * copy() const;
};

#endif


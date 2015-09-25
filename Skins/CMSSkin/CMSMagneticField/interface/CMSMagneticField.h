#ifndef _CMSMagneticField_H_
#define _CMSMagneticField_H_

#include <rave/MagneticField.h>
#include "../../cmssw/src/OAEParametrizedMagneticField.h"
#include "../../cmssw/src/PolyFit2DParametrizedMagneticField.h"

/** \class CMSMagneticField.
 * The cmssw ParametrizedMagneticField for vertigo.
 * This field describes the "full" cms Magnetic Field as accurately
 * as about 1 permille (Feb 2008)
 */

class OAEParametrizedMagneticField;

class CMSMagneticField : public rave::MagneticField
{
public:
    CMSMagneticField();
    CMSMagneticField( const CMSMagneticField & );
    ~CMSMagneticField();
    rave::Vector3D inTesla ( const rave::Point3D & ) const;
    CMSMagneticField * copy() const;

private:
    // mutable OAE85lParametrizedMagneticField theEngine;
    // mutable PolyFit2DParametrizedMagneticField * theFallbackEngine;
    OAEParametrizedMagneticField theEngine;
    PolyFit2DParametrizedMagneticField theFallbackEngine;
};

#endif

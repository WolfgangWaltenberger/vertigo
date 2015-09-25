#ifndef _CMSPropagator_H_
#define _CMSPropagator_H_

#include <rave/Propagator.h>

/** \class A CMSPropagator guarantees
 * that all necessary propagations can be
 * performed. It returns another rave::Track,
 * albeit at a different position.
 */

class CMSPropagator : public rave::Propagator
{
public:
    CMSPropagator * copy() const;
    rave::Track closestTo ( const rave::Track &,
      const rave::Point3D &, bool transverse ) const;
    std::pair < rave::Track, double > to ( const rave::Track &,
        const ravesurf::Plane & ) const;
    std::pair < rave::Track, double > to ( const rave::Track &,
        const ravesurf::Cylinder & ) const;
};

#endif

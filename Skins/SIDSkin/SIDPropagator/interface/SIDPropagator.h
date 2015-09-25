#ifndef _SIDPropagator_H_
#define _SIDPropagator_H_

#include <rave/Propagator.h>

/** \class A SIDPropagator guarantees
 * that all necessary propagations can be
 * performed. It returns another rave::Track,
 * albeit at a different position.
 */

class SIDPropagator : public rave::Propagator
{
public:
    SIDPropagator * copy() const;
    rave::Track closestTo ( const rave::Track &,
      const rave::Point3D &, bool transverse ) const;
    std::pair < rave::Track, double > to ( const rave::Track &,
        const ravesurf::Plane & ) const;
    std::pair < rave::Track, double > to ( const rave::Track &,
        const ravesurf::Cylinder & ) const;
};

#endif

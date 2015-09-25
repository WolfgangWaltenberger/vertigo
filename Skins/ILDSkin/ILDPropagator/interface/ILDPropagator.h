#ifndef _ILDPropagator_H_
#define _ILDPropagator_H_

#include <rave/Propagator.h>

/** \class A ILDPropagator guarantees
 * that all necessary propagations can be
 * performed. It returns another rave::Track,
 * albeit at a different position.
 */

class ILDPropagator : public rave::Propagator
{
public:
    ILDPropagator * copy() const;
    rave::Track closestTo ( const rave::Track &,
      const rave::Point3D &, bool transverse ) const;
    std::pair < rave::Track, double > to ( const rave::Track &,
        const ravesurf::Plane & ) const;
    std::pair < rave::Track, double > to ( const rave::Track &,
        const ravesurf::Cylinder & ) const;
};

#endif

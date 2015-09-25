#ifndef LDCPropagator_H
#define LDCPropagator_H

#include "rave/Propagator.h"

namespace marlinrave {

/** 
 * @brief Used for general propagation of track parameters.
 * @ingroup MarlinRave
 * @author Fabian Moser
 *
 * Guarantees that all necessary propagations can be performed no matter how
 * the detector looks like in detail. It returns another rave::Track, albeit 
 * at a different position taking into account the specific properties of the
 * detector along the path of propagation.
 */

class LDCPropagator : 
      public rave::Propagator
{
  public:
    /**
     * @return A duplicate of the current instance.
     */
    LDCPropagator* copy() const;
    
    /*
    void setPropagationDirection( const PropagationDirection ) const;
    rave::Track impactPointState ( const rave::Track & ) const;
    TrajectoryStateOnSurface propagate( const FreeTrajectoryState&, const Surface& ) const;
    TrajectoryStateOnSurface propagate( const TrajectoryStateOnSurface&, const Surface& ) const;    
    */
};

} // namespace marlinrave

#endif


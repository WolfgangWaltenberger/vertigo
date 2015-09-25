#include "LDCPropagator.h"

/*
#include "TrackingTools/TrajectoryState/interface/FreeTrajectoryState.h"
#include "TrackingTools/TrajectoryState/interface/TrajectoryStateOnSurface.h"
*/

namespace marlinrave {

LDCPropagator * LDCPropagator::copy() const
{
    return new LDCPropagator ( *this );
}

/*
rave::Track LDCPropagator::impactPointState ( const rave::Track & r ) const
{
  std::cout << "[LDCPropagator] argh!" << std::endl;
  rave::Track ret ( r );
  return ret;
}


void LDCPropagator::setPropagationDirection( const PropagationDirection ) const
{
    // FIXME : empty
}

TrajectoryStateOnSurface LDCPropagator::propagate( const FreeTrajectoryState& fts, const Surface& sf ) const
{
  // FIXME : empty
  std::cout << "[LDCPropagator] argh!" << std::endl;
    
    TrajectoryStateOnSurface res;
    return res;
}

TrajectoryStateOnSurface LDCPropagator::propagate( const TrajectoryStateOnSurface& tsos, 
                                                   const Surface& sf ) const
{
    // FIXME : empty
  std::cout << "[LDCPropagator] argh!" << std::endl;
    
    TrajectoryStateOnSurface res;
    return res;
}
*/

} // namespace marlinrave


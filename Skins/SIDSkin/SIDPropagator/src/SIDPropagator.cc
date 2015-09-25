#include "Skins/SIDSkin/SIDPropagator/interface/SIDPropagator.h"

#include <rave/impl/TrackingTools/TrajectoryState/interface/FreeTrajectoryState.h>
#include <rave/impl/TrackingTools/TrajectoryState/interface/TrajectoryStateOnSurface.h>

SIDPropagator * SIDPropagator::copy() const
{
    return new SIDPropagator ( *this );
}

rave::Track SIDPropagator::closestTo ( 
    const rave::Track & r,
    const rave::Point3D & p, bool transverse ) const
{
  std::cout << "[SIDPropagator] argh!" << std::endl;
  rave::Track ret ( r );
  return ret;
}

std::pair < rave::Track, double > SIDPropagator::to ( 
    const rave::Track & r,
    const ravesurf::Plane & s ) const
{
  std::cout << "[SIDPropagator] argh!" << std::endl;
  rave::Track ret ( r );
  return std::pair < rave::Track, double > ( ret, 0. );
}

std::pair < rave::Track, double > SIDPropagator::to ( 
    const rave::Track & r,
    const ravesurf::Cylinder & s ) const
{
  std::cout << "[SIDPropagator] argh!" << std::endl;
  rave::Track ret ( r );
  return std::pair < rave::Track, double > ( ret, 0. );
}


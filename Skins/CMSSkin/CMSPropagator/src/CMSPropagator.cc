#include "Skins/CMSSkin/CMSPropagator/interface/CMSPropagator.h"

#include <rave/impl/TrackingTools/TrajectoryState/interface/FreeTrajectoryState.h>
#include <rave/impl/TrackingTools/TrajectoryState/interface/TrajectoryStateOnSurface.h>

CMSPropagator * CMSPropagator::copy() const
{
    return new CMSPropagator ( *this );
}

rave::Track CMSPropagator::closestTo ( 
    const rave::Track & r,
    const rave::Point3D & s, bool transverse ) const
{
  std::cout << "[CMSPropagator] argh!" << std::endl;
  rave::Track ret ( r );
  return ret;
}

std::pair < rave::Track, double > CMSPropagator::to ( 
    const rave::Track & r,
    const ravesurf::Plane & s ) const
{
  std::cout << "[CMSPropagator] argh!" << std::endl;
  rave::Track ret ( r );
  return std::pair < rave::Track, double >  ( ret, 0.);
}

std::pair < rave::Track, double > CMSPropagator::to ( 
    const rave::Track & r,
    const ravesurf::Cylinder & s ) const
{
  std::cout << "[CMSPropagator] argh!" << std::endl;
  rave::Track ret ( r );
  return std::pair < rave::Track, double >  ( ret, 0.);
}


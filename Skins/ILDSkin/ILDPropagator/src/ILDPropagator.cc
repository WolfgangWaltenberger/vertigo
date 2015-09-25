#include "Skins/ILDSkin/ILDPropagator/interface/ILDPropagator.h"

#include <rave/impl/TrackingTools/TrajectoryState/interface/FreeTrajectoryState.h>
#include <rave/impl/TrackingTools/TrajectoryState/interface/TrajectoryStateOnSurface.h>

using namespace std;

ILDPropagator * ILDPropagator::copy() const
{
    return new ILDPropagator ( *this );
}

rave::Track ILDPropagator::closestTo ( 
    const rave::Track & r,
    const rave::Point3D & s, bool transverse ) const
{
  std::cout << "[ILDPropagator] argh!" << std::endl;
  rave::Track ret ( r );
  return ret;
}

pair < rave::Track, double > ILDPropagator::to ( 
    const rave::Track & r,
    const ravesurf::Plane & s ) const
{
  std::cout << "[ILDPropagator] argh!" << std::endl;
  rave::Track ret ( r );
  return pair < rave::Track, double > ( ret, 0. );
}

pair < rave::Track, double > ILDPropagator::to ( 
    const rave::Track & r,
    const ravesurf::Cylinder & s ) const
{
  std::cout << "[ILDPropagator] argh!" << std::endl;
  rave::Track ret ( r );
  return pair < rave::Track, double > ( ret, 0. );
}


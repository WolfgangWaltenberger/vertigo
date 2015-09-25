#include "Skins/SIDSkin/SIDBeamSpot/interface/SIDBeamSpot.h"
#include <rave/Units.h>

using namespace raveunits;                                                                                     

namespace {
  // the "ideal" beamspot
  static const double sigma_z=100. * micrometer;
  // static const double sigma_y=130. * nanometer;
  // static const double sigma_x=  2. * nanometer;

  // the more realistic beamspot, accounting for statistical effects
  // 190 is about 130 * sqrt(2)
  static const double sigma_y=190. * nanometer;
  static const double sigma_x=130. * nanometer;
}

SIDBeamSpot::SIDBeamSpot() :
  rave::Ellipsoid3D ( rave::Point3D(0.,0.,0.),
      rave::Covariance3D ( sigma_x*sigma_x,0,0,sigma_y*sigma_y,0,sigma_z*sigma_z ) )
{}

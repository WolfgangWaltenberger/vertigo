#include "Skins/BelleSkin/BelleBeamSpot/interface/BelleBeamSpot.h"

namespace {
  static const double sigma_x=0.0000002; // 2 nanometer
  static const double sigma_y=0.0000130; // 130 nanometer
  static const double sigma_z=0.01; // 100 microns
  /*
  static const double sigma_x=0.0001; // 1 micron
  static const double sigma_y=0.0001;
  static const double sigma_z=0.01; // 100 microns
  */
}

BelleBeamSpot::BelleBeamSpot() :
  rave::Ellipsoid3D ( rave::Point3D(0.,0.,0.),
      rave::Covariance3D ( sigma_x*sigma_x,0,0,sigma_y*sigma_y,0,sigma_z*sigma_z ) )
{}

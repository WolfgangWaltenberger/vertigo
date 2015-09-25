#include "Skins/SHiPSkin/SHiPBeamSpot/interface/SHiPBeamSpot.h"

namespace {
  static const double sigma_x= 6.18e-4; // 2 nanometer
  static const double sigma_y= 5.9e-6 ; // 130 nanometer
  static const double sigma_z= 154e-4; // 154 microns
  static const double sigma_xz = tan(-1.11e-2)*sigma_x * sigma_x * sigma_y * sigma_y;
}

SHiPBeamSpot::SHiPBeamSpot() :
  rave::Ellipsoid3D ( rave::Point3D(0.,0.,0.),
      rave::Covariance3D ( sigma_x*sigma_x,0,sigma_xz,sigma_y*sigma_y,0,sigma_z*sigma_z ) ) {}

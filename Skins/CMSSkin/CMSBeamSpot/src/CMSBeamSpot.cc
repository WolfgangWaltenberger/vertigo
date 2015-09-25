#include "Skins/CMSSkin/CMSBeamSpot/interface/CMSBeamSpot.h"
#include <rave/Units.h>

namespace {
  using namespace raveunits;
  // static const double width= 15. * micrometer;
  static const float width= 32. * micrometer; // about RelVal value 200pre2?!?
  // static const double length=7.55 * cm; // high lumi?
  static const float length=5.3 * cm ; // RelVal value = low lumi?
  // static const double length=6.7 * cm ; // about RelVal value 200pre2?!?
}

CMSBeamSpot::CMSBeamSpot() :
  rave::Ellipsoid3D ( rave::Point3D(0.,0.,0.),
      rave::Covariance3D ( width*width,0.,0.,width*width,0.,length*length ) )
{}

#include "Skins/CMSSkin/CMSMagneticField/interface/CMSMagneticField.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Utilities/interface/Exception.h"
#include <rave/impl/RaveBase/Converters/interface/RaveStreamers.h>
#include <iostream>

CMSMagneticField::CMSMagneticField()  : theEngine ( OAEParametrizedMagneticField ( "3_8T" ) )
{}

CMSMagneticField::~CMSMagneticField()
{
  // we do not delete!
}

CMSMagneticField::CMSMagneticField ( const CMSMagneticField & o )
{
  // we do not clone
}

rave::Vector3D CMSMagneticField::inTesla ( const rave::Point3D & rp ) const
{
  using namespace std;
  GlobalPoint p ( rp.x(), rp.y(), rp.z() );
  if ( theEngine.isDefined ( p ) )
  {
    GlobalVector r = theEngine.inTesla ( p );
    return rave::Vector3D ( r.x(), r.y(), r.z() );
  } else {
    GlobalVector r = theFallbackEngine.inTesla ( p );
    return rave::Vector3D ( r.x(), r.y(), r.z() );
  }
  /*
  try {
    GlobalVector r = theEngine.inTesla ( p );
    return rave::Vector3D ( r.x(), r.y(), r.z() );
  } catch ( cms::Exception & c ) {
    // we need a field outside the |r|<300cm, |z|<120cm
    // double val[3];
    GlobalVector val = theFallbackEngine.inTesla ( p );
    // rave::Vector3D ret ( 1000.*val[0] , 1000.*val[1] , 1000.*val[2] );
    rave::Vector3D ret ( val.x(), val.y() , val.z() );
    // edm::LogWarning("CMSMagneticField") << "mag field at " << rp << " outside boundaries. Will return " << ret;
    return ret;
  }*/
}

CMSMagneticField * CMSMagneticField::copy() const
{
    return new CMSMagneticField ( *this );
}

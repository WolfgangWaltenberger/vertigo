#include "Skins/SHiPSkin/SHiPMagneticField/interface/SHiPMagneticField.h"
#include <iostream>

SHiPMagneticField::SHiPMagneticField() {}

rave::Vector3D SHiPMagneticField::inTesla ( const rave::Point3D & p ) const
{
  using namespace std;
  double kilogauss = 1.;
  double tesla     = 10*kilogauss;
  double cm  = 1;       // cm
  double m   = 100*cm;  //  m
  // double mm  = 0.1*cm;  //  mm
  double fMiddle=3068.0;
  double fBtube=500.0;
  double fPeak=1.4361;
  double x=p.x();
  double y=p.y();
  double z=p.z();
  double zlocal=fabs((z-fMiddle)/100.);
  double Bz = 0.;
  double By = 0.;
  double Bx = 0.;

  //field in box 20 cm larger than inner tube.
  // cout << "[SHiPMagneticField] at " << x << "  zlocal=" << zlocal  << endl;

  if ( (fabs(x)<2.7*m) && (fabs(y)<fBtube+0.2*m) )  {
    if (zlocal<3.8) {
      Bx=0.14361*exp( -0.5 * pow((zlocal-0.45479E-01)/2.5046,2.));
    }else if (zlocal<11.9) {
      Bx=0.19532-0.61512E-01*zlocal+0.68447E-02*pow(zlocal,2.)-0.25672E-03*pow(zlocal,3.);
    }
    Bx=((fPeak/tesla)/0.14361)*Bx*tesla;
  }
  return rave::Vector3D ( Bx , By, Bz );
}

SHiPMagneticField * SHiPMagneticField::copy() const
{
  return new SHiPMagneticField ( *this );
}

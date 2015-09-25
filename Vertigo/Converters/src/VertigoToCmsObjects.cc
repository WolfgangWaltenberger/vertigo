#include "Vertigo/Converters/interface/VertigoToCmsObjects.h"
#include <rave/impl/DataFormats/GeometryVector/interface/GlobalPoint.h>
#include <rave/impl/DataFormats/GeometryVector/interface/GlobalVector.h>
#include <rave/impl/RaveTools/Converters/interface/MagneticFieldSingleton.h>

GlobalTrajectoryParameters VertigoToCmsObjects::convert ( const vertigo::SimTrack & s ) const
{
  GlobalPoint pos ( s.state().x(), s.state().y(), s.state().z() );
  GlobalVector mom ( s.state().px(), s.state().py(), s.state().pz() );
  return GlobalTrajectoryParameters ( pos , mom, s.charge(), MagneticFieldSingleton::Instance() ); 
}

GlobalTrajectoryParameters VertigoToCmsObjects::convert(const vertigo::SimParticle & s) const
{
  GlobalPoint pos ( s.state().x(), s.state().y(), s.state().z() );
  GlobalVector mom ( s.state().px(), s.state().py(), s.state().pz() );
  return GlobalTrajectoryParameters ( pos , mom, s.charge(), MagneticFieldSingleton::Instance() );
}

FreeTrajectoryState VertigoToCmsObjects::freeState ( const vertigo::SimTrack & s ) const
{
  return FreeTrajectoryState ( convert ( s ) );
}

AlgebraicSymMatrix VertigoToCmsObjects::convert ( const rave::Covariance3D & c ) const
{
  AlgebraicSymMatrix ret(3,0);
  ret(1,1)=c.dxx();
  ret(2,2)=c.dyy();
  ret(3,3)=c.dzz();
  ret(2,1)=c.dxy();
  ret(3,1)=c.dxz();
  ret(3,2)=c.dyz();
  return ret;
}

AlgebraicVector VertigoToCmsObjects::convert ( const rave::Vector3D & c ) const
{
  AlgebraicVector ret(3);
  ret[0]=c.x();
  ret[1]=c.y();
  ret[2]=c.z();
  return ret;
}

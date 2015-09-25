#include "VertexGunPropagator.h"
#include "Environment/Configuration/interface/SimpleConfigurable.h"

#include <rave/impl/DataFormats/GeometrySurface/interface/Cylinder.h>
#include <rave/impl/DataFormats/GeometrySurface/interface/Plane.h>
#include <rave/impl/TrackingTools/GeomPropagators/interface/HelixBarrelCylinderCrossing.h>
#include <rave/impl/TrackingTools/GeomPropagators/interface/HelixForwardPlaneCrossing.h>
#include <rave/impl/RaveTools/Converters/interface/MagneticFieldSingleton.h>
// #include <rave/impl/TrackingTools/PatternTools/interface/TransverseImpactPointExtrapolator.h>
#include <rave/impl/TrackingTools/TrajectoryState/interface/FreeTrajectoryState.h>
#include <rave/impl/TrackingTools/TrajectoryState/interface/TrajectoryStateOnSurface.h>

using namespace std;

namespace {
  void failed()
  {
    static bool once=true;
    if ( once )
    {
      cout << "[VertexGunPropagator] propagation failed. revert to original parameters."
           << endl 
           << "                      will warn only once."
           << endl;
      once=false;
    }
  }
}

GlobalTrajectoryParameters VertexGunPropagator::propagate ( const GlobalTrajectoryParameters & r )
{
  bool propagate = SimpleConfigurable<bool>(false,"VertexGun:Propagate").value();
  if ( !propagate )
  {
    return r;
    // propagate to impact point!
    // cout << "[VertexGunPropagator] propagating to impact point!" << endl;
    /*
    TransverseImpactPointExtrapolator ex ( MagneticFieldSingleton::Instance() );
    FreeTrajectoryState prior ( r );
    TrajectoryStateOnSurface res = ex.extrapolate ( prior, GlobalPoint(0.,0.,0.) );
    return res.globalParameters();
    */
  }
  // Cylinder * c = new Cylinder ( GlobalPoint ( 0., 0., 0. ), TkRotation<float>(), 4.3 );
  float brr=SimpleConfigurable<float>(4.3,"BarrelRegion:r").value();
  Cylinder c ( GlobalPoint ( 0., 0., 0. ), TkRotation<float>(), brr );
  HelixBarrelCylinderCrossing cross ( r.position(), r.momentum(), r.transverseCurvature(), anyDirection, c ); 
  GlobalTrajectoryParameters ngtp=GlobalTrajectoryParameters ( cross.position(), cross.direction(), 
        r.charge(), MagneticFieldSingleton::Instance() );

  if ( ngtp.momentum().mag() < 1.e-5 && r.momentum().mag() > 0.5 )
  {
    float frz=SimpleConfigurable<float>(100.,"ForwardRegion:dz").value();
    Plane p ( GlobalPoint ( 0., 0., copysign ( frz, ngtp.momentum().z() ) ),
              TkRotation<float>() );
    HelixPlaneCrossing::PositionType pos ( r.position() );
    HelixPlaneCrossing::DirectionType dir ( r.momentum () );
    HelixForwardPlaneCrossing cross ( pos, dir, r.transverseCurvature(), anyDirection );
    std::pair < bool, double > l = cross.pathLength ( p );
    if ( l.first )
    {
      GlobalPoint pt ( cross.position( l.second ) );
      GlobalVector dir ( cross.direction( l.second ) );
      ngtp=GlobalTrajectoryParameters ( pt, dir, r.charge(), MagneticFieldSingleton::Instance() );
    } else {
      failed();
      ngtp=r;
    };

    if ( ngtp.momentum().mag() < 1.e-5 )
    {
      failed();
      // ok, propagation failed, evidently (barrel region?)
      // in this case we just dont propagate.
      ngtp=r;
    };
  }
  /* 
  cout << "[dbg] r mag=" << r.momentum().mag() << endl;
  cout << "[dbg] dir=" << cross.direction() << "  mag=" << cross.direction().mag() << endl;
  cout << "[VertexGunPropagator] new=" << ngtp.position() << ", "
       << ngtp.momentum() << ", old=" << r.position()
       << endl;
       */
  return ngtp;
  // return r;
}

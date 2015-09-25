#include "Vertigo/TrackAssociation/interface/TrackDistanceByChiSquared.h"
#include "Environment/Configuration/interface/SimpleConfigurable.h"
#include <rave/impl/TrackingTools/TrajectoryParametrization/interface/LocalTrajectoryParameters.h>
#include <rave/impl/DataFormats/CLHEP/interface/AlgebraicObjects.h>
#include <rave/impl/TrackingTools/TrajectoryState/interface/TrajectoryStateOnSurface.h>
#include <rave/impl/DataFormats/GeometryVector/interface/GlobalPoint.h>
#include <rave/impl/TrackingTools/TransientTrack/interface/TransientTrack.h>
#include <rave/impl/RaveBase/Converters/interface/RaveStreamers.h>
// #include <rave/impl/TrackingTools/GeomPropagators/interface/AnalyticalPropagator.h>
#include <rave/impl/DataFormats/GeometryCommonDetAlgo/interface/PerpendicularBoundPlaneBuilder.h>
#include <rave/impl/RaveBase/Converters/interface/MagneticFieldWrapper.h>
#include <rave/impl/RaveBase/Converters/interface/RaveToCmsObjects.h>
#include "Vertigo/Converters/interface/VertigoToCmsObjects.h"
#include "Environment/Utilities/interface/VertigoStreamers.h"

using namespace std;

float TrackDistanceByChiSquared::tsosDistance (
                     const TrajectoryStateOnSurface & rec,
                     const TrajectoryStateOnSurface & sim ) const
{
  const LocalTrajectoryParameters & sim_loc = sim.localParameters();
  const LocalTrajectoryParameters & rec_loc = rec.localParameters();

  AlgebraicVector5 diff = sim_loc.vector() - rec_loc.vector();

  int fail = 0;
  AlgebraicSymMatrix55 weight = rec.localError().matrix().Inverse ( fail );
  /*
  cout << "[TrackDistanceByChiSquared] global err=" << rec.hasError() << endl;
  cout << "[TrackDistanceByChiSquared] global err=" << rec.cartesianError().matrix() << endl;
  cout << "[TrackDistanceByChiSquared] local err=" << rec.localError().matrix() << endl;
  */
  if ( fail )
  {
    cout << "[TrackDistanceByChiSquared] Matrix inversion failed. " << endl;
  };
  // float chi2 = weight.similarity ( diff );
  float chi2 = ROOT::Math::Similarity ( weight, diff );
  // cout << "[TrackDistanceByChiSquared] chi2=" << chi2 << " cut=" << theCut << endl;
  // exit(0);
  return chi2 < theCut ? chi2 : -1.;
}

float TrackDistanceByChiSquared::cut() const
{
  return theCut;
}

TrackDistanceByChiSquared::TrackDistanceByChiSquared() :
    theCut (  SimpleConfigurable<float> ( 25000.,
              "TrackDistanceByChiSquared:Max").value() )
{}

TrackDistanceByChiSquared::TrackDistanceByChiSquared( float cut ) :
    theCut ( cut ) {}

float TrackDistanceByChiSquared::distance ( const vertigo::SimTrack & sim,
                                            const rave::Track & rec ) const
{
  float p_sim = sim.state().momentum().mag();
  float p_rec = rec.state().momentum().mag();
  if ( fabs ( p_sim - p_rec ) / p_sim > 0.2 ) return -1;
  RaveToCmsObjects converter;
  reco::TransientTrack trec ( converter.tTrack ( rec ) );
  FreeTrajectoryState gsim = VertigoToCmsObjects().freeState ( sim );
  BoundPlane *p = PerpendicularBoundPlaneBuilder () ( gsim.position(), gsim.momentum() );
  TrajectoryStateOnSurface rec_tsos  ( trec.initialFreeState(), *p );
  TrajectoryStateOnSurface sim_tsos ( gsim, *p );

  /*
  // first we try compatibility at the impactPointState.
  // TrajectoryStateOnSurface rec_tsos = trec.impactPointState();
  TrajectoryStateOnSurface rec_tsos = trec.innermostMeasurementState();
  
  AnalyticalPropagator prop ( MagneticFieldWrapper::Instance() );
  TrajectoryStateOnSurface sim_tsos = prop.propagate ( gsim, rec_tsos.surface() );

  GlobalTrajectoryParameters gsim = VertigoToCmsObjects().convert ( sim );
  TrajectoryStateOnSurface sim_tsos ( gsim, rec_tsos.surface() );
  */

  if ( !sim_tsos.isValid() )
  {
    cout << "[TrackDistanceByChiSquared] tsos not valid!" << endl;
    return -1.;
  }

  /*
  cout << "[TrackDistanceByChiSquared] debug:" << endl
       << " sim: " << sim << endl 
       << " rec: " << rec << endl << endl
       << "gsim: " << gsim << endl 
       << "trec: " << trec << endl << endl
       << "lsim: " << sim_tsos.localParameters() << endl 
       << "lrec: " << rec_tsos.localParameters() << endl << endl
       << "Tsim: " << sim_tsos << endl << endl
       << "Trec: " << rec_tsos << endl << endl
   ;
   */

  if ( rec_tsos.isValid() && sim_tsos.isValid() )
  {
    double ret= tsosDistance ( rec_tsos, sim_tsos );
    return ret;
  }
  cout << "[TrackDistanceByChiSquared] invalid state! rec: " << rec_tsos.isValid() 
       << "  sim: " << sim_tsos.isValid() << endl;
  // return non-compatible
  return -1.;
}

TrackDistanceByChiSquared * TrackDistanceByChiSquared::clone() const
{
  return new TrackDistanceByChiSquared( *this );
}

#include "Vertigo/TrackAssociation/interface/TrackDistanceByPulls.h"
#include "Vertigo/Converters/interface/VertigoToCmsObjects.h"

#include <rave/impl/TrackingTools/TransientTrack/interface/TransientTrack.h>
#include <rave/impl/TrackingTools/TrajectoryState/interface/TrajectoryStateOnSurface.h>
#include <rave/impl/RaveBase/Converters/interface/RaveToCmsObjects.h>

float 
TrackDistanceByPulls::distance(const vertigo::SimTrack& sim, const rave::Track& rec) const {
  RaveToCmsObjects converter;
  reco::TransientTrack trec ( converter.tTrack ( rec ) );
  TrajectoryStateOnSurface recip =  trec.impactPointState();
  GlobalTrajectoryParameters gsim = VertigoToCmsObjects().convert ( sim );
  TrajectoryStateOnSurface simip ( gsim, recip.surface() );
  const double p_sim  = sim.state().momentum().mag();
  const double ph_sim = gsim.momentum().phi();
  const double th_sim = gsim.momentum().theta();
  const double x_sim  = simip.localPosition().x();
  const double y_sim  = simip.localPosition().y();
  const double p_rec  = recip.globalMomentum().mag();
  const double ph_rec = recip.globalMomentum().phi();
  const double th_rec = recip.globalMomentum().theta();
  const double x_rec  = recip.localPosition().x();
  const double y_rec  = recip.localPosition().y();

  /* These factors can be eventually used to make the pulls normal distributed */
  double corr_p   = 1.0;
  double corr_ph  = 1.0;
  double corr_th  = 1.0;
  double corr_x   = 1.0;
  double corr_y   = 1.0;
  double corr_dp  = 0.0;
  double corr_dph = 0.0;
  double corr_dth = 0.0;
  double corr_dx  = 0.0;
  double corr_dy  = 0.0;
  
  const double err_p  = recip.curvilinearError().matrix()(1,1) * corr_p * corr_p;
  const double err_th = recip.curvilinearError().matrix()(2,2) * corr_th * corr_th;
  const double err_ph = recip.curvilinearError().matrix()(3,3) * corr_ph * corr_ph;
  const double err_x  = recip.localError().matrix()(4,4) * corr_x  * corr_x;
  const double err_y  = recip.localError().matrix()(5,5) * corr_y * corr_y;
  double pull_p = (1/p_rec - 1/p_sim)/sqrt(err_p)-corr_dp;
  double pull_ph = (ph_rec-ph_sim)/sqrt(err_ph)-corr_dph;
  double pull_th = (th_rec-th_sim)/sqrt(err_th)-corr_dth;
  double pull_x  = (x_rec-x_sim)/sqrt(err_x)-corr_dx;
  double pull_z  = (y_rec-y_sim)/sqrt(err_y)-corr_dy;
  if(fabs(pull_p)>max_pull_p) return -1.;
  if(fabs(pull_ph)>max_pull_ph) return -1.;
  if(fabs(pull_th)>max_pull_th) return -1.;
  if(fabs(pull_x)>max_pull_x) return -1.;
  if(fabs(pull_z)>max_pull_z) return -1.;
  float dist = sqrt(pull_p*pull_p+pull_ph*pull_ph+pull_th*pull_th+pull_x*pull_x+pull_z*pull_z);
  return dist < max_distance ? dist : -1. ; 
}

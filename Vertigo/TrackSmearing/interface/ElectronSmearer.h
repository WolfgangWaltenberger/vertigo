#ifndef _ElectronSmearer_H_
#define _ElectronSmearer_H_

#include "Vertigo/TrackSmearing/interface/RecTrackSmearer.h"
#include <rave/impl/TrackingTools/TrajectoryParametrization/interface/GlobalTrajectoryParameters.h>
#include <rave/Track.h>
#include <boost/random.hpp>

/** ElectronSmearer.
 *  Creates a Gaussian Smeared RecTrack from a GlobalTrajectoryParameter.
 *  The track also looses some of its energy, just like a bresmsstrahlungs-
 *  photon emitting electron. FIXME this is a rough, first implementation.
 */

class ElectronSmearer : public RecTrackSmearer {
public:
  /** \param energy_loss FIXME what parameter do I want here?
   *  \param position_sigma Smearing sigma on the xy-plane.
   *  \param direction_sigma Smearing sigma in the angle  (in rad).
   *  \param momentum_sigm a Smearing sigma in the total track momentum,
   *  given as a fraction to the total momentum.
   *  We make the assumption that sigma(1/p) = sigma(p)/p^2 is roughly
   *  constant.
   *  This constructor is for the case that all errors are to be reported
   *  correctly. There will be no correlations with this constructor.
   */

  ElectronSmearer(
      float energy_loss = .3,
      float position_sigma     = .0038,
      float direction_sigma    = .0006,
      float inv_momentum_sigma = .0021 );

  /**
   * This reconstructor is just as above, it only allows to report wrong
   * errors. The defaults try to simulate an "average" monte carlo track.
   * \param corr will introduce correlations.
   */

  ElectronSmearer(
      float energy_loss,
      float position_sigma,
      float direction_sigma,
      float inv_momentum_sigma,
      float position_sigma_err_matrix,
      float direction_sigma_err_matrix,
      float inv_momentum_sigma_err_matrix );

  /**
   *  This method does the smearing.
   */
  virtual rave::Track recTrack ( const GlobalTrajectoryParameters & gp ) const;

  virtual ElectronSmearer * clone () const;
  void printDescription() const;

  /**
   *  This method gives you the "mean" of the smearing"
   */
  GlobalTrajectoryParameters delta( const GlobalTrajectoryParameters & gp ) const;

private:
  /**
   *  produce the random displacements in dx,dy,dxdir,dydir,invp.
   */
  AlgebraicVector createRandoms ( const GlobalTrajectoryParameters & ) const;

private:
  typedef boost::mt19937 base_generator_type;
  //typedef boost::minstd_rand base_generator_type;
  typedef boost::variate_generator< base_generator_type&, boost::normal_distribution<float> > normal_generator_type;
  float theEnergyLoss;
  boost::normal_distribution<float> thePosDist;
  boost::normal_distribution<float> theDirDist;
  boost::normal_distribution<float> theInvPDist;
  mutable base_generator_type theGenerator;

  const float thePosSigmaErrMatrix; // position sigma in error matrix
  const float theDirSigmaErrMatrix; // direction sigma in error matrix
  const float theInvPSigmaErrMatrix; // p^-1 sigma in error matrix
};

#endif // _ElectronSmearer_H_

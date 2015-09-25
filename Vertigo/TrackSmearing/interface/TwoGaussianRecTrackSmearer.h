#ifndef _TwoGaussianRecTrackSmearer_H_
#define _TwoGaussianRecTrackSmearer_H_

#include "Vertigo/TrackSmearing/interface/RecTrackSmearer.h"
#include <rave/Track.h>

/*! TwoGaussianRecTrackSmearer.
 *  Creates a smeared RecTrack, smearing distribution is
 *  the superposition of two Gaussians; we interpret one Gaussian as the
 *  "inlier" or "signal" distribution; the second Gaussian would then be
 *  the "outlier" or the "background". Non-gaussian tails are
 *  simply simulated with two such Gaussians, as well.
 */

class TwoGaussianRecTrackSmearer : public RecTrackSmearer {
public:
  /** \param inlier First distribution.
   *  \param outlier Second distribution.
   *  \param fraction_outliers Ratio outlier / total. Value of zero would mean
   *  no outliers. Value of one means no inliers.
   *  \param fixed_sequence. Should we really randomly choose which distribution
   *  we use, or shall we have a fixed sequence?  Fixed sequence would mean
   *  that every 'ratio'-th track has smearing parameters from the first
   *  distribution.
   */
  TwoGaussianRecTrackSmearer(
      const RecTrackSmearer & inlier,
      const RecTrackSmearer & outlier,
      float fraction_outliers = 0.0, bool fixed_sequence = false );

  /**
   *  This constructor instantiates two GaussianRecTrackSmearer.  The "inlier"
   *  smearer is instantiated with the first triple of the values below, and
   *  with the right values in the covariance matrix.
   *  The "outlier" smearer smears with the second triplet, but in the
   *  covariance matrix the numbers of the inliers are reported.
   */
  TwoGaussianRecTrackSmearer(
      float sigma_pos_inl, float sigma_dir_inl, float sigma_total_mom_inl,
      float sigma_pos_outl, float sigma_dir_outl, float sigma_total_mom_outl,
      float fraction_outliers, bool fixed_sequence = false );

  TwoGaussianRecTrackSmearer( const TwoGaussianRecTrackSmearer & );

  ~TwoGaussianRecTrackSmearer();

  /**
   *  This method creates the RecTrack, smeared according
   *  to the sepcified criteria.
   */
  virtual rave::Track recTrack ( const GlobalTrajectoryParameters & gp ) const;

  virtual TwoGaussianRecTrackSmearer * clone() const;

  void printDescription() const;

private:
  bool theFixed;
  float theRatio;
  mutable int theCtr;
  RecTrackSmearer * theFirstSmearer;
  RecTrackSmearer * theSecondSmearer;
};

#endif // _TwoGaussianRecTrackSmearer_H_

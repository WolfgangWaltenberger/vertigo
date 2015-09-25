#ifndef Tracker_TrackDistanceByChiSquared_H
#define Tracker_TrackDistanceByChiSquared_H

#include "Vertigo/TrackAssociation/interface/TrackDistance.h"
#include "Environment/MCObjects/interface/SimTrack.h"
#include <rave/Track.h>

class TrajectoryStateOnSurface;

/**
 *  TrackDistanceByChiSquared: compares track parameters - position and
 *  momentum at the SimVertex - weighted with the errors on those parameters.
 *  If "chi2" is above a certain threshold we return -1.
 */

class TrackDistanceByChiSquared : public TrackDistance
{
public:
  /**
   *  Default constructor, compatibility cutoff defined
   *  in .orcarc
   *  TrackDistanceByChiSquared:Max
   */
  TrackDistanceByChiSquared();

  /** cutoff defines when we classify "incompatible"
   *  (distance=-1.)
   */
  TrackDistanceByChiSquared( float cut );

  float cut() const;

  float tsosDistance ( const TrajectoryStateOnSurface & rec,
                       const TrajectoryStateOnSurface & sim ) const;

  float distance ( const vertigo::SimTrack &, const rave::Track & ) const;
  TrackDistanceByChiSquared * clone() const;
private:
  float theCut;
};

#endif

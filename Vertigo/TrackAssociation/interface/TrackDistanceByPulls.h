#ifndef Tracker_TrackDistanceByPulls_H
#define Tracker_TrackDistanceByPulls_H

#include "Vertigo/TrackAssociation/interface/TrackDistance.h"
#include "Environment/MCObjects/interface/SimTrack.h"
#include "Environment/MCObjects/interface/SimVertex.h"
#include <rave/Track.h>

/** \class TrackDistanceByPulls
 *  Defines distance metric between vertigo::SimTrack and rave::Track as the 
 *  5-dimensional distance in helix parameter space, scaled by
 *  the uncertainty on the RecTrack parameters
 */

class TrackDistanceByPulls: public TrackDistance {
public:

  /** The default constructor sets the cuts to the TDR values 
   */
  TrackDistanceByPulls() :
  max_pull_p(10.), max_pull_ph(10.), max_pull_th(10.), 
  max_pull_x(10.), max_pull_z(10.), max_distance(22.36) {}

  /** With this constructor it is possible to specify
   * the Max acceptable value for each pull
   * separately, and the cut on the maximum distance between two tracks 
   */
  TrackDistanceByPulls (float mpt, float mph, float mth, 
			float mx, float mz, float mdist):
   max_pull_p(mpt), max_pull_ph(mph), max_pull_th(mth),
   max_pull_x(mx), max_pull_z(mz), max_distance(mdist) {}

  float distance(const vertigo::SimTrack& sim, const rave::Track& rec) const;

  TrackDistanceByPulls * clone() const {
    return new TrackDistanceByPulls(*this);
  }

private:
    float max_pull_p;
    float max_pull_ph;
    float max_pull_th;
    float max_pull_x;
    float max_pull_z;
    float max_distance;
};


#endif



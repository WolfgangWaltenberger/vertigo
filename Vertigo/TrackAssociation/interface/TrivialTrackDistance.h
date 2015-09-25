#ifndef Tracker_TrivialTrackDistance_H
#define Tracker_TrivialTrackDistance_H

#include "Vertigo/TrackAssociation/interface/TrackDistance.h"
#include "Environment/MCObjects/interface/SimTrack.h"
#include <rave/Track.h>

/** Trivial Track Distance. Only compares the momenta, and requires
 *  2% agreement.
 *  Not useful, except maybe for trivial tests.
 */

class TrivialTrackDistance: public TrackDistance {
public:
  TrivialTrackDistance() {}
  float distance(const vertigo::SimTrack& sim, const rave::Track& rec) const;

  TrivialTrackDistance * clone() const {
    return new TrivialTrackDistance(*this);
  }
};

#endif

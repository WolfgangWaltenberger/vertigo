#ifndef Tracker_TrackDistance_H
#define Tracker_TrackDistance_H

#include "Environment/MCObjects/interface/SimTrack.h"
#include <rave/Track.h>

//class vertigo::SimTrack;
//class rave::Track;

/** 
 *  Abstract interface for distance metric between vertigo::SimTrack and rave::Track
 *  WARNING: the concrete implementations (TrivialTrackDistance and
 *  TrackDistanceByChiSquared) dont actually implement a distance.
 *  They implement a compatibility criterion that equals -1 
 *  if tracks are not compatible ("associatable")
 */
class TrackDistance {
public:

  virtual ~TrackDistance() {}

  virtual float distance(const vertigo::SimTrack&,const rave::Track&) const = 0;

  virtual TrackDistance * clone() const = 0;

};
#endif

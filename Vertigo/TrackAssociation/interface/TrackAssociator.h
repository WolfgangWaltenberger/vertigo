#ifndef TrackAssociator_H
#define TrackAssociator_H

#include "Environment/MCObjects/interface/SimTrack.h"
#include <rave/Track.h>
#include <vector>

/**
 *  TrackAssociator by pulls, i.e. by some specific distance criterion between
 *  a SimTrack and a rave::Track.
 *
 *  The TrackAssociator does not initiate track reconstruction.  if you want it
 *  to associate rave::Tracks to a given SimTrack you must pass to it a container
 *  of rave::Tracks at construction time.  It does not perform track selection
 *  either: you must construct it with tracks satisfying your selection
 *  criteria.
 */

class TrackAssociator {
public:
  virtual ~TrackAssociator() {};

  virtual std::vector < vertigo::SimTrack > simTracks( const rave::Track & ) = 0;
  virtual std::vector < rave::Track > recTracks( const vertigo::SimTrack & ) = 0;

  virtual TrackAssociator * clone() const = 0;
};

#endif // TrackAssociator_H

#ifndef Tracker_TrackAssociatorByPulls_H
#define Tracker_TrackAssociatorByPulls_H

#include "Vertigo/TrackAssociation/interface/TrackDistanceByChiSquared.h"
#include "Vertigo/TrackAssociation/interface/TrackAssociator.h"

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

class TrackAssociatorByPulls : public TrackAssociator {
public:
  TrackAssociatorByPulls(const std::vector < vertigo::SimTrack > & sim,
                         const std::vector < rave::Track > & rec,
                         const TrackDistance & dist = TrackDistanceByChiSquared() );
  TrackAssociatorByPulls(const std::vector < vertigo::SimTrack > & sim,
                         const TrackDistance & dist = TrackDistanceByChiSquared() );
  TrackAssociatorByPulls(const std::vector < rave::Track > & rec,
                         const TrackDistance & dist = TrackDistanceByChiSquared() );

  TrackAssociatorByPulls(const TrackAssociatorByPulls & other);

  ~TrackAssociatorByPulls();

  std::vector < vertigo::SimTrack > simTracks( const rave::Track & );
  std::vector < rave::Track > recTracks( const vertigo::SimTrack & );

  TrackAssociatorByPulls * clone() const {
    return new TrackAssociatorByPulls(*this);
  }
private:
  std::vector < vertigo::SimTrack > theSimTracks;
  std::vector < rave::Track > theRecTracks;
  TrackDistance * theDistance;
};

#endif // Tracker_TrackAssociatorByPulls_H

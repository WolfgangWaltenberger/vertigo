#ifndef Tracker_TrackParameterHistogrammer_H_
#define Tracker_TrackParameterHistogrammer_H_

#include "Analysis/Observer/interface/Observer.h"
#include "Environment/MCObjects/interface/SimTrack.h"
#include <rave/Track.h>

#include <vector>
#include <string>

namespace rave {
  class Vertex;
}

class TrajectoryStateOnSurface;

/** Track resolution histograms.
 */

class TrackParameterHistogrammer : public Observer {
public:

  TrackParameterHistogrammer();
  TrackParameterHistogrammer( const std::string & str );
  ~TrackParameterHistogrammer();
  void process ( const vertigo::Event & e );
  void process ( const std::exception &, const std::string & );

  void configure ( const std::string & );
  TrackParameterHistogrammer * clone() const;

private:
  void fillCorrelations ( const TrajectoryStateOnSurface & );
  void stamp();
  void saveVertexTracks ( const rave::Vertex & v, const vertigo::Event & );
  void saveSimTracks ( const vertigo::Event & );
  void saveSimParticles ( const vertigo::Event & );
  void fill( const vertigo::SimTrack &, const rave::Track &, const std::string & name = "Tk",
             bool refitted=false, double w=1. );
  void fill( const rave::Track &, const std::string & name = "UnassociatedTk", 
             bool refitted=false, double w=1. );

  void write(); //< needs to be called explicitly when stopping with ev-enough()


private:
  std::string theFileName;
  int theTrackNr;
};

#endif

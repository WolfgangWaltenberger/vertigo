#ifndef EventHistogrammer_H
#define EventHistogrammer_H

#include "Analysis/Observer/interface/Observer.h"
#include "Environment/MCObjects/interface/SimTrack.h"
#include "Environment/MCObjects/interface/SimVertex.h"
#include <rave/Track.h>
#include <string>

class EventHistogrammer : public Observer {
  /**
   *  \class EventHistogrammer
   *  Class that stores some info about the events.
   */
public:
  EventHistogrammer();
  ~EventHistogrammer();
  void process ( const vertigo::Event & );
  void process ( const std::exception &, const std::string & );

  void configure ( const std::string & );

  EventHistogrammer * clone() const;

private:
  void analysis ( const std::vector < vertigo::SimTrack > & simtracks,
                  const std::vector < vertigo::SimVertex > & simvtces,
                  const std::vector < rave::Track > & rectracks, int evnr, float time );

  std::string theFileName;
  /// fix filename means filename=LoopSettings::Instance()+"event.hdf"
  bool theFixFileName;
};

#endif

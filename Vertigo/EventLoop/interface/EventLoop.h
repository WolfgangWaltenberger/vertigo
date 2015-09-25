#ifndef _EventLoop_H_
#define _EventLoop_H_

/*! \class EventLoop
 *  Main Loop.
 */

#include <string>
#include <map>
#include "Vertigo/EventGeneration/interface/EventFactory.h"
#include "Analysis/Observer/interface/ObserverManager.h"
#include <rave/FlavorTagFactory.h>
#include <rave/VertexFactory.h>

class EventLoop {
public:
  EventLoop ( const std::string & source, const std::string & vertexmethod,
              const std::string & flavortagmethod, const std::string & skin,
              int min, signed max, int verbosity,
              bool usebeamspot, int nthreads, int expect );
  static EventLoop * current();
  void loop();

  int verbosity() const;

  std::string method() const;

  std::string source() const;

  const rave::VertexFactory & vertexFactory() const;

private:
  EventLoop();

  void configureObserver();
};

#endif // _EventLoop_H_

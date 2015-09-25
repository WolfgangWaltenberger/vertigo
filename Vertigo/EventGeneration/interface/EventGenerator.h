#ifndef EventGenerator_H
#define EventGenerator_H

#include "Vertigo/Event/interface/Event.h"
#include <rave/Ellipsoid3D.h>
#include <string>

 /**
  *  Abstract base class for an event generator
  */

class EventGenerator {
public:
  virtual void source ( const std::string & name ) = 0;
  virtual vertigo::Event next() const = 0;
  virtual int skip ( int ) const = 0;
  virtual std::string describe() const = 0;

  /// event generators are resettable, 
  /// i.e. they can be put back into their original state.
  virtual void reset() = 0;

  /// does generator have beamspot constraint?
  virtual bool hasBeamSpot() const = 0;

  virtual rave::Ellipsoid3D beamSpot() const = 0;
  virtual ~EventGenerator() {};
};

#endif

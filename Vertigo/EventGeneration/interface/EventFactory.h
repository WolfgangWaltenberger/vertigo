#ifndef EventFactory_H
#define EventFactory_H

#include <string>
#include <vector>
#include <utility>
#include "Vertigo/EventGeneration/interface/EventGenerator.h"

class EventFactory : public EventGenerator {
public:
  /**
   *  EventFactory is an EventGenerator that
   *  decides "internally" upon which event generator to use.
   *
   *  The event factory does _not_ guarantee thread safety;
   *  the particular event generators have to implement
   *  thread safety, themselves.
   */

  EventFactory( const std::string & generator );
  // EventFactory ( const EventFactory & );
  // EventFactory operator= ( const EventFactory & );
  void source ( const std::string & );
  void initialise() const; // set iterator to .begin()
  void reset();
  ~EventFactory();
  vertigo::Event next() const;
  std::string describe() const;
  int skip ( int ) const;
  /// turn off complaining about objects with the same id
  void dontComplainAboutDoubles();

  /// Does event generator supply beamspot?
  bool hasBeamSpot() const;
  rave::Ellipsoid3D beamSpot() const;


  /** 
   *  Python convenience method,
   *  returning itself
   */
  const EventFactory & __iter__() const;

private:
  void nextGenerator() const;

private:
  std::string theGeneratorName;
  std::vector < std::pair < std::string, std::string > > theGenerators;
  mutable std::vector < std::pair < std::string, std::string > >::const_iterator 
    theGeneratorIter;
  mutable EventGenerator * theGenerator;
  bool theDontComplainAboutDoubles;
};

#endif

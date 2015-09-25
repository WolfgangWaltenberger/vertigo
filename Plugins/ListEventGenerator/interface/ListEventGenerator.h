#ifndef ListEventGenerator_H
#define ListEventGenerator_H

#include "Vertigo/EventGeneration/interface/EventGenerator.h"

class ListEventGenerator : public EventGenerator {
public:
  /**
   *  "meta" event generator, that needs a list of files.
   *  See sample/ for such files.
   */
  ListEventGenerator( const std::string & filename );
  ListEventGenerator();
  ~ListEventGenerator();
  void source ( const std::string & filename );
  void reset();
  std::string describe() const;
  vertigo::Event next() const;
  int skip ( int n ) const;
  bool hasBeamSpot() const;
  rave::Ellipsoid3D beamSpot() const;

private:
  void nextGenerator() const;
  std::string theFilename;
  mutable EventGenerator * theCurGenerator;
};

#endif

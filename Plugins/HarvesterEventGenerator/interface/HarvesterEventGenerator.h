#ifndef HarvesterEventGenerator_H
#define HarvesterEventGenerator_H

#include <string>
#include "Vertigo/EventGeneration/interface/EventGenerator.h"

class VertigoObjectReader;

class HarvesterEventGenerator : public EventGenerator {
public:
  HarvesterEventGenerator( const std::string & filename );
  HarvesterEventGenerator( const HarvesterEventGenerator & );
  HarvesterEventGenerator();
  ~HarvesterEventGenerator();
  void source ( const std::string & filename );
  void reset();
  std::string describe() const;
  vertigo::Event next() const;
  int skip ( int n ) const;
  bool hasBeamSpot() const;
  rave::Ellipsoid3D beamSpot() const;

private:
  std::string theFilename;
  mutable VertigoObjectReader * theReader;
};

#endif

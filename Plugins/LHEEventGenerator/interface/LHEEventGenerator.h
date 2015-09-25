#ifndef LHEEventGenerator_H
#define LHEEventGenerator_H

#include <string>
#include "Vertigo/EventGeneration/interface/EventGenerator.h"
#include "LHEF.h"

class VertigoObjectReader;

class LHEEventGenerator : public EventGenerator {
public:
  LHEEventGenerator( const std::string & filename );
  LHEEventGenerator();
  ~LHEEventGenerator();
  void source ( const std::string & filename );
  void reset();
  std::string describe() const;
  vertigo::Event next() const;
  int skip ( int n ) const;
  bool hasBeamSpot() const;
  rave::Ellipsoid3D beamSpot() const;

private:
  std::string theFilename;
  LHEF::Reader * theReader;
  bool theDebug;
  mutable int theCtr;
};

#endif

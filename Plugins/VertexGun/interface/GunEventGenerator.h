#ifndef GunEventGenerator_H
#define GunEventGenerator_H

#include <string>
#include "Vertigo/EventGeneration/interface/EventGenerator.h"

class ConfigurableVertexGun;

class GunEventGenerator : public EventGenerator {
public:
  GunEventGenerator( const std::string & gunname );
  GunEventGenerator();
  ~GunEventGenerator();
  void source ( const std::string & gunname );
  void reset();
  std::string describe() const;
  vertigo::Event next() const;
  int skip ( int n ) const;
  bool hasBeamSpot() const;
  rave::Ellipsoid3D beamSpot() const;

private:
  mutable ConfigurableVertexGun * theGun;
  // mutable int theId;
};

#endif

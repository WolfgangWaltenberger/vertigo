/** @file
 *
 * This defines the LCIO event generator used for reading
 * track data from LCIO (*.slcio) files. This will only be 
 * compiled, if the LCIO header files are present during 
 * building.
 */

#ifndef LcioEventGenerator_H
#define LcioEventGenerator_H

#include "Vertigo/EventGeneration/interface/EventGenerator.h"

#include <rave/impl/RaveBase/RaveEngine/interface/RaveBeamSpotSingleton.h>

#include "lcio.h"
#include "IO/LCReader.h"

#include <string>

/**
 * The actual EventGenerator class with the common interface.
 * Note that this class is currently very ugly,
 * a cleanup is required.
 */
class LcioEventGenerator : public EventGenerator
{
public:
  // FIXME implement RecoParticle::getStartVertex SimVertex.
  LcioEventGenerator( const std::string & filename );
  LcioEventGenerator();
  void source ( const std::string & filename );
  std::string describe() const;

  bool hasBeamSpot() const;
  rave::Ellipsoid3D beamSpot() const;

  ~LcioEventGenerator();
  vertigo::Event next() const;  //!< Fetches the contained events one by one.
  int skip ( int n ) const;  //!< Skips some events if necessary.

private:
  void setup();
  std::string theFileName;  //!< Holds the filename of the LCIO input file
  std::vector<std::string> theCollections;  //!< Holds the collection names to select the collections from the input file.
  mutable int theId;
  lcio::LCReader * theLcReader;  //!< The almighty LCIO Reader which does all the real work.
};

#endif

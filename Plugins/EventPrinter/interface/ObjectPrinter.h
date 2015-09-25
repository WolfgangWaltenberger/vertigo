#ifndef ObjectPrinter_H
#define ObjectPrinter_H

#include "Environment/MCObjects/interface/SimTrack.h"
#include "Environment/MCObjects/interface/SimParticle.h"
#include <rave/Track.h>
#include <rave/KinematicParticle.h>
#include <string>

class ObjectPrinter {
public:
  /**
   *  @param euclidean print euclidean parameters?
   *  @param perigee print perigee parameters?
   */
  static void print ( const vertigo::SimTrack &, bool euclidean=true,
                      bool perigee=false, const std::string & prefix = "SimTrack #",
                      const std::string & postfix = "", float w=-1.,
                      bool printweight=false );

  static void print ( const rave::Track &, bool euclidean=true,
                      bool perigee=true, const std::string & prefix = "Track #",
                      const std::string & postfix = "", float w=-1.,
                      bool printweight=false, bool collapsed=true );
  static void printMultiComponent ( const rave::Track &, bool euclidean=true,
                      bool perigee=true, const std::string & prefix = "NCompTrack #",
                      const std::string & postfix = "", float w=-1.,
                      bool printweight=false );

  static void print ( const vertigo::SimParticle &, bool euclidean=true,
                      bool perigee=false, const std::string & prefix = "SimParticle #",
                      const std::string & postfix = "", float w=-1.,
                      bool printweight=false );

  static void print ( const rave::KinematicParticle &, bool euclidean=true,
                      bool perigee=true, const std::string & prefix = "Particle #",
                      const std::string & postfix = "", float w=-1.,
                      bool printweight=false );
};

#endif

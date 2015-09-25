#ifndef PARTICLEDECAYPRINTER_H
#define PARTICLEDECAYPRINTER_H

#include "Analysis/Observer/interface/Observer.h"
#include "Vertigo/Event/interface/Event.h"
#include <map>

/**
 * @class ParticleDecayPrinter reconstructs a decay chain from a given set of
 * TransientTracks
 *
 * This observer makes use of the RaveVertexKinematics possibilities to
 * produce a tree representing the decay chain. It allows the application of
 * kinematic constraints during this process.
 *
 * @author Fabian Moser <fabiamos@gmail.com>
 */

class ParticleDecayPrinter :
      public Observer
{

  public:
    ParticleDecayPrinter();

    ~ParticleDecayPrinter();

    void process ( const vertigo::Event & );
    void process ( const std::exception &, const std::string & );

    void configure ( const std::string & );
    void except ( const std::string & e, int id );

    ParticleDecayPrinter * clone() const;

  private:
    void print ( const vertigo::Event & ) const;
    void printOld ( const vertigo::Event & ) const;
    std::string printVertex ( const vertigo::SimVertex & ) const;
    std::string printTrack ( const vertigo::SimTrack & ) const;
};

#endif

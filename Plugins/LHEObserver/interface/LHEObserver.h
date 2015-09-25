#ifndef LHEOBSERVER_H
#define LHEOBSERVER_H

#include "Analysis/Observer/interface/Observer.h"
#include "Vertigo/Event/interface/Event.h"
#include <map>

/**
 * @class LHEObserver reconstructs a decay chain from a given set of
 * TransientTracks
 *
 * This observer makes use of the RaveVertexKinematics possibilities to
 * produce a tree representing the decay chain. It allows the application of
 * kinematic constraints during this process.
 *
 * @author Fabian Moser <fabiamos@gmail.com>
 */

class LHEObserver :
      public Observer
{

  public:
    LHEObserver();

    ~LHEObserver();

    void process ( const vertigo::Event & );
    void process ( const std::exception &, const std::string & );

    void configure ( const std::string & );
    void except ( const std::string & e, int id );

    LHEObserver * clone() const;

  private:
    void store ( const vertigo::Event & ) const;
    void storeSmsMasses ( const vertigo::Event & ) const;
    void storeMasses() const;
    std::string theFileName;
    bool writePids_; // write out the pids?

    /// the masses. but the particles have widths, so I keep
    /// all masses, and later compute an average
    mutable std::map < int, std::vector < float > > masses_;

    bool massesPerEvent_; // write out masses in every event?
    bool smsMasses_; // write out the nominal masses? (for SMSes)
    mutable int theCtr;
};

#endif

//
// C++ Interface: TestAnalyser
//
// Description:
//
//
// Author: Fabian Moser <fabiamos@gmail.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef TEST_ANALYSER_H
#define TEST_ANALYSER_H

#include "Analysis/Observer/interface/Observer.h"
#include "Vertigo/Event/interface/Event.h"
#include <map>

/**
 * @class TestAnalyser reconstructs a decay chain from a given set of
 * TransientTracks
 *
 * This observer makes use of the RaveVertexKinematics possibilities to
 * produce a tree representing the decay chain. It allows the application of
 * kinematic constraints during this process.
 *
 * @author Fabian Moser <fabiamos@gmail.com>
 */

class TestAnalyser :
      public Observer
{

  public:
    TestAnalyser();

    ~TestAnalyser();

    void process ( const vertigo::Event & );
    void process ( const std::exception &, const std::string & );

    void configure ( const std::string & );
    void except ( const std::string & e, int id );

    TestAnalyser * clone() const;

  private:
    void lheSMS ( const std::string & t, bool debug ) const;
    void store ( const vertigo::Event & ) const;
    void storeSmsMasses ( const vertigo::Event & ) const;
    void print ( const vertigo::Event & ) const;
    void printOld ( const vertigo::Event & ) const;
    void printVertex ( const vertigo::SimVertex & ) const;
    void storeMasses() const;
    void storePts() const;
    std::string theFileName;
    bool theTree;
    bool writePids_; // write out the pids?

    /// the masses. but the particles have widths, so I keep
    /// all masses, and later compute an average
    mutable std::map < int, std::vector < float > > masses_;
    mutable std::map < int, std::vector < float > > pts_;
    mutable std::map < int, std::vector < float > > rapidities_;
    mutable std::map < std::string, float > smsParams_;

    bool massesPerEvent_; // write out masses in every event?
    bool smsMasses_; // write out the nominal masses? (for SMSes)
    mutable int theCtr;
};

#endif

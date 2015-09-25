//
// C++ Interface: KinematicsObserver
//
// Description:
//
//
// Author: Fabian Moser <fabiamos@gmail.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef KINEMATICSOBSERVER_H
#define KINEMATICSOBSERVER_H

#include "Analysis/Observer/interface/Observer.h"
#include "Vertigo/Event/interface/Event.h"

/**
 * @class KinematicsObserver reconstructs a decay chain from a given set of
 * TransientTracks
 *
 * This observer makes use of the RaveVertexKinematics possibilities to
 * produce a tree representing the decay chain. It allows the application of
 * kinematic constraints during this process.
 *
 * @author Fabian Moser <fabiamos@gmail.com>
 */

class KinematicsObserver :
      public Observer
{

  public:
    KinematicsObserver();

    ~KinematicsObserver();

    void process ( const vertigo::Event & );
    void process ( const std::exception &, const std::string & );

    void configure ( const std::string & );
    void except ( const std::string & e, int id );

    KinematicsObserver * clone() const;

  private:
    std::string theFileName;
};

#endif

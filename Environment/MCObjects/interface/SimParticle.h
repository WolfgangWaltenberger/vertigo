//
// C++ Interface: SimParticle
//
// Description:
//
//
// Author: Fabian Moser <fabiamos@gmail.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef VERTIGOSIMPARTICLE_H
#define VERTIGOSIMPARTICLE_H

#include "Environment/MCObjects/interface/BasicSimParticle.h"

namespace vertigo
{

/**
        @author Fabian Moser <fabiamos@gmail.com>
*/
class SimParticle :
      public BasicSimParticle::Proxy
{
    typedef BasicSimParticle::Proxy Base;

  public:
    /**
   *  Create a SimParticle from a 7-dimensional euclidean trajectory state
    *  plus covariance matrix, plus charge, plus an optional
    *  pointer to the "original track", whatever that means
    *
     */
    SimParticle ( const rave::Vector7D &, rave::Charge,
               void * originaltrack = 0, int pid = 0,
               const std::string & tag = "",
               const vertigo::SimVertex * vtx = 0 );
    SimParticle();

    rave::Charge charge() const;
    const rave::Vector7D & state() const;
    rave::Vector4D p4() const;
    void * originalObject() const;
    std::string tag() const;
    int id() const;
    int pid() const;
    bool operator< ( const SimParticle & ) const;
    bool operator== ( const SimParticle & ) const;

    void setVertex ( const vertigo::SimVertex * v );

    const vertigo::SimVertex * vertex() const;

};

}

#endif

//
// C++ Interface: BasicSimParticle
//
// Description:
//
//
// Author: Fabian Moser <fabiamos@gmail.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef VERTIGOBASICSIMPARTICLE_H
#define VERTIGOBASICSIMPARTICLE_H

#include <rave/Vector7D.h>

#include "Environment/MCObjects/interface/BasicSimTrack.h"

namespace vertigo
{

/**
        @author Fabian Moser <fabiamos@gmail.com>
*/
class BasicSimParticle :
      public BasicSimTrack
{
  public:
    typedef RaveProxyBase< BasicSimParticle, CopyUsingNew<BasicSimParticle> > Proxy;

    BasicSimParticle ( const rave::Vector7D &, rave::Charge,
                       void * originalobject = 0, int pid = 0,
                       const std::string & tag = "",
                       const SimVertex * vtx = 0 );

    BasicSimParticle();

    const rave::Vector7D & state() const;

  private:
    rave::Vector7D theState;

};

}

#endif

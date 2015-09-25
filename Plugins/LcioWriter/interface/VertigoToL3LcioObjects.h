//
// C++ Interface: VertigoToL3LcioObjects
//
// Description:
//
//
// Author: Fabian Moser <fabiamos@gmail.com>, (C) 2008
//
//
#ifndef VERTIGOTOL3LCIOOBJECTS_H
#define VERTIGOTOL3LCIOOBJECTS_H

#include <IMPL/MCParticleImpl.h>

#include "Environment/MCObjects/interface/SimParticle.h"

/**
 @author Fabian Moser <fabiamos@gmail.com>
 */
class VertigoToL3LcioObjects
{
  public:
    VertigoToL3LcioObjects ( double Bz = 4.0 );
    ~VertigoToL3LcioObjects();

    /// Converts Vertigo SimParticles to LCIO MCParticles
    IMPL::MCParticleImpl* convert (
      const vertigo::SimParticle & vertigoSimParticle ) const;

  private:
    double theBz;

};

#endif

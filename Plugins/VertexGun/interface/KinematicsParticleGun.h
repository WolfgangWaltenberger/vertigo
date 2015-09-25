//
// C++ Interface: KinematicsParticleGun
//
// Description:
//
//
// Author: Fabian Moser <fabiamos@gmail.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef KINEMATICSPARTICLEGUN_H
#define KINEMATICSPARTICLEGUN_H

#include <boost/random.hpp>

#include "ParticleGun.h"
#include "Environment/MCObjects/interface/SimParticle.h"
#include "Environment/MCObjects/interface/SimTrack.h"


/**
 * Produces full-blown kinematic events
 * 
 * @author Fabian Moser <fabiamos@gmail.com>
*/
class KinematicsParticleGun :
      public ParticleGun
{
  public:
    KinematicsParticleGun( double mass=91.1876, double mass_width=.0001 );

    ~KinematicsParticleGun();

    std::vector< vertigo::SimTrack > tracks() const;
    std::vector< vertigo::SimParticle > particles() const;
    std::vector< vertigo::SimVertex > vertices() const;

    void shoot();
    void validate();

    std::string name() const;

    KinematicsParticleGun * clone() const;

  private:
    void clear();

  private:
    typedef boost::mt19937 base_generator_type;
    //typedef boost::minstd_rand base_generator_type;
    typedef boost::variate_generator< base_generator_type&, boost::uniform_real<> > uni_generator_type;
    typedef boost::variate_generator< base_generator_type&, boost::normal_distribution<> > normal_generator_type;
    /// These are uniform distribution representations
    boost::uniform_real<> thePrimaryP;
    boost::uniform_real<> thePrimaryPhi;
    boost::uniform_real<> thePrimaryEta;
    boost::uniform_real<> theMuonPhi;
    boost::uniform_real<> theMuonTheta;

    /// This is a normal distribution
    boost::normal_distribution<> thePrimaryMass;

    // Try boost::mt19937 or boost::ecuyer1988 instead of boost::minstd_rand
    base_generator_type theGenerator;

    std::string theName;
    std::vector< vertigo::SimParticle > theParticles;
    std::vector< vertigo::SimTrack > theTracks;
    std::vector< vertigo::SimVertex > theVertices;

};

#endif

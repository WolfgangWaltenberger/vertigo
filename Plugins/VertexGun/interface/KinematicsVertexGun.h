//
// C++ Interface: KinematicsVertexGun
//
// Description:
//
//
// Author: Fabian Moser <fabiamos@gmail.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef KINEMATICSVERTEXGUN_H
#define KINEMATICSVERTEXGUN_H

#include "VertexGunFromParticleGun.h"
#include "Vertigo/TrackSmearing/interface/GaussianRecTrackSmearer.h"

/**
 @author Fabian Moser <fabiamos@gmail.com>
*/
class KinematicsVertexGun :
      public VertexGun
{
  public:
    KinematicsVertexGun ( const rave::Point3D & vertex = rave::Point3D ( 0., 0., 0. ),
                          const RecTrackSmearer & smearer = GaussianRecTrackSmearer (.002F, .0003F, 1.e-4F ),
                          //const RecTrackSmearer & smearer = GaussianRecTrackSmearer (), // .002F, .0003F, 1.e-4F ),
                          const std::string & name = "KinematicsVertexGun" );
    KinematicsVertexGun ( const std::string & name );
    KinematicsVertexGun ( const KinematicsVertexGun & );
    void configure ( const std::string name );
    virtual vertigo::Event shoot();
    virtual std::string name() const;
    virtual KinematicsVertexGun * clone() const;
  private:
    boost::shared_ptr<VertexGunFromParticleGun> theGun;
    std::string theName;
    RecTrackSmearer * theSmearer;
};

#endif

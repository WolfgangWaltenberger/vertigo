#ifndef _VertigoToCmsObjects_H_
#define _VertigoToCmsObjects_H_

#include "Environment/MCObjects/interface/SimTrack.h"
#include "Environment/MCObjects/interface/SimParticle.h"
#include <rave/impl/TrackingTools/TrajectoryParametrization/interface/GlobalTrajectoryParameters.h>
#include <rave/impl/TrackingTools/TrajectoryState/interface/FreeTrajectoryState.h>
#include <rave/impl/DataFormats/CLHEP/interface/AlgebraicObjects.h>
#include <rave/Covariance3D.h>
#include <rave/Vector3D.h>
#include "Environment/VertigoObjects/interface/VertigoJet.h"
#include "DataFormats/Candidate/interface/Particle.h"                                                           

class VertigoToCmsObjects 
{
  public:
    GlobalTrajectoryParameters convert ( const vertigo::SimTrack & ) const;
    GlobalTrajectoryParameters convert ( const vertigo::SimParticle & ) const;
    FreeTrajectoryState freeState ( const vertigo::SimTrack & ) const;
    AlgebraicSymMatrix convert ( const rave::Covariance3D & ) const;
    AlgebraicVector convert ( const rave::Vector3D & ) const;
};

#endif

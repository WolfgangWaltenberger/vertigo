#ifndef Vertigo_VertigoStreamers_H_
#define Vertigo_VertigoStreamers_H_

#include <ostream>
#include "Environment/MCObjects/interface/SimTrack.h"
#include "Environment/MCObjects/interface/SimVertex.h"
#include <rave/impl/RaveBase/Converters/interface/RaveStreamers.h>
#include <rave/impl/TrackingTools/TrajectoryState/interface/TrajectoryStateOnSurface.h>
#include <rave/impl/TrackingTools/TrajectoryState/interface/TrajectoryStateOnSurface.h>
#include <rave/impl/TrackingTools/TransientTrack/interface/TransientTrack.h>
#include <rave/impl/TrackingTools/TrajectoryParametrization/interface/LocalTrajectoryParameters.h>
#include <rave/impl/TrackingTools/TrajectoryParametrization/interface/GlobalTrajectoryParameters.h>


std::ostream& operator<< (std::ostream& os, const vertigo::SimTrack & );
std::ostream& operator<< (std::ostream& os, const vertigo::SimVertex & );
std::ostream& operator<< (std::ostream& os, const TrajectoryStateOnSurface & );
std::ostream& operator<< (std::ostream& os, const FreeTrajectoryState & );
std::ostream& operator<< (std::ostream& os, const LocalTrajectoryParameters & );
std::ostream& operator<< (std::ostream& os, const GlobalTrajectoryParameters & );
std::ostream& operator<< (std::ostream& os, const reco::TransientTrack & );
std::ostream& operator<< (std::ostream& os, const Surface & );
#ifdef WITH_KINEMATICS
std::ostream& operator<< (std::ostream& os, const vertigo::SimParticle & );
#endif


#endif // Vertigo_VertigoStreamers_H_

#include "Environment/Utilities/interface/VertigoStreamers.h"
#include <rave/impl/RaveBase/Converters/interface/RaveStreamers.h>
#include <rave/impl/DataFormats/GeometrySurface/interface/Surface.h>
#include "Environment/MCObjects/interface/PDGCodes.h"

using namespace std;

ostream& operator<< (ostream& os, const vertigo::SimTrack & t )
{
    os << t.state() << " q=" << t.charge();
    return os;
}

ostream& operator<< (ostream& os, const vertigo::SimVertex & t )
{
    os << t.position();
    if ( t.pid() != 0 )
    {
      os << " [pid " << t.pid() << "]";
    }

    if ( t.tag() != "" )
    {
      os << " <" << t.tag() << ">";
    }

    if ( !t.isValid() )
    {
      os << " invalid";
    }
    return os;
}


ostream& operator<< (ostream& os, const reco::TransientTrack & t )
{
    os << t.initialFreeState();
    return os;
}


ostream& operator<< (ostream& os, const Surface & s )
{
    os << "pos=" << s.position()
       << " rot=" << s.rotation();
    return os;
}


ostream& operator<< (ostream& os, const TrajectoryStateOnSurface & t )
{
    os << endl
       << "[tsos:global] " << t.globalParameters() << endl
       << "[tsos:local] pars=" << t.localParameters() << endl
       << "[tsos:local] dir=" << t.localDirection() << endl
       //<< >[tsos:surface] > << t.surface()
       ;
    return os;
}

ostream& operator<< (ostream& os, const FreeTrajectoryState & t )
{
    os << t.parameters();
    return os;
}


ostream& operator<< (ostream& os, const LocalTrajectoryParameters & t )
{
    os << t.position() << ", " << t.momentum();
    return os;
}

ostream& operator<< (ostream& os, const GlobalTrajectoryParameters & t )
{
    os << t.position() << ", " << t.momentum();
    return os;
}

#ifdef WITH_KINEMATICS
ostream& operator<< (std::ostream& os, const vertigo::SimParticle  & t )
{
  os << t.state() << " q=" << std::setw(2) << t.charge() 
     << " [" << PDGCodes::name(t.pid()) << "]";
  return os;
}
#endif

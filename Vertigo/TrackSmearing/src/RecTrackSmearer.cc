#include "Vertigo/TrackSmearing/interface/RecTrackSmearer.h"
#include <rave/impl/RaveTools/Converters/interface/MagneticFieldSingleton.h>
#include <iostream>

using namespace std;

rave::Track RecTrackSmearer::fromVector6D ( const rave::Vector6D & w, rave::Charge charge )
{
  GlobalPoint position ( w.x(), w.y(), w.z() );
  GlobalVector momentum ( w.px(), w.py(), w.pz() );
  if ( !(MagneticFieldSingleton::Instance()->hasField()) )
  {
    cout << "[RecTrackSmearer] Error: no magnetic field known to vertigo." << endl;
    cout << "[RecTrackSmearer] Please instantiate a rave Factory before smearing tracks"
         << endl;
    cout << "[RecTrackSmearer] returning dummy track" << endl;
    return rave::Track();
  }
  return recTrack ( GlobalTrajectoryParameters ( 
        position, momentum, charge, MagneticFieldSingleton::Instance() ) ); 
}

rave::Track RecTrackSmearer::fromSimTrack ( const vertigo::SimTrack & s )
{
  return fromVector6D ( s.state(), s.charge() );
}

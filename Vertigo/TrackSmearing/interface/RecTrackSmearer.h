#ifndef _RecTrackSmearer_H_
#define _RecTrackSmearer_H_

#include <rave/Track.h>
#include <rave/Vector6D.h>
#include <rave/impl/TrackingTools/TrajectoryParametrization/interface/GlobalTrajectoryParameters.h>
#include "Environment/MCObjects/interface/SimTrack.h"

/*! RecTrackSmearer.
 *  defines the interface to the different RecTrack smearing factories
 */

class RecTrackSmearer {
public:
  virtual ~RecTrackSmearer() {};
  virtual rave::Track recTrack ( const GlobalTrajectoryParameters & ) const = 0;

  rave::Track fromVector6D ( const rave::Vector6D & w, rave::Charge charge );
  rave::Track fromSimTrack ( const vertigo::SimTrack & s );

  /** 
   * given the parameters of a simulated tracks,
   * this method gives you the systematic bias on the GTP induced by the smearer.
   */
  virtual GlobalTrajectoryParameters delta( const GlobalTrajectoryParameters & simtrack ) const 
  {
    return GlobalTrajectoryParameters();
  }

  /**
   * A smearer must be able to describe itself
   * to STDOUT, in a few words.
   */
  virtual void printDescription() const = 0;

  virtual RecTrackSmearer * clone () const = 0;
};

#endif // _RecTrackSmearer_H_

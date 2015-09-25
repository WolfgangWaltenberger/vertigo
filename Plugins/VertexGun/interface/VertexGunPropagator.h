#ifndef _VertexGunPropagator_H_
#define _VertexGunPropagator_H_

#include <rave/impl/TrackingTools/TrajectoryParametrization/interface/GlobalTrajectoryParameters.h>

/*! \class VertexGunPropagator
 *  Moves the GTPs provided by the particle guns to a realistic initial state
 *  (innermost barrel or innermost forward disc)
 */

class VertexGunPropagator {

public:
  static GlobalTrajectoryParameters propagate ( const GlobalTrajectoryParameters & r );
};

#endif // _VertexGunPropagator_H_

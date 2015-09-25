#ifndef _JetVertexGun_H_
#define _JetVertexGun_H_

#include "VertexGun.h"
#include <rave/Units.h>

class JetVertexGun : public VertexGun {

public:
  /**
   *
   * \class JetVertexGun, two vertices "along" the jet axis.
   * The distance between the vertices follows a flat distribution
   * in the interval between (minflightdistance,maxflightdistance).
   *
   * \paramname minflightdistance minimum distance between primary
   * and secondary vertex.
   * \paramname maxflightdistance maximum distance between primary
   * and secondary vertex.
   * \paramname noutliers number of tracks which originate from
   * _no_ vertex.
   * \paramname secondjet a second jet at the primary vertex,
   * but in a different direction?
   */
  JetVertexGun( float minflightdistance = 10. * raveunits::um, 
                float maxflightdistance = 5. * raveunits::mm,
                int noutliers = 2, bool secondjet=false );
  vertigo::Event shoot();

  void clear();
  virtual std::string name() const;

  JetVertexGun * clone() const;

private:
  int theId;
  float theMinFlightDistance;
  float theMaxFlightDistance;
  int theNOutliers;
  bool theSecondJet;
};

#endif // _JetVertexGun_H_

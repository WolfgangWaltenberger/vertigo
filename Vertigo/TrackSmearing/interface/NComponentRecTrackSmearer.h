#ifndef _NComponentRecTrackSmearer_H_
#define _NComponentRecTrackSmearer_H_

#include "Vertigo/TrackSmearing/interface/RecTrackSmearer.h"
#include <vector>
#include <utility>

/** NComponentRecTrackSmearer.
 *  A composition of RecTrackSmearers, to produce n-component tracks.
 */

class NComponentRecTrackSmearer : public RecTrackSmearer {
public:
  /**  Produce this new Smearer from a set of RecTrackSmearers.
   *   We own the smearers!
   */
  NComponentRecTrackSmearer(
      const std::vector < std::pair < float, const RecTrackSmearer * > > &,
      bool correct_statistical_properties = true );
  NComponentRecTrackSmearer ( const NComponentRecTrackSmearer & );

  ~NComponentRecTrackSmearer();

  NComponentRecTrackSmearer * clone() const;

  void printDescription() const;
  rave::Track recTrack ( const GlobalTrajectoryParameters & ) const;

private:
  std::vector < std::pair < float, const RecTrackSmearer * > > theSmearer;
  bool theCorrectStatisticalProperties;
};

#endif // _NComponentRecTrackSmearer_H_

/** @file
 *
 * Uses dataharvester to check 
 * track parameter pulls
 */

#ifndef _L3TrackChecker_H_
#define _L3TrackChecker_H_

#include "EVENT/Track.h"
#include "EVENT/MCParticle.h"

class L3TrackChecker
{
public:
  void check ( const EVENT::Track * t, EVENT::MCParticle * ) const;
};

#endif

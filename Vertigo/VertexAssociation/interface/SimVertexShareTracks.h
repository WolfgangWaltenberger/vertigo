#ifndef _Tracker_SimVertexShareTracks_H_
#define _Tracker_SimVertexShareTracks_H_

#include "Environment/MCObjects/interface/SimVertex.h"

/** \class SimVertexShareTracks
 *  Checks if two TkSimVertex objects have tracks in common
 */

class SimVertexShareTracks {

public:

  /** return true if vertices share at least 1 track, 
   *  false otherwise
   */
  static bool check(const vertigo::SimVertex & sv1, const vertigo::SimVertex & sv2 );

};

#endif

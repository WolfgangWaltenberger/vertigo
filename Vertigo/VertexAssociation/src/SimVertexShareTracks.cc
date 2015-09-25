#include "Vertigo/VertexAssociation/interface/SimVertexShareTracks.h"
#include "Environment/MCObjects/interface/SimVertex.h"
#include <algorithm>

using namespace std;

bool SimVertexShareTracks::check(const vertigo::SimVertex & sv1, 
				   const vertigo::SimVertex & sv2)
{
  vector< vertigo::SimTrack > tks1 = sv1.tracks();
  vector< vertigo::SimTrack > tks2 = sv2.tracks();
  
  for (vector< vertigo::SimTrack >::const_iterator it = tks2.begin();
       it != tks2.end(); it++) {
    vector< vertigo::SimTrack >::const_iterator here 
      = find(tks1.begin(), tks1.end(), (*it));
    if (here != tks1.end()) return true;
  }
  return false;
}


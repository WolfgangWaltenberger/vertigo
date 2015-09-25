#ifndef _AssociationManager_H_
#define _AssociationManager_H_

#include <rave/Track.h>
#include <rave/KinematicParticle.h>
#include <rave/Vertex.h>
#include "Environment/MCObjects/interface/SimTrack.h"
#include "Environment/MCObjects/interface/SimParticle.h"
#include "Environment/MCObjects/interface/SimVertex.h"
#include <vector>
#include <set>
#include <map>

/** \class AssociationManager
 * facility that manages associations between
 * reconstructed tracks and simulated tracks
 * (m:n associations)
 */

namespace vertigo {
class AssociationManager
{
  friend class BasicEvent;
  public:

    /// clear association maps
    void clear();

    /// define a simtrk - rectrk association
    void associate ( const vertigo::SimTrack &, const rave::Track & );

    /// define a simParticle - KinematicParticle association
    void associate ( const vertigo::SimParticle &, const rave::KinematicParticle & );

    /// define a simvtx - recvtx association
    void associate ( const vertigo::SimVertex &, const rave::Vertex & );

    std::vector < vertigo::SimTrack > tracks ( const rave::Track & ) const;
    std::vector < rave::Track > tracks ( const vertigo::SimTrack & ) const;

    std::vector < vertigo::SimParticle > particles ( const rave::KinematicParticle & ) const;
    std::vector < rave::KinematicParticle > particles ( const vertigo::SimParticle & ) const;

    std::vector < rave::Vertex > vertices ( const vertigo::SimVertex & ) const;
    std::vector < vertigo::SimVertex > vertices ( const rave::Vertex & ) const;


  protected:
    AssociationManager();

  private:
    mutable std::map < vertigo::SimTrack, std::vector < rave::Track > > theTrackSimMap;
    mutable std::map < rave::Track, std::vector < vertigo::SimTrack > > theTrackRecMap;
    mutable std::map < int, std::set < int > > theHasTracksAssociated; //< log associations in a single container
    mutable std::map < vertigo::SimParticle, std::vector < rave::KinematicParticle > > theParticleSimMap;
    mutable std::map < rave::KinematicParticle, std::vector < vertigo::SimParticle > > theParticleRecMap;
    mutable std::map < int, std::set < int > > theHasParticlesAssociated; //< log associations in a single container
    mutable std::map < vertigo::SimVertex, std::vector < rave::Vertex > > theVtxSimMap;
    mutable std::map < rave::Vertex, std::vector < vertigo::SimVertex > > theVtxRecMap;
    mutable std::map < int, std::set < int > > theHasVerticesAssociated; //< log associations in a single container
};
}

#endif

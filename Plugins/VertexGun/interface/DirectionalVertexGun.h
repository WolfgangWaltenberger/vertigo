#ifndef _DirectionalVertexGun_H_
#define _DirectionalVertexGun_H_

#include "VertexGunFromParticleGun.h"
#include "Vertigo/TrackSmearing/interface/GaussianRecTrackSmearer.h"

/**
 *   DirectionalVertexGun; creates a collimated particle jet, user defines the
 *   angular spread, the number of particles, and the total momentum.
 *   \param min_n_tracks Give the _total_ minimum number of tracks.
 *   \param max_n_tracks Give the _total_ maximum number of tracks.
 *   \param vertex where is your vertex.
 *   \param true_vtx_pos_sigma you can smear your SimVertex.
 *   The "true" vertex is vertex plus gaussian smearing with these values.
 *   \param angular_spread the opening angle of the jet cone.
 *   \param primary_vertices Mark SimVertices as primaries?
 */

class DirectionalVertexGun : public VertexGunFromParticleGun {
public:
  DirectionalVertexGun( int min_n_tracks=10, int max_n_tracks=10,
       const GlobalPoint & vertex = GlobalPoint(0.,0.,0.),
       const GlobalPoint & true_vtx_pos_sigma = GlobalPoint(1e-5,1e-5,.5),
       const GlobalVector & total_momentum = GlobalVector(20,0,0),
       float angular_spread = .5,
       const RecTrackSmearer & smearer = GaussianRecTrackSmearer(),
       const std::string & name = "DirectionalVertexGun",
       bool primary_vertices=false );
};

#endif // _DirectionalVertexGun_H_

#ifndef _DirectionalParticleGun_H_
#define _DirectionalParticleGun_H_

#include <rave/impl/DataFormats/GeometryVector/interface/GlobalPoint.h>
#include <rave/impl/DataFormats/GeometryVector/interface/GlobalVector.h>
#include "ParticleGun.h"
#include <string>

class DirectionalParticleGun : public ParticleGun {
public:
  /***
   *   \class DirectionalParticleGun
   *   A ParticleGun that is capable of generating a jet of particles
   *   in a given direction with a given total momentum.
   */

  /**
   *  \param energetic_spread The momenta of the tracks follow a gaussian
   *  distribution in this gun. This determines the sigma of the distribution,
   *  given in GeV. It is per track.
   *  \param angular_spread The angular spread of the (imaginary) cone around
   *  the total momentum std::vector, within which all tracks lie.
   *  \param total_momentum The total momentum std::vector, summing over all tracks.
   *  Note that this is only fulfilled in the statistical limit; individual
   *  jets may differ a bit from this std::vector, the variation depending
   *  heavily on the angular_spread and the number of tracks.
   *  \param min_n_tracks The minimum number of tracks in the jet cone.
   *  \param max_n_tracks The maximum number of tracks in the jet cone.
   *  \param vertex The vertex position, up to smaller variations defined in
   *  trueVertex_position_sigma
   *  \param trueVertex_position_sigma The sigmas of the distribution of the true
   *  SimVertex.
   *  \param name Gives the gun a name.
   *  \param vertices_are_primary Mark SimVertices as primaries?
   */

  DirectionalParticleGun( const int min_n_tracks=10,
      const int max_n_tracks=10,
      const GlobalPoint & vertex = GlobalPoint(0,0,0),
      const GlobalPoint & trueVertex_position_sigma = GlobalPoint ( 0, 0, .5 ),
      const GlobalVector & total_momentum = GlobalVector(20,0,0),
      const float angular_spread = .5,
      const float energetic_spread = 1.,
      const std::string & name="",
      bool vertices_are_primary=false
  );

  ~DirectionalParticleGun();

  DirectionalParticleGun * clone() const;

  void printDescription( std::string name ) const;

  std::vector < vertigo::SimTrack > tracks() const;

  std::vector < vertigo::SimParticle > particles() const;

  std::vector < vertigo::SimVertex > vertices() const;

  void shoot();
  std::string name() const;


private:
  vertigo::SimTrack createATrack( const int, int id, const GlobalPoint & pos );
  void clear();

private:
  int theMinNtracks, theMaxNtracks;
  GlobalPoint theGp;
  GlobalVector theAvgTotMom;
  float theSpread, theEnergeticSpread;
  GlobalPoint thePosSigma;
  std::string theName;
  bool theIsPrimaryVertex;
  std::vector < vertigo::SimTrack > theTracks;
  std::vector < vertigo::SimVertex > theVertices;
};

#endif // _DirectionalParticleGun_H_

#ifndef VertexGunFromParticleGun_H
#define VertexGunFromParticleGun_H

#include "VertexGun.h"
#include "ParticleGun.h"
#include "Vertigo/TrackSmearing/interface/GaussianRecTrackSmearer.h"
#include <string>

class VertexGunFromParticleGun : public VertexGun
{
/**
 *  \class VertexGunFromParticleGun
 *  Creates a VertexGun out of a ParticleGun.
 */
public:
  /**
   *  This constructor lets the user supply the the rave::Track smearer.
   *  \param fraction_outliers The fraction of total tracks that should be
   *  created as outliers.
   */
  VertexGunFromParticleGun( const ParticleGun &,
      const RecTrackSmearer &, const std::string & name = "FromParticleGun" );

  VertexGunFromParticleGun( const VertexGunFromParticleGun & );

  ~VertexGunFromParticleGun();

  vertigo::Event shoot();
  void clear();
  std::string name() const;
  VertexGunFromParticleGun * clone() const;
  /// access to the particle gun, e.g. for configuration
  ParticleGun * gun ();

private:
  void associate ( const vertigo::SimTrack &, const rave::Track & );

private:
  ParticleGun * theGun;
  int theId;
  vertigo::Event theEvent;
  RecTrackSmearer * theSmearer;
  std::string theName;
};

#endif

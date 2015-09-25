#include "DirectionalVertexGun.h"
#include "DirectionalParticleGun.h"

DirectionalVertexGun::DirectionalVertexGun(  int min_ntracks,
     int max_ntracks, const GlobalPoint & gp, const GlobalPoint & pos_sigma,
     const GlobalVector & atm,  float spread,
     const RecTrackSmearer & smearer, const std::string & name,
     bool primary_vertices ) :
  VertexGunFromParticleGun (
      DirectionalParticleGun ( min_ntracks, max_ntracks, gp,
        pos_sigma, atm, spread, 1.0 , name, primary_vertices ) , smearer, name )
{}

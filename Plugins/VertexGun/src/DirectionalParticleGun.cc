#include "DirectionalParticleGun.h"
#include "CLHEP/Random/RandGauss.h"
#include "CLHEP/Random/RandFlat.h"
#include "Environment/Configuration/interface/SimpleConfigurable.h"
#include <rave/impl/RaveBase/Converters/interface/CmsToRaveObjects.h>
#include <rave/impl/RaveTools/Converters/interface/MagneticFieldSingleton.h>
#include <rave/impl/DataFormats/GeometryCommonDetAlgo/interface/PerpendicularBoundPlaneBuilder.h>
#include <rave/impl/DataFormats/GeometryVector/interface/GlobalVector.h>
#include <rave/impl/TrackingTools/TrajectoryParametrization/interface/GlobalTrajectoryParameters.h>
#include <rave/impl/TrackingTools/TrajectoryState/interface/TrajectoryStateOnSurface.h>
#include "VertexGunPropagator.h"

using namespace std;

namespace {
  bool describe()
  {
    bool descr = SimpleConfigurable<bool>
      (false,"VertexFastSim:GunDescription").value();
    return descr;
  }
}

DirectionalParticleGun::~DirectionalParticleGun() {}

void DirectionalParticleGun::clear()
{
  theTracks.clear();
  theVertices.clear();
}

void DirectionalParticleGun::shoot()
{
  clear();
  GlobalPoint mypos ( theGp.x() + CLHEP::RandGauss::shoot ( 0 , thePosSigma.x() ),
                      theGp.y() + CLHEP::RandGauss::shoot ( 0 , thePosSigma.y() ),
                      theGp.z() + CLHEP::RandGauss::shoot ( 0 , thePosSigma.z() ));
  // HepLorentzVector vertexpos ( mypos.x(), mypos.y(), mypos.z(), 0. );
  int id=0;

  const unsigned ntracks = 
    (int) CLHEP::RandFlat::shoot ( theMinNtracks, theMaxNtracks+1 );
  rave::Point3D pos ( mypos.x(), mypos.y(), mypos.z() );
  float pt=0.5;
  unsigned ctr=0;
  while ( theTracks.size() < ntracks )
  {
    ctr++;
    vertigo::SimTrack trk = createATrack ( ntracks, id++, mypos );
    if ( trk.state().momentum().mag() >= pt )
    {
      theTracks.push_back ( trk );
    };
    if ( ctr > ntracks * 20 )
    {
      cout << "[DirectionalParticleGun] error: most tracks produced have too low pt!"
           << endl;
      break;
    }
  };
  if ( theTracks.size() > 1 )
  {
    vertigo::SimVertex vtx ( pos, theTracks, 0, "", theIsPrimaryVertex );
    /* nono, mustnt do. screws association _AND_ reference counting (?!?)
    for ( vector< vertigo::SimTrack >::iterator i=theTracks.begin(); 
          i!=theTracks.end() ; ++i )
    {
      i->setVertex ( &vtx );
    } */
    theVertices.push_back ( vtx );
  }
}

// create one (out of <ntracks>) tracks, id=<id>.
vertigo::SimTrack DirectionalParticleGun::createATrack( const int ntracks, int id,
    const GlobalPoint & vpos )
{
  try {
    signed int charge = CLHEP::RandGauss::shoot() < 0 ? -1 : 1;

    GlobalTrajectoryParameters tmp ( vpos, theAvgTotMom, charge, MagneticFieldSingleton::Instance() );
    GlobalTrajectoryParameters gtp = VertexGunPropagator::propagate ( tmp );
    const BoundPlane *bp_mom = PerpendicularBoundPlaneBuilder()( gtp.position(), gtp.momentum() 
        /* theAvgTotMom*/ );
    TrajectoryStateOnSurface tsos_mom( gtp , *bp_mom  );
    float spread = CLHEP::RandFlat::shoot ( 0. , theSpread );
    const double lphi=CLHEP::RandFlat::shoot ( 0. , 2 * M_PI );
    double r;
    do {
      r = CLHEP::RandGauss::shoot ( tsos_mom.localMomentum().z() / ntracks ,
                             theEnergeticSpread );
    } while ( r < 0.0 ); // no negative momenta.
    LocalPoint lp ( 0., 0., 0. );
    LocalVector lv_mom ( r * sin ( spread ) * cos ( lphi ) ,
                         r * sin ( spread ) * sin ( lphi ) , r * cos ( spread ) );
    LocalTrajectoryParameters ltp_mom( lp, lv_mom, charge );
    TrajectoryStateOnSurface newtsos_mom ( ltp_mom, *bp_mom, MagneticFieldSingleton::Instance() );
    rave::Vector6D state = CmsToRaveObjects().convert( newtsos_mom.globalParameters() );
    // cout << "[DirectionalParticleGun] created at " << gtp.position().x() << endl;
    // cout << "[DirectionalParticleGun] produced at " << state.x() << endl;
    return vertigo::SimTrack ( state, (rave::Charge) charge, NULL, 0, "" );
  }
  catch ( ... )
  {
    cout << "Exception in " << __FILE__ << ": " << __LINE__ << endl;
    cout << "Dunno what to do with it." << endl;
  };
  rave::Vector6D state ( 0., 0., 0., 0., 0., 0. );
  return vertigo::SimTrack ( state, (rave::Charge) 0, 0, 0, "illegal" );
}

void DirectionalParticleGun::printDescription( std::string name ) const
{
  if ( !describe() ) return;
  cout << "DirectionalParticleGun ";
  if ( !name.empty() ) { cout << "``" << name << "'' "; };
  cout << "instantiated: " << endl;
  cout << "   Tracks: between " << theMinNtracks << " and " << theMaxNtracks 
       << endl;
  cout << "   Vertex Position: " << theGp << " +- " << thePosSigma << endl;
  cout << "    Total Momentum: " << theAvgTotMom << " +- " << theEnergeticSpread 
       << " GeV per track." << endl;
  cout << "    Angular Spread: " << theSpread << endl;
}

DirectionalParticleGun::DirectionalParticleGun( const int min_ntracks,
    const int max_ntracks, 
    const GlobalPoint & gp, const GlobalPoint & pos_sigma, 
    const GlobalVector & atm, const float spread, const float e_spread,
    const std::string & name, bool is_prim
    ) :
  theMinNtracks (min_ntracks) , theMaxNtracks (max_ntracks), 
  theGp (gp) , theAvgTotMom (atm), theSpread ( spread ),
  theEnergeticSpread ( e_spread ), thePosSigma(pos_sigma),
  theName ( name ), theIsPrimaryVertex ( is_prim )
{
  // printDescription( name );
  // shoot();
}

string DirectionalParticleGun::name() const
{
  return theName;
}

vector< vertigo::SimTrack > DirectionalParticleGun::tracks() const
{
  return theTracks;
}

std::vector< vertigo::SimParticle > DirectionalParticleGun::particles() const
{
  return std::vector< vertigo::SimParticle >();
}

vector< vertigo::SimVertex > DirectionalParticleGun::vertices() const
{
  return theVertices;
}

DirectionalParticleGun * DirectionalParticleGun::clone() const
{
  return new DirectionalParticleGun ( * this );
}

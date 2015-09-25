#include "Vertigo/TrackSmearing/interface/GaussianRecTrackSmearer.h"
#include <vector>
#include <rave/Vertex.h>
#include <rave/impl/RaveBase/Converters/interface/CmsToRaveObjects.h>
#include <rave/impl/RaveTools/Converters/interface/MagneticFieldSingleton.h>
#include <rave/impl/DataFormats/GeometryCommonDetAlgo/interface/PerpendicularBoundPlaneBuilder.h>
#include <rave/impl/TrackingTools/TrajectoryState/interface/TrajectoryStateOnSurface.h>
#include <rave/impl/TrackingTools/PatternTools/interface/TransverseImpactPointExtrapolator.h>
#include "Environment/Utilities/interface/VertigoStreamers.h"
// #include "TrackingTools/TrajectoryParametrization/interface/LocalTrajectoryParameters.h"
// #include "TrackingTools/TrajectoryParametrization/interface/LocalTrajectoryError.h"

// #define DEBUG
#ifdef DEBUG
#include "TrackingTools/TrajectoryState/interface/PerigeeConversions.h"
#include <dataharvester/Writer.h>
#endif


using namespace std;
using namespace CLHEP;

namespace
{

enum COORDINATES { INVP = 1, DIRX = 2, DIRY = 3, POSX = 4, POSY = 5 };

template < class T> T square ( T a )
{
  return a*a;
}
}

void GaussianRecTrackSmearer::printDescription() const
{
  cout << "   Position Smearing   (cm): " << setw ( 7 ) << thePosDist.sigma()
  << " error matrix: " << thePosSigmaErrMatrix << endl;
  cout << "   Direction Smearing (rad): " << setw ( 7 ) << theDirDist.sigma()
  << " error matrix: " << theDirSigmaErrMatrix << endl;
  cout << "  Momentum Smearing (ratio): " << setw ( 7 ) << theInvPDist.sigma()
  << " error matrix: " << theInvPSigmaErrMatrix << endl;
}

GaussianRecTrackSmearer * GaussianRecTrackSmearer::clone () const
{
  return new GaussianRecTrackSmearer ( *this );
}

GaussianRecTrackSmearer::GaussianRecTrackSmearer (
  float pos_sgm , float dir_sgm , float inv_mom_sgm,
  float pos_sgm_em , float dir_sgm_em, float inv_mom_sgm_em ) :
    thePosDist ( 0., pos_sgm ), theDirDist ( 0., dir_sgm ),
    theInvPDist ( 0., inv_mom_sgm ), theGenerator(),
    thePosSigmaErrMatrix ( pos_sgm_em ), theDirSigmaErrMatrix ( dir_sgm_em ),
    theInvPSigmaErrMatrix ( inv_mom_sgm_em )
{}

GaussianRecTrackSmearer::GaussianRecTrackSmearer ( const GaussianRecTrackSmearer & o ) :
    thePosDist ( o.thePosDist ), theDirDist ( o.theDirDist ),
    theInvPDist ( o.theInvPDist ), theGenerator(),
    thePosSigmaErrMatrix ( o.thePosSigmaErrMatrix ),
    theDirSigmaErrMatrix ( o.theDirSigmaErrMatrix ),
    theInvPSigmaErrMatrix ( o.theInvPSigmaErrMatrix )
{}

GaussianRecTrackSmearer::GaussianRecTrackSmearer (
  float pos_sgm , float dir_sgm , float inv_mom_sgm ) :
    thePosDist ( 0., pos_sgm ), theDirDist ( 0., dir_sgm ),
    theInvPDist ( 0., inv_mom_sgm ), theGenerator(),
    thePosSigmaErrMatrix ( pos_sgm ), theDirSigmaErrMatrix ( dir_sgm ),
    theInvPSigmaErrMatrix ( inv_mom_sgm )
{}

GlobalTrajectoryParameters GaussianRecTrackSmearer::meanParameters() const
{
  return GlobalTrajectoryParameters ();
}

AlgebraicVector GaussianRecTrackSmearer::createRandoms (
  const GlobalTrajectoryParameters & gp ) const
{
  // create the random "deltas".
  AlgebraicVector ret ( 5 );
  float tmp = normal_generator_type( theGenerator, theInvPDist )();
  ret ( INVP ) = tmp;

  //float p = gp.momentum().mag();
  //float pz = gp.momentum().z();
  // float pt = gp.momentum().perp();

  // FIXME sq2 is empiric and works poorly!
  /*
  static const float sq2 = sqrt(2. * pt / p );
  float dtsigma = sq2 * thePosSigma;
  float dt = RandGauss::shoot ( 0., dtsigma );
  float phi_pos = RandFlat::shoot ( - M_PI, M_PI );
  ret(POSX) = dt * cos ( phi_pos );
  ret(POSY) = dt * sin ( phi_pos );
  */
  ret ( POSX )  = normal_generator_type( theGenerator, thePosDist )();
  ret ( POSY )  = normal_generator_type( theGenerator, thePosDist )();

  /*
  float ddir = RandGauss::shoot ( 0., theDirSigma );
  float phi_dir = RandFlat::shoot ( 0., 2 * M_PI );
  ret(DIRX) = ddir * cos ( phi_dir );
  ret(DIRY) = ddir * sin ( phi_dir );
  */
  ret ( DIRX )  = normal_generator_type( theGenerator, theDirDist )();
  ret ( DIRY )  = normal_generator_type( theGenerator, theDirDist )();
  return ret;
}

rave::Track
GaussianRecTrackSmearer::recTrack ( const GlobalTrajectoryParameters & gp ) const
{
  const BoundPlane *bp = PerpendicularBoundPlaneBuilder() ( gp.position(),
                         gp.momentum() );
  TrajectoryStateOnSurface tsos ( gp, *bp );
  float mom = tsos.localMomentum().z(); // this really is the total momentum!
  // cout << "[GaussianRecTrackSmearer] mom=" << mom << endl;
  if ( !finite ( mom ) )
  {
    cout << "[GaussianRecTrackSmearer] mom=" << mom << endl;
    cout << "[GaussianRecTrackSmearer] momentum=" << gp.momentum() << endl;
  }

  // std::vector diff, vector that contains the deviation
  // measured - true, in the coordinates
  // dpinv, dx, dy, dxdir, dydir
  AlgebraicVector diff = createRandoms ( gp );

  LocalPoint lPt ( diff ( POSX ), diff ( POSY ), 0. );
  // LocalPoint lPt ( 0., 0., 0. );

  // cout << "[GaussianRecTrackSmearer] lpt=" << lPt << endl;

  /*
   * now make sure that the length of the lVec is distributed
   * symmetrically around |dz|, with the right sigma.
   */

  float newp = mom + mom * mom * diff ( INVP ); // new total momentum

  float dpx = diff ( DIRX ) * newp; // delta of px (local coords)
  float dpy = diff ( DIRY ) * newp; // delta of py (local coords)

  // compute the new z component, such that the total
  // length of the std::vector is newp
  float dpz = sqrt ( newp * newp - dpx * dpx - dpy * dpy );

  /*
   *  and here is the momentum std::vector,
   *  with a total length of newp
   */
  LocalVector lVec ( dpx, dpy, dpz );
  LocalTrajectoryParameters ltp ( lPt, lVec, gp.charge() );
  LocalTrajectoryError lErr ( thePosSigmaErrMatrix, thePosSigmaErrMatrix,
                              theDirSigmaErrMatrix, theDirSigmaErrMatrix, theInvPSigmaErrMatrix );

  TrajectoryStateOnSurface newtsos ( ltp, lErr, *bp, MagneticFieldSingleton::Instance() );

  /*
  TransverseImpactPointExtrapolator tipe ( MagneticFieldSingleton::Instance() );
  cout << "[GaussianRecTrackSmearer] why? " << newtsos << endl;
  TrajectoryStateOnSurface propagated = tipe.extrapolate ( newtsos, GlobalPoint(1.,0.,0.) );
  cout << "[GaussianRecTrackSmearer] why why?" << endl;
  */

#ifdef DEBUG
  // cout << "[GaussianRecTrackSmearer DEBUG] st(dpinv)=" << diff[0]/sqrt(cov[0][0]) << endl;
  {
    dataharvester::Tuple r ( "d" );
    float dx = diff ( POSX );
    float dy = diff ( POSY );
    r["invp"] = diff ( INVP );
    r["dx"] = dx;
    r["dy"] = dy;
    r["dxdir"] = diff ( DIRX );
    r["dydir"] = diff ( DIRY );

    float dt = sqrt ( dx * dx + dy * dy );

    GlobalVector vec = newtsos.globalPosition() - GlobalPoint ( 0., 0., 0. );
    float phi = vec.phi();
    if ( phi < 0. ) phi += 2 * M_PI;

    float signdt = ( phi > M_PI ) ? -1. : 1.;
    float pz = gp.momentum().z();
    float p = fabs ( mom );
    float tip = dt * pz / p;

    r["tip"] = signdt * tip;
    r["dt"] = signdt * dt;
    r["p"] = p;
    r["pz"] = pz;
    /*
    r["phi_sim"]=(double) (gp.momentum().phi());

    TrajectoryStateOnSurface simip ( gp, newtsos.surface() );
    r["phi_sim_2"]=simip.globalMomentum().phi();
    cout << "[GaussianRecTrackSmearer] debug: sim gtp=" << gp << endl;
    cout << "[GaussianRecTrackSmearer] debug: simip=" << simip << endl;

    r["phi_rec"]=newtsos.globalMomentum().phi();
    r["phi"]=newtsos.globalMomentum().phi()-gp.momentum().phi();
    */
    dataharvester::Writer::file ( "devs.root" ) << r;

    PerigeeConversions conv;
    dataharvester::Tuple r2 ( "perigee" );
    double pt = 0.;
    PerigeeTrajectoryParameters pars = conv.ftsToPerigeeParameters ( * ( newtsos.freeState() ),
                                       GlobalPoint ( 0., 0., 0. ), pt );
    r2["theta"] = pars.theta();
    r2["phi"] = pars.phi();
    r2["tip"] = pars.transverseImpactParameter();
    r2["lip"] = pars.longitudinalImpactParameter();
    r2["curv"] = pars.transverseCurvature();
    dataharvester::Writer::file ( "devs.root" ) << r2;
  }

  {
    /*
    AlgebraicSymMatrix errcov = createCov ( thePosSigmaErrMatrix,
                                            theDirSigmaErrMatrix,
                                            theInvPSigmaErrMatrix );
    dataharvester::Tuple s("st");
    s["dinvp"]=diff(INVP)/sqrt(errcov(INVP,INVP));
    s["dx"]=diff(POSX)/sqrt(errcov(POSX,POSX));
    s["dy"]=diff(POSY)/sqrt(errcov(POSY,POSY));
    s["dxdir"]=diff(DIRX)/sqrt(errcov(DIRX,DIRX));
    s["dydir"]=diff(DIRY)/sqrt(errcov(DIRY,DIRY));
    dataharvester::Writer::file("devs.root") << s;
    */

    dataharvester::Tuple t ( "MCst" );
    t["dinvp"] = diff ( INVP ) / theInvPDist.sigma();
    t["dx"] = diff ( POSX ) / thePosDist.sigma();
    t["dy"] = diff ( POSY ) / thePosDist.sigma();
    t["dxdir"] = diff ( DIRX ) / theDirDist.sigma();
    t["dydir"] = diff ( DIRY ) / theDirDist.sigma();
    /*
    t["phi"]=( newtsos.globalMomentum().phi()-gp.momentum().phi() ) /
               sqrt ( newtsos.curvilinearError().matrix_old()(3,3) );
               */
    dataharvester::Writer::file ( "devs.root" ) << t;
  }
#endif

  return CmsToRaveObjects().convert ( newtsos );
}

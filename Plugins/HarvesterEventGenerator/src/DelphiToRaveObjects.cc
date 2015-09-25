/** @file
 *
 * This implements the obvious converter class.
 * The #defines referenced here can be found (and chosen)
 * in the associated header file.
 */

#include "DelphiToRaveObjects.h"

#include <rave/TransientTrackKinematicParticle.h>

#include "CLHEP/Matrix/Matrix.h"
#include "CLHEP/Matrix/SymMatrix.h"

#include <boost/numeric/conversion/cast.hpp>
#include <boost/numeric/ublas/symmetric.hpp>
#include <boost/numeric/ublas/io.hpp>

#include <exception>
#include <vector>
#include <utility>

using namespace std;

namespace {
static const float cm = 0.1F;
}

namespace vertigo {

DelphiToRaveObjects::DelphiToRaveObjects ( float Bz ) : theBz ( Bz ),
    theHasOffset ( false ), theGenerator ( 42u ),
    theXDelta ( 0 ), theYDelta ( 0 ), theZDelta ( 0 ),
    theCurX ( 0. ), theCurY ( 0. ), theCurZ ( 0. ),
    theHasBeamspot ( false )
{}

DelphiToRaveObjects::DelphiToRaveObjects (
  const rave::Ellipsoid3D & beamspot, float Bz ) :
    theBz ( Bz ), theHasOffset ( false ), theGenerator ( 42u ),
    theXDelta ( 0 ), theYDelta ( 0 ), theZDelta ( 0 ),
    theCurX ( 0. ), theCurY ( 0. ), theCurZ ( 0. ),
    theBeamspot( beamspot ), theHasBeamspot( true )
{}

DelphiToRaveObjects::DelphiToRaveObjects ( float Bz,
    float x, float y, float z ) :
    theBz ( Bz ), theHasOffset ( false ), theGenerator ( 42u ),
    theXDelta ( 0 ), theYDelta ( 0 ), theZDelta ( 0 ),
    theCurX ( 0. ), theCurY ( 0. ), theCurZ ( 0. ),
    theHasBeamspot( false )
{
  defineOffset ( x, y, z );
}

DelphiToRaveObjects::~DelphiToRaveObjects()
{
  if ( theXDelta ) delete theXDelta;
  if ( theYDelta ) delete theYDelta;
  if ( theZDelta ) delete theZDelta;
}

void DelphiToRaveObjects::defineOffset ( float x, float y, float z )
{
  // cout << "[DelphiToRaveObjects] ::defineOffset not implemented!" << endl;
  if ( theXDelta ) delete theXDelta;
  if ( theYDelta ) delete theYDelta;
  if ( theZDelta ) delete theZDelta;
  theHasOffset = true;
  theXDelta = new normal_generator_type ( theGenerator, boost::normal_distribution< float > ( 0., x ) );
  theYDelta = new normal_generator_type ( theGenerator, boost::normal_distribution< float > ( 0., y ) );
  theZDelta = new normal_generator_type ( theGenerator, boost::normal_distribution< float > ( 0., z ) );
}

void DelphiToRaveObjects::defineBz ( float bz )
{
  theBz = bz;
}

void DelphiToRaveObjects::defineBeamspot( const rave::Ellipsoid3D & beamspot )
{
  theHasBeamspot = true;
  theBeamspot = beamspot;
}

void DelphiToRaveObjects::createOffset()
{
  if ( theHasOffset )
  {
    theCurX = ( *theXDelta ) ();
    theCurY = ( *theYDelta ) ();
    theCurZ = ( *theZDelta ) ();
  };
}

/**
 * The actual conversion code. If there are problems with the LCIO
 * event generator, look here first.
 *
 * @param lcTrack The LCIO Track as defined in their header
 * @return The generated Rave Track (not all fields are used)
 */
rave::Track DelphiToRaveObjects::toTrack ( 
    dataharvester::Tuple & lcTrack ) const
{
  // Implementation of the L3->Rave parameter conversion
  float kappa = (float) (2.99792458 / 1000. * cm);

  signed charge = signed ( copysign ( 1.0, lcTrack["omega"].asDouble() * theBz ) );
  float lPT = fabs ( float(charge) * theBz / lcTrack["omega"].asFloat() * kappa );
  // float lPT = lcTrack->getPt();
  //double refx=lcTrack->getReferencePoint()[0];
  //double refy=lcTrack->getReferencePoint()[1];
  //double refz=lcTrack->getReferencePoint()[2];

  rave::Vector6D s ( theCurX + /* cm * refx */ - cm * lcTrack["d0"].asFloat() * sin ( lcTrack["phi"].asDouble() ) ,
                     theCurY + /* cm * refy */ + cm * lcTrack["d0"].asFloat() * cos ( lcTrack["phi"].asDouble() ) ,
                     theCurZ + /* cm * refz */ + cm * lcTrack["z0"].asFloat() ,
                     lPT * cos ( lcTrack["phi"].asFloat() ) ,
                     lPT * sin ( lcTrack["phi"].asFloat() ) ,
                     lPT * lcTrack["tanlambda"].asFloat() ) ;

  float ldPTdOmega = - lPT / lcTrack["omega"].asDouble();
  // float ldPTdOmega = (-1.0) * charge * theBz * kappa / lcTrack["omega"] / lcTrack["omega"];

  CLHEP::HepSymMatrix bCov ( 5 ) ;
  CLHEP::HepMatrix brJacob ( 6, 5 ) ;
  CLHEP::HepMatrix rCov ( 6, 6 ) ;

  // order is d0, phi, omega, z0, tan(lambda)
  // says: http://lcio.desy.de/v01-08/doc/api/hep/lcio/event/Track.html#getCovMatrix()

  enum { D0 = 0, PHI0 = 1, OMEGA = 2, Z0 = 3, TANLAMBDA = 4 };
  enum { X = 0, Y = 1, Z = 2, PX = 3, PY = 4, PZ = 5 };

  // WARNING we assume that the covariance matrix is ordered:
  // d0, phi0, omega, z0, tanlambda
  bCov[D0][D0]               = lcTrack["dd0d0"]; // d0 d0
  bCov[D0][PHI0]             = lcTrack["dphiphi"]; // phi d0
  bCov[PHI0][PHI0]           = lcTrack["dphiphi"]; // phi phi
  bCov[D0][OMEGA]            = lcTrack["domegad0"]; // omega d0
  bCov[PHI0][OMEGA]          = lcTrack["domegaphi"]; // omega phi
  bCov[OMEGA][OMEGA]         = lcTrack["domegaomega"]; // omega omega
  bCov[D0][Z0]               = lcTrack["dz0d0"]; // z0 d0
  bCov[PHI0][Z0]             = lcTrack["dz0phi"]; // z0 phi
  bCov[OMEGA][Z0]            = lcTrack["dz0omega"]; // z0 omega
  bCov[Z0][Z0]               = lcTrack["dz0z0"]; // z0 z0
  bCov[D0][TANLAMBDA]        = lcTrack["dtanlambdad0"]; // tanlambda d0
  bCov[PHI0][TANLAMBDA]      = lcTrack["dtanlambdaphi"]; // tanlambda phi
  bCov[OMEGA][TANLAMBDA]     = lcTrack["dtanlambdaomega"]; // tanlambda omega
  bCov[Z0][TANLAMBDA]        = lcTrack["dtanlambdaz0"]; // tanlambda z0
  bCov[TANLAMBDA][TANLAMBDA] = lcTrack["dtanlambdatanlambda"]; // tanlambda tanlambda

  /*
  if ( bCov[4][4] > 10. )
  {
    cout << "[LCIOEventGenerator] WHAT??? " << lcTrack->getType() << endl;
    throw std::string ( "malformed track!" );
  }*/

  brJacob[X][D0] = -cm * sin ( lcTrack["phi"].asDouble() ) ; // x d0
  brJacob[Y][D0] = cm * cos ( lcTrack["phi"].asDouble() ) ; // y d0
  brJacob[Z][D0] = 0.0 ; // z d0
  brJacob[PX][D0] = 0.0 ; // px d0
  brJacob[PY][D0] = 0.0 ; // py d0
  brJacob[PZ][D0] = 0.0 ; // pz d0

  brJacob[X][PHI0] = -cm * lcTrack["d0"].asDouble() * cos ( lcTrack["phi"].asDouble() ) ; // x phi
  brJacob[Y][PHI0] = -cm * lcTrack["d0"].asDouble() * sin ( lcTrack["phi"].asDouble() ) ; // y phi
  brJacob[Z][PHI0] = 0.0 ; // z phi
  brJacob[PX][PHI0] = ( -1.0 ) * lPT * sin ( lcTrack["phi"].asDouble() ) ; // px phi
  brJacob[PY][PHI0] = ( + 1.0 ) * lPT * cos ( lcTrack["phi"].asDouble() ) ; // py phi
  brJacob[PZ][PHI0] = 0.0 ; // pz phi

  brJacob[X][OMEGA] = 0.0 ; // x omega?
  brJacob[Y][OMEGA] = 0.0 ; // y omega?
  brJacob[Z][OMEGA] = 0.0 ; // z omega?
  brJacob[PX][OMEGA] = ldPTdOmega * cos ( lcTrack["phi"].asDouble() ) ; // px omega?
  brJacob[PY][OMEGA] = ldPTdOmega * sin ( lcTrack["phi"].asDouble() ) ; // py omega?
  brJacob[PZ][OMEGA] = ldPTdOmega * lcTrack["tanlambda"].asDouble(); // lcTrack["omega"] ; // pz omega?

  brJacob[X][Z0] = 0.0 ; // x z0
  brJacob[Y][Z0] = 0.0 ; // y z0
  brJacob[Z][Z0] = cm ; // z z0
  brJacob[PX][Z0] = 0.0 ; // px z0
  brJacob[PY][Z0] = 0.0 ; // py z0
  brJacob[PZ][Z0] = 0.0 ; // pz z0

  brJacob[X][TANLAMBDA] = 0.0 ; // x tanlambda
  brJacob[Y][TANLAMBDA] = 0.0 ; // y tanlambda
  brJacob[Z][TANLAMBDA] = 0.0 ; // z tanlambda
  brJacob[PX][TANLAMBDA] = 0.0 ; // px tanlambda
  brJacob[PY][TANLAMBDA] = 0.0 ; // py tanlambda
  brJacob[PZ][TANLAMBDA] = lPT ; // pz tanlambda

  rCov = brJacob * bCov * brJacob.T() ;

  /*
    cout << endl;
    cout << "[LCIOEventGenerator] bCov="<< bCov << endl;
    cout << "[LCIOEventGenerator] bCov="<< bCov.determinant() << endl;
    cout << "[LCIOEventGenerator] type=" << lcTrack->getType() << endl;
    cout << "[LCIOEventGenerator] ndf=" << lcTrack->getNdf() << endl;
    cout << "[LCIOEventGenerator] s: "<< s << endl ;
    cout << "[LCIOEventGenerator] Cov: "<< rCov << " det=" << rCov.determinant() << endl ;
    cout << "[LCIOEventGenerator] refx [cm]= " << 0.1 * refx << endl;
    cout << "[LCIOEventGenerator] mCov=" << rCov[2][2]*rCov[5][5]-rCov[2][5]*rCov[5][2] << endl;
    */

  rave::Covariance6D err ( float(rCov[X][X]), rCov[X][Y], rCov[X][Z],
                           rCov[Y][Y], rCov[Y][Z],
                           rCov[Z][Z],
                           rCov[X][PX],  rCov[X ][PY], rCov[X ][PZ],
                           rCov[Y][PX],  rCov[Y ][PY], rCov[Y ][PZ],
                           rCov[Z][PX],  rCov[Z ][PY], rCov[Z ][PZ],
                           rCov[PX][PX], rCov[PX][PY], rCov[PX][PZ],
                           rCov[PY][PY], rCov[PY][PZ],
                           rCov[PZ][PZ] ) ;

  // origin = lcTrack;
  rave::Track trk ( s, err, charge, lcTrack["chi2"], lcTrack["ndf"], 0, lcTrack["tag"] );
                    // dynamic_cast<void *>( NULL ), tag );

  return trk;
}

void DelphiToRaveObjects::printOffset() const
{
  std::cout << "[DelphiToRaveObjects] offset: (" << theCurX << ", " << theCurY 
            << ", " << theCurZ << ")" << std::endl;
}

rave::Point3D DelphiToRaveObjects::toPoint ( dataharvester::Tuple & position ) const
{
  return rave::Point3D ( theCurX + position["x"].asFloat()  * cm, 
                         theCurY + position["y"].asFloat()  * cm,
                         theCurZ + position["z"].asFloat()  * cm );
}

rave::Covariance3D DelphiToRaveObjects::toCov3D ( 
    dataharvester::Tuple & cov ) const
{
  float cm2 = cm*cm;
  return rave::Covariance3D(
      (float)(cov["dxx"].asDouble()) * cm2, (float)(cov["dxy"].asDouble()) * cm2, (float) (cov["dxz"].asDouble()) * cm2,
                    (float) ( cov["dyy"].asDouble() ) * cm2, (float) (cov["dyz"].asDouble()) * cm2,
                                  float ( cov["dzz"].asDouble() ) * cm2);
}

rave::KinematicParticle DelphiToRaveObjects::toKinParticle (
    dataharvester::Tuple & lcioParticle ) const
{
  // First build a rave::Track

  std::vector< float > lcioPosition( 3 );
  std::vector< float > lcioPositionError( 6 );

  // Sometimes no StartVertex is provided. Use the beam spot information in 
  // that case
  if ( lcioParticle.hasValue("x") )
  {
    lcioPosition[0] = lcioParticle["x"];
    lcioPosition[1] = lcioParticle["y"];
    lcioPosition[2] = lcioParticle["z"];
    lcioPositionError[0] = lcioParticle["dxx"];
    lcioPositionError[1] = lcioParticle["dxy"];
    lcioPositionError[2] = lcioParticle["dxz"];
    lcioPositionError[3] = lcioParticle["dyy"];
    lcioPositionError[4] = lcioParticle["dyz"];
    lcioPositionError[5] = lcioParticle["dzz"];
  }
  else if ( theHasBeamspot )
  {
    lcioPosition[0] = theBeamspot.point().x();
    lcioPosition[1] = theBeamspot.point().y();
    lcioPosition[2] = theBeamspot.point().z();
    lcioPositionError[0] = theBeamspot.covariance().dxx();
    lcioPositionError[1] = theBeamspot.covariance().dxy();
    lcioPositionError[3] = theBeamspot.covariance().dxz();
    lcioPositionError[2] = theBeamspot.covariance().dyy();
    lcioPositionError[4] = theBeamspot.covariance().dyz();
    lcioPositionError[5] = theBeamspot.covariance().dzz();
  }
  else
  {
    throw std::string( "Particle conversion failed because of missing position information. Try to define a beamspot to provide proper defaults." );
  }

  // const double * lcioMomentum = lcioParticle->getMomentum();
  const float lcioEnergy = lcioParticle["e"];
  
#define CALC_MASS_FROM_ENERGY
#if defined CALC_MASS_FROM_ENERGY
  const float lcioMass2 = lcioEnergy*lcioEnergy
                           - lcioParticle["px"].asDouble()*lcioParticle["px"].asDouble()
                           - lcioParticle["py"].asDouble()*lcioParticle["py"].asDouble() 
                           - lcioParticle["pz"].asDouble()*lcioParticle["pz"].asDouble();
  float lcioMass = lcioParticle["m"]; // Default to given mass
  if (lcioMass2 > 0.) {
    lcioMass = sqrt(lcioMass2);
    std::cout << "[DelphiToRaveObjects]  Calculated particle mass from energy: " 
              << lcioEnergy << " -> " << lcioMass << std::endl;
  }
#else
  const float lcioMass     = lcioParticle["m"];
#endif

  rave::Vector7D raveState ( lcioPosition[0], lcioPosition[1], lcioPosition[2],
                             lcioParticle["x"], lcioParticle["py"], lcioParticle["pz"],
                             lcioMass );

  // Do the error propagation
  // if ( cov.size() != 10 ) cov = lcioParticle->getCovMatrix();
  
  enum { PX = 0, PY = 1, PZ = 2, E = 3 };
  using namespace boost::numeric::ublas;
  symmetric_matrix<double, lower> lcioPECovMatrix ( 4, 4 );
  lcioPECovMatrix ( PX, PX ) = lcioParticle ["dpxpx"];
  lcioPECovMatrix ( PY, PX ) = lcioParticle ["dpypx"];
  lcioPECovMatrix ( PY, PY ) = lcioParticle ["dpypy"];
  lcioPECovMatrix ( PZ, PX ) = lcioParticle ["dpzpx"];
  lcioPECovMatrix ( PZ, PY ) = lcioParticle ["dpzpy"];
  lcioPECovMatrix ( PZ, PZ ) = lcioParticle ["dpzpz"];
  lcioPECovMatrix ( E,  PX ) = lcioParticle ["depx"];
  lcioPECovMatrix ( E,  PY ) = lcioParticle ["depy"];
  lcioPECovMatrix ( E,  PZ ) = lcioParticle ["depz"];
  lcioPECovMatrix ( E,  E ) = lcioParticle ["dee"];

  //cout << "Energy: " << lcioEnergy << "; Mass: " << lcioMass << endl;
  //cout << "PECov:" << endl << lcioPECovMatrix << endl;

  matrix<double> jacobian ( 4, 4 );
  jacobian = identity_matrix<double> ( 4 );
  jacobian ( 3, 0 ) = -lcioParticle["x"].asDouble() / lcioMass;
  jacobian ( 3, 1 ) = -lcioParticle["y"].asDouble() / lcioMass;
  jacobian ( 3, 2 ) = -lcioParticle["z"].asDouble() / lcioMass;
  jacobian ( 3, 3 ) = lcioEnergy / lcioMass;

  //cout << "Jac:" << endl << jacobian << endl;

  matrix<double> tempMatrix ( 4, 4 );
  matrix<double> ravePMCovMatrix ( 4, 4 );
  tempMatrix = prod ( lcioPECovMatrix, jacobian );
  ravePMCovMatrix = prod ( trans ( jacobian ), tempMatrix );
  
  //cout << "PMCov:" << endl << ravePMCovMatrix << endl;

  // Assert: ravePMCovMatrix is symmetric!

  rave::Covariance7D raveCovariance ( lcioParticle["dxx"],
                                      lcioParticle["dxy"],
                                      lcioParticle["dxz"],
                                      lcioParticle["dyy"],
                                      lcioParticle["dyz"],
                                      lcioParticle["dzz"],
                                      0., 0., 0., 0., 0., 0., 0., 0., 0.,
                                      (float) ( ravePMCovMatrix ( PX, PX ) ),
                                      (float) ( ravePMCovMatrix ( PX, PY ) ),
                                      (float) ( ravePMCovMatrix ( PX, PZ ) ),
                                      (float) ( ravePMCovMatrix ( PY, PY ) ),
                                      (float) ( ravePMCovMatrix ( PY, PZ ) ),
                                      (float) ( ravePMCovMatrix ( PZ, PZ ) ),
                                      0., 0., 0., 
                                      (float) ( ravePMCovMatrix ( E, PX ) ),
                                      (float) ( ravePMCovMatrix ( E, PY ) ),
                                      (float) ( ravePMCovMatrix ( E, PZ ) ),
                                      float ( ravePMCovMatrix ( E, E ) ) );

  rave::Charge raveCharge = boost::numeric_cast<rave::Charge> ( lcioParticle["q"].asInt() );

  // double massSigma   = sqrt ( ravePMCovMatrix ( 3, 3 ) );
  float chiSquared  = 1000;
  float degreesOfFr = 100;
  
  //cout << "Particle conversion: State: " << raveState << endl
  //     << raveCovariance << endl;

  // rave::Track initialTrack ( raveState, raveCovariance, raveCharge, chiSquared, degreesOfFr );
  // return rave::TransientTrackKinematicParticle ( initialTrack, lcioMass, massSigma, chiSquared, degreesOfFr );
  return rave::TransientTrackKinematicParticle ( raveState, raveCovariance, raveCharge, chiSquared, degreesOfFr );
}

rave::Vertex DelphiToRaveObjects::toVertex ( 
    dataharvester::Tuple & lcioVertex ) const
{
  rave::Point3D position = this->toPoint ( lcioVertex );
  rave::Covariance3D error = this->toCov3D ( lcioVertex );
  std::vector< std::pair< float, rave::Track > > tracks;
  return rave::Vertex( position, error, tracks, 0.0, (float) lcioVertex["chi2"].asDouble(), 
                       vector < pair < float, rave::Vertex > > () );
}

} // namespace vertigo


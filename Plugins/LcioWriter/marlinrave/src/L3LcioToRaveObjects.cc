/** @file
 *
 * This implements the obvious converter class.
 * The #defines referenced here can be found (and chosen)
 * in the associated header file.
 */

#include "L3LcioToRaveObjects.h"

#include <rave/TransientTrackKinematicParticle.h>

#include "CLHEP/Matrix/Matrix.h"
#include "CLHEP/Matrix/SymMatrix.h"

#include <boost/numeric/conversion/cast.hpp>
#include <boost/numeric/ublas/symmetric.hpp>
#include <boost/numeric/ublas/io.hpp>

#include <exception>
#include <vector>
#include <utility>

namespace {
static const float cm = 0.1;
}

namespace marlinrave {

L3LcioToRaveObjects::L3LcioToRaveObjects ( float Bz ) : theBz ( Bz ),
    theHasOffset ( false ), theGenerator ( 42u ),
    theXDelta ( 0 ), theYDelta ( 0 ), theZDelta ( 0 ),
    theCurX ( 0. ), theCurY ( 0. ), theCurZ ( 0. ),
    theHasBeamspot ( false )
{}

L3LcioToRaveObjects::L3LcioToRaveObjects (
  const rave::Ellipsoid3D & beamspot, float Bz ) :
    theBz ( Bz ), theHasOffset ( false ), theGenerator ( 42u ),
    theXDelta ( 0 ), theYDelta ( 0 ), theZDelta ( 0 ),
    theCurX ( 0. ), theCurY ( 0. ), theCurZ ( 0. ),
    theBeamspot( beamspot ), theHasBeamspot( true )
{}

L3LcioToRaveObjects::L3LcioToRaveObjects ( float Bz,
    float x, float y, float z ) :
    theBz ( Bz ), theHasOffset ( false ), theGenerator ( 42u ),
    theXDelta ( 0 ), theYDelta ( 0 ), theZDelta ( 0 ),
    theCurX ( 0. ), theCurY ( 0. ), theCurZ ( 0. ),
    theHasBeamspot( false )
{
  defineOffset ( x, y, z );
}

L3LcioToRaveObjects::~L3LcioToRaveObjects()
{
  if ( theXDelta ) delete theXDelta;
  if ( theYDelta ) delete theYDelta;
  if ( theZDelta ) delete theZDelta;
}

void L3LcioToRaveObjects::defineOffset ( float x, float y, float z )
{
  // cout << "[L3LcioToRaveObjects] ::defineOffset not implemented!" << endl;
  if ( theXDelta ) delete theXDelta;
  if ( theYDelta ) delete theYDelta;
  if ( theZDelta ) delete theZDelta;
  theHasOffset = true;
  theXDelta = new normal_generator_type ( theGenerator, boost::normal_distribution< float > ( 0., x ) );
  theYDelta = new normal_generator_type ( theGenerator, boost::normal_distribution< float > ( 0., y ) );
  theZDelta = new normal_generator_type ( theGenerator, boost::normal_distribution< float > ( 0., z ) );
}

void L3LcioToRaveObjects::defineBz ( float bz )
{
  theBz = bz;
}

void L3LcioToRaveObjects::defineBeamspot( const rave::Ellipsoid3D & beamspot )
{
  theHasBeamspot = true;
  theBeamspot = beamspot;
}

void L3LcioToRaveObjects::createOffset()
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
rave::Track L3LcioToRaveObjects::convert ( 
    EVENT::Track * lcTrack, EVENT::LCObject * origin, const std::string & tag ) const
{
  // Implementation of the L3->Rave parameter conversion
  double kappa = 2.99792458 / 1000. * cm;

  signed charge = signed ( copysign ( 1.0, lcTrack->getOmega() * theBz ) );
  double lPT = fabs ( charge * theBz / lcTrack->getOmega() * kappa );
  // float lPT = lcTrack->getPt();
  //double refx=lcTrack->getReferencePoint()[0];
  //double refy=lcTrack->getReferencePoint()[1];
  //double refz=lcTrack->getReferencePoint()[2];

  rave::Vector6D s ( theCurX + /* cm * refx */ - cm * lcTrack->getD0() * sin ( lcTrack->getPhi() ) ,
                     theCurY + /* cm * refy */ + cm * lcTrack->getD0() * cos ( lcTrack->getPhi() ) ,
                     theCurZ + /* cm * refz */ + cm * lcTrack->getZ0() ,
                     lPT * cos ( lcTrack->getPhi() ) ,
                     lPT * sin ( lcTrack->getPhi() ) ,
                     lPT * lcTrack->getTanLambda() ) ;

  double ldPTdOmega = - lPT / lcTrack->getOmega();
  // float ldPTdOmega = (-1.0) * charge * theBz * kappa / lcTrack->getOmega() / lcTrack->getOmega();

  CLHEP::HepSymMatrix bCov ( 5 ) ;
  CLHEP::HepMatrix brJacob ( 6, 5 ) ;
  CLHEP::HepMatrix rCov ( 6, 6 ) ;

  // order is d0, phi, omega, z0, tan(lambda)
  // says: http://lcio.desy.de/v01-08/doc/api/hep/lcio/event/Track.html#getCovMatrix()

  enum { D0 = 0, PHI0 = 1, OMEGA = 2, Z0 = 3, TANLAMBDA = 4 };
  enum { X = 0, Y = 1, Z = 2, PX = 3, PY = 4, PZ = 5 };

  // WARNING we assume that the covariance matrix is ordered:
  // d0, phi0, omega, z0, tanlambda
  bCov[D0][D0]               = lcTrack->getCovMatrix() [ 0] ; // d0 d0
  bCov[D0][PHI0]             = lcTrack->getCovMatrix() [ 1] ; // phi d0
  bCov[PHI0][PHI0]           = lcTrack->getCovMatrix() [ 2] ; // phi phi
  bCov[D0][OMEGA]            = lcTrack->getCovMatrix() [ 3] ; // omega d0
  bCov[PHI0][OMEGA]          = lcTrack->getCovMatrix() [ 4] ; // omega phi
  bCov[OMEGA][OMEGA]         = lcTrack->getCovMatrix() [ 5] ; // omega omega
  bCov[D0][Z0]               = lcTrack->getCovMatrix() [ 6] ; // z0 d0
  bCov[PHI0][Z0]             = lcTrack->getCovMatrix() [ 7] ; // z0 phi
  bCov[OMEGA][Z0]            = lcTrack->getCovMatrix() [ 8] ; // z0 omega
  bCov[Z0][Z0]               = lcTrack->getCovMatrix() [ 9] ; // z0 z0
  bCov[D0][TANLAMBDA]        = lcTrack->getCovMatrix() [10] ; // tanlambda d0
  bCov[PHI0][TANLAMBDA]      = lcTrack->getCovMatrix() [11] ; // tanlambda phi
  bCov[OMEGA][TANLAMBDA]     = lcTrack->getCovMatrix() [12] ; // tanlambda omega
  bCov[Z0][TANLAMBDA]        = lcTrack->getCovMatrix() [13] ; // tanlambda z0
  bCov[TANLAMBDA][TANLAMBDA] = lcTrack->getCovMatrix() [14] ; // tanlambda tanlambda

  /*
  if ( bCov[4][4] > 10. )
  {
    cout << "[LCIOEventGenerator] WHAT??? " << lcTrack->getType() << endl;
    throw std::string ( "malformed track!" );
  }*/

  brJacob[X][D0] = -cm * sin ( lcTrack->getPhi() ) ; // x d0
  brJacob[Y][D0] = cm * cos ( lcTrack->getPhi() ) ; // y d0
  brJacob[Z][D0] = 0.0 ; // z d0
  brJacob[PX][D0] = 0.0 ; // px d0
  brJacob[PY][D0] = 0.0 ; // py d0
  brJacob[PZ][D0] = 0.0 ; // pz d0

  brJacob[X][PHI0] = -cm * lcTrack->getD0() * cos ( lcTrack->getPhi() ) ; // x phi
  brJacob[Y][PHI0] = -cm * lcTrack->getD0() * sin ( lcTrack->getPhi() ) ; // y phi
  brJacob[Z][PHI0] = 0.0 ; // z phi
  brJacob[PX][PHI0] = ( -1.0 ) * lPT * sin ( lcTrack->getPhi() ) ; // px phi
  brJacob[PY][PHI0] = ( + 1.0 ) * lPT * cos ( lcTrack->getPhi() ) ; // py phi
  brJacob[PZ][PHI0] = 0.0 ; // pz phi

  brJacob[X][OMEGA] = 0.0 ; // x omega?
  brJacob[Y][OMEGA] = 0.0 ; // y omega?
  brJacob[Z][OMEGA] = 0.0 ; // z omega?
  brJacob[PX][OMEGA] = ldPTdOmega * cos ( lcTrack->getPhi() ) ; // px omega?
  brJacob[PY][OMEGA] = ldPTdOmega * sin ( lcTrack->getPhi() ) ; // py omega?
  brJacob[PZ][OMEGA] = ldPTdOmega * lcTrack->getTanLambda(); // lcTrack->getOmega() ; // pz omega?

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

  rave::Covariance6D err ( rCov[X][X], rCov[X][Y], rCov[X][Z],
                           rCov[Y][Y], rCov[Y][Z],
                           rCov[Z][Z],
                           rCov[X][PX],  rCov[X ][PY], rCov[X ][PZ],
                           rCov[Y][PX],  rCov[Y ][PY], rCov[Y ][PZ],
                           rCov[Z][PX],  rCov[Z ][PY], rCov[Z ][PZ],
                           rCov[PX][PX], rCov[PX][PY], rCov[PX][PZ],
                           rCov[PY][PY], rCov[PY][PZ],
                           rCov[PZ][PZ] ) ;

  if ( !origin ) origin = lcTrack;
  rave::Track trk ( s, err, charge, lcTrack->getChi2(), lcTrack->getNdf(),
                    dynamic_cast<void *>( origin ), tag );

  return trk;
}

void L3LcioToRaveObjects::printOffset() const
{
  std::cout << "[L3LcioToRaveObjects] offset: (" << theCurX << ", " << theCurY 
            << ", " << theCurZ << ")" << std::endl;
}

rave::Point3D L3LcioToRaveObjects::convert ( const float * position ) const
{
  return rave::Point3D ( theCurX + position[0] * cm, 
                         theCurY + position[1] * cm,
                         theCurZ + position[2] * cm );
}

rave::Point3D L3LcioToRaveObjects::convert ( const double * position ) const
{
  return rave::Point3D ( theCurX + position[0] * cm, 
                         theCurY + position[1] * cm,
                         theCurZ + position[2] * cm );
}

rave::Covariance3D L3LcioToRaveObjects::convert ( 
    const std::vector< float > cov ) const
{
  double cm2 = cm*cm;
  return rave::Covariance3D(
      cov[0] * cm2, cov[1] * cm2, cov[3] * cm2,
                    cov[2] * cm2, cov[4] * cm2,
                                  cov[5] * cm2);
}

rave::KinematicParticle L3LcioToRaveObjects::convert (
    EVENT::ReconstructedParticle * lcioParticle, std::vector< float > cov, 
    const std::string & tag ) const
{
  // First build a rave::Track

  std::vector< float > lcioPosition( 3 );
  std::vector< float > lcioPositionError( 6 );

  // Sometimes no StartVertex is provided. Use the beam spot information in 
  // that case
  EVENT::Vertex * lcioVertex   = lcioParticle->getStartVertex();
  if ( lcioVertex )
  {
    lcioPosition[0] = lcioVertex->getPosition()[0];
    lcioPosition[1] = lcioVertex->getPosition()[1];
    lcioPosition[2] = lcioVertex->getPosition()[2];
    lcioPositionError = lcioVertex->getCovMatrix();
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

  const double * lcioMomentum = lcioParticle->getMomentum();
  const double lcioEnergy = lcioParticle->getEnergy();
  
#define CALC_MASS_FROM_ENERGY
#if defined CALC_MASS_FROM_ENERGY
  const double lcioMass2 = lcioEnergy*lcioEnergy
                           - lcioMomentum[0]*lcioMomentum[0]
                           - lcioMomentum[1]*lcioMomentum[1]
                           - lcioMomentum[2]*lcioMomentum[2];
  double lcioMass = lcioParticle->getMass(); // Default to given mass
  if (lcioMass2 > 0.) {
    lcioMass = sqrt(lcioMass2);
    std::cout << "  Calculated particle mass from energy: " 
              << lcioEnergy << " -> " << lcioMass << std::endl;
  }
#else
  const double lcioMass     = lcioParticle->getMass();
#endif

  rave::Vector7D raveState ( lcioPosition[0], lcioPosition[1], lcioPosition[2],
                             lcioMomentum[0], lcioMomentum[1], lcioMomentum[2],
                             lcioMass );

  // Do the error propagation
  if ( cov.size() != 10 ) cov = lcioParticle->getCovMatrix();
  
  enum { PX = 0, PY = 1, PZ = 2, E = 3 };
  using namespace boost::numeric::ublas;
  symmetric_matrix<double, lower> lcioPECovMatrix ( 4, 4 );
  lcioPECovMatrix ( PX, PX ) = cov [0];
  lcioPECovMatrix ( PY, PX ) = cov [1];
  lcioPECovMatrix ( PY, PY ) = cov [2];
  lcioPECovMatrix ( PZ, PX ) = cov [3];
  lcioPECovMatrix ( PZ, PY ) = cov [4];
  lcioPECovMatrix ( PZ, PZ ) = cov [5];
  lcioPECovMatrix ( E,  PX ) = cov [6];
  lcioPECovMatrix ( E,  PY ) = cov [7];
  lcioPECovMatrix ( E,  PZ ) = cov [8];
  lcioPECovMatrix ( E,  E ) = cov [9];

  //cout << "Energy: " << lcioEnergy << "; Mass: " << lcioMass << endl;
  //cout << "PECov:" << endl << lcioPECovMatrix << endl;

  matrix<double> jacobian ( 4, 4 );
  jacobian = identity_matrix<double> ( 4 );
  jacobian ( 3, 0 ) = -lcioMomentum[0] / lcioMass;
  jacobian ( 3, 1 ) = -lcioMomentum[1] / lcioMass;
  jacobian ( 3, 2 ) = -lcioMomentum[2] / lcioMass;
  jacobian ( 3, 3 ) = lcioEnergy / lcioMass;

  //cout << "Jac:" << endl << jacobian << endl;

  matrix<double> tempMatrix ( 4, 4 );
  matrix<double> ravePMCovMatrix ( 4, 4 );
  tempMatrix = prod ( lcioPECovMatrix, jacobian );
  ravePMCovMatrix = prod ( trans ( jacobian ), tempMatrix );
  
  //cout << "PMCov:" << endl << ravePMCovMatrix << endl;

  // Assert: ravePMCovMatrix is symmetric!

  rave::Covariance7D raveCovariance ( lcioPositionError [0],
                                      lcioPositionError [1],
                                      lcioPositionError [3],
                                      lcioPositionError [2],
                                      lcioPositionError [4],
                                      lcioPositionError [5],
                                      0., 0., 0., 0., 0., 0., 0., 0., 0.,
                                      ravePMCovMatrix ( PX, PX ),
                                      ravePMCovMatrix ( PX, PY ),
                                      ravePMCovMatrix ( PX, PZ ),
                                      ravePMCovMatrix ( PY, PY ),
                                      ravePMCovMatrix ( PY, PZ ),
                                      ravePMCovMatrix ( PZ, PZ ),
                                      0., 0., 0., 
                                      ravePMCovMatrix ( E, PX ),
                                      ravePMCovMatrix ( E, PY ),
                                      ravePMCovMatrix ( E, PZ ),
                                      ravePMCovMatrix ( E, E ) );

  rave::Charge raveCharge = boost::numeric_cast<rave::Charge> ( lcioParticle->getCharge() );

  // double massSigma   = sqrt ( ravePMCovMatrix ( 3, 3 ) );
  double chiSquared  = 1000;
  double degreesOfFr = 100;
  
  //cout << "Particle conversion: State: " << raveState << endl
  //     << raveCovariance << endl;

  // rave::Track initialTrack ( raveState, raveCovariance, raveCharge, chiSquared, degreesOfFr );
  // return rave::TransientTrackKinematicParticle ( initialTrack, lcioMass, massSigma, chiSquared, degreesOfFr );
  return rave::TransientTrackKinematicParticle ( raveState, raveCovariance, raveCharge, chiSquared, degreesOfFr );
}

rave::Vertex L3LcioToRaveObjects::convert ( 
    const EVENT::Vertex * lcioVertex ) const
{
  rave::Point3D position = this->convert( lcioVertex->getPosition() );
  rave::Covariance3D error = this->convert( lcioVertex->getCovMatrix() );
  std::vector< std::pair< float, rave::Track > > tracks;
  return rave::Vertex( position, error, tracks, 0.0, lcioVertex->getChi2(), 
                       std::vector < std::pair < float, rave::Vertex > > () );
}

} // namespace marlinrave


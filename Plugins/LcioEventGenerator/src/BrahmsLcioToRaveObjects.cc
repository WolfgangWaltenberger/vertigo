/** @file
 *
 * This implements the obvious converter class. 
 * The #defines referenced here can be found (and chosen) 
 * in the associated header file.
 */

#include "BrahmsLcioToRaveObjects.h"
#include <rave/impl/DataFormats/CLHEP/interface/AlgebraicObjects.h>

/**
 * The actual conversion code. If there are problems with the LCIO
 * event generator, look here first.
 *
 * @param lcTrack The LCIO Track as defined in their header
 * @return The generated Rave Track (not all fields are used)
 */
rave::Track BrahmsLcioToRaveObjects::convert ( const EVENT::Track * lcTrack ) const
{
  /* mos : The hacked MarlinReco track reconstruction outputs Brahms parameters
   * .. so here they are converted to Rave convention
   */

  // for clearness, i introduce beautifully named variables
  float bInvP    = lcTrack->getOmega() ;
  float bTheta   = lcTrack->getTanLambda() ;
  float bPhi     = lcTrack->getPhi() ;
  float bRRef    = lcTrack->getReferencePoint()[0] ;
  float bRPhiRef = lcTrack->getReferencePoint()[1] ;
  float bZRef    = lcTrack->getReferencePoint()[2] ;

  rave::Vector6D s ( bRRef * cos( bRPhiRef / bRRef ) ,
                     bRRef * sin( bRPhiRef / bRRef ) ,
                     bZRef ,
                     sin( bTheta ) * cos( bPhi ) / fabs( bInvP ) ,
                     sin( bTheta ) * sin( bPhi ) / fabs( bInvP ) ,
                     cos( bTheta ) / fabs( bInvP ) ) ;

  AlgebraicSymMatrix bCov( 5 ) ;
  AlgebraicMatrix brJacob( 6, 5 ) ;
  AlgebraicMatrix rCov( 6, 6 ) ;

  bCov[0][0] = lcTrack->getCovMatrix()[ 0] ;
  bCov[0][1] = lcTrack->getCovMatrix()[ 1] ;
  bCov[1][1] = lcTrack->getCovMatrix()[ 2] ;
  bCov[0][2] = lcTrack->getCovMatrix()[ 3] ;
  bCov[1][2] = lcTrack->getCovMatrix()[ 4] ;
  bCov[2][2] = lcTrack->getCovMatrix()[ 5] ;
  bCov[0][3] = lcTrack->getCovMatrix()[ 6] ;
  bCov[1][3] = lcTrack->getCovMatrix()[ 7] ;
  bCov[2][3] = lcTrack->getCovMatrix()[ 8] ;
  bCov[3][3] = lcTrack->getCovMatrix()[ 9] ;
  bCov[0][4] = lcTrack->getCovMatrix()[10] ;
  bCov[1][4] = lcTrack->getCovMatrix()[11] ;
  bCov[2][4] = lcTrack->getCovMatrix()[12] ;
  bCov[3][4] = lcTrack->getCovMatrix()[13] ;
  bCov[4][4] = lcTrack->getCovMatrix()[14] ;

  float signInvP = bInvP / fabs( bInvP ) ;

  brJacob[0][0] = (-1) * sin( bRPhiRef / bRRef ) ;
  brJacob[1][0] = cos( bRPhiRef / bRRef ) ;
  brJacob[2][0] = 0.0 ;
  brJacob[3][0] = 0.0 ;
  brJacob[4][0] = 0.0 ;
  brJacob[5][0] = 0.0 ;
  brJacob[0][1] = 0.0 ;
  brJacob[1][1] = 0.0 ;
  brJacob[2][1] = 1.0 ;
  brJacob[3][1] = 0.0 ;
  brJacob[4][1] = 0.0 ;
  brJacob[5][1] = 0.0 ;
  brJacob[0][2] = 0.0 ;
  brJacob[1][2] = 0.0 ;
  brJacob[2][2] = 0.0 ;
  brJacob[3][2] = cos( bTheta ) * cos( bPhi ) / fabs( bInvP ) ;
  brJacob[4][2] = cos( bTheta ) * sin( bPhi ) / fabs( bInvP ) ;
  brJacob[5][2] = (-1.0) * sin( bTheta ) / fabs( bInvP ) ;
  brJacob[0][3] = 0.0 ;
  brJacob[1][3] = 0.0 ;
  brJacob[2][3] = 0.0 ;
  brJacob[3][3] = (-1.0) * sin( bTheta ) * sin( bPhi ) / fabs( bInvP ) ;
  brJacob[4][3] = sin( bTheta ) * cos( bPhi ) / fabs( bInvP ) ;
  brJacob[5][3] = 0.0 ;
  brJacob[0][4] = 0.0 ;
  brJacob[1][4] = 0.0 ;
  brJacob[2][4] = 0.0 ;
  brJacob[3][4] = (-1.0) * signInvP * sin( bTheta ) * cos( bPhi ) / pow( bInvP, 2 ) ;
  brJacob[4][4] = (-1.0) * signInvP * sin( bTheta ) * sin( bPhi ) / pow( bInvP, 2 ) ;
  brJacob[5][4] = (-1.0) * signInvP * cos( bTheta ) / pow( bInvP, 2 ) ;

  rCov  = brJacob * bCov * brJacob.T() ;

  //    cout << "[LCIOEventGenerator] Cov: "<< rCov << endl ;

  rave::Covariance6D err( rCov[0][0], rCov[0][1], rCov[0][2],
                          rCov[1][1], rCov[1][2], rCov[2][2],
                          rCov[0][3], rCov[0][4], rCov[0][5],
                          rCov[1][3], rCov[1][4], rCov[1][5],
                          rCov[2][3], rCov[2][4], rCov[2][5],
                          rCov[3][3], rCov[3][4], rCov[3][5],
                          rCov[4][4], rCov[4][5],
                          rCov[5][5] ) ;

  signed charge = signed ( copysign ( 1.0, (-1.0)*signInvP ) );
  double chi2=lcTrack->getChi2();
  double ndof=lcTrack->getNdf();
  rave::Track trk( s, err, charge, chi2, ndof,
                   (void*) 0, std::string("id") /* ID */ );

  // Some minor checking
  // float rInvP = 1 / sqrt( pow( s.px(), 2 ) + pow( s.py(), 2 ) + pow( s.pz(), 2 ) );

  return trk;
}

rave::Point3D BrahmsLcioToRaveObjects::convert ( const double * position ) const
{
  return rave::Point3D( position[0], position[1], position[2] );
}

vertigo::SimTrack BrahmsLcioToRaveObjects::associate( 
    const EVENT::MCParticle * particle, const vertigo::SimVertex & vtx ) const
{
  rave::Vector6D state( particle->getVertex()[0],
                        particle->getVertex()[1],
                        particle->getVertex()[2],
                        particle->getMomentum()[0],
                        particle->getMomentum()[1],
                        particle->getMomentum()[2] );
  vertigo::SimTrack simTrack( state, signed( particle->getCharge() ), 0, 
                              particle->getPDG(), "", &vtx );
  return simTrack;
}

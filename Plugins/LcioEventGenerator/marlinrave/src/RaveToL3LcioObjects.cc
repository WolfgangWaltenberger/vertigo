#include "RaveToL3LcioObjects.h"

#include "EVENT/LCIO.h"

#include "boost/assert.hpp"
#include "boost/numeric/conversion/cast.hpp"
#include "boost/numeric/ublas/symmetric.hpp"

#include <math.h>

namespace marlinrave {

RaveToL3LcioObjects::RaveToL3LcioObjects ( double Bz ) :
    theBz ( Bz )
{
}

RaveToL3LcioObjects::~RaveToL3LcioObjects()
{
}

IMPL::VertexImpl* RaveToL3LcioObjects::convert ( const rave::Vertex & raveVertex ) const
{
  rave::Point3D ravePosition = raveVertex.position();
  rave::Covariance3D raveError = raveVertex.error();

  float lcioCovMatrix[6] = { raveError.dxx(), raveError.dxy(), raveError.dyy(),
                             raveError.dxz(), raveError.dyz(), raveError.dzz()
                           };

  IMPL::VertexImpl* lcioVertex = new IMPL::VertexImpl();

  lcioVertex->setPosition ( ravePosition.x(), ravePosition.y(), ravePosition.z() );
  lcioVertex->setCovMatrix ( lcioCovMatrix );
  lcioVertex->setChi2 ( raveVertex.chiSquared() );

  return lcioVertex;
}

IMPL::TrackImpl* RaveToL3LcioObjects::convert (
  const rave::Track & raveTrack, const rave::Point3D & raveRefPoint ) const
{
  /* TODO The track has to be propagated to the point of closest approach to
   *      the reference point
   */

  // Do the conversion here
  rave::Charge raveCharge  = raveTrack.charge();
  rave::Vector6D raveState = raveTrack.state();

  double phi = atan2( raveState.py(),  raveState.px() );

  rave::Point3D P0 = raveState.position();
  rave::Vector3D d = P0 - raveRefPoint;
  // float d0 = d.perp(); // easier, but the below is the definition
  double d0 = -1. * d.x() * sin ( phi ) + d.y() * cos ( phi );

  double tanLambda = raveState.pz() / raveState.momentum().perp();

  double pT = raveState.pz() / tanLambda; // [GeV/c]
  double a = 2.99792458e-4;

  double omega = copysign ( a * theBz / pT, raveCharge ); // [1/mm]

  double z0 = d.z();

  float lcioRefPoint[3] = { raveRefPoint.x(), raveRefPoint.y(), raveRefPoint.z() };

  IMPL::TrackImpl* lcioTrack = new IMPL::TrackImpl();

  lcioTrack->setTypeBit ( 0 ); // TODO set the real type
  lcioTrack->setOmega ( omega );
  lcioTrack->setTanLambda ( tanLambda ) ;
  lcioTrack->setPhi ( phi );
  lcioTrack->setD0 ( d0 );
  lcioTrack->setZ0 ( z0 );
  lcioTrack->setChi2 ( raveTrack.chi2() );
  lcioTrack->setNdf ( boost::numeric_cast<int>( round( raveTrack.ndof() ) ) ); // TODO LCIO only supports int ndf
  lcioTrack->setReferencePoint ( lcioRefPoint );

  // TODO lcioTrack->setCovMatrix

  return lcioTrack;
}

boost::tuple< IMPL::ReconstructedParticleImpl*, IMPL::VertexImpl* >
RaveToL3LcioObjects::convert (
  const rave::KinematicParticle & raveParticle, IMPL::VertexImpl * lcioVertex )
{
  // Do the conversion here
  rave::Charge       raveCharge = raveParticle.charge();
  rave::Vector7D     raveState  = raveParticle.fullstate();
  rave::Covariance7D raveError  = raveParticle.fullerror();

  double lcioMass = raveState.m();
  double lcioEnergy = sqrt ( lcioMass * lcioMass + raveState.momentum().mag2() );

  double lcioMomentum[3] = { raveState.px(), raveState.py(), raveState.pz() };
  float  lcioRefPoint[3] = { 0.,  0.,  0. }; // TODO This is not really the reference point is it??

  using namespace boost::numeric::ublas;
  symmetric_matrix<double, upper> ravePMCovMatrix ( 4, 4 );
  ravePMCovMatrix ( 0, 0 ) = raveError.dpxpx();
  ravePMCovMatrix ( 0, 1 ) = raveError.dpxpy();
  ravePMCovMatrix ( 0, 2 ) = raveError.dpxpz();
  ravePMCovMatrix ( 0, 3 ) = raveError.dpxm();
  ravePMCovMatrix ( 1, 1 ) = raveError.dpypy();
  ravePMCovMatrix ( 1, 2 ) = raveError.dpypz();
  ravePMCovMatrix ( 1, 3 ) = raveError.dpym();
  ravePMCovMatrix ( 2, 2 ) = raveError.dpzpz();
  ravePMCovMatrix ( 2, 3 ) = raveError.dpzm();
  ravePMCovMatrix ( 3, 3 ) = raveError.dmm();

  matrix<double> jacobian ( 4, 4 );
  jacobian = identity_matrix<double> ( 4 );
  jacobian ( 3, 0 ) = raveState.px() / lcioEnergy;
  jacobian ( 3, 1 ) = raveState.py() / lcioEnergy;
  jacobian ( 3, 2 ) = raveState.pz() / lcioEnergy;
  jacobian ( 3, 3 ) = lcioMass / lcioEnergy;

  matrix<double> tempMatrix ( 4, 4 );
  matrix<double> lcioPECovMatrix ( 4, 4 );
  tempMatrix = prod ( ravePMCovMatrix, jacobian );
  lcioPECovMatrix = prod ( trans( jacobian ), tempMatrix );

  // Assert: lcioPECovMatrix is symmetric!

  float lcioParticleCovMatrix[10] =
    { lcioPECovMatrix ( 0, 0 ),
      lcioPECovMatrix ( 1, 0 ), lcioPECovMatrix ( 1, 1 ),
      lcioPECovMatrix ( 2, 0 ), lcioPECovMatrix ( 2, 1 ), lcioPECovMatrix ( 2, 2 ),
      lcioPECovMatrix ( 3, 0 ), lcioPECovMatrix ( 3, 1 ), lcioPECovMatrix ( 3, 2 ), lcioPECovMatrix ( 3, 3 )
    };

  IMPL::ReconstructedParticleImpl* lcioParticle = new IMPL::ReconstructedParticleImpl();
  if ( !lcioVertex )
  {
    lcioVertex = new IMPL::VertexImpl();

    float lcioVertexCovMatrix[6] =
      { raveError.dxx(),
        raveError.dxy(), raveError.dyy(),
        raveError.dxz(), raveError.dyz(), raveError.dzz()
      };

    lcioVertex->setPosition ( raveState.x(), raveState.y(), raveState.z() );
    lcioVertex->setCovMatrix ( lcioVertexCovMatrix );
  }

  lcioParticle->setType ( 0 ); // TODO set the real type
  lcioParticle->setMomentum ( lcioMomentum );
  lcioParticle->setEnergy ( lcioEnergy );
  lcioParticle->setMass ( lcioMass );
  lcioParticle->setCharge ( raveCharge );
  lcioParticle->setStartVertex ( lcioVertex );
  lcioParticle->setReferencePoint ( lcioRefPoint );
  lcioParticle->setCovMatrix ( lcioParticleCovMatrix );

  return boost::make_tuple ( lcioParticle, lcioVertex );
}

boost::tuple< IMPL::LCCollectionVec*, IMPL::LCCollectionVec* >
RaveToL3LcioObjects::convert (
  const rave::KinematicTree & raveTree )
{
  IMPL::LCCollectionVec* outputParticles = new IMPL::LCCollectionVec ( EVENT::LCIO::RECONSTRUCTEDPARTICLE );
  IMPL::LCCollectionVec* outputVertices  = new IMPL::LCCollectionVec ( EVENT::LCIO::VERTEX );

  // This stores the top particle of the tree together with its
  // production and decay vertex

  if (raveTree.isValid())
  {
    IMPL::ReconstructedParticleImpl* particle;
    IMPL::VertexImpl* vertex;
    boost::tie ( particle, vertex ) = convert ( raveTree.topParticle(), 0 );
    outputParticles->addElement ( particle );
    outputVertices->addElement ( vertex );
  
    std::vector< rave::KinematicParticle > daughters = raveTree.daughterParticles();
    for ( std::vector< rave::KinematicParticle >::iterator i = daughters.begin();
          i != daughters.end(); ++i )
    {
      boost::tie ( particle, vertex ) = convert ( *i, 0 );
      outputParticles->addElement ( particle );
      outputVertices->addElement ( vertex );
    }
  }

  return boost::make_tuple ( outputParticles, outputVertices );
}

} // namespace marlinrave


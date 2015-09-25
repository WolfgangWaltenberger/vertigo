#include "KinematicsParticleGun.h"

#include <rave/Vector7D.h>
#include <rave/Vector6D.h>

#include <rave/impl/RaveBase/Converters/interface/RaveStreamers.h>

#include <rave/impl/FWCore/MessageLogger/interface/MessageLogger.h>

#include "CLHEP/Vector/LorentzVector.h"
#include "CLHEP/Vector/LorentzRotation.h"

#include <boost/bind.hpp>

#include <sstream>

namespace {
  const double pi = 3.14159265358979323846;
}

KinematicsParticleGun::KinematicsParticleGun( double mass, double width ) :
    thePrimaryP ( 99.99, 100.01 ),
    thePrimaryPhi ( 0.0, 2.0 * pi ),
    thePrimaryEta ( -2.5, 2.5 ),
    theMuonPhi ( 0.0, 2.0 * pi ),
    theMuonTheta ( 0.0, pi ),
    thePrimaryMass ( mass, width ),
    theGenerator(), theName ( "KGun" )
{
  validate();
}

KinematicsParticleGun::~KinematicsParticleGun()
{}

void KinematicsParticleGun::validate()
{
  for ( int i = 0; i < 9999; i++ )
  {
    theGenerator();
  }
  base_generator_type::result_type val = theGenerator();
  // bool result = base_generator_type::validation ( val );
  // edm::LogInfo ( "KinematicsParticleGun" ) << "validation result = " << result;
}

void KinematicsParticleGun::shoot()
{
  clear();

  rave::Point3D origin ( 0., 0., 0. );

  // mass smearing
  double primaryMass = normal_generator_type( theGenerator, thePrimaryMass )();

  // momentum and direction of primary particle
  double p   = uni_generator_type( theGenerator, thePrimaryP )();
  double phi = uni_generator_type( theGenerator, thePrimaryPhi )();
  double eta = uni_generator_type( theGenerator, thePrimaryEta )();
  double theta = 2. * atan ( exp ( -eta ) );

  // 4-momentum of primary particle
//   primaryMomentum.setPx( p*sin( theta )*cos( phi ) );
//   primaryMomentum.setPy( p*sin( theta )*sin( phi ) );
//   primaryMomentum.setPz( p*cos( theta ) );
//   primaryMomentum.setE( sqrt( p*p + primaryMass*primaryMass ) );
  rave::Vector7D theTruth ( origin.x(), origin.y(), origin.z(),
                            p*sin ( theta ) *cos ( phi ),
                            p*sin ( theta ) *sin ( phi ),
                            p*cos ( theta ),
                            primaryMass );

  edm::LogInfo ( "KinematicsParticleGun" ) << "Primary momentum = " << theTruth.momentum().mag();

  // decay direction for muons
  double muPhi   = uni_generator_type( theGenerator, theMuonPhi )();
  double muTheta = uni_generator_type( theGenerator, theMuonTheta )();

//   theCurrentPhi = muPhi;
//   theCurrentTheta = muTheta;

  double muMass = 0.105658;
  double alpha = 0.5 * primaryMass / muMass;

  double p0 = muMass * sqrt ( alpha * alpha - 1. );
  double p0x = p0 * cos ( muPhi ) * sin ( muTheta );
  double p0y = p0 * sin ( muPhi ) * sin ( muTheta );
  double p0z = p0 * cos ( muTheta );
  double p00 = 0.5 * primaryMass;

  double energy = sqrt ( p * p + primaryMass * primaryMass );
  double beta = p / energy;

  // muon momenta in the primary's rest frame
  CLHEP::HepLorentzVector pMu1 ( p0x, p0y, p0z, p00 );
  CLHEP::HepLorentzVector pMu2 ( -p0x, -p0y, -p0z, p00 );

  // muon momenta in the boosted frame
  CLHEP::HepLorentzVector boostedPMu1 = pMu1.boostZ ( beta );
  CLHEP::HepLorentzVector boostedPMu2 = pMu2.boostZ ( beta );

  // rotation to lab-frame
  CLHEP::HepLorentzRotation rotation;
  rotation.rotateY ( theta );
  rotation.rotateZ ( phi );

  // muon momenta in the lab-frame
  CLHEP::HepLorentzVector muonMomentum = boostedPMu1.transform ( rotation );
  CLHEP::HepLorentzVector antimuonMomentum = boostedPMu2.transform ( rotation );

  theParticles.push_back (
    vertigo::SimParticle ( rave::Vector7D ( origin.x(), origin.y(), origin.z(),
                                            muonMomentum.px(), muonMomentum.py(), muonMomentum.pz(),
                                            muMass /*muonMomentum.e()*/ ), rave::Charge( -1.0 ) ) );
  theParticles.push_back (
    vertigo::SimParticle ( rave::Vector7D ( origin.x(), origin.y(), origin.z(),
                                            antimuonMomentum.px(), antimuonMomentum.py(), antimuonMomentum.pz(),
                                            muMass /*antimuonMomentum.e()*/ ), rave::Charge( 1.0 ) ) );
  if (true)
  {
    theTracks.push_back (
      vertigo::SimTrack ( rave::Vector6D ( origin.x(), origin.y(), origin.z(),
                                   muonMomentum.px(), muonMomentum.py(), muonMomentum.pz() ),
                          rave::Charge( -1.0 ) ) );
    theTracks.push_back (
      vertigo::SimTrack ( rave::Vector6D ( origin.x(), origin.y(), origin.z(),
                                           antimuonMomentum.px(), antimuonMomentum.py(), antimuonMomentum.pz() ),
                           rave::Charge( 1.0 ) ) );
  }

  std::vector< vertigo::SimParticle > monteCarloParticles = theParticles;
  monteCarloParticles.push_back (
    vertigo::SimParticle ( theTruth, rave::Charge( 0.0 ) ) );

  std::ostringstream os;
  os << "Primary particle truth = " << theTruth;
  theVertices.push_back ( vertigo::SimVertex ( origin, monteCarloParticles, 0, os.str() ) );

  std::for_each ( monteCarloParticles.begin(),
                  monteCarloParticles.end(),
                  boost::bind ( &vertigo::SimParticle::setVertex, _1, & ( theVertices.back() ) ) );

  edm::LogInfo ( "KinematicsParticleGun" ) << "Generated " << theParticles.size() << " SimParticles";

  return;
}

void KinematicsParticleGun::clear()
{
  theParticles.clear();
  theTracks.clear();
  theVertices.clear();
}

std::string KinematicsParticleGun::name() const
{
  return theName;
}

std::vector< vertigo::SimTrack > KinematicsParticleGun::tracks() const
{
  return theTracks;
}

std::vector< vertigo::SimParticle > KinematicsParticleGun::particles() const
{
  return theParticles;
}

std::vector< vertigo::SimVertex > KinematicsParticleGun::vertices() const
{
  return theVertices;
}

KinematicsParticleGun * KinematicsParticleGun::clone() const
{
  return new KinematicsParticleGun ( * this );
}

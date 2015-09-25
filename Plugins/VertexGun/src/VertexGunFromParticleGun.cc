#include "VertexGunFromParticleGun.h"

#include <rave/TransientTrackKinematicParticle.h>

#include "Environment/MCObjects/interface/SimTrack.h"
#include "Environment/MCObjects/interface/SimVertex.h"
#include "Environment/Configuration/interface/SimpleConfigurable.h"
#include "Vertigo/Converters/interface/VertigoToCmsObjects.h"
#include "VertexGunPropagator.h"

#include <rave/impl/FWCore/MessageLogger/interface/MessageLogger.h>

#include "boost/cast.hpp"

using namespace std;

VertexGunFromParticleGun::VertexGunFromParticleGun(
      const ParticleGun & g, const RecTrackSmearer & smearer, const std::string & name ) :
    theGun ( g.clone() ), theId(0), theSmearer ( smearer.clone() ),
    theName ( name )
{
  static bool descr = SimpleConfigurable<bool>
    (false,"VertexFastSim:GunDescription").value();
  if (descr)
  {
    theSmearer->printDescription();
  };
}


VertexGunFromParticleGun::VertexGunFromParticleGun(
    const VertexGunFromParticleGun & o ) : theGun ( o.theGun->clone() ),
    theId(0), theSmearer ( o.theSmearer->clone() ), theName ( o.theName )
{
  clear();
}


void VertexGunFromParticleGun::clear()
{
  theEvent=vertigo::Event( theId, 0, theName );
}

std::string VertexGunFromParticleGun::name() const
{
  return theName;
}

VertexGunFromParticleGun::~VertexGunFromParticleGun()
{
  delete theSmearer;
  delete theGun;
}

VertexGunFromParticleGun * VertexGunFromParticleGun::clone() const
{
  return new VertexGunFromParticleGun ( * this );
}

vertigo::Event VertexGunFromParticleGun::shoot()
{
  theId++;
  clear();
  // edm::LogInfo ( "VertexGunFromParticleGun" ) << "Shooting particle gun ...";
  theGun->shoot();

  vector < vertigo::SimTrack > strks = theGun->tracks();
  for ( vector< vertigo::SimTrack >::const_iterator i=strks.begin(); i!=strks.end() ; ++i )
  {
    theEvent.add ( *i );
  }

  vector < vertigo::SimParticle > sparticles = theGun->particles();
  for ( vector< vertigo::SimParticle >::const_iterator i=sparticles.begin(); i!=sparticles.end() ; ++i )
  {
    theEvent.add ( *i );
  }

  vector < vertigo::SimVertex > vtces = theGun->vertices();
  for ( vector< vertigo::SimVertex >::const_iterator i=vtces.begin(); i!=vtces.end() ; ++i )
  {
    theEvent.add ( *i );
  }

  float pt=0.5;

  for ( std::vector< vertigo::SimTrack >::iterator strk=strks.begin();
      strk!=strks.end() ; ++strk )
  {
    GlobalTrajectoryParameters /* tmp */ gp = VertigoToCmsObjects().convert ( *strk );
    /*
    if ( tmp.momentum().mag() < pt )
    {
      cout << "[VertexGunFromParticleGun] simtrack " << strk->id() << ": " 
           << tmp.momentum().mag() << " < "
           << pt << "GeV. Discarding."<< endl;
      continue;
    };*/
    // GlobalTrajectoryParameters gp = VertexGunPropagator::propagate ( tmp );

    if ( gp.momentum().mag() < pt )
    {
      cout << "[VertexGunFromParticleGun] GTP to simtrk " << strk->id() << ": " 
           << gp.momentum().mag() << " < "
           << pt << "GeV. Discarding." << endl;
      continue;
    };

    rave::Track trk = theSmearer->recTrack ( gp );

    /*
    cout << "[VertexGunFromParticleGun] simtrk at " << gp.position().x()
         << " rtrk at " << trk.state().position().x() << endl;
         */
    theEvent.add ( trk );
    theEvent.associate ( *strk, trk );
    if ( vtces.size() == 1 && strks.size() > 1 )
    {
      strk->setVertex ( &(vtces[0]) );
      // theEvent.attach ( *strk, vtces[0] );
    /* } else {
     cout << "[VertexGunFromParticleGun] error: number of vertices="
           << vtces.size() << endl;*/
    }
  };

  for ( std::vector< vertigo::SimParticle >::iterator sparticle = sparticles.begin();
        sparticle != sparticles.end() ; ++sparticle )
  {
    GlobalTrajectoryParameters gp = VertigoToCmsObjects().convert ( *sparticle );

    if ( gp.momentum().mag() < pt )
    {
      cout << "[VertexGunFromParticleGun] GTP to simparticle " << sparticle->id() << ": "
          << gp.momentum().mag() << " < "
          << pt << "GeV. Discarding." << endl;
      continue;
    };

    rave::Track trk = theSmearer->recTrack ( gp );
    rave::KinematicParticle ttkp = rave::TransientTrackKinematicParticle( trk, sparticle->state().m(), 2.4952 );

    theEvent.add ( ttkp );
    theEvent.associate ( *sparticle, ttkp );
  };

  return theEvent;
}

ParticleGun * VertexGunFromParticleGun::gun()
{
  return theGun;
}

void VertexGunFromParticleGun::associate ( const vertigo::SimTrack & s, const rave::Track & r )
{
    theEvent.associate ( s, r );
}

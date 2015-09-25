//
// C++ Implementation: KinematicsObserver
//
// Description:
//
//
// Author: Fabian Moser <fabiamos@gmail.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "KinematicsObserver.h"

#include <rave/TransientTrackKinematicParticle.h>
#include <rave/KinematicTreeFactory.h>
#include <RaveBase/Converters/interface/RaveStreamers.h>

#include <rave/impl/FWCore/MessageLogger/interface/MessageLogger.h>

#include "boost/bind.hpp"

#include <iostream>

using namespace std;

KinematicsObserver::KinematicsObserver()
  : Observer(), theFileName( "KinematicsObserver.output.txt" )
{
}


KinematicsObserver::~KinematicsObserver()
{}

void KinematicsObserver::process ( const vertigo::Event & e )
{
  std::vector< rave::KinematicParticle > particles = e.particles();
  for ( vector< rave::KinematicParticle >::const_iterator 
        i=particles.begin(); i!=particles.end() ; ++i )
  {
    cout << " --- particle " << i->fullstate() << endl;
  }
  cout << endl;
}

void KinematicsObserver::process ( const std::exception & e, const std::string & type )
{
  except ( e.what(), -1 );
}

void KinematicsObserver::configure( const std::string & s )
{
  theFileName = s;
}

void KinematicsObserver::except( const std::string & e, int id )
{
}

KinematicsObserver * KinematicsObserver::clone() const
{
  return new KinematicsObserver ( *this );
}

#include "Analysis/Observer/interface/ObserverBuilder.h"

namespace
{
ObserverBuilder<KinematicsObserver> KinematicsObserverBuilder (
  "KinematicsObserver",
  "Observer that applies kinematic constraints and reconstructs the decay chain",
  false );
}


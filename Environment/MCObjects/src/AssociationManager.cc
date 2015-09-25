#include "Environment/MCObjects/interface/AssociationManager.h"
#include "Environment/Configuration/interface/SimpleConfigurable.h"

using namespace std;
using namespace vertigo;

void AssociationManager::clear()
{
  theTrackSimMap.clear();
  theTrackRecMap.clear();
  theParticleSimMap.clear();
  theParticleRecMap.clear();
  theVtxSimMap.clear();
  theVtxRecMap.clear();
}

AssociationManager::AssociationManager()
{}

void AssociationManager::associate ( const vertigo::SimTrack & s, const rave::Track & r )
{
  // associate only not-yet associated tracks!
  if (  ( theHasTracksAssociated[s.id()].find ( r.id() ) == theHasTracksAssociated[s.id()].end() ) )
  {
    theTrackSimMap[s].push_back ( r );
    theTrackRecMap[r].push_back ( s );
    theHasTracksAssociated[s.id()].insert ( r.id() );
    theHasTracksAssociated[r.id()].insert ( s.id() );
  }
}

void AssociationManager::associate ( const vertigo::SimParticle & s, const rave::KinematicParticle & r )
{
  theParticleSimMap[s].push_back ( r );
  theParticleRecMap[r].push_back ( s );
  theHasParticlesAssociated[s.id()].insert ( r.id() );
  theHasParticlesAssociated[r.id()].insert ( s.id() );
}

void AssociationManager::associate ( const vertigo::SimVertex & s, const rave::Vertex & r )
{
  // associate only not-yet associated vertices!
  if ( ( theHasVerticesAssociated[s.id()].find ( r.id() ) == theHasVerticesAssociated[s.id()].end() ) )
  {
    theVtxSimMap[s].push_back ( r );
    theVtxRecMap[r].push_back ( s );
    theHasVerticesAssociated[s.id()].insert ( r.id() );
    // theHasVerticesAssociated[r.id()].insert ( s.id() );
  }
}

vector < vertigo::SimTrack > AssociationManager::tracks ( const rave::Track & r ) const
{
  return theTrackRecMap[r];
}

vector < rave::Track > AssociationManager::tracks( const vertigo::SimTrack & s ) const
{
  return theTrackSimMap[s];
}

vector < vertigo::SimParticle > AssociationManager::particles ( const rave::KinematicParticle & r ) const
{
  return theParticleRecMap[r];
}

vector < rave::KinematicParticle > AssociationManager::particles ( const vertigo::SimParticle & s ) const
{
  return theParticleSimMap[s];
}

vector < vertigo::SimVertex > AssociationManager::vertices ( const rave::Vertex & r ) const
{
  return theVtxRecMap[r];
}

vector < rave::Vertex > AssociationManager::vertices( const vertigo::SimVertex & s ) const
{
  return theVtxSimMap[s];
}

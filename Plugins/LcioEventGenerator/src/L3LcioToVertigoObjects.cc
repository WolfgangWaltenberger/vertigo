/** @file
 *
 * This implements the obvious converter class.
 * The #defines referenced here can be found (and chosen)
 * in the associated header file.
 */

#include "L3LcioToVertigoObjects.h"
#include <rave/impl/DataFormats/CLHEP/interface/AlgebraicObjects.h>
#include "RaveBase/Converters/interface/RaveStreamers.h"
#include <iostream>

using namespace std;

namespace
{
static const float cm = 0.1;
}

L3LcioToVertigoObjects::L3LcioToVertigoObjects() : 
    theHasOffset ( false ), theGenerator ( 42u ),
    theXDelta ( 0 ), theYDelta ( 0 ), theZDelta ( 0 ),
    theCurX ( 0. ), theCurY ( 0. ), theCurZ ( 0. )
{}

L3LcioToVertigoObjects::~L3LcioToVertigoObjects()
{
  if ( theXDelta ) delete theXDelta;
  if ( theYDelta ) delete theYDelta;
  if ( theZDelta ) delete theZDelta;
}

L3LcioToVertigoObjects::L3LcioToVertigoObjects ( float x, float y, float z ) :
    theHasOffset ( false ), theGenerator ( 42u ),
    theXDelta ( 0 ), theYDelta ( 0 ), theZDelta ( 0 ),
    theCurX ( 0. ), theCurY ( 0. ), theCurZ ( 0. )
{
  defineOffset ( x, y, z );
}

void L3LcioToVertigoObjects::defineOffset ( float x, float y, float z )
{
  // cout << "[L3LcioToVertigoObjects] ::defineOffset not implemented!" << endl;
  if ( theXDelta ) delete theXDelta;
  if ( theYDelta ) delete theYDelta;
  if ( theZDelta ) delete theZDelta;
  theHasOffset = true;
  theXDelta = new normal_generator_type ( theGenerator, boost::normal_distribution< float > ( 0., x ) );
  theYDelta = new normal_generator_type ( theGenerator, boost::normal_distribution< float > ( 0., y ) );
  theZDelta = new normal_generator_type ( theGenerator, boost::normal_distribution< float > ( 0., z ) );
}

void L3LcioToVertigoObjects::createOffset()
{
  if ( theHasOffset )
  {
    theCurX = ( *theXDelta ) ();
    theCurY = ( *theYDelta ) ();
    theCurZ = ( *theZDelta ) ();
  };
}

void L3LcioToVertigoObjects::printOffset() const
{
  cout << "[L3LcioToVertigoObjects] offset: (" << theCurX << ", " << theCurY << ", "
  << theCurZ << ")" << endl;
}

vertigo::SimTrack L3LcioToVertigoObjects::associate ( const EVENT::MCParticle * lcioMCParticle,
    const vertigo::SimVertex & simVertex, const std::string & tag ) const
{
  rave::Vector6D state ( theCurX + lcioMCParticle->getVertex() [0]*cm,
                         theCurY + lcioMCParticle->getVertex() [1]*cm,
                         theCurZ + lcioMCParticle->getVertex() [2]*cm,
                         lcioMCParticle->getMomentum() [0],
                         lcioMCParticle->getMomentum() [1],
                         lcioMCParticle->getMomentum() [2] );
  vertigo::SimTrack simTrack ( state, signed ( lcioMCParticle->getCharge() ), 0, lcioMCParticle->getPDG(), tag, &simVertex );
  return simTrack;
}

vertigo::SimParticle L3LcioToVertigoObjects::convert(
    const EVENT::MCParticle * lcioMCParticle ) const
{
  const double *lcioVertex   = lcioMCParticle->getVertex();
  const double *lcioMomentum = lcioMCParticle->getMomentum();
  double lcioMass         = lcioMCParticle->getMass();
  rave::Charge raveCharge = lcioMCParticle->getCharge();

  rave::Vector7D raveState( lcioVertex[0], lcioVertex[1], lcioVertex[2], 
                            lcioMomentum[0], lcioMomentum[1], lcioMomentum[2], 
                            lcioMass );

  return vertigo::SimParticle( raveState, raveCharge, 0, lcioMCParticle->getPDG() );
}


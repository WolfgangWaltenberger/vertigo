#include "MultipleVertexGun.h"
#include "Vertigo/Event/interface/EventCombiner.h"
#include <vector>

using namespace std;

MultipleVertexGun::MultipleVertexGun( bool merge, const string & name ) : 
    theName ( name ), theMerge (merge)
{
  theGuns.clear();
}

MultipleVertexGun::MultipleVertexGun ( const MultipleVertexGun & o ) :
    theEvent ( o.theEvent ), theName (o.theName), theMerge (o.theMerge)
{
  for ( vector< VertexGun * >::const_iterator i=o.theGuns.begin();
      i!=o.theGuns.end() ; ++i )
  {
    theGuns.push_back ( (**i).clone() );
  };
}

string MultipleVertexGun::name() const
{
  return theName;
}

void MultipleVertexGun::add( const VertexGun & vg )
{
  theGuns.push_back ( vg.clone() );
}

MultipleVertexGun::~MultipleVertexGun()
{
  for ( vector< VertexGun *>::iterator i=theGuns.begin();
      i!=theGuns.end(); ++i ) {
    delete (*i);
  };
}

vertigo::Event MultipleVertexGun::shoot()
{
  if ( theGuns.size() )
  {
    // first, simple copy from first gun
    theEvent = theGuns[0]->shoot();
  }

  if ( theGuns.size() < 2 ) return theEvent;

  vertigo::EventCombiner combiner;
    // no add events, one by one
  for ( vector< VertexGun *>::iterator gun=theGuns.begin()+1; gun!=
      theGuns.end() ; gun++ )
  {
    vertigo::Event tmp = (*gun)->shoot();
    combiner.add ( theEvent, tmp );
  }

  theEvent.setTag ( theName );
  return theEvent;
}

MultipleVertexGun * MultipleVertexGun::clone() const
{
  return new MultipleVertexGun ( *this );
}

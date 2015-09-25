#include "Vertigo/Event/interface/EventCombiner.h"

using namespace std;
using namespace vertigo;

void EventCombiner::add ( vertigo::Event & a, const vertigo::Event & b ) const
{
  vector < vertigo::SimVertex > simvertices = b.simVertices();

  for ( vector<vertigo::SimVertex>::const_iterator oldV=simvertices.begin();
      oldV!=simvertices.end() ; ++oldV )
  {
    a.add ( *oldV );
  };

  vector < rave::Track > rectracks = b.tracks();
  for ( vector< rave::Track >::iterator t=rectracks.begin();
      t!=rectracks.end() ; ++t )
  {
    a.add ( *t );

    vector < vertigo::SimTrack > ass = b.associatedTracks ( *t );
    for ( vector< vertigo::SimTrack >::const_iterator s=ass.begin();
          s!=ass.end() ; ++s )
    {
      a.associate ( *s, *t );
    }
  }

  vector < vertigo::SimTrack > simtracks = b.simTracks();
  for ( vector<vertigo::SimTrack>::const_iterator st=simtracks.begin();
      st!=simtracks.end() ; ++st )
  {
    a.add ( *st );
  }
}

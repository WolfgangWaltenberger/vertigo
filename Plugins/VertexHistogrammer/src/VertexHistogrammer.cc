#include "VertexHistogrammer.h"

#include <rave/impl/RaveBase/Converters/interface/RaveToCmsObjects.h>
#include <rave/impl/RaveBase/Converters/interface/CmsToRaveObjects.h>

#include "Vertigo/EventLoop/interface/LoopSettings.h"
#include "Vertigo/EventLoop/interface/EventLoop.h"
#include "Environment/Configuration/interface/SimpleConfigurable.h"

using namespace std;

namespace {
  int nthInVector ( const vertigo::SimVertex & vtx, const vector < vertigo::SimVertex > & cont )
  {
    int nthInVector=0;
    for ( vector< vertigo::SimVertex >::const_iterator i=cont.begin(); 
          i!=cont.end() ; ++i )
    {
      nthInVector++;
      if ( vtx == (*i) ) return nthInVector;
    }
    return -1;
  }
}

VertexHistogrammer::VertexHistogrammer() :
    theHisto ( new VertexHisto ( "vertices.root" ) )
{ }

VertexHistogrammer::VertexHistogrammer ( const VertexHistogrammer & o ) :
    theHisto ( new VertexHisto ( "vertices.root" ) )
{}

void VertexHistogrammer::destroy()
{
  delete theHisto;
}

VertexHistogrammer::~VertexHistogrammer()
{
  destroy();
}

void VertexHistogrammer::configure ( const string & f )
{
    theHisto->configure ( f );
}

void VertexHistogrammer::process ( const vertigo::Event & e )
{
  vector < rave::Track > trks = e.tracks();
  if ( trks.size() < 2 ) return;
  theHisto->tick();
  theHisto->event( e );
  int evctr=e.id();
  vector < vertigo::SimVertex > svtces = e.simVertices();
  vector < rave::Vertex > vtces = e.vertices();

  /*
  if ( svtces.size() == 0 )
  {
    return;
  }

  if ( vtces.size() == 0 )
  {
    theHisto->except("[VertexHistogrammer] No vertex in event", evctr );
    return;
  }*/

  map < vertigo::SimVertex, bool > has_filled;
  for ( vector< vertigo::SimVertex >::const_iterator i=svtces.begin();
        i!=svtces.end() ; ++i )
  {
    has_filled[*i]=false;
  }

  for ( vector< rave::Vertex >::const_iterator i=vtces.begin();
        i!=vtces.end() ; ++i )
  {
    vector < vertigo::SimVertex > asvtces=e.associatedVertices ( *i );
    if ( asvtces.size() )
    {
      int nth_in_vector = nthInVector ( asvtces[0], svtces );
      theHisto->fill ( asvtces[0], *i, e.time(), evctr, nth_in_vector );
      has_filled[ asvtces[0] ]=true;
    }
  }

  for ( map < vertigo::SimVertex, bool  >::const_iterator i=has_filled.begin(); 
        i!=has_filled.end() ; ++i )
  {
    if ( i->second == false )
    {
      int nth_in_vector = nthInVector ( i->first, svtces );
      theHisto->fillMissing ( i->first, e.time() , evctr, nth_in_vector );
    }
  }

  for ( vector< rave::Vertex >::const_iterator i=vtces.begin();
        i!=vtces.end() ; ++i )
  {
    vector < vertigo::SimVertex > asvtces=e.associatedVertices ( *i );
    if ( asvtces.size()==0 )
    {
      // non-associated rec vertex
      theHisto->fillExtra ( *i, e.time(), evctr, -1 );
    }
  }

}

void VertexHistogrammer::process ( const exception & e, const string & type )
{
  theHisto->except ( e.what(), "std", -1 );
}

VertexHistogrammer * VertexHistogrammer::clone() const
{
  return new VertexHistogrammer ( *this );
}

#include "Analysis/Observer/interface/ObserverBuilder.h"
namespace {
  ObserverBuilder<VertexHistogrammer> t( "VertexHistogrammer", "Analyses the specified fitter", false );
}


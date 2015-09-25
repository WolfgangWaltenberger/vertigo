#include "Vertigo/VertexAssociation/interface/VertexAssociatorByDistance.h"
#include "Vertigo/VertexAssociation/interface/VertexDistance.h"
#include <rave/impl/FWCore/MessageLogger/interface/MessageLogger.h>
#include <rave/impl/FWCore/Utilities/interface/Exception.h>
#include <algorithm>
#include <cmath>
#include <iostream>

using namespace std;

VertexAssociatorByDistance::VertexAssociatorByDistance (
    const vector < vertigo::SimVertex > & sim,
    const vector < rave::Vertex > & rec,
    bool reduced, float maxdist )
{
  if ( sim.size()==0 || rec.size()==0 ) return;
  VertexDistance distor ( reduced );

  set < rave::Vertex > vtxSet;
  set < vertigo::SimVertex > simVtxSet;
  map < rave::Vertex, map < vertigo::SimVertex, float > > distances;

  for ( vector < rave::Vertex >::const_iterator r=rec.begin(); r!=rec.end() ; ++r )
  {
    vtxSet.insert ( *r );
    for ( vector< vertigo::SimVertex >::const_iterator s=sim.begin(); s!=sim.end() ; ++s )
    {
      try {
        distances[*r][*s]=distor.distance ( *s, *r );
      } catch ( cms::Exception & c ) {
        edm::LogError ( "VertexAssociatorByDistance" ) 
          << "Cannot compute distance: " << c.what();
      }
    }
  }

  for ( vector < vertigo::SimVertex >::const_iterator s=sim.begin(); s!=sim.end() ; ++s )
  {
    simVtxSet.insert ( *s );
  }


  while ( vtxSet.size() && simVtxSet.size() )
  {
    float min=maxdist;
    if ( min < 0. ) min = 1e25;

    // cout << "[VertexAssociatorByDistance] starting with " << min << endl;
    set < rave::Vertex >::const_iterator rmin=vtxSet.begin();
    set < vertigo::SimVertex >::const_iterator smin=simVtxSet.begin();

    bool hasfound=false;

    for ( set< rave::Vertex >::const_iterator r=vtxSet.begin(); r!=vtxSet.end() ; ++r )
    {
      for ( set< vertigo::SimVertex >::const_iterator s=simVtxSet.begin(); 
            s!=simVtxSet.end() ; ++s )
      {
        if ( distances[*r][*s] < min )
        {
          // cout << "[VertexAssociatorByDistance] this is better: " << distances[*r][*s] << endl;
          min=distances[*r][*s];
          rmin=r;
          smin=s;
          hasfound=true;
        }
      }
    }

    if ( hasfound )
    {
      // found the smallest "link"
      theSimVertexMap[*smin]=(*rmin);
      theVertexMap[*rmin]=(*smin);
      vtxSet.erase ( rmin );
      simVtxSet.erase ( smin );
    } else {
      break;
    }
  }
}

vector < vertigo::SimVertex >
  VertexAssociatorByDistance::simVertices( const rave::Vertex & vertex ) const
{
  vector < vertigo::SimVertex > ret;
  if ( theVertexMap.count(vertex) )
  {
    ret.push_back ( (theVertexMap[vertex]) );
  }
  return ret;
}

vector < rave::Vertex >
  VertexAssociatorByDistance::recVertices( const vertigo::SimVertex& vertex ) const
{
  vector < rave::Vertex > ret;
  if ( theSimVertexMap.count(vertex) )
  {
    ret.push_back ( (theSimVertexMap[vertex]) );
  }
  return ret;
}

VertexAssociatorByDistance * VertexAssociatorByDistance::clone() const
{
  return new VertexAssociatorByDistance ( *this );
}

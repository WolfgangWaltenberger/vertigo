#ifndef VertexAssociatorByDistance_H
#define VertexAssociatorByDistance_H

#include "Vertigo/VertexAssociation/interface/VertexAssociator.h"
#include <map>
#include <set>

/** \class VertexAssociatorByDistance
 *  Associator, that computes the distances
 *  between all simvertex - recvertex pairs.
 *  then makes the best associations,
 *  where no simvertex and no recvertex is associated twice.
 */


class VertexAssociatorByDistance : public VertexAssociator {

public:

  /** \paramname reduced Use reduced distances (divided by the errors?)
   *  \paramname maxdist The maximum distance to associate, maxdist < 0. means no limit.
   */

  VertexAssociatorByDistance(const std::vector < vertigo::SimVertex > & sim, 
     const std::vector < rave::Vertex > & rec, bool reduced=false,
     float maxdist = -1. );

  std::vector < vertigo::SimVertex > simVertices ( const rave::Vertex & ) const;
  std::vector < rave::Vertex > recVertices ( const vertigo::SimVertex & ) const;

  VertexAssociatorByDistance * clone() const;

private:
  mutable std::map < vertigo::SimVertex, rave::Vertex > theSimVertexMap;
  mutable std::map < rave::Vertex, vertigo::SimVertex > theVertexMap;
};

#endif // VertexAssociatorByDistance_H

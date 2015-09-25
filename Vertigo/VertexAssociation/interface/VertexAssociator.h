#ifndef VertexAssociator_H
#define VertexAssociator_H

#include <vector>

/** \class VertexAssociator
 *  Abstract class for VertexAssociators
 */

#include <rave/Vertex.h>
#include "Environment/MCObjects/interface/SimVertex.h"

class VertexAssociator {
public:
    
  typedef std::vector<rave::Vertex> RecVertexContainer;
  typedef std::vector<vertigo::SimVertex> SimVertexContainer;   

  virtual ~VertexAssociator() {}

  virtual SimVertexContainer simVertices( const rave::Vertex & ) const = 0;
  virtual RecVertexContainer recVertices( const vertigo::SimVertex & ) const = 0;

  virtual VertexAssociator * clone() const = 0;
  
};

#endif // VertexAssociator_H

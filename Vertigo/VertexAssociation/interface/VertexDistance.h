#ifndef _VertexDistance_H_
#define _VertexDistance_H_

#include "Environment/MCObjects/interface/SimVertex.h"
#include <rave/Vertex.h>

class VertexDistance {
public:
  VertexDistance ( bool reduced );
  float distance(const vertigo::SimVertex & sv1, const rave::Vertex & sv2 );
  float distanceSquared(const vertigo::SimVertex & sv1, const rave::Vertex & sv2 );
private:
  bool theReduced;

};

#endif

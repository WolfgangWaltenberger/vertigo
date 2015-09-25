#include "Vertigo/VertexAssociation/interface/VertexDistance.h"
#include <rave/impl/RecoVertex/VertexPrimitives/interface/VertexException.h>
#include <rave/impl/RaveBase/Converters/interface/RaveToAlgebraicObjects.h>

using namespace std;

VertexDistance::VertexDistance ( bool reduced ) : theReduced ( reduced )
{}

float VertexDistance::distance( const vertigo::SimVertex & v1,
                                const rave::Vertex & v2 )
{
  return sqrt( distanceSquared ( v1, v2 ) );
}

float VertexDistance::distanceSquared( const vertigo::SimVertex & v1,
                                const rave::Vertex & v2 )
{
  RaveToAlgebraicObjects converter;
  AlgebraicVector3 a1 = converter.convert ( v1.position() );
  AlgebraicVector3 a2 = converter.convert ( v2.position() );
  AlgebraicVector3 diff = a2-a1;
  if ( !theReduced )
  {
    return ROOT::Math::Mag2 ( diff );
  }
  
  rave::Covariance3D rc = v2.error();
  AlgebraicSymMatrix33 error = converter.convert ( rc );

  // Invert error matrix of residuals
  bool success = error.Invert();
  if ( !success ) {
    throw VertexException("VertexDistance::matrix inversion problem");
  }

  return ROOT::Math::Similarity(diff,error);
}


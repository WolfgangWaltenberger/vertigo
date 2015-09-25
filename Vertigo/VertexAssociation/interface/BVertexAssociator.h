#ifndef Tracker_BVertexAssociator_H
#define Tracker_BVertexAssociator_H

#include "Vertigo/VertexAssociation/interface/VertexAssociator.h"
#include <map>

/** \class BVertexAssociator
 *  Finds for each XXXVertex (XXX = Sim or Rec)
 *  the best associated vertices of the other kind.
 *  This one "simulates" what the SimpleVertexBasedTagger does:
 *  The first RecVertex is assumed to be the primary vertex,
 *  the second is the B vertex!
 *
 *  We take the first simvertex marked as primary as the primary vertex.
 *  The B-SimVertex which is closest to the RecVertex, is associated
 *  with this RecVertex, if it's less than 5 sigmas off.
 *
 *  FIXME there is a thinko!! Simple Vertex Based Tagger sorts first!!!
 */

class BVertexAssociator : public VertexAssociator {

public:

  /** Constructor that lets you specify the SimVertices
   */
  BVertexAssociator( const std::vector < vertigo::SimVertex > &, 
                     const std::vector< rave::Vertex > & );

  // association methods
  virtual std::vector<vertigo::SimVertex> simVertices( const rave::Vertex & ) const;
  virtual std::vector<rave::Vertex> recVertices( const vertigo::SimVertex & ) const;

  // clone
  BVertexAssociator * clone() const {
    return new BVertexAssociator(*this);
  }

private:
  void associate ( const std::vector < vertigo::SimVertex > &,
                   const std::vector< rave::Vertex > & );
  void associate ( const vertigo::SimVertex &, const rave::Vertex & );

  mutable std::map< rave::Vertex, std::vector < vertigo::SimVertex > > theRecAss;
  mutable std::map< vertigo::SimVertex, std::vector < rave::Vertex > > theSimAss;
};

#endif // Tracker_BVertexAssociator_H

#ifndef Tracker_VertexAssociatorByTracks_H
#define Tracker_VertexAssociatorByTracks_H

#include "Vertigo/VertexAssociation/interface/VertexAssociator.h"
#include "Vertigo/TrackAssociation/interface/TrackAssociator.h"
#include "Environment/MCObjects/interface/AssociationManager.h"

#include <map>

/** \class VertexAssociatorByTracks
 *  Finds for each XXXVertex (XXX = Sim or Rec)
 *  the best associated vertices of the other kind
 *  by looking at the number of tracks they have in common.
 *  The associated vertices are ordered by the number of tracks in common.
 *  
 *  The track association are queried from the supplied AssociationManager.
 */

class VertexAssociatorByTracks : public VertexAssociator {

public:

  /** Constructor that lets you specify the SimVertices
   */
  VertexAssociatorByTracks( const std::vector < vertigo::SimVertex > & sim, 
                            const std::vector< rave::Vertex > & rec, const vertigo::AssociationManager & ta );
  VertexAssociatorByTracks ( const VertexAssociatorByTracks & );

  ~VertexAssociatorByTracks() {}

  // association methods
  virtual std::vector<vertigo::SimVertex> simVertices( const rave::Vertex & ) const;
  virtual std::vector<rave::Vertex> recVertices( const vertigo::SimVertex & ) const;

  // access
  std::vector< rave::Vertex > recVertices() const;
  float purityCut() const;
  const TrackAssociator * trackAssociator() const;

  // clone
  VertexAssociatorByTracks * clone() const {
    return new VertexAssociatorByTracks(*this);
  }


protected:

  virtual void printMaps() const;
  virtual void check();

private:

  void update();
  void updateSimVertices();
  void setPurityCut();

  /** \class MoreTracksInCommon
   *  A private class to determine which vertex association 
   *  is best by comparing the number of tracks in common 
   *  in the rec and sim vertices
   */
  template <class T> class MoreTracksInCommon;

  typedef std::map< vertigo::SimVertex, int > SimMap;
  typedef std::map< rave::Vertex, SimMap > RecToSimMap;

  typedef std::map< rave::Vertex , int > RecMap;
  typedef std::map< vertigo::SimVertex, RecMap > SimToRecMap;

  // Map with all sim vertices which have tracks 
  // contributing to rave::Vertex vtx
  SimMap simMap(const rave::Vertex & vtx);


private:
  float thePurityCut;

  const vertigo::AssociationManager & theAss;

  RecToSimMap theRecToSimMap;
  SimToRecMap theSimToRecMap;

  std::vector< rave::Vertex > theRecVertices;
  std::vector< vertigo::SimVertex > theSimVertices;

  bool upToDate;
};

#endif // Tracker_VertexAssociatorByTracks_H

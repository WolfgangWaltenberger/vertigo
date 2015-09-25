#ifndef BasicEvent_H
#define BasicEvent_H

#include <vector>
#include <set>

#include <rave/Vertex.h>
#include <rave/Track.h>
#include <rave/KinematicParticle.h>
#include <rave/ReferenceCounted.h>
#include <rave/ProxyBase.h>
#include <rave/CopyUsingNew.h>

#include "Environment/MCObjects/interface/SimVertex.h"
#include "Environment/MCObjects/interface/SimTrack.h"
#include "Environment/MCObjects/interface/SimParticle.h"
#include "Environment/MCObjects/interface/AssociationManager.h"
#include "Environment/VertigoObjects/interface/VertigoJet.h"

namespace vertigo {
class BasicEvent : public RaveReferenceCounted {
public:
  typedef RaveProxyBase< BasicEvent, CopyUsingNew<BasicEvent> > Proxy;

  /// supply an event with all tracks
  BasicEvent( const std::vector < rave::Track > &,
             const std::vector < vertigo::SimTrack > &,
             const std::vector < vertigo::SimVertex > &,
             int id=0, int run=0, const std::string & tag="",
             const std::string & algo="" );

  /// supply an event jet-by-jet
  BasicEvent( const std::vector < vertigo::Jet > &,
      const std::vector < vertigo::SimTrack > &,
      const std::vector < vertigo::SimVertex > &,
      int id=0, int run=0, const std::string & tag="",
      const std::string & algo="" );

  /// supply an event with particles
  BasicEvent( const std::vector < rave::KinematicParticle > &,
                     const std::vector < vertigo::SimParticle > &,
                     const std::vector < vertigo::SimVertex > &,
                     int id=0, int run=0, const std::string & tag="",
                     const std::string & algo="" );

  BasicEvent( int id=0, int run=0, const std::string & tag="",
      const std::string & algo="" );

  std::vector < rave::Track > tracks() const;
  std::vector < rave::Vertex > vertices() const;
  std::vector < rave::KinematicParticle > particles() const;
  std::vector < vertigo::SimTrack > simTracks() const;
  std::vector < vertigo::SimVertex > simVertices() const;
  std::vector < vertigo::SimParticle > simParticles() const;
  std::vector < vertigo::Jet > jets() const;
  std::vector < vertigo::Jet > & jetRefs();

  void add ( const rave::Track & );
  void add ( const rave::KinematicParticle & );
  void clearTracks();
  void removeParticles();
  void add ( const vertigo::SimTrack & );
  void add ( const vertigo::SimParticle & );
  void add ( const vertigo::SimVertex & );
  void add ( const rave::Vertex & );
  void add ( const vertigo::Jet &, bool addtracks=true );
  void setTime ( float );
  void setTag ( const std::string & t );
  void setAlgorithm( const std::string & algo );
  void associateTracks();
  //void associateParticles();
  void associateVertices();
  void associate ( const vertigo::SimTrack &, const rave::Track & );
  void associate ( const vertigo::SimParticle &, const rave::KinematicParticle & );
  void associate ( const vertigo::SimVertex &, const rave::Vertex & );
  ~BasicEvent();

  int id() const; //< event id
  int run() const; //< run id
  std::string tag() const; //< optional describing tag
  void tag ( const vertigo::Jet & j, double value ); //< tag a jet
  std::string algorithm() const; //< rave algo used
  float time() const; //< time it took to reconstruct vertices

  /// deletes simtracks' references to simvertices.
  /// otherwise simtracks and simvertices are never dereferenced.
  void unReference();

  /**
   *  return associations
   */
  std::vector < vertigo::SimTrack > associatedTracks ( const rave::Track & ) const;
  std::vector < rave::Track > associatedTracks ( const vertigo::SimTrack & ) const;
  std::vector < vertigo::SimParticle > associatedParticles ( const rave::KinematicParticle & ) const;
  std::vector < rave::KinematicParticle > associatedParticles ( const vertigo::SimParticle & ) const;
  std::vector < vertigo::SimVertex > associatedVertices ( const rave::Vertex & ) const;
  std::vector < rave::Vertex > associatedVertices ( const vertigo::SimVertex & ) const;
  /** number of rec tracks that can be associated with a sim vertex */
  int nRecTracks ( const vertigo::SimVertex & vtx ) const;

  /** is a vertex reconstructible?
   *  does it have two or more associated rec tracks? */
  bool isReconstructible ( const vertigo::SimVertex & vtx ) const; 
  

private:

  void associateVertices ( const std::vector < vertigo::SimVertex > &, const std::vector < rave::Vertex > & );
  vertigo::AssociationManager theManager;
  int theId;
  int theRun;
  std::string theTag;
  std::string theAlgo;
  float theTime;
  std::vector < rave::Track > theTracks;
  std::vector < vertigo::SimTrack > theSimTracks;
  std::vector < rave::KinematicParticle > theParticles;
  std::vector < vertigo::SimParticle > theSimParticles;
  std::vector < vertigo::SimVertex > theSimVertices;
  std::vector < rave::Vertex > theVertices;
  std::vector < vertigo::Jet > theJets;
};
}

#endif

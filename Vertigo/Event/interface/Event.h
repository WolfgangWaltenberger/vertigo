#ifndef Event_H
#define Event_H

#include <vector>
#include <string>
#include <rave/Vertex.h>
#include <rave/Track.h>
#include <rave/KinematicParticle.h>
#include "Environment/VertigoObjects/interface/VertigoJet.h"
#include "Environment/MCObjects/interface/SimVertex.h"
#include "Environment/MCObjects/interface/SimTrack.h"
#include "Vertigo/Event/interface/BasicEvent.h"

namespace vertigo {
class Event : private BasicEvent::Proxy
{
    typedef BasicEvent::Proxy Base;
  public:
    Event ( const std::vector < rave::Track > &,
                   const std::vector < vertigo::SimTrack > &,
                   const std::vector < vertigo::SimVertex > &,
                   int id = 0, int run = 0, const std::string & tag = "",
                   const std::string & algo = "" );

    Event ( const std::vector < vertigo::Jet > &,
                   const std::vector < vertigo::SimTrack > &,
                   const std::vector < vertigo::SimVertex > &,
                   int id = 0, int run = 0, const std::string & tag = "",
                   const std::string & algo = "" );

    Event ( const std::vector < rave::KinematicParticle > &,
                   const std::vector < vertigo::SimParticle > &,
                   const std::vector < vertigo::SimVertex > &,
                   int id = 0, int run = 0, const std::string & tag = "",
                   const std::string & algo = "" );

    Event ( int id = 0, int run = 0, const std::string & tag = "",
                   const std::string & algo = "" );

    ~Event();

    std::vector < rave::Track > tracks() const;
    std::vector < rave::Vertex > vertices() const;
    std::vector < rave::KinematicParticle > particles() const;
    std::vector < vertigo::SimTrack > simTracks() const;
    std::vector < vertigo::SimParticle > simParticles() const;
    std::vector < vertigo::SimVertex > simVertices() const;
    std::vector < vertigo::Jet > jets() const;
    std::vector < vertigo::Jet > & jetRefs();
    void clearTracks();
    void removeParticles();
    
    void add ( const rave::Track & );
    void add ( const std::vector < rave::Track > & );
    void addTracks ( const std::vector < rave::Track > & );

    void add ( const rave::KinematicParticle & );
    void add ( const std::vector < rave::KinematicParticle > & );
    void addParticles ( const std::vector < rave::KinematicParticle > & );

    void add ( const vertigo::Jet &, bool addtracks = true );
    void add ( const std::vector < vertigo::Jet > &, bool addtracks = true );
    void addJets ( const std::vector < vertigo::Jet > &, bool addtracks = true );

    void add ( const vertigo::SimTrack & );
    void add ( const std::vector < vertigo::SimTrack > & );
    void addSimTracks ( const std::vector < vertigo::SimTrack > & );

    void add ( const vertigo::SimParticle & );
    void add ( const std::vector < vertigo::SimParticle > & );
    void addSimParticles ( const std::vector < vertigo::SimParticle > & );

    void add ( const vertigo::SimVertex & );
    void add ( const std::vector < vertigo::SimVertex > & );
    void addSimVertices ( const std::vector < vertigo::SimVertex > & );

    void add ( const rave::Vertex & );
    void add ( const std::vector < rave::Vertex > & );
    void addVertices ( const std::vector < rave::Vertex > & );

    void associateTracks();
    void associateVertices();
    void associate ( const vertigo::SimTrack &, const rave::Track & );
    void associate ( const vertigo::SimParticle &, const rave::KinematicParticle & );
    void associate ( const vertigo::SimVertex &, const rave::Vertex & );
    void setTime ( float ); //< set time (in ms?)
    void setAlgorithm ( const std::string & algo ); //< define algo used
    void setTag ( const std::string & tag ); //< set event tag
    void unReference();

    int id() const;
    int run() const;
    std::string tag() const;
    void tag ( const vertigo::Jet & j, double value ); //< tag a jet
    std::string algorithm() const;
    float time() const; //< time, given in ms?

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
  };
}

#endif

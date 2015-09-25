#ifndef EventPrinter_H
#define EventPrinter_H

#include "Analysis/Observer/interface/Observer.h"
#include "Analysis/Score/interface/SimpleScore.h"

class EventPrinter : public Observer {
public:
  /**
   *  Prints the event to stdout
   *  Options (being passed to configure)
   *  - short: make it short
   */
  EventPrinter();

  void process ( const vertigo::Event & );
  void process ( const std::exception &, const std::string & );
  void configure ( const std::string & );
  ~EventPrinter();
  EventPrinter * clone() const;
private:
  void printJets ( const vertigo::Event & ) const;
  void printJetVertices ( const vertigo::Event &, const vertigo::SimVertex & primary ) const;
  void printParticles ( const vertigo::Event & ) const;
  void printHeader ( const vertigo::Event & ) const;
  void printRecTrack ( const rave::Track & trk, bool print_params=true,
     const std::string & comment="", float weight=1.0, bool print_weight=false ) const;
  std::vector < rave::Track > printSimTrack ( const vertigo::SimTrack & trk,
     const std::vector < rave::Track > & iids ) const;
  void printRefittedRecTrack ( const rave::Track & trk, const vertigo::SimTrack & sim,
    bool print_params=true, const std::string & comment="", float weight=1.0, bool print_weight=false ) const;
  void printRefittedTracks ( const rave::Vertex & vtx, const vertigo::Event & e ) const;
  void printRecVertex ( const rave::Vertex & vtx, const vertigo::SimVertex & pr, const std::vector < vertigo::SimVertex > & ass, const vertigo::Event & e, bool Short ) const;
  void printRecVertex ( const rave::Vertex & vtx, const vertigo::Event & e, bool Short ) const;
  std::vector < rave::Track > printSimVertex ( const vertigo::SimVertex & vtx,
                const vertigo::SimVertex & pr, const vertigo::Event & e, bool Short ) const;
  void printKinematicParticle ( const rave::KinematicParticle & p, bool print_params,
                                             const std::string & comment, float weight, bool print_weight ) const;

private:
  SimpleScore theScore;
  bool theShort;
  bool printEuclidean;
  bool printPerigee;
};

#endif

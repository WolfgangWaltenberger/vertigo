#ifndef FindingObserver_H
#define FindingObserver_H

#include "Analysis/Observer/interface/Observer.h"
#include "Environment/MCObjects/interface/SimTrack.h"
#include "Environment/MCObjects/interface/SimVertex.h"
#include <rave/Vertex.h>
#include <rave/Track.h>
#include <string>

class FindingObserver : public Observer {
  /**
   *  \class FindingObserver
   *  Observer that is meant to implement the analyses
   *  that will go into a paper
   *  "Geometrical properties of vertex finding algorithms"
   *
   *  It's about how vertex finders score as a function
   *  of event topology (vertexgun).
   */
public:
  FindingObserver();
  ~FindingObserver();
  void process ( const vertigo::Event & );
  void process ( const std::exception &, const std::string & );

  void configure ( const std::string & );

  FindingObserver * clone() const;

private:
  static const int mu = 10000;
  void analysis ( const std::vector < vertigo::SimTrack > & simtracks,
                  const std::vector < vertigo::SimVertex > & simvtces,
                  const std::vector < rave::Track > & rectracks, int evnr );
  void except ( const std::string & e, int id );
  void logCpuTime ( float t );
  
  /// not even primary vertex was found
  void missedPrimary ( float d ) const;
  
  /// primary but no secondary vertex was reconstructed
  void missedSecondary ( float d, float dprim, float dx, float dy, float dz,
                         const std::string & tuplename="missed_secondaries" ) const;

  /// primary and secondary vertex were reconstructed
  void foundSecondary ( const vertigo::SimVertex & sim_primary, const vertigo::SimVertex & sim_secondary,
                        const rave::Vertex & rec_primary, const rave::Vertex & rec_secondary,
                        const std::string & tuple="secondaries" ) const;

  /// a tertiary rec vertex, with no adequate sim vertex
  void extraTertiary ( const rave::Vertex & rec_secondary ) const;

  std::string theFileName;
  bool theFixFileName;
};

#endif

#ifndef _BTagAnalyser_H_
#define _BTagAnalyser_H_

#include "Analysis/Observer/interface/Observer.h"
#include "Environment/MCObjects/interface/SimVertex.h"
#include <rave/Vertex.h>
#include <rave/impl/RecoBTag/SecondaryVertex/interface/VertexSorting.h>

/*! \class BTagAnalyser
 *  Observer that analyses btags, if a tagger has been run.  It also produces
 *  info about B-Mesons, for vertex reconstruction.
 */

class BTagAnalyser : public Observer {

public:
  BTagAnalyser();
  void process ( const vertigo::Event & e );
  void process ( const std::exception &, const std::string & type, const std::string & where );
  void process ( const std::exception &, const std::string & type );
  // void process ( const edm::ParameterSet & );

  /**
   *  configure your observer - it's up to you,
   *  what you want to do with the string.
   */
  void configure ( const std::string & );

  ~BTagAnalyser();
  BTagAnalyser * clone() const;

private:
  void saveMeta( const vertigo::Event & ); //< save meta information
  void tagJets( const vertigo::Event & ) const; //< store jet tags

  void saveEvent(const vertigo::Event &, float time ) const; //< save per-event data
  float saveJets(const vertigo::Event & ) const; //< save per-jet data, return total time

  /// save the monte carlo truth about the B Mesons'
  /// flight
  void saveBFlightPath ( int eventid, const vertigo::SimVertex & primary, 
                         const vertigo::SimVertex & secondary ) const;

  ///
  /// For some reason we cannot define a "true" B Meson flight path.
  /// This method documents why.
  void saveMissingMonteCarloFlightPath ( int eventid, const vertigo::SimVertex & primary,
      const vertigo::SimVertex & secondary ) const;

  /// save some vertex reconstruction info
  void saveRecoVertices ( const vertigo::Event &,
                          const vertigo::SimVertex & primary,
                          const vertigo::SimVertex & b ) const;

  /// save statistics, no details.
  void saveRecoVertexStat ( const vertigo::Event &,
                          const vertigo::SimVertex & primary,
                          const vertigo::SimVertex & b ) const;


  /// save primary vertex stats
  void savePrimaryVertex ( const rave::Vertex &,
                     const vertigo::SimVertex & ) const;

  /// save stats of the B decay vertex
  void saveBVertex ( const rave::Vertex &,
                     const vertigo::SimVertex & ) const;

  /// here is the flight path, sim and reco
  void saveFlightPath ( const rave::Vertex & primrec,
                        const vertigo::SimVertex & primsim,
                        const rave::Vertex & bmesonrec,
                        const vertigo::SimVertex & bmesonsim ) const;

  /// describe what went wrong?
  void describeFailure ( bool has_prim, bool has_sec ) const;

  std::string theFileName;
  bool theSaveTagsOnly;
  bool theIsFirst;
  bool theDescribe;
  reco::VertexSorting theSorter;
};

#endif // _BTagAnalyser_H_

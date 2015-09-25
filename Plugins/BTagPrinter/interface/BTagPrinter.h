#ifndef _BTagPrinter_H_
#define _BTagPrinter_H_

#include "Analysis/Observer/interface/Observer.h"
#include "Environment/MCObjects/interface/SimVertex.h"
#include <rave/Vertex.h>

/*! \class BTagPrinter
 *  Observer that prints btag info to stdout.
 */

class BTagPrinter : public Observer {

public:
  BTagPrinter();
  void process ( const vertigo::Event & e );
  void process ( const std::exception &, const std::string & type );

  /**
   *  configure your observer - it's up to you,
   *  what you want to do with the string.
   */
  void configure ( const std::string & );

  ~BTagPrinter();
  BTagPrinter * clone() const;

private:
  /// print the monte carlo truth about the B Mesons'
  /// flight
  void printBFlightPath ( const vertigo::SimVertex & primary, 
                         const vertigo::SimVertex & secondary ) const;

  /// print some vertex reconstruction info
  void printRecoVertices ( const vertigo::Event &,
                          const vertigo::SimVertex & primary,
                          const vertigo::SimVertex & b ) const;

  /// here is the flight path, sim and reco
  void printFlightPath ( const rave::Vertex & primrec,
                        const vertigo::SimVertex & primsim,
                        const rave::Vertex & bmesonrec,
                        const vertigo::SimVertex & bmesonsim ) const;

};

#endif // _BTagPrinter_H_

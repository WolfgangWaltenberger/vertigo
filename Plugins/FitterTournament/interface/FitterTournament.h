#ifndef _FitterTournament_H_
#define _FitterTournament_H_

#include "Analysis/Observer/interface/Observer.h"
#include "VertigoHarvest/interface/VertexHisto.h"
#include "Vertigo/Event/interface/Event.h"
#include "RecoVertex/VertexPrimitives/interface/VertexReconstructor.h"

/*! \class FitterTournament
 * write event into file
 */

class FitterTournament : public Observer {

public:
  FitterTournament();
  ~FitterTournament();
  void process ( const vertigo::Event & );
  void process ( const std::exception &, const std::string & );
  void configure ( const std::string & filename );
  void writeSource ( const std::string & s );
  FitterTournament * clone() const;
private:
  void destroy();
  void setup ( const std::string & prefix );
  void setup ( );
  std::map < std::string, VertexReconstructor * > theRectors;
  std::map < std::string, VertexHisto * > theHistos;
  bool hasWrittenSource;
};

#endif

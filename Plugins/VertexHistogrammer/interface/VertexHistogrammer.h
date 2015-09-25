#ifndef _VertexHistogrammer_H_
#define _VertexHistogrammer_H_

#include "Analysis/Observer/interface/Observer.h"
#include "VertigoHarvest/interface/VertexHisto.h"
#include "Vertigo/Event/interface/Event.h"
#include "RecoVertex/VertexPrimitives/interface/VertexReconstructor.h"

/*! \class VertexHistogrammer
 * write event into file
 */

class VertexHistogrammer : public Observer {

public:
  VertexHistogrammer();
  VertexHistogrammer ( const VertexHistogrammer & );
  ~VertexHistogrammer();
  void process ( const vertigo::Event & );
  void process ( const std::exception &, const std::string & );
  void configure ( const std::string & filename );
  VertexHistogrammer * clone() const;
private:
  void destroy();
  void setup ( );
  VertexHisto * theHisto;
};

#endif

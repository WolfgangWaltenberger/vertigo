#ifndef Score_H
#define Score_H

#include "Vertigo/Event/interface/Event.h"
#include <rave/impl/DataFormats/GeometryCommonDetAlgo/interface/Measurement1D.h>

class Score {
public:
  /**
   *   pABC for all classes that score rave::Events.
   *   This will be useful for automatic tuning of algorithms.
   *   And comparisons.
   */
  virtual void operator() ( const vertigo::Event & ) = 0;
  virtual Measurement1D efficiency() const = 0;
  virtual Measurement1D fake() const = 0;

  virtual Score * clone() const = 0;
  virtual ~Score() {};
};

#endif

#ifndef EventCombiner_H
#define EventCombiner_H

#include "Vertigo/Event/interface/Event.h"

namespace vertigo {
class EventCombiner {
public:
  void add ( vertigo::Event &, const vertigo::Event & ) const;
};
}

#endif

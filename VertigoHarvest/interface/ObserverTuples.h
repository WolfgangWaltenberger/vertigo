#ifndef _ObserverTuples_H_
#define _ObserverTuples_H_

#include <dataharvester/MultiType.h>
#include <dataharvester/Tuple.h>

#include <rave/Vertex.h>
#include <rave/Track.h>
#include "Environment/MCObjects/interface/SimVertex.h"
#include "Environment/MCObjects/interface/SimTrack.h"
#include "Environment/VertigoObjects/interface/VertigoJet.h"

/**
 *  A class/namespace which 
 *  creates standard dh::Tuples from standard rave/vertigo objects.
 *  For standard Observers.
 */

class ObserverTuples {
public:
  static void addSimVertex ( const vertigo::SimVertex &, dataharvester::Tuple & );
  static void addVertex ( const rave::Vertex &, dataharvester::Tuple &, float w=1.0 );
  static void addAssociatedVertex ( const vertigo::SimVertex &, const rave::Vertex &, dataharvester::Tuple & );
  static void addJet ( const vertigo::Jet &, dataharvester::Tuple & );
};

#endif

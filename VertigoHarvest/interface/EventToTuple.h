#ifndef EventToTuple_H
#define EventToTuple_H

#include <string>
#include <vector>
#include <map>
#include <dataharvester/MultiType.h>
#include <dataharvester/Tuple.h>
#include <rave/Point3D.h>
#include <rave/Vector3D.h>
#include <rave/Track.h>
#include <rave/KinematicParticle.h>
#include <rave/Vertex.h>
#include "Environment/VertigoObjects/interface/VertigoJet.h"
#include "Environment/MCObjects/interface/SimTrack.h"
#include "Environment/MCObjects/interface/SimVertex.h"

class RaveVector6D;
class RaveCovariance3D;
class RaveCovariance6D;

namespace vertigo {
class Event;
class EventToTuple {
  /**
   *  \class A class that creates a Tuple from a vertigo::Event.
   */
public:
  dataharvester::Tuple create ( const vertigo::Event &, bool propagate=false, bool colors=false );

private:
  void add ( dataharvester::Tuple &, const rave::Track &, const std::vector < vertigo::SimTrack > & ,
             const vertigo::SimVertex & vtx,
             const std::map < std::string, dataharvester::MultiType > &, bool propagate=true,
             const std::string & prefix = "" );
  void add ( dataharvester::Tuple &, const rave::KinematicParticle &,
             const std::vector < vertigo::SimParticle > & ,
             const vertigo::SimVertex & vtx,
             const std::map < std::string, dataharvester::MultiType > &,
             const std::string & prefix = "" );
  void add ( dataharvester::Tuple &, const rave::Vertex &,
             const std::map < std::string, dataharvester::MultiType > & );
  void add ( dataharvester::Tuple &, const vertigo::SimTrack &,
             const std::map < std::string, dataharvester::MultiType > & );
  void add ( dataharvester::Tuple &, const vertigo::Jet &,
             const std::map < std::string, dataharvester::MultiType > & );
  void add ( dataharvester::Tuple &, const vertigo::SimVertex &, 
             const std::map < std::string, dataharvester::MultiType > &, bool primary=false );
  void add ( dataharvester::Tuple &, const vertigo::SimParticle &, 
             const std::map < std::string, dataharvester::MultiType > & );

  // remember the objects that have been added already
  std::map < int, bool > hasAdded_;
  std::map < int, bool > hasAddedP_;
};
}

#endif

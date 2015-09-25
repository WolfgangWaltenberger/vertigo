#ifndef _LoopSettings_H_
#define _LoopSettings_H_

/*! @class LoopSettings
 *  Singleton class that holds all data
 *  that define the "environment" in which the
 *  event loop runs. 
 *  Available to all threads.
 */

#include <string>
#include <rave/FlavorTagFactory.h>
#include <rave/VertexFactory.h>
#include "Vertigo/EventGeneration/interface/EventFactory.h"

class LoopSettings {
public:
  static LoopSettings & Instance();
  void set ( const std::string & source, const std::string & method, 
             const std::string & flavortagmethod, int min, signed max,
             int verbosity, bool usebeamspot, int nthreads, int expect );

  int verbosity() const;
  std::string method() const;
  std::string flavorTagMethod() const;
  std::string source() const;
  int min() const;
  signed max() const;
  int expect() const; //< how many events do we expect?
  bool useBeamSpot() const;
  bool performFlavorTagging() const;
  bool fancyCounter() const;
  int barLength() const;

  /** define the verbosity.
   *  \param silent be verbose setting the verbosity?
   */
  void setVerbosity ( int, bool silent=false );
  void setPerformFlavorTagging ( bool );
  void setLogToFile ( bool );
  bool logToFile () const; //< do we log to file or to stdout?
  
  void setBarLength ( int n );

  void setVertexFactory ( const rave::VertexFactory * );
  void setFlavorTagFactory ( rave::FlavorTagFactory * );

  const rave::VertexFactory * vertexFactory() const;
  rave::VertexFactory * nonConstVertexFactory() const;
  rave::FlavorTagFactory * flavorTagFactory() const;

  void setEventFactory ( const EventFactory & f );
  const EventFactory & eventFactory() const;

  int nThreads() const;

private:
  LoopSettings();

private:
  std::string theSource;
  std::string theMethod;
  std::string theFlavorTagMethod;
  int theMin;
  signed theMax;
  int theVerbosity;
  bool theUseBeamSpot;
  bool thePerformFlavorTagging;
  const rave::VertexFactory * theVtxFactory;
  rave::FlavorTagFactory * theFlavorTagFactory;
  EventFactory theEventFactory;
  int theNThreads;
  bool theFancyCounter;
  int theBarLength;
  int theExpect;
  bool theLogToFile;
};

#endif // _LoopSettings_H_

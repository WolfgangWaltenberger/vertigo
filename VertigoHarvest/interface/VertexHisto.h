#ifndef _VertexHisto_H_
#define _VertexHisto_H_

#include <string>
#include <utility>
#include <vector>
#include <map>

#include <dataharvester/MultiType.h>
#include <dataharvester/Tuple.h>

#include <rave/Vertex.h>
#include <rave/Track.h>

#include "Environment/MCObjects/interface/SimVertex.h"
#include "Vertigo/Event/interface/Event.h"

/**
 *  Vertex histograms
 */

class VertexHisto {
public:
  VertexHisto( const std::string & str );
  void configure ( const std::string & str );
  const std::string & fileName() const;

  // saves the relevant numbers,
  void fill( const vertigo::SimVertex &, const rave::Vertex &, float time,
             dataharvester::Tuple & extras, int evctr, int vtxnr=0 );
  void fill( const vertigo::SimVertex &, const rave::Vertex &, float time,
             int evctr, int vtxnr=0 );
  void fill( const vertigo::SimVertex &, float time, int evctr, int vtxnr=0 );
  void fillMissing( const vertigo::SimVertex &, float time, int evctr, int vtxnr=0 );
  void fillExtra( const rave::Vertex &, float time, int evctr, int vtxnr=0 );

  void writeSource ( const std::string & s ) const;

  void except ( const std::string & what, const std::string & type, int evctr=-1 );
  void event ( const vertigo::Event & e ); //< also log event stats
  void tick();
  void stamp();

  void close(); //< needs to be called explicitly at end

private:
  void add ( const vertigo::SimVertex &, const rave::Vertex &,
             float time, dataharvester::Tuple &, int evctr, int vtxnr );
  void add ( const vertigo::SimVertex &, float time,
             dataharvester::Tuple &, int evctr, int vtxnr );
  void add ( const rave::Vertex &, float time,
             dataharvester::Tuple &, int evctr, int vtxnr, float w=1.0 );

  /** Add this classes' data members.
   */
  void addDataMembers ( dataharvester::Tuple & ) const;
  void discussTracks ( const rave::Vertex & rv, const vertigo::SimVertex & sv, int evnr );
  void save ( const dataharvester::Tuple & ) const;

private:
  std::string theFileName;
  int theNEvents;
  int theNSimEvents;
  // std::vector < std::map < std::string, dataharvester::MultiType > > theLastTrackMaps;
  bool hasStamped;
  vertigo::Event theEvent;
};

#endif

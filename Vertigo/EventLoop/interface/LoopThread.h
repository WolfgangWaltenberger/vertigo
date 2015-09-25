#ifndef _LoopThread_H_
#define _LoopThread_H_

/*! \class LoopThread
 *  One thread of a loop.
 */

#include "Vertigo/Event/interface/Event.h"

class LoopThread {
public:
  LoopThread ( int first, signed n, bool has_method );
  LoopThread ();
  void operator()();
  bool processEvent( int n );

private:
  void tagJets( vertigo::Event & e, int n, int spev );
  void mark ( int );
  void nextMarker();

private:
  int theFirst;
  signed theN;
  bool theHasMethod;
  int theLastMark;
  time_t theStart;
  char theMarker;

};

#endif // _LoopThread_H_

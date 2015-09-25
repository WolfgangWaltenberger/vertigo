#ifndef ObjectCoder_H
#define ObjectCoder_H

#include "Analysis/Observer/interface/Observer.h"
#include <rave/Track.h>
#include <rave/KinematicParticle.h>
#include <rave/Covariance6D.h>

class ObjectCoder : public Observer {
public:
  /**
   *  Prints the objects to stdout,
   *  such that they can be copied and
   *  pasted into C++ / python 
   *  code
   *  Options (being passed to configure)
   *  - python: output python, not c++
   */
  ObjectCoder();

  void process ( const vertigo::Event & );
  void process ( const std::exception &, const std::string & );
  void configure ( const std::string & );
  ObjectCoder * clone() const;
private:
  void printRecTrack ( const rave::Track & trk ) const;
  void printKinematicParticle ( const rave::KinematicParticle & ) const;
  void printCovariance ( const rave::Covariance6D & ) const;
  void printCovariance ( const rave::Covariance7D & ) const;
private:
  bool python_;
};

#endif

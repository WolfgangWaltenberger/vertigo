#ifndef _Observer_H_
#define _Observer_H_

#include <string>
#include <exception>
// #include <rave/impl/FWCore/ParameterSet/interface/ParameterSet.h>

namespace vertigo {
class Event;
}

/*! \class Observer
 *  The definition of an event observer
 */

class Observer {

public:

  /**
   *  propagate an event to the observer
   */
  virtual void process ( const vertigo::Event & e ) = 0;

  /**
   *  propagate an exception to the observer
   */
  virtual void process ( const std::exception &, const std::string & type ) = 0;

  /**
   * propagate the ParameterSet to the observer
   */
  // virtual void process ( const edm::ParameterSet & ) {};

  /**
   *  configure the observer
   */
  virtual void configure ( const std::string & ) = 0;
  virtual ~Observer() {};
  virtual Observer * clone() const = 0;
};

#endif // _Observer_H_

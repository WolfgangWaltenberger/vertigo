#ifndef Observer_Skeleton_H_
#define Observer_Skeleton_H_

#include "Analysis/Observer/interface/Observer.h"

/*! \class Skeleton
 *  Skeleton code for an observer
 */

class Skeleton : public Observer {

public:

  /**
   *  propagate an event to your observer
   */
  void operator() ( const vertigo::Event & e );

  /**
   *  propagate an exception to your observer
   */
  void operator() ( const std::exception & );

  /**
   *  configure your observer - it's up to you,
   *  what you want to do with the string.
   */
  void configure ( const std::string & );

  ~Skeleton();
  Skeleton * clone() const;
};

#endif // _Skeleton_H_

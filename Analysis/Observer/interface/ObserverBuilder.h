#ifndef _ObserverBuilder_H_
#define _ObserverBuilder_H_

#include "Analysis/Observer/interface/ObserverManager.h"

/*! @class ObserverBuilder
 *  template class that registers an observer
 */

template < class O > class ObserverBuilder {
public:
  ObserverBuilder < O > ( const std::string & name, const std::string & description, bool enable  )
  {
    ObserverManager::Instance().registerObserver ( name, new O(), description, enable );
  }
};

#endif // _ObserverBuilder_H_

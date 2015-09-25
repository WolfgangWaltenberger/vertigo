#ifndef _GeneratorBuilder_H_
#define _GeneratorBuilder_H_

#include "Vertigo/EventGeneration/interface/GeneratorManager.h"

/*! @class GeneratorBuilder
 *  template class that registers an observer
 */

template < class O > class GeneratorBuilder {
public:
  GeneratorBuilder < O > ( const std::string & name, const std::string & description )
  {
    GeneratorManager::Instance().registerGenerator ( name, new O(), description );
  }
};

#endif // _GeneratorBuilder_H_

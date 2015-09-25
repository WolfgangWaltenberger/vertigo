#ifndef _VertexFactoryBuilder_H_
#define _VertexFactoryBuilder_H_

#include "Vertigo/EventLoop/interface/VertexMetaFactory.h"
#include <iostream>

/*! @class VertexFactoryBuilder
 *  template class that registers a vertex factory
 */

template < class O > class VertexFactoryBuilder {
public:
  VertexFactoryBuilder < O > ( const std::string & name, const std::string & description )
  {
    VertexMetaFactory::Instance().registerFactory ( name, new O(), description );
  }
};

#endif // _VertexFactoryBuilder_H_

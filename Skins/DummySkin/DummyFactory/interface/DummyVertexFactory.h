#ifndef _RaveInterface_DummyVertexFactory_H_
#define _RaveInterface_DummyVertexFactory_H_

#include <rave/VertexFactory.h>

namespace rave
{

class DummyVertexFactory  : public VertexFactory
{
  public:
    DummyVertexFactory ();
    void init();
};
}

#endif

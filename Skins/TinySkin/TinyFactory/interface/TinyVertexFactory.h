#ifndef _RaveInterface_TinyVertexFactory_H_
#define _RaveInterface_TinyVertexFactory_H_

#include <rave/VertexFactory.h>

namespace rave
{

class TinyVertexFactory  : public VertexFactory
{
  public:
    TinyVertexFactory ();
    void init();
};
}

#endif

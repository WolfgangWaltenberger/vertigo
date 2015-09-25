#ifndef _RaveInterface_ZeroVertexFactory_H_
#define _RaveInterface_ZeroVertexFactory_H_

#include <rave/VertexFactory.h>

namespace rave
{

class ZeroVertexFactory  : public VertexFactory
{
  public:
    ZeroVertexFactory ();
    void init();
};
}

#endif

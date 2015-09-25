#ifndef _RaveInterface_Belle2VertexFactory_H_
#define _RaveInterface_Belle2VertexFactory_H_

#include <rave/VertexFactory.h>

namespace rave
{

class Belle2VertexFactory  : public VertexFactory
{
  public:
    Belle2VertexFactory ();
    void init();
};
}

#endif

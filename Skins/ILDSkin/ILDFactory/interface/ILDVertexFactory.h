#ifndef _RaveInterface_ILDVertexFactory_H_
#define _RaveInterface_ILDVertexFactory_H_

#include <rave/VertexFactory.h>

namespace rave
{

class ILDVertexFactory  : public VertexFactory
{
  public:
    ILDVertexFactory ();
    void init();
};
}

#endif

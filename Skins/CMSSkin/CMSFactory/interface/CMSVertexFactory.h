#ifndef _RaveInterface_CMSVertexFactory_H_
#define _RaveInterface_CMSVertexFactory_H_

#include <rave/VertexFactory.h>

namespace rave
{

class CMSVertexFactory  : public VertexFactory
{
  public:
    CMSVertexFactory ();
    void init();
};
}

#endif

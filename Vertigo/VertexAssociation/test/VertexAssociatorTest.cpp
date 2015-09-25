#include "Utilities/Configuration/interface/Architecture.h"

#include "Vertex/VertexAssociation/interface/VertexAssociatorTest.h"
#include "Vertex/PrincipalVertexReco/interface/DefaultPrincipalVertexReconstructor.h"
#include "Utilities/Notification/interface/PackageInitializer.h"

class VertexAssociatorTestWithPVR : 
    public VertexAssociatorTest 
{
public:
  VertexAssociatorTestWithPVR()
    : VertexAssociatorTest(new DefaultPrincipalVertexReconstructor()) {}

  ~VertexAssociatorTestWithPVR () {}

};

static PKBuilder<VertexAssociatorTestWithPVR>
VertexAssociatorTestWithPVR("VertexAssociatorTestWithPVR");

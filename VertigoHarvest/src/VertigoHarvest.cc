#include "VertigoHarvest/interface/VertigoHarvest.h"

#ifdef WITH_HARVESTER
#include <dataharvester/Writer.h>
using namespace dataharvester;
#endif

void VertigoHarvest::close()
{
  #ifdef WITH_HARVESTER
  Writer::close();
  #endif
}


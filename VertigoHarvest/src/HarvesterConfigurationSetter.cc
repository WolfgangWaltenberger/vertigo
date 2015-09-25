#include "VertigoHarvest/interface/HarvesterConfigurationSetter.h"

#ifdef WITH_HARVESTER
#include <dataharvester/ConfigurationSetter.h>
#endif

void HarvesterConfigurationSetter::set ( const std::string & s )
{
  #ifdef WITH_HARVESTER
  dataharvester::ConfigurationSetter::set ( s );
  #endif
}


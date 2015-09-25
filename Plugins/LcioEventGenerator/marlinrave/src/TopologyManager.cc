#include "TopologyManager.h"

#include <vector>
#include <iostream>
#include <dlfcn.h>
#include <cstdlib>

namespace marlinrave {

void TopologyManager::registerTopology(
  const std::string & name, 
  boost::shared_ptr< KinematicTopology > t, 
  const std::string & description)
{
  theTopologies[ name ] = t;
  theDescriptions[ name ] = description;
}

TopologyManager::~TopologyManager()
{
}

std::string TopologyManager::describe ( const std::string & name )
{
  return theDescriptions[ name ];
}

TopologyManager::TopologyManager ( const TopologyManager & t )
{
  std::cout << "[TopologyManager] copy constructor! Error!" << std::endl;
  exit(0);
}

TopologyManager & TopologyManager::Instance()
{
  static TopologyManager singleton;
  return singleton;
}

boost::shared_ptr< KinematicTopology > TopologyManager::get ( 
    const std::string & name, bool verbose )
{
  std::cout << "[TopologyManager] trying to obtain " << name << std::endl;
  if ( theTopologies[ name ] ) return theTopologies[ name ];
  std::cout << "[TopologyManager] could not find " << name << std::endl;
  return boost::shared_ptr< KinematicTopology >();
}

std::map < std::string, boost::shared_ptr< KinematicTopology > > 
TopologyManager::get()
{
  return theTopologies;
}

TopologyManager::TopologyManager()
{
}

} // namespace marlinrave


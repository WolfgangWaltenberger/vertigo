#ifndef _TopologyManager_H_
#define _TopologyManager_H_

#include "KinematicTopology.h"

#include "boost/shared_ptr.hpp"

#include <map>
#include <string>

namespace marlinrave {

typedef std::map < std::string, boost::shared_ptr< KinematicTopology > >
        topology_map;

/**
 * @brief Manages topologies for kinematic reconstruction
 */

class TopologyManager {

  public:
    static TopologyManager & Instance();
    void registerTopology( const std::string & name,
                           boost::shared_ptr< KinematicTopology > t,
                           const std::string & description );
    std::string describe( const std::string & );
    boost::shared_ptr< KinematicTopology > get( const std::string &,
                                                bool verbose = true );
    topology_map get();
    ~TopologyManager();

  private:
    TopologyManager(const TopologyManager &);
    TopologyManager();
    topology_map theTopologies;
    std::map < std::string, std::string > theDescriptions;

};

} // namespace marlinrave

#endif // _TopologyManager_H_


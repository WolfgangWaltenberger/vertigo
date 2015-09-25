#ifndef TopologyBuilder_H
#define TopologyBuilder_H

#include "TopologyManager.h"

namespace marlinrave {

/** 
 * @brief Template class that registers the topologies.
 * @ingroup MarlinRave
 * @author Fabian Moser
 *
 * This class is a simple helper which facilitates an easy-to-use and elegant
 * way of registering new topologies with the #TopologyManager. It avoids the
 * need of direct use of the manager by the creator of the topology.
 */

template< class T > class TopologyBuilder 
{
  public:
    /**
     * @brief Registers the template class with the #TopologyManager.
     *
     * The registration is done upon instantiation of the templated
     * TopologyBuilder.
     *
     * @param name The name of the topology to be registered. It is this
     *             name which has to be used in the Marlin steer file to
     *             access the topology.
     * @param description This description allows for a more detailed 
     *                    explanation of the topology than the name.
     */
    TopologyBuilder< T >( const std::string & name, 
                          const std::string & description )
    {
      TopologyManager::Instance().registerTopology( 
        name, 
        boost::shared_ptr< T >( new T() ), 
        description );
    }
};

} // namespace marlinrave

#endif // _TopologyBuilder_H_


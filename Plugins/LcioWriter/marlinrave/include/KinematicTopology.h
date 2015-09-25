#ifndef KinematicTopology_H
#define KinematicTopology_H

#include "marlin/Processor.h"
#include "rave/KinematicParticle.h"
#include "rave/KinematicTree.h"
#include "rave/KinematicTreeFactory.h"

#include <string>
#include <vector>

namespace marlinrave {

/** 
 * @brief Base class for kinematic fitting topologies.
 * @ingroup MarlinRave
 * @author Fabian Moser
 *
 * This abstract base class defines the interface for decay topologies 
 * involved by the RaveKinematics processor. Its derivatives only need to
 * override the build() method to implement the desired decay hypothesis
 * using the supplied #KinematicTreeFactory together with the given
 * particles.
 */

class KinematicTopology 
{
  public:
    /**
     * The abstract build method needs to be overridden by the deriving 
     * topology. This is where the reconstruction code has to be placed.
     * Use the supplied #rave::KinematicTreeFactory together with the 
     * #rave::KinematicParticle instances and perhaps additional instances 
     * of #rave::KinematicConstraint which can be generated using the
     * #rave::KinematicConstraintBuilder
     *
     * @param factory Use this factory class to involve the kinematic 
     *                fitting facilities provided by the Rave library
     * @param particles These are the particle instances read from the LCIO
     *                  database and converted to their Rave familiars.
     * @param verbose This additional parameter serves as a simple debugging
     *                flag with a per-topology control.
     * @return The result of the reconstruction is a #rave::KinematicTree
     *         containing the input particles together with newly generated
     *         mother particles structured in graph-like manner.
     */
    virtual rave::KinematicTree build(
              const rave::KinematicTreeFactory & factory,
              const std::vector< rave::KinematicParticle > & particles,
              const int verbose ) const = 0;
    
    /**
     * This method is called immediately after the construction. It allows
     * the parsing of optional parameters read from the steering file.
     *
     * @param parameters This is a string read from the Marlin steering file
     *                   It contains the value of the "parameters" XML tag of
     *                   the controlling RaveKinematics instance.
     */
    virtual void setup( const std::string & parameters ) {};

    /** 
     * This is called immediately before the first invocation of the build() 
     * method. It also gives access to the #RaveKinematics instance which 
     * actually involves the KinematicTopology.
     *
     * @param processor This is a pointer to the involving RaveKinematics
     *                  processor.
     */
    virtual void prepare( const marlin::Processor* processor ) {};
       
    /**
     * This method serves as validator of the class parameters. If the 
     * topology is not usable return false here to neglect any invocations
     * of the build() method. This can be used for incomplete or erroneous 
     * topologies and to validate the parameters given by the setup() method.
     *
     * @return The build() method is only involved if this returns true.
     */
    virtual bool valid() const { return true; };

};

} // namespace marlinrave

#endif


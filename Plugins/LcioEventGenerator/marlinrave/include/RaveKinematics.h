#ifndef RaveKinematics_H
#define RaveKinematics_H

#include "KinematicTopology.h"
#include "marlin/Processor.h"
#include "rave/KinematicTreeFactory.h"
#include "boost/shared_ptr.hpp"

#include <string>
#include <fstream>

namespace marlinrave {

/**
 * @brief Processor for kinematic reconstruction of particle decays.
 * @ingroup MarlinRave
 * @author Fabian Moser
 *
 * The RaveKinematics processor provides the Marlin toolchain with the 
 * kinematic reconstruction capabilities of the Rave toolkit. It allows the
 * application of arbitrary decay topologies which have to be implemented 
 * by deriving from the abstract #KinematicTopology class. The available 
 * topologies are dynamically identified upon loading of the MarlinRave plugin
 * and are named in the steering file comment describing the "Topology" 
 * steering parameter of this processor.
 *  
 * For instructions on how to use this processor from within the Marlin
 * framework, please refer to the @ref RaveKinematicsUsage "main page".
 */

class RaveKinematics :
      public marlin::Processor
{
  public:
    /**
     * @brief Initializes and registers the processor parameters.
     *
     * The following parameters are registered:
     *  - Particles
     *  - Beamspot
     *  - KinematicParticles
     *  - KinematicVertices
     *  - Topology
     *  - Parameters
     *  - Verbose
     *  - ParticleCovariance
     */
    RaveKinematics();

    /**
     * @brief Needed to automatically clean the reference-counted topology 
     *        pointer.
     */
    ~RaveKinematics();

    /**
     * @brief Used by the Marlin framework to duplicate instances.
     */
    virtual RaveKinematics* newProcessor();

    /**
     * @brief Initializes and configures the selected topology.
     */
    virtual void init();

    /**
     * @brief Performs the kinematic reconstruction using the data from
     *        the given event.
     *
     * This method converts the data from the selected collections of the 
     * given event and passes is to the KinematicTopology::build() method of
     * the selected topology. Afterwards it converts the resulting 
     * #rave::KinematicTree to particles and vertices storable in the LCIO
     * format.
     */
    virtual void processEvent( EVENT::LCEvent* event );

    /**
     * @brief Unused but useful for statistical checks on internat data not
     *        stored in the LCIO database.
     */
    virtual void check( EVENT::LCEvent * event );

  protected:
    /**
     * @brief Particles used as input for the kinematic reconstruction.
     */
    std::string _inputParticlesCollectionName;

    /**
     * @brief Beamspot information used to complete the particles position 
     *        information if necessary.
     */
    std::string _inputBeamspotCollectionName;

    /**
     * @brief Particles returned from the fit, linked to the vertices allow 
     *        inspection of the tree resulted from the reconstruction.
     */
    std::string _outputParticlesCollectionName;

    /**
     * @brief Vertices returned from the fit, linked to allow inspection of 
     *        their order (decay chain)
     */
    std::string _verticesCollectionName;

    /**
     * @brief Controls the verbosity on a per-processor level.
     */
    int _verbose;

    /**
     * @brief A vector of 10 floats overriding the p-E covariance of the input 
     *        particles.
     */
    FloatVec _inputParticleErrors;

    /**
     * @brief The name of the selected KinematicTopology.
     */
    std::string _topologyName;

    /**
     * @brief The parameters to be passed to the KinematicTopology::setup() 
     *        method.
     */
    std::string _topologyParameters;

    /**
     * @brief A "global" instance of the rave::KinematicTreeFactory which is 
     *        passed to the KinematicTopology::build() method.
     */
    rave::KinematicTreeFactory _factory;

    /**
     * @brief Reference counted pointer to an instance of the selected 
     *        KinematicTopology derivative.
     */
    boost::shared_ptr< KinematicTopology > _topology;
};

} // namespace marlinrave

#endif


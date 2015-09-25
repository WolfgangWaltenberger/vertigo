#ifndef RaveParticleVertexing_H
#define RaveParticleVertexing_H

#include "marlin/Processor.h"
#include "rave/VertexFactory.h"
#include "boost/shared_ptr.hpp"

#include <string>
#include <fstream>

namespace marlinrave {

/**
 * @brief Processor for vertex reconstruction from tracks or particles.
 * @ingroup MarlinRave
 * @author Fabian Moser
 *
 * The RaveParticleVertexing processor provides the Marlin toolchain with the vertex
 * fitting capabilities of the Rave toolkit. It provides various different 
 * algorithms such as:
 *  - Kalman vertex fitter
 *  - Trimmed Kalman vertex fitter
 *  - Adaptive vertex fitter
 *  - Multivertex fitter
 *  - Adaptive vertex reconstructor
 *
 * It allows detailed configuration and tuning of the available algorithms
 * through a flexible string-based interface accessible directly from the 
 * Marlin steering file or GUI.
 *
 * The RaveParticleVertexing processor takes LCIO objects of the types EVENT::Track
 * or EVENT::ReconstructedParticle as input and produces EVENT::Vertex
 * instances and optionally also new instances of the input objects refitted
 * with the newly gained vertex information.
 *  
 * For instructions on how to use this processor from within the Marlin
 * framework, please refer to the @ref RaveVertexingUsage "main page".
 */

class RaveParticleVertexing :
  public marlin::Processor
{
  public:
    /**
     * @brief Initializes and registers the processor parameters.
     *
     * The following parameters are registered:
     *  - InputParticles
     *  - OutputParticles
     *  - RelationsCandFit
     *  - Method
     *  - Verbose
     */
    RaveParticleVertexing();

    /**
     * @brief Needed to automatically clean the reference-counted pointers.
     */
    ~RaveParticleVertexing();

    /**
     * @brief Used by the Marlin framework to duplicate instances.
     */
    virtual RaveParticleVertexing* newProcessor();

    /**
     * @brief Initializes and configures the #rave::VertexFactory.
     */
    virtual void init();

    /**
     * @brief Performs the vertex fit using the data from the given event.
     *
     * This method converts the data from the selected collections of the 
     * given event. If the input collection is of type ReconstructedParticle,
     * the tracks contained with the particle are used. After calling the 
     * rave::VertexFactory it converts the resulting #rave::Vertex and the
     * refitted #rave::Track instances to vertices and tracks storable in the 
     * LCIO format.
     */
    virtual void processEvent( EVENT::LCEvent* event );

    /**
     * @brief Unused but useful for statistical checks on internat data not
     *        stored in the LCIO database.
     */
    virtual void check( EVENT::LCEvent* event );

    /**
     * @brief cleanup at end of job
     */
    virtual void end();


  protected:
    /**
     * @brief Name of the collection containing the input particles.
     *        all tracks associated to on reconstructed particle are fitted to
     *        a common vertex
     */
    std::string _inputParticleCollectionName;

    /**
     * @brief Name of the collection to be created containing the refitted
     *        reconstructedParticles updated with the vertex information.
     */
    std::string _outputParticleCollectionName;

    /**
     * @brief Name of the collection to be created containing the relations
     *        between the input candidates and the fitted decays.
     */
    std::string _relationsCandFitName;


    /**
     * @brief Name of the collection to be created containing 
     *        the fitted Vertices.
     */
    std::string _outputVertexCollName;

    /**
     * @brief Name of the collection to be created containing 
     *        the refitted Tracks.
     */
    std::string _outputTrackCollName;

    /**
     * @brief Name of the collection to be created containing 
     *        the daughter particles of the decay candidate.
     */
    std::string _outputDaughterCollName;



  
    /**
     * @brief Configuration string for the vertex fit.
     */
    std::string _method;

    /**
     * @brief Controls the verbosity on a per-processor level.
     */
    int _verbose;


    /**
     * @brief Reference-counted pointer to an instance of the 
     *        #rave::VertexFactory used for the fit.
     */
    boost::shared_ptr< rave::VertexFactory > _factory;


};

} // namespace marlinrave

#endif


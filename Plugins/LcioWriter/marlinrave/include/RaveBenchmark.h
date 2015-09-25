#ifndef RaveBenchmark_H
#define RaveBenchmark_H

#include "marlin/Processor.h"
#include "AIDA/IHistogram1D.h"
#include "boost/shared_ptr.hpp"

#include <string>

namespace marlinrave {

/**
 * @brief Generates AIDA histograms for performance evaluation purposes.
 * @ingroup MarlinRave
 * @author Fabian Moser
 *
 * This processor is intended for evaluation and benchmarking of the 
 * RaveVertexing and RaveKinematics processors. It generates a number of AIDA
 * histograms depending on the configuration. It supports a flexible number of
 * collections used for benchmarking and selects the appropriate histograms 
 * depending on the types of the configured collections.
 *  
 * For instructions on how to use this processor from within the Marlin
 * framework, please refer to the @ref RaveBenchmarkUsage "main page".
 */

class RaveBenchmark : 
      public marlin::Processor
{
  public:
    /**
     * @brief Initializes and registers the processor parameters.
     *
     * The following parameters are registered:
     *  - CollectionNames
     *  - HistBorderLow
     *  - HistBorderHigh
     */
    RaveBenchmark();

    /**
     * @brief Needed to automatically clean the reference-counted pointers.
     */
    ~RaveBenchmark();

    /**
     * @brief Used by the Marlin framework to duplicate instances.
     */
    virtual RaveBenchmark* newProcessor();

    /**
     * @brief Used to initialize the histograms for later use.
     */
    virtual void init();

    /**
     * @brief Currently not used by this processor but may be useful
     *        to later provide run dependent histograms.
     */
    virtual void processRunHeader( EVENT::LCRunHeader* );

    /**
     * @brief Generates the histogram entries and fills the histograms.
     *
     * This method acts as a router and calls one of the other process methods 
     * depending on the types of the collections to be benchmarked.
     */
    virtual void processEvent( EVENT::LCEvent* );

  protected:
    /**
     * @brief Holds the names of the collections to be benchmarked.
     */
    StringVec _collectionNames;

    /**
     * @brief Holds the lower border of the generated histograms.
     *
     * Histogram entries i.e. benchmarking results below this number are not
     * included in the histogram.
     */
    float _histBorderLow;

    /**
     * @brief Holds the upper border of the generated histograms.
     *
     * Histogram entries i.e. benchmarking results above this number are not
     * included in the histogram.
     */
    float _histBorderHigh;

    /**
     * @brief Associates a histogram with a name.
     */
    typedef std::pair< std::string, boost::shared_ptr< AIDA::IHistogram1D > >
            CollHist;

    /**
     * @brief Stores the histograms associated with their names.
     */
    std::vector< CollHist > _histograms;

    /**
     * @brief Helper method to access the collections while catching possible 
     *        exceptions.
     */
    EVENT::LCCollection* getCollection( 
        EVENT::LCEvent* event, const std::string & name );

    /**
     * @brief Benchmarks collections of type EVENT::Track
     *
     * This method gets called by the processEvent() method for every 
     * collection of tracks to be benchmarked.
     *
     * @param inputCollection The collection containing the tracks
     * @param hist The histogram to be filled with the results
     */
    void processTracks( 
        LCEvent* event, EVENT::LCCollection* inputCollection,
        boost::shared_ptr< AIDA::IHistogram1D > hist );

    /**
     * @brief Benchmarks collections of type EVENT::Vertex
     *
     * This method gets called by the processEvent() method for every 
     * collection of vertices to be benchmarked.
     *
     * @param inputCollection The collection containing the vertices
     * @param hist The histogram to be filled with the results
     */
    void processVertices( 
        LCEvent* event, EVENT::LCCollection* inputCollection,
        boost::shared_ptr< AIDA::IHistogram1D > hist );
};

} // namespace marlinrave

#endif


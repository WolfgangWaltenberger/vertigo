#ifdef MARLIN_USE_AIDA

#include "RaveBenchmark.h"

// Marlin
#include "marlin/AIDAProcessor.h"
#include "marlin/Global.h"

// LCIO
#include "EVENT/LCCollection.h"
#include "EVENT/LCRelation.h"
#include "EVENT/MCParticle.h"
#include "EVENT/Track.h"
#include "EVENT/Vertex.h"

// Aida
#include "AIDA/AIDA.h"
#include "AIDA/IHistogramFactory.h"

// C++
#include <iostream>
#include <cmath>

marlinrave::RaveBenchmark RaveBenchmarkInstance;

namespace marlinrave {

RaveBenchmark::RaveBenchmark(): 
    marlin::Processor("RaveBenchmark")
{
  _description = "Fill residuals of reconstructed parameters into an AIDA histgram";
  registerProcessorParameter( "CollectionNames", 
                              "Space seperated list of collections to benchmark", 
                              _collectionNames, 
                              _collectionNames );
  registerProcessorParameter( "HistBorderLow", "Lower border of histograms", _histBorderLow, float(-1.0) );
  registerProcessorParameter( "HistBorderHigh", "Upper border of histograms", _histBorderHigh, float(1.0) );
}

RaveBenchmark::~RaveBenchmark()
{
  end();
}

RaveBenchmark* RaveBenchmark::newProcessor()
{
  return new RaveBenchmark;
}

void RaveBenchmark::init()
{
  streamlog_out( DEBUG ) << "RaveBenchmark::init ..." << std::endl;
  printParameters();

  for ( StringVec::iterator name = _collectionNames.begin();
        name != _collectionNames.end(); name++ )
  {
    boost::shared_ptr< AIDA::IHistogram1D > hist = 
        boost::shared_ptr< AIDA::IHistogram1D >(
            marlin::AIDAProcessor::histogramFactory(this)->
                createHistogram1D( *name, *name, 100, _histBorderLow, _histBorderHigh ) );
    _histograms.push_back( std::make_pair( *name, hist ) );
  }
  streamlog_out( MESSAGE ) << "Registered " << _histograms.size() << " histograms for benchmarking." << std::endl;
}

void RaveBenchmark::processRunHeader( EVENT::LCRunHeader* run )
{
}

void RaveBenchmark::processEvent( LCEvent * event ) 
{
  for ( std::vector< CollHist >::iterator chist = _histograms.begin();
        chist != _histograms.end(); chist++ )
  {
    EVENT::LCCollection * inputCollection = getCollection( event, chist->first );
    if ( !inputCollection )
    {
      streamlog_out( WARNING ) << "Collection """ << chist->first << """ could not be retrieved." << std::endl;
      continue;
    }
    if ( inputCollection->getTypeName() == EVENT::LCIO::LCRELATION )
    {
      processTracks( event, inputCollection, chist->second );
    }
    else 
    if ( inputCollection->getTypeName() == EVENT::LCIO::VERTEX ) 
    {
      processVertices( event, inputCollection, chist->second );
    }
    else
    {
      streamlog_out( WARNING ) << "Collection """ << chist->first << """ of type """ 
          << inputCollection->getTypeName() << """ can not be used for benchmarking." << std::endl;
    }
  }
}

void RaveBenchmark::processTracks( 
    LCEvent * event, EVENT::LCCollection * inputCollection, 
    boost::shared_ptr< AIDA::IHistogram1D > hist  )
{
  for (int i = 0; i != inputCollection->getNumberOfElements(); i++)
  {
    EVENT::LCRelation * rel = 
        dynamic_cast< EVENT::LCRelation * >( inputCollection->getElementAt( i ) );
    if (!rel)
    {
      streamlog_out(WARNING) << "Relation could not be retrieved." << std::endl;
      continue;
    }
    EVENT::Track * track = dynamic_cast<EVENT::Track *>(rel->getFrom());
    if (!track)
    {
      streamlog_out(WARNING) << "Related Track could not be retrieved." << std::endl;
      continue;
    }
    EVENT::MCParticle * mcp = dynamic_cast<EVENT::MCParticle *>(rel->getTo());
    if (!mcp)
    {
      streamlog_out(WARNING) << "Related MCParticle could not be retrieved." << std::endl;
      continue;
    }

    double resD0 = track->getD0();
    // double resZ0 = track->getZ0();
    hist->fill(resD0);
  }
}

void RaveBenchmark::processVertices( 
    LCEvent * event, EVENT::LCCollection * inputCollection, 
    boost::shared_ptr< AIDA::IHistogram1D > hist  )
{
  for (int i = 0; i != inputCollection->getNumberOfElements(); i++)
  {
    EVENT::Vertex * vtx = dynamic_cast<EVENT::Vertex *>(inputCollection->getElementAt(i));
    if (!vtx)
    {
      streamlog_out(WARNING) << "Vertex could not be retrieved." << std::endl;
      continue;
    }

    double resX = vtx->getPosition()[0];
    double resY = vtx->getPosition()[1];
    double resZ = vtx->getPosition()[2];
    hist->fill( sqrt( resX*resX + resY*resY + resZ*resZ ) );
  }
}

EVENT::LCCollection * RaveBenchmark::getCollection( EVENT::LCEvent * event, const std::string & name )
{
  EVENT::LCCollection * collection = 0;
  try {
    collection = dynamic_cast<LCCollection *>( event->getCollection(name) );
    streamlog_out( MESSAGE ) << "Found " << collection->getNumberOfElements() << " entries in collection " << name << std::endl;
  }
  catch ( DataNotAvailableException &e ) {
    streamlog_out( WARNING ) << "Input collection """ << name << """ not found." << std::endl;
    return 0;
  }
  return collection;
}

} // namespace marlinrave

#endif


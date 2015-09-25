/*
 * RaveVertexing.cc
 */

// Plugin includes
#include "RaveVertexing.h"
#include "L3LcioToRaveObjects.h"
#include "RaveToL3LcioObjects.h"
#include "LDCMagneticField.h"
#include "LDCPropagator.h"

// LCIO includes
#include <EVENT/LCIO.h>
#include <IMPL/LCCollectionVec.h>
#include <IMPL/LCRelationImpl.h>

// CLHEP includes
#include "CLHEP/Matrix/Matrix.h"
#include "CLHEP/Matrix/Vector.h"
#include "CLHEP/Matrix/SymMatrix.h"

// Rave includes
#include <rave/Track.h>
#include <rave/Vertex.h>

// HistFile io
#include <fstream>

marlinrave::RaveVertexing RaveVertexingInstance;

namespace marlinrave {

RaveVertexing::RaveVertexing() :
    Processor ( "RaveVertexing" ), 
    _tracksCollectionName ( "Tracks" ),
    _refittedTracksCollectionName ( "" ),
    _verticesCollectionName ( "RaveVertices" ), 
    _relationsCollectionName ( "RaveVertexRelations" ), 
    _method ( "default" ), _verbose ( 0 )
{
  _description = "RaveVertexing does vertex finding and fitting using a wide variety of algorithms";

  registerProcessorParameter( "Tracks",
                              "Name of the input track collection",
                              _tracksCollectionName,
                              _tracksCollectionName ) ;
  registerOutputCollection( EVENT::LCIO::VERTEX,
                            "Vertices",
                            "Name of the reconstructed vertex collection",
                            _verticesCollectionName,
                            _verticesCollectionName );
  registerOutputCollection( EVENT::LCIO::TRACK,
                            "RefittedTracks",
                            "Name of output the collection of refitted tracks",
                            _refittedTracksCollectionName,
                            _refittedTracksCollectionName ) ;
  registerOutputCollection( EVENT::LCIO::LCRELATION,
                            "Relations",
                            "Name of the collection to hold the weighted relations between the vertices and the input tracks",
                            _relationsCollectionName,
                            _relationsCollectionName );
  registerProcessorParameter( "Method",
                              "Method used for vertex finding and fitting",
                              _method,
                              _method );
  registerOptionalParameter( "Verbose",
                             "Print results to stdout",
                             _verbose, _verbose );
}

RaveVertexing::~RaveVertexing()
{
  end();
}

RaveVertexing* RaveVertexing::newProcessor()
{
  return new RaveVertexing;
}

void RaveVertexing::init()
{
  streamlog_out(DEBUG) << "RaveVertexing::init ..." << std::endl;
  printParameters() ;

  int raveVerbosity = 1;
  if (_verbose) raveVerbosity = _verbose;

  _factory = boost::shared_ptr< rave::VertexFactory >(
      new rave::VertexFactory ( LDCMagneticField(), LDCPropagator(), _method, 
                                raveVerbosity ) );
}

void RaveVertexing::processEvent ( EVENT::LCEvent * event )
{
  streamlog_out( DEBUG ) << "processEvent ..." << std::endl;

  // Fetch the specified collection (parameter Tracks)
  EVENT::LCCollection* inputTracks = 
      event->getCollection ( _tracksCollectionName ) ;

  // .. and check its type
  if ( ( inputTracks->getTypeName() != EVENT::LCIO::TRACK ) &&
       ( inputTracks->getTypeName() != EVENT::LCIO::RECONSTRUCTEDPARTICLE ) )
  {
    streamlog_out( ERROR ) << "Given LCIO collection has wrong type" 
                           << std::endl;
    exit( 1 );
  }

  streamlog_out( MESSAGE ) 
      << "Found " << inputTracks->getNumberOfElements() 
      << " Tracks, now searching vertices ... " << std::endl;

  // Convert the found tracks to Rave objects
  L3LcioToRaveObjects inputConverter;
  std::vector< rave::Track > raveTracks;
  if ( inputTracks->getTypeName() == EVENT::LCIO::TRACK )
  {
    for ( int i = 0; i != inputTracks->getNumberOfElements(); i++ )
    {
      EVENT::Track * lcioTrack = 
          dynamic_cast< EVENT::Track * >( inputTracks->getElementAt( i ) ) ;
      try {
        raveTracks.push_back( inputConverter.convert( lcioTrack ) );
      } 
      catch(std::string msg) {
        streamlog_out( ERROR ) << "Track could not be converted: " << msg 
                               << std::endl;
      }
      catch( ... ) {
        streamlog_out( ERROR ) << "Track could not be converted!" 
                               << std::endl;
      }
    }
  }
  else 
  if ( inputTracks->getTypeName() == EVENT::LCIO::RECONSTRUCTEDPARTICLE )
  {
    streamlog_out( DEBUG ) 
        << "Using tracks assigned to EVENT::ReconstructedParticle ..." 
        << std::endl;
    for ( int i = 0; i != inputTracks->getNumberOfElements(); i++ )
    {
      EVENT::ReconstructedParticle * lcioParticle = 
          dynamic_cast<EVENT::ReconstructedParticle*> ( inputTracks->getElementAt ( i ) ) ;
      if (!lcioParticle) 
      {
        streamlog_out( ERROR ) << "Particle could not be read!" << std::endl;
        continue;
      }
      EVENT::TrackVec lcioTracks = lcioParticle->getTracks();
      if (!lcioTracks.size()) 
      {
        // Particle contains no tracks
        continue;
      }
      try {
        raveTracks.push_back( 
            inputConverter.convert( lcioTracks[0], lcioParticle ) );
      } 
      catch(std::string msg) {
        streamlog_out( ERROR ) << "Track could not be converted: " << msg 
                               << std::endl;
      }
      catch( ... ) {
        streamlog_out( ERROR ) << "Track could not be converted!" << std::endl;
      }
    }
  }

  streamlog_out( DEBUG ) << "Calling the VertexFactory ..." << std::endl;
  // Do the reconstruction using the method given (parameter Method)
  std::vector < rave::Vertex > raveVertices = _factory->create ( raveTracks );

  streamlog_out( DEBUG ) << "Found " << raveVertices.size() << " vertices" 
                         << std::endl;

  // Add the aquired information to the event
  RaveToL3LcioObjects outputConverter;
  bool storeRefittedTracks = ( _refittedTracksCollectionName != "" );
  IMPL::LCCollectionVec* outputVertices = 
      new IMPL::LCCollectionVec( EVENT::LCIO::VERTEX );
  IMPL::LCCollectionVec* outputRelations = 
      new IMPL::LCCollectionVec( EVENT::LCIO::LCRELATION );
  IMPL::LCCollectionVec* outputTracks = 0;
  if (storeRefittedTracks)
    outputTracks = new IMPL::LCCollectionVec ( EVENT::LCIO::TRACK );

  for ( std::vector< rave::Vertex >::iterator raveVertex = raveVertices.begin();
        raveVertex != raveVertices.end(); raveVertex++ )
  {
    IMPL::VertexImpl* outputVertex = outputConverter.convert ( *raveVertex );
    outputVertex->setAlgorithmType( _method );

    if ( storeRefittedTracks && raveVertex->hasRefittedTracks() )
    {
      // Add the assigned refitted tracks
      std::vector< rave::Track > raveTracks = raveVertex->refittedTracks();

      for ( std::vector< rave::Track >::iterator raveTrack = 
                raveTracks.begin();
            raveTrack != raveTracks.end(); raveTrack++ )
      {
        IMPL::TrackImpl* outputTrack = 
            outputConverter.convert ( *raveTrack, raveVertex->position() );
        streamlog_out( DEBUG ) 
            << "Adding refitted track to collection " 
            << _refittedTracksCollectionName << std::endl;
        outputTracks->addElement( outputTrack );
      }
    }

    streamlog_out( DEBUG ) 
        << "Adding vertex to collection " << _verticesCollectionName 
        << std::endl;
    outputVertices->addElement( outputVertex );

    streamlog_out( DEBUG ) 
        << "Adding track to vertex relations to collection " 
        << _relationsCollectionName << std::endl;
    typedef std::vector< std::pair< float, rave::Track > > WTrkVec;
    WTrkVec wTrks = raveVertex->weightedTracks();
    for ( WTrkVec::iterator wTrk = wTrks.begin(); wTrk != wTrks.end(); wTrk++ )
    {
      // TODO reinterpret_cast is dangerous !!!! maybe switch to boost::any ??
      EVENT::LCObject * vtxTrk = 
          reinterpret_cast< EVENT::LCObject * >( wTrk->second.originalObject() );
      if ( !vtxTrk )
      {
        streamlog_out( ERROR ) 
            << "Track to vertex relation could not be added." << std::endl;
        continue;
      }
      IMPL::LCRelationImpl * vtxRel = 
          new IMPL::LCRelationImpl( vtxTrk, outputVertex, wTrk->first );
      outputRelations->addElement( vtxRel );
    }
  }

  event->addCollection( outputVertices , _verticesCollectionName );
  event->addCollection( outputRelations , _relationsCollectionName );
  if ( storeRefittedTracks )
    event->addCollection( outputTracks , _refittedTracksCollectionName );
}

void RaveVertexing::check ( EVENT::LCEvent * event )
{
  // nothing to check here - could be used to fill checkplots
  // in reconstruction processor
}

} // namespace marlinrave


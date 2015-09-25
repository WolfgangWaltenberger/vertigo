/*
 * RaveVertexing.cc
 */

// Plugin includes
#include "RaveKinematics.h"
#include "L3LcioToRaveObjects.h"
#include "RaveToL3LcioObjects.h"
#include "LDCMagneticField.h"
#include "LDCPropagator.h"
#include "TopologyManager.h"

// LCIO includes
#include "EVENT/LCIO.h"
#include "IMPL/ReconstructedParticleImpl.h"
#include "IMPL/LCCollectionVec.h"

// CLHEP includes
#include "CLHEP/Matrix/Matrix.h"
#include "CLHEP/Matrix/Vector.h"
#include "CLHEP/Matrix/SymMatrix.h"

// Rave includes
#include "rave/KinematicConstraintBuilder.h"
#include "rave/KinematicConstraint.h"

// Boost includes
#include "boost/spirit/core.hpp"
#include "boost/spirit/actor/assign_actor.hpp"

#include <iostream>

marlinrave::RaveKinematics RaveKinematicsInstance;

namespace marlinrave {

RaveKinematics::RaveKinematics() :
    Processor ( "RaveKinematics" ), 
    _inputParticlesCollectionName ( "Particles" ),
    _inputBeamspotCollectionName( "" ),
    _outputParticlesCollectionName ( "KinematicParticles" ),
    _verticesCollectionName ( "KinematicVertices" ),
    _verbose ( 0 ), _topologyName ( "SingleVertex" ),
    _topologyParameters ( "" )
{
  _description = "RaveKinematics uses a set of given particles to reconstruct the decay chain using a specified topology";

  std::map< std::string, boost::shared_ptr< KinematicTopology > > ts = TopologyManager::Instance().get();
  std::string allts = "";
  for ( std::map< std::string, boost::shared_ptr< KinematicTopology > >::iterator i = ts.begin();
       i != ts.end(); i++) {
    allts.append(i->first + ", ");
  }

  registerInputCollection( EVENT::LCIO::RECONSTRUCTEDPARTICLE ,
                           "Particles" ,
                           "Name of the input particle collection" ,
                           _inputParticlesCollectionName ,
                           _inputParticlesCollectionName ) ;
  registerInputCollection( EVENT::LCIO::VERTEX ,
                           "Beamspot" ,
                           "Name of a vertex collection containing beamspot information (optional)" ,
                           _inputBeamspotCollectionName ,
                           _inputBeamspotCollectionName );
  registerOutputCollection( EVENT::LCIO::RECONSTRUCTEDPARTICLE ,
                            "KinematicParticles" ,
                            "Name of the output collection of particles linked to a decay chain" ,
                            _outputParticlesCollectionName ,
                            _outputParticlesCollectionName );
  registerOutputCollection( EVENT::LCIO::VERTEX ,
                            "KinematicVertices" ,
                            "Name of the reconstructed vertex collection linked to a decay chain" ,
                            _verticesCollectionName ,
                            _verticesCollectionName ); 
  registerProcessorParameter( "Topology" ,
                              "Decay topology to be used for the fitting; registered: " + allts ,
                              _topologyName,
                              _topologyName );
  registerProcessorParameter( "Parameters" ,
                              "Topology parameters forwarded to the selected topology" ,
                              _topologyParameters ,
                              _topologyParameters );
  registerProcessorParameter( "Verbose" ,
                              "Print results to stdout" ,
                              _verbose, _verbose ) ;
  registerProcessorParameter( "ParticleCovariance" ,
                              "Overrides the covariance information suppied by the input particle collection (optional)" ,
                              _inputParticleErrors, _inputParticleErrors ) ;
}

RaveKinematics::~RaveKinematics()
{}

RaveKinematics* RaveKinematics::newProcessor()
{
  return new RaveKinematics;
}

void RaveKinematics::init()
{
  streamlog_out( DEBUG ) << "--- init ---------" << std::endl;
  printParameters() ;

  int raveVerbosity = 1;
  if ( _verbose ) raveVerbosity = _verbose;

  _factory = rave::KinematicTreeFactory( LDCMagneticField(), LDCPropagator(),
		                         raveVerbosity );

  // Get the selected topology from the Manager
  _topology = TopologyManager::Instance().get( _topologyName, _verbose );

  if (_topology && _topology->valid()) {
    // Forward the parameters to the selected topology
    _topology->setup( _topologyParameters );
    if (_verbose)
      streamlog_out( MESSAGE ) << "Topology " << _topologyName << " param: " 
                               << _topologyParameters 
                               << " set up successfully." << std::endl;
  } else {
    streamlog_out( ERROR ) << "Topology " << _topologyName << " not known!" 
                           << std::endl;
  }

}

void RaveKinematics::processEvent ( EVENT::LCEvent * event )
try {
  streamlog_out( DEBUG ) << "--- processEvent ---------" << std::endl;

  // Fetch the specified collection (parameter Tracks)
  EVENT::LCCollection* inputParticles = 
      event->getCollection( _inputParticlesCollectionName );
  
  streamlog_out( DEBUG ) 
      << "Found " << inputParticles->getNumberOfElements() << " particles" 
      << std::endl;

  L3LcioToRaveObjects inputConverter;

  if ( _inputBeamspotCollectionName != "" )
  {
    EVENT::LCCollection * inputBeamspot = 
        event->getCollection( _inputBeamspotCollectionName );
    if ( inputBeamspot->getNumberOfElements() > 0 )
    {
      EVENT::Vertex * lcioVertex = 
          dynamic_cast< EVENT::Vertex * >( inputBeamspot->getElementAt ( 0 ) );
      rave::Vertex vtx = inputConverter.convert( lcioVertex );
      rave::Ellipsoid3D bs( vtx.position(), vtx.error() );
      inputConverter.defineBeamspot( bs );
      streamlog_out( DEBUG ) << "Found beamspot information" << std::endl;
    }
  }

  // Convert the found tracks to Rave objects
  std::vector< rave::KinematicParticle > raveParticles;
  for ( int i = 0; i != inputParticles->getNumberOfElements(); i++ )
  {
    IMPL::ReconstructedParticleImpl * lcioParticle = 
        dynamic_cast< IMPL::ReconstructedParticleImpl* >( 
            inputParticles->getElementAt( i ) );
    try {
      raveParticles.push_back( 
          inputConverter.convert( lcioParticle, _inputParticleErrors ) );
    } catch ( std::string msg ) {
      streamlog_out( ERROR ) << msg << std::endl;
    }
  }
  
  streamlog_out( DEBUG ) << "... now fitting tree ... " << std::endl;

  rave::KinematicTree raveTree;
  if ( _topology && _topology->valid() ) {
    if ( isFirstEvent() ) _topology->prepare( this ); 
    raveTree = _topology->build( _factory, raveParticles, _verbose );
  } else {
    streamlog_out( WARNING ) << "  No decay topology registered." << std::endl;
  }

  if ( raveTree.isValid() ) {
    RaveToL3LcioObjects outputConverter;
    IMPL::LCCollectionVec *outputParticles, *outputVertices;
    boost::tie( outputParticles, outputVertices ) = 
        outputConverter.convert( raveTree );
    event->addCollection( outputParticles , _outputParticlesCollectionName );
    event->addCollection( outputVertices , _verticesCollectionName );
  } else {
    streamlog_out( WARNING ) << "  The KinematicTree is invalid." << std::endl;
  }

  streamlog_out( DEBUG ) << "--------------------------------" << std::endl;
} catch ( rave::CheckedFloatException & e ) {
  streamlog_out( ERROR ) << e.what() << std::endl;
}

void RaveKinematics::check ( EVENT::LCEvent * event )
{
  // nothing to check here - could be used to fill checkplots
  // in reconstruction processor
}

} // namespace marlinrave


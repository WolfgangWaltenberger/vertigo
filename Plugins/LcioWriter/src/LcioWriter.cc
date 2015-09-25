#include "LcioWriter.h"
#include "RaveToL3LcioObjects.h"
#include "VertigoToL3LcioObjects.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "Vertigo/Event/interface/Event.h"

#include <EVENT/LCIO.h>
#include <IMPL/LCCollectionVec.h>
#include <IMPL/LCEventImpl.h>
#include <IMPL/ReconstructedParticleImpl.h>

#include <boost/scoped_ptr.hpp>

using namespace std;

LcioWriter::LcioWriter ( const string & filename ) :
    Observer()
{
  configure ( filename );
}

LcioWriter::~LcioWriter()
{}

void LcioWriter::process ( const vertigo::Event & vertigoevent )
{
  marlinrave::RaveToL3LcioObjects raveConverter;
  VertigoToL3LcioObjects vertigoConverter;

  boost::scoped_ptr< IMPL::LCEventImpl > lcioevent( new IMPL::LCEventImpl() );

  lcioevent->setRunNumber( vertigoevent.run() );
  lcioevent->setEventNumber( vertigoevent.id() );

  vector< vertigo::SimParticle > vertigoSimParticles = vertigoevent.simParticles();
  edm::LogInfo ( "LcioWriter" ) << "Writing " << vertigoSimParticles.size() << " simulated particles";

  IMPL::LCCollectionVec* lcioSimParticles = new IMPL::LCCollectionVec( EVENT::LCIO::MCPARTICLE ); // LCIO takes care of deleting this after attaching it to the event

  for ( vector< vertigo::SimParticle >::iterator vertigoSimParticle = vertigoSimParticles.begin();
        vertigoSimParticle != vertigoSimParticles.end(); ++vertigoSimParticle )
  {
    IMPL::MCParticleImpl* lcioSimParticle = vertigoConverter.convert( *vertigoSimParticle );
    lcioSimParticles->addElement( lcioSimParticle );
  }

  lcioevent->addCollection( lcioSimParticles , "MCParticle") ;

  vector< rave::Vertex > ravevertices = vertigoevent.vertices();
  edm::LogInfo ( "LcioWriter" ) << "Writing " << ravevertices.size() << " vertices";

  IMPL::LCCollectionVec* lciovertices = new IMPL::LCCollectionVec( EVENT::LCIO::VERTEX ); // LCIO takes care of deleting this after attaching it to the event
  IMPL::LCCollectionVec* lciotracks = new IMPL::LCCollectionVec( EVENT::LCIO::TRACK ); // LCIO takes care of deleting this after attaching it to the event

  for ( vector< rave::Vertex >::iterator ravevertex = ravevertices.begin();
        ravevertex != ravevertices.end(); ravevertex++ )
  {
    IMPL::VertexImpl* lciovertex = raveConverter.convert( *ravevertex );

    if ( ravevertex->hasRefittedTracks() )
    {
      // Add the assigned refitted tracks
      vector< rave::Track > ravetracks = ravevertex->refittedTracks();
      edm::LogInfo ( "LcioWriter" ) << "  Writing " << ravetracks.size() << " refitted tracks.";

      for ( vector< rave::Track >::iterator ravetrack = ravetracks.begin();
            ravetrack != ravetracks.end(); ravetrack++ )
      {
        IMPL::TrackImpl* lciotrack = raveConverter.convert( *ravetrack, ravevertex->position() );
        lciotracks->addElement( lciotrack );
      }
    }

    lciovertices->addElement( lciovertex );
  }

  lcioevent->addCollection( lciovertices , "Vertices") ;
  lcioevent->addCollection( lciotracks , "RefittedTracks") ;

  vector< rave::KinematicParticle > raveParticles = vertigoevent.particles();
  edm::LogInfo ( "LcioWriter" ) << "Writing " << raveParticles.size() << " particles";

  IMPL::LCCollectionVec* lcioParticles        = new IMPL::LCCollectionVec( EVENT::LCIO::RECONSTRUCTEDPARTICLE );
  IMPL::LCCollectionVec* lcioParticleVertices = new IMPL::LCCollectionVec( EVENT::LCIO::VERTEX );

  for ( vector< rave::KinematicParticle >::iterator raveParticle = raveParticles.begin();
        raveParticle != raveParticles.end(); ++raveParticle )
  {
    IMPL::ReconstructedParticleImpl* lcioParticle;
    IMPL::VertexImpl* lcioParticleVertex;
    boost::tie(lcioParticle, lcioParticleVertex) = raveConverter.convert( *raveParticle, 0 );
    lcioParticles->addElement( lcioParticle );
    lcioParticleVertices->addElement( lcioParticleVertex );
  }

  lcioevent->addCollection( lcioParticles , "Particles") ;
  lcioevent->addCollection( lcioParticleVertices , "ParticleVertices") ;

  theLCWriter->writeEvent( lcioevent.get() );
}

void LcioWriter::process ( const exception & e, const string & type )
{}

namespace
{
void LCWriterClose ( lcio::LCWriter* p )
{
  if ( p != 0 )
    p->close();
}
}

void LcioWriter::configure ( const string & filename )
{
  if ( filename !="" )
  {
    theLCWriter = boost::shared_ptr<lcio::LCWriter> ( lcio::LCFactory::getInstance()->createLCWriter(), &LCWriterClose );
    theLCWriter->open ( filename, EVENT::LCIO::WRITE_NEW );
  }
}

LcioWriter * LcioWriter::clone() const
{
  return new LcioWriter ( *this );
}

#include "Analysis/Observer/interface/ObserverBuilder.h"
namespace
{
ObserverBuilder<LcioWriter> LcioWriterBuilder (
  "LcioWriter",
  "Observer that writes events into a LCIO file",
  false );
}



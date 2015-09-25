/** @file
 *
 * This implements the LCIO event generator used for reading
 * track data from LCIO (*.slcio) files. This will only be
 * compiled, if the LCIO header files are present during
 * building.
 */

#include "LcioEventGenerator.h"

// #include <rave/ConstantMagneticField.h>

#include <rave/impl/TrackingTools/TrajectoryParametrization/interface/GlobalTrajectoryParameters.h>
#include <rave/impl/RaveTools/Converters/interface/MagneticFieldSingleton.h>
#include <rave/Units.h>

// #include "BrahmsLcioToRaveObjects.h"
#include "L3LcioToRaveObjects.h"
#include "L3LcioToVertigoObjects.h"
#include "RaveBase/Converters/interface/RaveStreamers.h"
#include "Environment/MCObjects/interface/PDGCodes.h"
#include "Vertigo/EventLoop/interface/LoopSettings.h"
#include "Environment/MCObjects/interface/SimVertex.h"
#include "Environment/VertigoObjects/interface/VertigoJet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

// LCIO stuff
#include "EVENT/LCCollection.h"
#include "EVENT/LCIO.h"
#include "EVENT/LCIntVec.h"
#include "IMPL/TrackImpl.h"
#include "IMPL/ReconstructedParticleImpl.h"
#include "IMPL/MCParticleImpl.h"
#include "IMPL/LCRelationImpl.h"

#include <boost/spirit/include/classic_core.hpp>
#include <boost/spirit/include/classic_lists.hpp>
#include <boost/spirit/include/classic_rule.hpp>
// #include <boost/spirit/core.hpp>
// #include <boost/spirit/utility/lists.hpp>

#ifdef WITH_THREADS
#include <boost/thread/mutex.hpp> 
#endif

#include <string>
#include <iostream>
#include <sstream>

using namespace std;
using namespace lcio;
using namespace boost::spirit::classic;
// using namespace boost::spirit;

namespace {
  #ifdef WITH_THREADS
  boost::mutex mutex;
  #endif

  std::string lcio_version()
  {
    ostringstream ostr;
    ostr << LCIO_MAJOR_VERSION << "." << LCIO_MINOR_VERSION << "." << LCIO_PATCH_LEVEL;
    return ostr.str();
  }

  // provide conversion routines
  // BrahmsLcioToRaveObjects converter;
  // FIXME this one is with hard coded beamspot constraint!
  marlinrave::L3LcioToRaveObjects raveConverter;
  L3LcioToVertigoObjects vertigoConverter;

  marlinrave::L3LcioToRaveObjects buildRaveConverter()
  {
    if ( rave::BeamSpotSingleton::hasBeamSpot() )
    {
      return marlinrave::L3LcioToRaveObjects( rave::BeamSpotSingleton::get() );
    }
    else
    {
      return marlinrave::L3LcioToRaveObjects();
    }
  }

  // structure holding some LCIO format definitions
  LCIO lcConsts;

  // Minimal Vertex distance for detecting SimVertices from MCParticle data
  const double minVertexDistance = 0.0001;

  /**
   *  set the jet's MC Tag; using the event's association information.
   */
  void setJetTag ( const vertigo::Event & event, vertigo::Jet & jet )
  {
    vector < rave::Track > trks = jet.tracks();
    char tag='?';
    for ( vector< rave::Track >::const_iterator i=trks.begin(); 
          i!=trks.end() ; ++i )
    {
      vector < vertigo::SimTrack > strks = event.associatedTracks ( *i );
      // cout << " strks size=" << strks.size() << endl;
      int pid=-1;
      if ( strks.size() ) pid=strks[0].pid();
      if ( int(fmod(float(pid),float(1000.))/100)==5 ) tag='b';
      int f=int(fmod(float(pid),float(1000))/100);
      switch (f)
      {
        case 5:
          { tag='b'; break; }
        case 4:
          { if ( tag!='b' ) tag='c'; break; }
        case 3:
          { if ( (tag!='b') && (tag!='c')  ) tag='s'; break; }
        case 2:
        case 1:
          { if ( (tag=='?') ) tag='q'; break; }
      }
    }
    // cout << "[LcioEventGenerator] setting jet tag to " << tag << endl;
    jet.mcTag ( tag );
  }

  void addTracks ( vertigo::Event & event, LCCollection * col )
  {
    // retrieve the number of tracks available
    int nTracks = col->getNumberOfElements();

    edm::LogInfo("LcioEventGenerator") << "Found " << nTracks << " LCIO::Tracks in given collection.";

    // convert the collection track by track and fill the vertigo::Event with track data
    for ( int i = 0 ; i != nTracks ; i++ )
    {
      TrackImpl* lcTrack = dynamic_cast<TrackImpl*> ( col->getElementAt(i)  ) ;
      try {
        event.add ( raveConverter.convert( lcTrack ) );
      } catch ( std::string & s ) {
        edm::LogError("LcioEventGenerator") << "Could not convert track: " << s;
      } catch ( ... ) {
        edm::LogError("LcioEventGenerator") << "Could not convert track.";
      }
    }
  }

  void noRelation ( const string & s )
  {
    if ( LoopSettings::Instance().verbosity() > 0 )
    {
      cout << "[LcioEventGenerator] Could not read relations or no relations found, trying tracks now."
           << endl;
      if ( s.size() )
      {
        cout << "[LcioEventGenerator] reason: " << s << endl;
      }
    }
  }

  void addParticles ( vertigo::Event & event, LCCollection * col )
  {
    LogDebug ( "LcioEventGenerator" ) << "Notice: " << col->getTypeName() << " is the type here!";

    // retrieve the number of tracks available
    int nParticles = col->getNumberOfElements();

    LogDebug ( "LcioEventGenerator" ) << "Found " << nParticles << " particles in given collection";

    int t = 0;
    // convert the collection track by track and fill the vertigo::Event with track data
    for ( int i = 0 ; i != nParticles ; i++ )
    {
      ReconstructedParticleImpl * lcioParticle =
          dynamic_cast<ReconstructedParticleImpl *> ( col->getElementAt(i)  ) ;

      try {
        event.add ( raveConverter.convert( lcioParticle ) );
      } catch ( std::string & s ) {
        edm::LogError ( "LcioEventGenerator" ) << "Could not convert particle: " << s;
      } catch ( ... ) {
        edm::LogError ( "LcioEventGenerator" ) << "Could not convert particle";
      }

      vector < Track* > lcioTracks = lcioParticle->getTracks();

      if ( lcioTracks.size() )
      {
        try {
          event.add ( raveConverter.convert( lcioTracks[0] ) );
        } catch ( std::string & s ) {
          edm::LogError ( "LcioEventGenerator" ) << "Could not convert track: " << s;
        } catch ( ... ) {
          edm::LogError ( "LcioEventGenerator" ) << "Could not convert track";
        }
        t++;
      }

      // RECONSTRUCTED vertices in lcio file!
      // Vertex * simv = lcTrack->getStartVertex();
    }

    LogDebug ( "LcioEventGenerator" ) << "Found " << t << " tracks in attached to particles";
  }

  void addVertex ( vector< vertigo::SimVertex > & foundVertices,
                   const MCParticleImpl * lcMCParticle,
                   vertigo::Event & event )
  {
    // add foundVertices to event.
    bool found = 0;
    rave::Point3D currPoint = raveConverter.convert( lcMCParticle->getVertex() );
    for ( vector< vertigo::SimVertex >::iterator foundThis = foundVertices.begin() ;
          foundThis != foundVertices.end() ; foundThis++ )
    {
      rave::Point3D diff( foundThis->position().x() - currPoint.x(),
                          foundThis->position().y() - currPoint.y(),
                          foundThis->position().z() - currPoint.z() );
      if ( diff.mag() < minVertexDistance )
      {
        // found = (&(*foundThis));
        found=true;
        vertigo::SimTrack simTrk = vertigoConverter.associate( lcMCParticle, *foundThis );
        foundThis->add( simTrk );
        event.add( simTrk );
      }
    }
    if ( !found )
    {
      vector< vertigo::SimTrack > simTrks;
      foundVertices.push_back(
          vertigo::SimVertex( raveConverter.convert( lcMCParticle->getVertex() ), simTrks ) );
      vertigo::SimTrack simTrk = vertigoConverter.associate( lcMCParticle, foundVertices.back() );
      foundVertices.back().add( simTrk );
      event.add( simTrk );
    }
  }

  void addMCParticles ( vertigo::Event & event, LCCollection * col,
                        vector< vertigo::SimVertex > & foundVertices )
  {
    // adds a collection of MC Particles
    // retrieve the number of particle available
    int nMCParticles = col->getNumberOfElements();

    edm::LogInfo("LcioEventGenerator") << "Found " << nMCParticles
           << " LCIO::MCParticles in given collection.";

    // convert the collection track by track and fill the vertigo::Event with track data
    for ( int i = 0 ; i != nMCParticles ; i++ )
    {
      MCParticleImpl* lcMCParticle = dynamic_cast< MCParticleImpl* > ( col->getElementAt(i)  ) ;
      vertigo::SimParticle simParticle = vertigoConverter.convert( lcMCParticle );
      event.add( simParticle );
      if ( ( lcMCParticle->getGeneratorStatus() == 1 ) &&
          ( lcMCParticle->getCharge() != 0.0 ) )
      {
        addVertex ( foundVertices, lcMCParticle, event );
      }
    }
  }

  void addCollection(vertigo::Event & event, LCCollection* col, 
                     vector< vertigo::SimVertex > & foundVertices)
  {
    // check if the selected collection is really holding tracks
    if (col->getTypeName() == lcConsts.TRACK)
    {
      addTracks(event, col);
      return;
    }

    if (col->getTypeName() == lcConsts.RECONSTRUCTEDPARTICLE)
    {
      addParticles(event, col);
      return;
    }

    if (col->getTypeName() == lcConsts.MCPARTICLE)
    {
      addMCParticles(event, col, foundVertices);
      event.associateTracks();
      return;
    }

    edm::LogWarning("LcioEventGenerator") << "Given LCIO track collection has unknown type: "
         << col->getTypeName();
  }

  struct ltPts {
    bool operator() ( const rave::Point3D & r1, const rave::Point3D & r2 )
    {
      return ( fabs ( r1.x() - r2.x() ) > 1.e-66 );
    }
  };

  int checkFlavors ( LCCollection * col )
  {
    // check flavor content of collection
    int ret=0;
    int nMCParticles = col->getNumberOfElements();
    map < rave::Point3D, bool, ltPts > hasListed;
    for ( int i = 0 ; i != nMCParticles ; i++ )
    {
      MCParticleImpl* lcMCParticle = dynamic_cast< MCParticleImpl* > ( col->getElementAt(i)  ) ;
      int pid=abs(lcMCParticle->getPDG());
      if ( pid==411 || pid==421 || pid==413 || pid == 423 )
      {
        rave::Point3D currPoint = raveConverter.convert( lcMCParticle->getVertex() );
        rave::Point3D endPoint = raveConverter.convert( lcMCParticle->getEndpoint() );
        float diff = fabs ( currPoint.x() - endPoint.x() );
        if ( (diff > 0.) && (!(hasListed[endPoint])) )
        {
          hasListed[endPoint]=true;
          cout << " - lcio event: D-meson " << PDGCodes::name(pid) << " detected at " << currPoint
               << " - " << endPoint << endl;
          ret=1;
          break;
        };
      }
      if ( pid==511 || pid==521 || pid==513 || pid == 523 )
      {
        rave::Point3D currPoint = raveConverter.convert( lcMCParticle->getVertex() );
        rave::Point3D endPoint = raveConverter.convert( lcMCParticle->getEndpoint() );
        float diff = fabs ( currPoint.x() - endPoint.x() );
        if ( diff > 0. && (!hasListed[endPoint]) )
        {
          hasListed[endPoint]=true;
          ret=2;
          cout << " - lcio event: B-meson " << PDGCodes::name(pid) << " detected at " << currPoint
               << " - " << endPoint << endl;
        };
      }
    }
    return ret;
  }


  void addRelation ( LCRelationImpl * lcRel, vertigo::Event & event,
      vector< vertigo::SimVertex > & foundVertices )
  {
    // expect "From" to be the reconstructed track and "To" to be the MC track
    TrackImpl* lcTrack = dynamic_cast<TrackImpl*> ( lcRel->getFrom() ) ;
    MCParticleImpl* lcMCParticle = dynamic_cast< MCParticleImpl* > ( lcRel->getTo() ) ;
    // trkchecker.check ( lcTrack, lcMCParticle );

    int pid=lcMCParticle->getPDG();
    // cout << "[LcioEventGenerator] " << PDGCodes::name(pid) << " (" << pid << ")" << endl;

    if ( ( lcMCParticle->getGeneratorStatus() == 1 ) &&
         ( lcMCParticle->getCharge() != 0.0 ) )
    {
      // addVertex ( foundVertices, lcMCParticle, event );
      vertigo::SimVertex* found = 0;
      rave::Point3D currPoint = raveConverter.convert( lcMCParticle->getVertex() );
      for ( vector< vertigo::SimVertex >::iterator foundThis = foundVertices.begin() ;
            foundThis != foundVertices.end() ; foundThis++ )
      {
        rave::Point3D diff( foundThis->position().x() - currPoint.x(),
                            foundThis->position().y() - currPoint.y(),
                            foundThis->position().z() - currPoint.z() );
        if ( diff.mag() < minVertexDistance )
        {
          found = &(*foundThis);
        }
      }
      if ( !found )
      {
        vector< vertigo::SimTrack > simTrks;
        string tag="?";
        int pid=0;
        if ( lcMCParticle->getParents().size() )
        {
          pid=lcMCParticle->getParents()[0]->getPDG();
          tag = PDGCodes::name ( lcMCParticle->getParents()[0]->getPDG() );
        }
        rave::Point3D pt = raveConverter.convert( lcMCParticle->getVertex() );
        foundVertices.push_back( vertigo::SimVertex( pt, simTrks, pid, tag ) );
        found = &(foundVertices.back());
      }
      if ( found )
      {
        vertigo::SimTrack simTrk = vertigoConverter.associate(
            lcMCParticle, foundVertices.back(), PDGCodes::name(pid) );
        try {
          rave::Track trk = raveConverter.convert( lcTrack );
          found->add( simTrk );
          event.add( simTrk );
          event.add( trk );
          event.associate( simTrk, trk );
        } catch ( std::string & s ) {
          cout << "[LcioEventGenerator] could not convert track: " << s << endl;
        } catch ( ... ) {
          cout << "[LcioEventGenerator] could not convert track:" << endl;
        }
      }
    }
  }

  int getNrRelations ( LCCollection * relCol )
  {
    // determine number  of relations in a file
    int nRels = 0;

    try
    {
      // initiate a pointer to the selected relation collection
      /*
      cout << "[LcioEventGenerator] debug: collection holds: " << relCol->getTypeName()
           << endl;
           */

      // check if the selected collection is really holding tracks
      if (relCol->getTypeName() == lcConsts.LCRELATION)
      {
        // retrieve the number of relations available
        nRels = relCol->getNumberOfElements();

        if ( LoopSettings::Instance().verbosity() > 9 )
        {
          cout << "[LcioEventGenerator] Found " << nRels << " LCIO::LCRelations in given collection" << endl;
        }

        if ( nRels == 0 )
        {
          cout << "[LcioEventGenerator] warning: no relations found" << endl;
          throw string("no relations found");
        }
      }
    } catch ( const string & text ) { noRelation ( text ); }
    catch ( exception & e ) { noRelation ( e.what() ); }
    catch ( ... ) { noRelation(""); }
    return nRels;
  }

  vertigo::Jet getJet ( ReconstructedParticleImpl * jet,
                 vector < vertigo::SimVertex > & foundVertices )
  {
    // create a single Jet
    vertigo::Jet vjet;
    vector < ReconstructedParticle * > particles = jet->getParticles();
    for ( vector< ReconstructedParticle * >::const_iterator i=particles.begin(); 
          i!=particles.end() ; ++i )
    {
      vector < Track * > trks = (**i).getTracks();
      for ( vector< Track * >::const_iterator t=trks.begin(); t!=trks.end() ; ++t )
      {
        vjet.add ( raveConverter.convert ( *t ) );
        // event.add ( raveConverter.convert ( *t ) );
      }
    }

    /*
    cout << "[LcioEventGenerator] addJet: adding jet with " << vjet.getTracks().size()
         << " tracks: FIXME no known jet flavor!!" << endl;
         */
    return vjet;
  }

  void setJetTags ( vertigo::Event & e )
  {
    vector < vertigo::Jet > & jrfs=e.jetRefs();
    for ( vector< vertigo::Jet >::iterator i=jrfs.begin(); i!=jrfs.end() ; ++i )
    {
      setJetTag ( e, *i );
    }
  }

  void addJets ( LCCollection * jetCol, LCCollection * mcCol, vertigo::Event & event,
                 vector < vertigo::SimVertex > & foundVertices,
                 LCCollection * flavors )
  {
    addMCParticles ( event, mcCol, foundVertices );
    // cout << "[LcioEventGenerator] addJets" << endl;
    // add a jet collection to event.
    int nJets = jetCol->getNumberOfElements();

    if ( LoopSettings::Instance().verbosity() > 3 )
    {
      cout << "[LcioEventGenerator] Found " << nJets << " Jets in given collection: " << flush;
    }
    // cout << "[LcioEventGenerator] DEBUG: Adding " << nJets << " Jets!" << endl;

    // int t=0;
    // convert the collection track by track and fill the vertigo::Event with track data
    for ( int i = 0 ; i != nJets ; i++ )
    {
      ReconstructedParticleImpl * jet = dynamic_cast
        <ReconstructedParticleImpl *> ( jetCol->getElementAt(i)  ) ;
      vertigo::Jet vjet = getJet ( jet, foundVertices );
      LCIntVec * liv=dynamic_cast < LCIntVec *> (flavors->getElementAt(i) );
      if ( liv->size() == 1 )
      {
        int v0=(*liv)[0];
        char f='?';
        switch ( v0 )
        {
          case 1:
          case 2:
          case 3: { f='Q'; break; };
          case 4: { f='C'; break; };
          case 5: { f='B'; break; };
          default:
            f='?';
        }
        vjet.mcTag ( f );
        if ( LoopSettings::Instance().verbosity() > 3 ) cout << f << " ";
      }

      event.add ( vjet );
    }
    if ( LoopSettings::Instance().verbosity() > 3 ) cout << endl;
    event.associateTracks();
    // setJetTags ( event );
  }


  void addFoundVertices ( vertigo::Event & event,
                 vector < vertigo::SimVertex > & foundVertices )
  {
    /*
    cout << "[LcioEventGenerator] adding " << foundVertices.size() << " SimVertices!"
         << endl; */
    for ( vector< vertigo::SimVertex >::iterator foundThis = foundVertices.begin() ;
          foundThis != foundVertices.end() ; foundThis++ )
    {
      event.add( *foundThis );
    }
  }
}

bool LcioEventGenerator::hasBeamSpot() const
{
  return false;
}

rave::Ellipsoid3D LcioEventGenerator::beamSpot() const
{
  return rave::Ellipsoid3D();
}

std::string LcioEventGenerator::describe() const
{
  return "LcioEventGenerator";
}

LcioEventGenerator::LcioEventGenerator() : theId(0), theLcReader(0)
{
  setup();
}

void LcioEventGenerator::setup()
{
  using namespace raveunits;
  raveConverter = buildRaveConverter();
  edm::LogInfo ( "LcioEventGenerator" ) << "Using lcio version " << lcio_version();
  edm::LogWarning ( "LcioEventGenerator" ) << "Using smeared primary (130nm, 190nm, 100um)";
  raveConverter.defineOffset ( 130. * nanometer, 190. * nanometer, 100. * micrometer );
}

void LcioEventGenerator::source ( const string & filename )
{
  LogDebug("LcioEventGenerator") << "Assigning \"" << filename << "\" as source.";
  if ( !theLcReader ) theLcReader = LCFactory::getInstance()->createLCReader();
  // close the old file
  if ( ( filename != theFileName) && theFileName.size() ) theLcReader->close();
  // check to see if the semicolon is there
  string::size_type loc = filename.find( ":", 0 );
  if (loc == string::npos)
  {
    theFileName = filename;
    // theCollection = "MCTracksRel";
    theCollections.push_back("RecoParticles");
    edm::LogWarning("LcioEventGenerator") << "No collection name given; assuming '" 
      << theCollections[0] << "'.";
  } else {
    // seperate the file- and the collectionnames
    theFileName = filename.substr( 0, loc );
    string collections = filename.substr( loc+1 );
    rule<> comma_separated_list;
    comma_separated_list = list_p[push_back_a(theCollections)];
    parse_info<> result = parse(collections.c_str(), comma_separated_list);
    if (result.hit)
    {
      edm::LogInfo("LcioEventGenerator") << "Successfully registered " 
	<< theCollections.size() << " collection name(s).";
      LogDebug("LcioEventGenerator") << "The first registered collection is \"" 
          << theCollections[0] << "\"." ;
    } else {
      edm::LogError("LcioEventGenerator") << "No collection names registered. " 
	<< "Failed to parse '" << collections << "'.";
    }
  }

  // instantiate the LCIO reader class
  try
  {
    // secure the file opening
    theLcReader->open( theFileName );

    edm::LogInfo("LcioEventGenerator") << "Opened LCIO file \"" << theFileName << "\".";
  }
  catch( IOException& e )
  {
    cout << "[LcioEventGenerator] " << e.what() << endl;
    throw;
  }
}

/**
 * The ctor for the class. It initializes the internal fields
 * and configures the EventGenerator.
 *
 * @param filename This string defines the data source:
 * The filename and the collection name seperated by a semicolon.
 */
LcioEventGenerator::LcioEventGenerator( const string & filename ) :
    theId(0), theLcReader ( 0 )
{
  setup();
  source( filename );
}

/**
 * The dtor for the class. It releases the LCIO file opened during construction.
 */
LcioEventGenerator::~LcioEventGenerator()
{
  if (theLcReader) theLcReader->close();
}

/**
 * Fetches the next event found in the LCIO stream.
 * @return The event as Vertigo structure
 */
vertigo::Event LcioEventGenerator::next() const
{
  #ifdef WITH_THREADS
  boost::mutex::scoped_lock lock(mutex);
  #endif

  raveConverter.createOffset();
  float bz = MagneticFieldSingleton::Instance()->inTesla(GlobalPoint(0.,0.,0.)).z();
  raveConverter.defineBz ( bz );

  // allocate the LCIO event structure
  // and let the LCIO reader do its work
  LCEvent* evt = theLcReader->readNextEvent();

  /*
  if ( LoopSettings::Instance().verbosity() > 9 )
  {
    cout << "[LcioEventGenerator] next evt: " << (void *) evt << endl;
  }*/

  if (!evt) // check for problems
  {
    edm::LogInfo("LcioEventGenerator") << "LCIO EOF reached.";
    throw IOException( "[LcioEventGenerator]  LCIO EOF reached" );
  }

  LogDebug("LcioEventGenerator") << "Next event has been read.";

  // allocate the Rave event structure and assign a sequential id
  vertigo::Event event ( theId++, 0 /* run number */, "lcio event" );

  // Collect the vertices taken from the MCParticle data
  vector< vertigo::SimVertex > foundVertices ;

  for (vector<string>::const_iterator col_name = theCollections.begin();
       col_name != theCollections.end(); col_name++)
  {
    LCCollection * collection = 0;
    try {
      collection = evt->getCollection(*col_name);
    } catch ( DataNotAvailableException & ex ) {
      edm::LogError("LcioEventGenerator") << ex.what();
      continue;
    }
    addCollection(event, collection, foundVertices);
  }


  // flavor checking?
  // int fl = checkFlavors( evt->getCollection( "MCParticle" ) );
  /*
  if ( fl )
  {
    cout << "[LcioEventGenerator] WARNING: skipping event." << endl;
    return next();
  }*/

  LCCollection * mccol = 0;
  try {
    mccol = evt->getCollection ( "MCParticle" );
  } catch ( ... ) {};

  for ( vector< string >::const_iterator coll=theCollections.begin(); 
        coll!=theCollections.end() ; ++coll )
  {
    LCCollection * relCol = evt->getCollection( *coll );
    if ( (*coll) == "Jet" || (*coll) == "Jets" )
    {
      LCCollection * flavors = evt->getCollection( "TrueJetFlavour" );
      addJets ( relCol, mccol, event, foundVertices, flavors );
      addFoundVertices ( event, foundVertices );
      return event;
    }

    int nRels = getNrRelations ( relCol );

    // cout << "[LcioEventGenerator] nRels=" << nRels << endl;
    if ( nRels == 0 )
    {
      LCCollection * col = evt->getCollection( *coll );
      addCollection(event, col, foundVertices);
      addCollection(event, mccol, foundVertices);
    } else {
      for ( int i = 0 ; i != nRels ; i++ )
      {
        LCRelationImpl* lcRel = dynamic_cast<LCRelationImpl*> ( relCol->getElementAt(i)  ) ;
        addRelation ( lcRel, event, foundVertices );
      }
    }
  }

  addFoundVertices ( event, foundVertices );
  return event;
}

/**
 * Tell the LCIO Reader to skip a given number of events.
 *
 * @param n The number of events to skip
 */
int LcioEventGenerator::skip ( int n ) const
{
  theId += n;
  theLcReader->skipNEvents(n);
  return n;
}

#include "Vertigo/EventGeneration/interface/GeneratorBuilder.h"
namespace {
    GeneratorBuilder<LcioEventGenerator> t( "lcio", "LCIO File Reader" );
}


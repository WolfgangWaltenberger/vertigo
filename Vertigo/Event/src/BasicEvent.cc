#include "Vertigo/Event/interface/BasicEvent.h"
#include "Vertigo/TrackAssociation/interface/TrackAssociatorByPulls.h"
#include "Vertigo/VertexAssociation/interface/VertexAssociatorByTracks.h"
#include "Vertigo/VertexAssociation/interface/BVertexAssociator.h"
#include "Vertigo/VertexAssociation/interface/VertexAssociatorByDistance.h"
#include "Environment/Configuration/interface/SimpleConfigurable.h"

#include <rave/impl/RaveBase/Converters/interface/RaveStreamers.h>
#include <rave/impl/FWCore/MessageLogger/interface/MessageLogger.h>

#include <vector>
#include <algorithm>
#include <set>

using namespace std;
using namespace vertigo;
using namespace rave;

namespace {
  VertexAssociator * getAssociator ( const vector < SimVertex > & simvertices,
      const vector < Vertex > & recvertices, const AssociationManager & manager )
  {
    static string assby = SimpleConfigurable<string> ( 
        "ReducedDistance", "Vertices:Association" ).value();

    VertexAssociator * ass = 0;

    /*
    vector < Vertex > allvertices = recvertices;
    for ( vector< Jet >::const_iterator i=theJets.begin(); i!=theJets.end() ; ++i )
    {
      vector < Vertex > tmp = i->vertices();
      allvertices.reserve ( allvertices.size() + tmp.size() );
      copy ( tmp.begin(), tmp.end(), back_inserter ( allvertices ) );
    }*/

    if ( assby == "Tracks" )
    {
      ass = new VertexAssociatorByTracks
            ( simvertices, recvertices, manager );
    }
    else if ( assby == "BVertex" )
    {
      ass = new BVertexAssociator ( simvertices, recvertices );
    }
    else if ( assby == "Distance" )
    {
      static float maxdist = SimpleConfigurable<float>
                            ( -1.0, "Vertices:AssociateByDistance:MaxDistance" ).value();
      ass = new VertexAssociatorByDistance ( simvertices, recvertices, false, maxdist );
    }
    else if ( assby == "ReducedDistance" )
    {
      static float maxdist = SimpleConfigurable<float>
                            ( 5.0, "Vertices:AssociateByDistance:MaxDistance" ).value();
      // cout << "[BasicEvent] reduced distance " << maxdist << endl;
      ass = new VertexAssociatorByDistance ( simvertices, recvertices, true, maxdist );
    }
    else if ( assby == "Primary" )
    {
      vector < SimVertex > primary;
      for ( vector< SimVertex >::const_iterator i=simvertices.begin(); 
            i!=simvertices.end() ; ++i )
      {
        if ( i->isPrimary() ) primary.push_back ( *i );
      }
      if ( primary.size() != 1 )
      {
        edm::LogWarning("BasicEvent" ) << "Event has " << primary.size()
                                       << " primary vertices!!!";
      }
      ass = new VertexAssociatorByDistance ( primary, recvertices, false );
    }
    else
    {
      edm::LogError ("BasicEvent") << "Association by " << assby
        << " unknown. Will associate by reduced distance!";
      static float maxdist = SimpleConfigurable<float>
                            ( 5.0, "Vertices:AssociateByDistance:MaxDistance" ).value();
      // cout << "[BasicEvent] reduced distance " << maxdist << endl;
      ass = new VertexAssociatorByDistance ( simvertices, recvertices, true, maxdist );
    }
    return ass;
  }
}


BasicEvent::BasicEvent ( int id, int run, const string & tag,
                                       const string & algo ) :
    theId ( id ), theRun ( run ), theTag ( tag ), theAlgo ( algo ), theTime ( 0. )
{}

void BasicEvent::setTime ( float t )
{
  theTime = t;
}

void BasicEvent::setTag ( const string & t )
{
  theTag = t;
}


BasicEvent::BasicEvent ( const vector < Track > & t,
                                       const vector < SimTrack > & s,
                                       const vector < SimVertex > & v, int id, int run,
                                       const string & tag, const string & algo ) :
    theId ( id ), theRun ( run ), theTag ( tag ), theAlgo ( algo ), theTime ( 0. ),
    theTracks ( t ), theSimTracks ( s ), theSimVertices ( v )
{}

BasicEvent::BasicEvent ( const vector < Jet > & jets,
                                       const vector < SimTrack > & s,
                                       const vector < SimVertex > & v, int id, int run,
                                       const string & tag, const string & algo ) :
    theId ( id ), theRun ( run ), theTag ( tag ), theAlgo ( algo ), theTime ( 0. ),
    theSimTracks ( s ), theSimVertices ( v )
{
  for ( vector< Jet >::const_iterator j = jets.begin(); j != jets.end() ; ++j )
  {
    vector< Track > tmp = j->tracks();
    for ( vector< Track >::const_iterator t = tmp.begin(); t != tmp.end() ; ++t )
    {
      theTracks.push_back ( *t );
    }
  }
}

BasicEvent::BasicEvent (
  const vector < KinematicParticle > & p,
  const vector < SimParticle > & s,
  const vector < SimVertex > & v, int id, int run,
  const string & tag, const string & algo ) :
    theId ( id ), theRun ( run ), theTag ( tag ), theAlgo ( algo ), theTime ( 0. ),
    theParticles ( p ), theSimParticles ( s ), theSimVertices ( v )
{
  // Eventually fill the particle data also into the tracks
}

void BasicEvent::add ( const Track & o )
{
  static bool addnonunique = SimpleConfigurable<bool>(false,"VertigoEvent:AddNonUniqueTracks").value();
  // check if it's already in the container
  // (good for in-jet tracks!)
  if ( addnonunique || ( find ( theTracks.begin(), theTracks.end(), o ) == theTracks.end() ) )
  {
    /* cout << "[BasicEvent] didnt find " << o.id() << " \""
         << o.tag() << "\". adding!" << endl; */
    theTracks.push_back ( o );
  } else {
    edm::LogError ( "BasicEvent" ) << "found rave::Track " << o.id() << "\""
         << o.tag() << "\" -- already added this id, so not adding.";
  }
}

void BasicEvent::add ( const KinematicParticle & p )
{
  theParticles.push_back ( p );
}

void BasicEvent::add ( const Vertex & o )
{
  theVertices.push_back ( o );
}

void BasicEvent::add ( const SimTrack & o )
{
  for ( vector< SimTrack >::const_iterator i=theSimTracks.begin(); 
        i!=theSimTracks.end() ; ++i )
  {
    if ( i->id() == o.id() )
    {
      static bool complain = SimpleConfigurable<bool>(true,"VertigoEvent:ComplainNonUniqueTracks").value();
      if (complain)
      edm::LogError ( "BasicEvent" ) << "already added sim track #" << i->id()
                                     << " before.";
      return;
    }
  }
  theSimTracks.push_back ( o );
}


void BasicEvent::add ( const SimParticle & sp )
{
  for ( vector< SimParticle >::const_iterator i=theSimParticles.begin(); 
        i!=theSimParticles.end() ; ++i )
  {
    if ( i->id() == sp.id() )
    {
      static bool complain = SimpleConfigurable<bool>(true,"VertigoEvent:ComplainNonUniqueTracks").value();
      if (complain)
        edm::LogError ( "BasicEvent" ) << "already added sim particle #" << i->id()
                                       << " before.";
      return;
    }
  }
  theSimParticles.push_back ( sp );
}

void BasicEvent::add ( const SimVertex & o )
{
  for ( vector< SimVertex >::const_iterator i=theSimVertices.begin(); 
        i!=theSimVertices.end() ; ++i )
  {
    if ( i->id() == o.id() )
    {
      static bool complain = SimpleConfigurable<bool>(true,"VertigoEvent:ComplainNonUniqueTracks").value();
      if (complain)
      edm::LogError ( "BasicEvent" ) << "already added sim vertex #" << i->id()
                                     << " before.";
      return;
    }
  }
  theSimVertices.push_back ( o );
}

void BasicEvent::add ( const Jet & o, bool do_add )
{
  for ( vector< Jet >::const_iterator i=theJets.begin(); 
        i!=theJets.end() ; ++i )
  {
    if ( i->id() == o.id() )
    {
      static bool complain = SimpleConfigurable<bool>(true,"VertigoEvent:ComplainNonUniqueTracks").value();
      if (complain)
      edm::LogError ( "BasicEvent" ) << "already added sim track #" << i->id()
                                     << " before.";
      return;
    }
  }
  theJets.push_back ( o );
  vector < Track > trks = o.tracks();

  if ( do_add )
  {
    for ( vector< Track >::const_iterator i = trks.begin(); i != trks.end() ; ++i )
    {
      add ( *i );
    }
  }
}

vector < Vertex > BasicEvent::vertices() const
{
  return theVertices;
}

vector < Track > BasicEvent::tracks() const
{
  return theTracks;
}

vector < KinematicParticle > BasicEvent::particles() const
{
  return theParticles;
}

vector < Jet > BasicEvent::jets() const
{
  return theJets;
}

vector < Jet > & BasicEvent::jetRefs()
{
  return theJets;
}

vector < SimTrack > BasicEvent::simTracks() const
{
  return theSimTracks;
}

vector < SimParticle > BasicEvent::simParticles() const
{
  return theSimParticles;
}

vector < SimVertex > BasicEvent::simVertices() const
{
  return theSimVertices;
}

int BasicEvent::id() const
{
  return theId;
}

int BasicEvent::run() const
{
  return theRun;
}

string BasicEvent::tag() const
{
  return theTag;
}

string BasicEvent::algorithm() const
{
  return theAlgo;
}

void BasicEvent::setAlgorithm ( const string & algo )
{
  theAlgo = algo;
}

void BasicEvent::associateVertices ( const vector < SimVertex > & simvertices,
    const vector < Vertex > & recvertices )
{
  static bool rectibles_only = SimpleConfigurable<bool> ( 
    true, "Vertices:AssociateOnlyRectibleVertices" ).value();
  /**
   *  we only associate reconstructible vertices! */
  vector < SimVertex > rectible_simvertices;

  if ( rectibles_only )
  {
    for ( vector< SimVertex >::const_iterator i=simvertices.begin(); 
          i!=simvertices.end() ; ++i )
    {
      if ( isReconstructible ( *i ) )
      {
        rectible_simvertices.push_back ( *i );
      }
    }
  } else {
    rectible_simvertices = simvertices;
  }

  VertexAssociator * ass = getAssociator( rectible_simvertices, recvertices, theManager );
  try
  {
    for ( vector< SimVertex >::const_iterator i = simvertices.begin();
          i != simvertices.end() ; ++i )
    {
      vector < Vertex > rs = ass->recVertices ( *i );
      for ( vector < Vertex >::const_iterator j = rs.begin();
            j != rs.end() ; ++j )
      {
        associate ( *i, *j );
      }
    }

    /*
    for ( vector< Vertex >::const_iterator i = recvertices.begin();
          i != recvertices.end() ; ++i )
    {
      vector < SimVertex > rs = ass->simVertices ( *i );
      for ( vector < SimVertex >::const_iterator j = rs.begin();
            j != rs.end() ; ++j )
      {
        associate ( *j, *i );
      }
    }
    */
  }
  catch ( ... )
  {
    // make sure we always delete.
  }
  delete ass;
}

void BasicEvent::associateVertices()
{
  /*
  cout << "[BasicEvent] debug: need to associate vertices!" << endl;
  cout << "we have: " << theSimVertices.size() << " simvertces, "
       << theVertices.size() << " event-vertices." << endl;
       */
  // FIXME dont associate one vertex twice!!
  // FIXME or dont present the same vertex twice!!
  associateVertices ( theSimVertices, theVertices );
  for ( vector< Jet >::iterator i=theJets.begin(); i!=theJets.end() ; ++i )
  {
    vector < Vertex > vtces = i->vertices();
    // cout << "in jet " << i->id() << " we have " << vtces.size() << " vertices." << endl;
    associateVertices ( theSimVertices, vtces );
  }
}

void BasicEvent::tag ( const Jet & j, double value )
{
  for ( vector< Jet >::iterator i = theJets.begin(); i != theJets.end() ; ++i )
  {
    if ( j.id() == i->id() )
    {
      i->tag ( value );
      break;
    }
  }
}

void BasicEvent::associateTracks()
{
  vector < rave::Track > tracks = theTracks;
  for ( vector< vertigo::Jet >::const_iterator i=theJets.begin(); i!=theJets.end() ; ++i )
  {
    vector < rave::Track > newtrks = i->tracks();
    tracks.reserve ( tracks.size() + newtrks.size() );
    copy ( newtrks.begin(), newtrks.end(), back_inserter ( tracks ) );
  }
  TrackAssociatorByPulls ass ( theSimTracks, tracks, TrackDistanceByChiSquared() );
  // TrackAssociatorByPulls ass ( theSimTracks, theTracks, TrivialTrackDistance() );
  // cout << "[BasicEvent] debug: associating tracks by pulls!" << endl;

  for ( vector< SimTrack >::const_iterator i = theSimTracks.begin();
        i != theSimTracks.end() ; ++i )
  {
    vector < Track > rs = ass.recTracks ( *i );
    for ( vector < Track >::const_iterator j = rs.begin();
          j != rs.end() ; ++j )
    {
      // cout << "  `- " << i->id() << " with " << j->id() << endl;
      associate ( *i, *j );
    }
  }
}

void BasicEvent::clearTracks()
{
  theTracks.clear();
}

void BasicEvent::removeParticles()
{
  // cout << "[BasicEvent] removing particles." << endl;
  //for ( vector< rave::KinematicParticle >::iterator i=theParticles.begin(); 
  //      i!=theParticles.end() ; ++i )
  //{
    // cout << "[BasicEvent] removing " << i->id() << endl;
    // i->unlink();
    // delete &(*i);
  //}
  // theParticles=vector< rave::KinematicParticle >();
  // cout << "[BasicEvent] removed particles." << endl;
}

void BasicEvent::associate ( const SimTrack & s, const Track & r )
{
  theManager.associate ( s, r );
}

void BasicEvent::associate ( const SimParticle & s, const KinematicParticle & r )
{
  theManager.associate ( s, r );
}

void BasicEvent::associate ( const SimVertex & s, const Vertex & r )
{
  theManager.associate ( s, r );
}

vector < SimTrack > BasicEvent::associatedTracks ( const Track & t ) const
{
  return theManager.tracks ( t );
}

vector < Track > BasicEvent::associatedTracks ( const SimTrack & t ) const
{
  return theManager.tracks ( t );
}

vector < SimParticle > BasicEvent::associatedParticles( const KinematicParticle & t ) const
{
  return theManager.particles ( t );
}

vector < KinematicParticle > BasicEvent::associatedParticles ( const SimParticle & t ) const
{
  return theManager.particles ( t );
}

vector < SimVertex > BasicEvent::associatedVertices ( const Vertex & t ) const
{
  return theManager.vertices ( t );
}

vector < Vertex > BasicEvent::associatedVertices ( const SimVertex & t ) const
{
  return theManager.vertices ( t );
}

float BasicEvent::time() const
{
  return theTime;
}

void BasicEvent::unReference()
{
  for ( vector< SimTrack >::iterator i = theSimTracks.begin();
        i != theSimTracks.end() ; ++i )
  {
    if ( i->vertex() )
    {
      i->setVertex ( ( const SimVertex * ) 0 );
    }
  }
}

int BasicEvent::nRecTracks ( const SimVertex & vtx ) const
{
  set < Track > recable_trks;
  vector < SimTrack > trks = vtx.tracks();
  for ( vector< SimTrack >::const_iterator i=trks.begin();
      i!=trks.end() ; ++i )
  {
    vector < Track > ass = this->associatedTracks ( *i );
    if ( ass.size() ) recable_trks.insert ( ass[0] );
  }
  return recable_trks.size();
}

bool BasicEvent::isReconstructible ( const SimVertex & vtx ) const
{
  return ( nRecTracks ( vtx ) > 1 );
}

BasicEvent::~BasicEvent()
{
//  unReference();
}

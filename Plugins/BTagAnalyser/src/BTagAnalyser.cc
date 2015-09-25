#include "BTagAnalyser.h"
#include <dataharvester/Writer.h>
#include <dataharvester/SystemWriter.h>
#include <rave/impl/RaveBase/Converters/interface/RaveStreamers.h>
#include "Environment/Configuration/interface/SimpleConfigurable.h"
#include "Environment/VertigoObjects/interface/VertigoJet.h"
#include "Vertigo/Event/interface/Event.h"
#include "VertigoHarvest/interface/ObserverTuples.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Utilities/interface/Exception.h"
#include <cmath>
#include <stdexcept>

using namespace std;
using namespace vertigo;
using namespace rave;
using namespace dataharvester;

namespace {

  void checkDuplicates ( const vector < Vertex > & v )
  {
    cout << "[BTagAnalyser] the following vertices are associated with the B-Meson: ";
    set < int > dupes;
    for ( vector< Vertex >::const_iterator i=v.begin(); i!=v.end() ; ++i )
    {
      cout << i->id() << " ";
      if ( dupes.find ( i->id() ) != dupes.end() )
      {
        cout << "(dupe!!)";
      }
      dupes.insert ( i->id() );
    }
    cout << endl;
  }

  const SimVertex & getPrimary ( const vector < SimVertex > & vtces )
  {
    // get the primary vertex
    if ( vtces.empty() ) throw cms::Exception ( "BTagAnalyser, No SimVertex, cannot find primary");
    for ( vector< SimVertex >::const_iterator i=vtces.begin(); i!=vtces.end() ; ++i )
    {
      // if ( i->pid() == 2212 && i->tag() == "7460.54" ) 
      if ( i->isPrimary() ) 
      {
        return (*i);
      }
    }

    edm::LogWarning("BTagAnalyser") 
      << "couldnt find primary vertex - will return first vertex in container!";
    return vtces[0];
  }
  
  SimVertex getBVertex ( const vector < SimVertex > & vtces )
  {
    // get the B Meson
    if ( vtces.size() < 2 ) 
      throw cms::Exception ( "BTagAnalyser,fewer than 2 SimVertices, cannot find bmeson");
    for ( vector< SimVertex >::const_iterator i=vtces.begin(); i!=vtces.end() ; ++i )
    {
      if ( ( fabs ( fmod ( (i->pid() / 100 ), 5. ) ) < .001 ) // is a B?
           && ( fabs ( i->pid() ) > 100 ) ) // exclude taus and the like!
      // if ( i->pid() / 100 == 5 )
      {
        return (*i);
      }
    }

    // FIXME dont!! Return nothing!!!!
    /*
    edm::LogWarning("BTagAnalyser") 
      << "couldnt find bmeson vertex - will return second vertex in container!";
    return vtces[1];
    */
    return SimVertex(); // return an invalid vertex
  }

  float getDistance ( const SimVertex & v1, const SimVertex & v2 )
  {
    float dx=v1.position().x()-v2.position().x();
    float dy=v1.position().y()-v2.position().y();
    float dz=v1.position().z()-v2.position().z();
    return sqrt ( dx*dx+dy*dy+dz*dz );
  }
  
  float getDistance ( const Vertex & v1, const SimVertex & v2 )
  {
    float dx=v1.position().x()-v2.position().x();
    float dy=v1.position().y()-v2.position().y();
    float dz=v1.position().z()-v2.position().z();
    return sqrt ( dx*dx+dy*dy+dz*dz );
  }
  
  float getDistance ( const Vertex & v1, const Vertex & v2 )
  {
    float dx=v1.position().x()-v2.position().x();
    float dy=v1.position().y()-v2.position().y();
    float dz=v1.position().z()-v2.position().z();
    return sqrt ( dx*dx+dy*dy+dz*dz );
  }

  void add ( Tuple & t, const SimVertex & primary, const SimVertex & bmeson, bool describe=false )
  {
    // save the flight path of the true B meson.
    float lab_d = getDistance ( primary, bmeson );
    float gamma = atof ( bmeson.tag().c_str() );
    float eigen_d = -1.;
    if ( finite(gamma) && gamma >= 1. )
    {
      eigen_d = lab_d / gamma;
    }
    t["mc_gamma"]=gamma;
    if (describe) t.describe("The Lorentz boost of the B Meson","mc_gamma" );
    t["mc_labdistance"]=lab_d;
    if (describe) t.describe ( "Distance primary - B-decay in the lab frame", "mc_labdistance" );
    t["mc_pdg"]=bmeson.pid();
    if (describe) t.describe("pdg of the secondary vertex","mc_pdg");
    t["mc_isb"]=( abs ( bmeson.pid() / 100 ) == 5 );
    if (describe) t.describe("is the secondary vertex a B?","mc_isb" );
    t["mc_eigendistance"]=eigen_d;
    if (describe) t.describe ( "Distance primary - B-decay in the eigen frame", "mc_eigendistance" );
  }

  edm::ParameterSet sorterSet ()
  {
    edm::ParameterSet ret;
    ret.addParameter<string>("sortCriterium","dist3dError");
    return ret;
  }
}

BTagAnalyser::BTagAnalyser() : theFileName ( "btagger.txt" ),
  theSaveTagsOnly(true), theIsFirst ( true ), theDescribe(false),
  theSorter ( reco::VertexSorting ( sorterSet() ) )
{
  theSaveTagsOnly=SimpleConfigurable<bool>(true,"BTagAnalyser:SaveTagsOnly").value();
}

void BTagAnalyser::saveMeta( const Event & e )
{
  if (!theIsFirst) return;
  // write down algorithm used
  Tuple t ( "Meta" );
  t["algorithm"]=e.algorithm();
  t["analyser"]="BTagAnalyser";
  Writer::file( theFileName ) << t;
  SystemWriter ( theFileName ).save ();                                                                         
  theIsFirst=false;
}

void BTagAnalyser::tagJets( const Event & e ) const
{
  vector < Jet > jets = e.jets();
  // cout << "[BTagAnalyser] njets=" << jets.size() << endl;
  for ( vector< Jet >::const_iterator i=jets.begin(); i!=jets.end() ; ++i )
  {
    /*
    cout << "[BTagAnalyser] position " << i->position() << " momentum " << i->momentum()
         << endl; */

    if ( i->isTagged() )
    {
      Tuple t ( "Tag" );
      t["jet"]=i->id();
      t["tag"]=i->getTag();
      t["time"]=i->time();
      string sflavor="";
      sflavor+=i->getMCTag();
      t["flavor"]=sflavor;
      string physflavor="";
      physflavor+=i->getMCPhysicsTag();
      t["physicsflavor"]=physflavor;
      edm::LogInfo ( "BTagAnalyser" ) << "Tagging jet #" << i->id()
                       << " (" << sflavor << ") with " << i->getTag();
      Writer::file( theFileName ) << t;
    }
  }
}

/*
void BTagAnalyser::process ( const edm::ParameterSet & s )
{
  ParameterSetWriter writer ( theFileName, "Params" );
  writer.save ( s );
}*/

void BTagAnalyser::process ( const Event & e )
{
  saveMeta( e );
  if ( !theSaveTagsOnly )
  {
    float t = saveJets ( e );
    saveEvent ( e, t );
  }
  tagJets( e );

  if ( !theSaveTagsOnly )
  {
    const vector < SimVertex > & simvtces = e.simVertices();
    try {
      SimVertex primary = getPrimary ( simvtces );
      SimVertex bmeson = getBVertex ( simvtces );
      if ( bmeson.isValid() && primary.isValid() )
      {
        saveBFlightPath ( e.id(), primary, bmeson );
        saveRecoVertices ( e, primary, bmeson );
      } else {
        saveMissingMonteCarloFlightPath ( e.id(), primary, bmeson );
      }
    } catch ( cms::Exception & e ) {
      edm::LogWarning("BTagAnalyser") << "Couldnt process data. Exception: " << e.what();
      process ( runtime_error ( e.what() ), "cms", "analysis" );
    }
  }
}

void BTagAnalyser::saveMissingMonteCarloFlightPath ( int id, const SimVertex & primary,
    const SimVertex & secondary ) const
{
  Tuple t ( "MissingMCFlightPath" );
  t["eventid"]=id;
  t["has_primary"]=primary.isValid();
  t["has_secondary"]=secondary.isValid();
  Writer::file( theFileName ) << t;
}

void BTagAnalyser::saveRecoVertexStat ( const Event & e,
    const SimVertex & primary, const SimVertex & bmeson ) const
{
  const vector < Vertex > & vtces = e.vertices();
  Tuple t ( "RecoVertexStat" );
  if (theDescribe) t.describe ( "some statistics about the reconstructed vertices per event");
  t["nrecvtces_event"]=vtces.size();
  const vector < Jet > & jets = e.jets();
  int nrecvtces_jet=0;
  for ( vector< Jet >::const_iterator i=jets.begin(); i!=jets.end() ; ++i )
  {
    nrecvtces_jet+=i->vertices().size();
  }
  t["nrecvtces_jet"]=nrecvtces_jet;

  t["time"]=e.time();
  if (theDescribe) t.describe("Number of reconstructed vertices","nvtces");
  const vector < Vertex > & rec_prims = e.associatedVertices( primary );
  const vector < Vertex > & rec_bs = e.associatedVertices( bmeson );
  t["n_ass_primaries"]=rec_prims.size();
  if (theDescribe) t.describe ( "Number of rec vertices associated to primary", "n_ass_primaries" );
  // checkDuplicates ( rec_bs );
  t["n_ass_bmesons"]=rec_bs.size();
  if (theDescribe) t.describe ( "Number of rec vertices associated to bmeson vertex", "n_ass_bmesons" );
  /*
  int nvtces=0;
  vector< Jet > jets=e.jets();
  for ( vector< Jet >::const_iterator i=jets.begin(); i!=jets.end() ; ++i )
  {
    nvtces+=i->vertices().size();
  }
  t["njetvtces"]=nvtces;
  if (theDescribe) t.describe("Number of reconstructed vertices in the jets","njetvtces");
  */

  Writer::file( theFileName ) << t;
}

void BTagAnalyser::saveEvent ( const Event & e, float total_jet_time ) const
{
  Tuple t("Event");
  t["time"]=e.time();
  t["id"]=e.id();
  t["time_all_jets"]=total_jet_time;
  Writer::file( theFileName ) << t;
}

float BTagAnalyser::saveJets ( const Event & e ) const
{
  vector < Jet > jets = e.jets();
  float total=0.;
  for ( vector< Jet >::const_iterator i=jets.begin(); i!=jets.end() ; ++i )
  {
    Tuple t("Jet");
    ObserverTuples::addJet ( *i, t );
    int n_not_associated_vertices=0;
    vector < Vertex > vertices = i->vertices();
    for ( vector< Vertex >::const_iterator v=vertices.begin(); v!=vertices.end() ; ++v )
    {
      vector < SimVertex > ass = e.associatedVertices ( *v );
      if ( ass.size() == 0 ) n_not_associated_vertices++;
    }
    t["not_associated_vertices"]=n_not_associated_vertices;
    total+=i->time();
    Writer::file( theFileName ) << t;
  }
  return total;
}

void BTagAnalyser::savePrimaryVertex ( const Vertex & rec,
    const SimVertex & sim ) const
{
  Tuple t("PrimaryVertex");
  ObserverTuples::addAssociatedVertex ( sim, rec, t );
  Writer::file( theFileName ) << t;
}

void BTagAnalyser::saveBVertex ( const Vertex & rec,
    const SimVertex & sim ) const
{
  Tuple t("BVertex");
  ObserverTuples::addAssociatedVertex ( sim, rec, t );
  Writer::file( theFileName ) << t;
}

void BTagAnalyser::saveFlightPath ( const Vertex & primrec,
    const SimVertex & primsim,
    const Vertex & bmesonrec,
    const SimVertex & bmesonsim ) const
{
  Tuple t ( "RecoFlightPath" );
  if (theDescribe) t.describe ( "The actual tuple that contains the flight paths" );
  add ( t, primsim, bmesonsim );
  float rfp=getDistance ( primrec, bmesonrec );
  t["rec_labdistance"]=rfp;
  float mc_gamma = atof ( bmesonsim.tag().c_str() );
  t["mc_gamma"]=mc_gamma;
  float eigenpath = -1.;
  if ( finite(mc_gamma) && mc_gamma >= 1. )
  {
    eigenpath = rfp / mc_gamma;
  }
  t["rec_eigendistance"]= eigenpath;
  if (theDescribe) t.describe("The flight path in the eigensystem, the gamma coming from the Monte Carlo","rec_eigenpath");

  float mcfp=getDistance ( primsim, bmesonsim );
  t["delta"]=rfp-mcfp;
  if (theDescribe) t.describe("The distance reco -sim, flight path","delta" );
  t["delta_prim"]=getDistance ( primrec, primsim );
  if (theDescribe) t.describe("The distance reco - sim, primary vertex","delta_prim" );
  t["delta_b"]=getDistance ( bmesonrec, bmesonsim );
  if (theDescribe) t.describe("The distance reco - sim, B meson decay vertex", "delta_b" );
  // float gamma = atof ( bmesonsim.tag().c_str() );
  Writer::file( theFileName ) << t;
}

void BTagAnalyser::saveRecoVertices ( const Event & e,
    const SimVertex & primary, const SimVertex & bmeson ) const
{
  saveRecoVertexStat ( e, primary, bmeson );
  // const vector < Vertex > & vtces = e.vertices();
  const vector < Vertex > & rec_prims = e.associatedVertices( primary );
  const vector < Vertex > & rec_bs = e.associatedVertices( bmeson );

  if ( rec_prims.size() )
  {
    savePrimaryVertex ( rec_prims[0], primary );
  }
  if ( rec_prims.size() && rec_bs.size() )
  {
    saveFlightPath ( rec_prims[0], primary, rec_bs[0], bmeson );
    saveBVertex ( rec_bs[0], bmeson );
  } else {
    describeFailure ( (rec_prims.size()>0), (rec_bs.size()>0) );
  }
}

void BTagAnalyser::describeFailure (
    bool prim, bool bmeson ) const
{
  // At least one of the simulated vertices doesnt have an associated
  // rec vertex.
  Tuple t ( "Failed" );
  if (theDescribe) t.describe ( "Document the failures." );
  t["has_primary"]=prim;
  if (theDescribe) t.describe("do we have an associated primary vertex?","has_primary");
  t["has_bmeson"]=bmeson;
  if (theDescribe) t.describe("do we have an associated bmeson decay vertex?","has_bmeson");
  Writer::file( theFileName ) << t;
}

void BTagAnalyser::saveBFlightPath ( int id, const SimVertex & primary,
    const SimVertex & bmeson ) const
{
  Tuple t ( "MCBFlightPath" );
  t["eventid"]=id;
  add ( t, primary, bmeson );
  if ( fmod ( fabs ( bmeson.pid()  ), 100 ) == 31 )
  {
    Tuple t ( "MCBsFlightPath" );
    add ( t, primary, bmeson );
  } else {
    Tuple t ( "MCB0FlightPath" );
    add ( t, primary, bmeson );
  };
  Writer::file( theFileName ) << t;
}

void BTagAnalyser::process ( const exception & e, const string & type )
{
  process ( e, type, "code" );
}

void BTagAnalyser::process ( const exception & e, const string & type, const string & where )
{
  Tuple t ( "Exceptions" );
  t["what"]=e.what();
  t["where"]=where;
  t["type"]=type;
  Writer::file( theFileName ) << t;
}

void BTagAnalyser::configure ( const string & conf )
{
  theFileName = conf;
}

BTagAnalyser::~BTagAnalyser() {}

BTagAnalyser * BTagAnalyser::clone() const
{
  return new BTagAnalyser ( *this );
}

#include "Analysis/Observer/interface/ObserverBuilder.h"
namespace {
  ObserverBuilder<BTagAnalyser> t( "BTagAnalyser",
      "Analyses rave's b-tagging algorithms", false );
}


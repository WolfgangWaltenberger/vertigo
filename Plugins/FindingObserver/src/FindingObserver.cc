#include "FindingObserver.h"
#include "Environment/Configuration/interface/SimpleConfigurable.h"
#include "Vertigo/Event/interface/Event.h"
#include "Vertigo/EventLoop/interface/LoopSettings.h"
#include <dataharvester/Writer.h>
#include <rave/impl/RaveBase/Converters/interface/RaveToAlgebraicObjects.h>
// #include "Vertex/DataHarvesting/interface/SystemHarvester.h"
// #include "Vertex/RobustVertexFitTest/interface/CmsInfoHarvester.h"

using namespace std;
using namespace dataharvester;

namespace {
  string shortName ( string src )
  {
    string::size_type pos=src.find(",");
    if ( pos != string::npos )
    {
      src.replace ( pos, string::npos , "" );
    }
    pos=src.find("file:");
    if ( pos != string::npos )
    {
      src.replace ( 0, 5, "" );
    }
    pos=src.find("gun:");
    if ( pos != string::npos )
    {
      src.replace ( 0, 4, "" );
    }
    pos=src.rfind(".");
    if ( pos != string::npos )
    {
      src.replace ( pos, string::npos, "" );
    }
    pos=src.rfind("/");
    if ( pos != string::npos )
    {
      src.replace ( 0, pos+1, "" );
    }

    if ( src!="" ) src=src+".";
    return src;
  }
}


FindingObserver::FindingObserver() :
    theFileName ("events.hdf"), theFixFileName ( true )
{
}

FindingObserver::~FindingObserver()
{
}

void FindingObserver::configure ( const string & s )
{
  theFileName=s;
}

FindingObserver * FindingObserver::clone() const
{
  return new FindingObserver ( *this );
}


void FindingObserver::missedPrimary ( float d ) const
{
  // not even a primary rec vtx?
  Tuple prim_miss ("missed_primaries");
  prim_miss.describe ( "tuple for the cases in which even "
      "the primaries have not been reconstructed" );
  prim_miss["d"]=d;
  prim_miss.describe("distance secondary - primary sim vertex (mu)","d");
  prim_miss.fill();
  dataharvester::Writer::file ( theFileName ) << prim_miss;
  return;
}

void FindingObserver::foundSecondary ( const vertigo::SimVertex & sim_primary,
                        const vertigo::SimVertex & sim_secondary,
                        const rave::Vertex & rec_primary,
                        const rave::Vertex & rec_secondary,
                        const string & tuplename ) const
{
  rave::Vector3D dist = rec_primary.position() - sim_primary.position();
  float dprim = mu * dist.mag();
  float d = mu *( sim_primary.position() - sim_secondary.position() ).mag();
  Tuple tuple( tuplename );
  tuple.describe ("histograms secondary vertex finding misses and resolutions, "
      "depending on distance between secondary and primary sim vertex" );
  tuple["d"]=d;
  tuple.describe("distance secondary - primary sim vertex (mu)","d");

  RaveToAlgebraicObjects converter;
  AlgebraicVector3 vec = converter.convert ( dist );
  AlgebraicSymMatrix33 cov = converter.convert ( rec_primary.error() );
  float sprim=-1.;
  bool succ = cov.Invert ( );
  if ( !succ )
  {
    cout << "[FindingObserver] cannot invert covariance matrix" << endl;
  } else {
    sprim = ROOT::Math::Similarity ( vec, cov );
  }
  
  tuple["dprim"]=dprim;
  tuple["dprim_x"]= dist.x();
  tuple["dprim_y"]= dist.y();
  tuple["dprim_z"]= dist.z();
  tuple.describe ( "distance reconstructed and simulated primary", "dprim" );
  tuple["sprim"]=sprim;
  tuple.describe ( "reduced distance reconstructed and simulated primary", "sprim" );
  
  dist = rec_secondary.position() - sim_secondary.position();
  float dsec = mu * dist.mag();
    
  tuple["dsec"]=dsec;
  tuple["dsec_x"]=dist.x();
  tuple["dsec_y"]=dist.y();
  tuple["dsec_z"]=dist.z();
  tuple.describe ( "distance reconstructed and simulated secondary" );
  
  vec = converter.convert ( dist );
  cov = converter.convert ( rec_secondary.error() );
  float ssec=-1.;
  bool succ2 = cov.Invert ( );
  if ( !succ2 )
  {
    cout << "[FindingObserver] cannot invert covariance matrix" << endl;
  } else {
    ssec = ROOT::Math::Similarity ( vec, cov );
  }
  
  tuple["ssec"]=ssec;
  tuple.describe ( "reduced distance reconstructed and simulated secondary", 
                   "ssec" );
  
  tuple.fill();
  dataharvester::Writer::file ( theFileName ) << tuple;
}

void FindingObserver::missedSecondary ( float d, float dprim, 
    float dx, float dy, float dz, const string & tuplename ) const
{
    // no secondary rec vtx?
    Tuple missed( tuplename );
    missed.describe ( "secondary sim vertex has no associated rec vertex" );
    missed["d"]=d;
    missed.describe ( "distance secondary - primary sim vertex (mu)","d");
    missed["dprim"]=dprim;
    missed["dprim_x"]= dx;
    missed["dprim_y"]= dy;
    missed["dprim_z"]= dz;
    missed.describe ( "distance reconstructed and simulated primary" ); 
    missed.fill();
    dataharvester::Writer::file ( theFileName ) << missed;
}

void FindingObserver::logCpuTime ( float t )
{
  Tuple cpu("cpu");
  cpu["t"]=t;
  cpu.fill();
  dataharvester::Writer::file ( theFileName ) << cpu;
}

void FindingObserver::process ( const vertigo::Event & e )
{
  logCpuTime( e.time() );
  vector < vertigo::SimVertex > simvtces = e.simVertices();
  if ( simvtces.size() < 2 )
  {
    except ( "Fewer than two simvtces", e.id() );
    return;
  };

  vertigo::SimVertex sim_primary = simvtces[0];
  vertigo::SimVertex sim_secondary = simvtces[1];

  // d: the distance between the two first simulated vertices
  float d = mu *( sim_primary.position() - sim_secondary.position() ).mag();
  
  vector < rave::Vertex > ass_prim = e.associatedVertices ( sim_primary );
  if ( ass_prim.size() == 0 )
  {
    missedPrimary ( d );
    return;
  };

  rave::Vertex rec_primary = ass_prim[0];
  rave::Vector3D dist = rec_primary.position() - sim_primary.position();
  float dprim = mu * dist.mag();
  
  vector < rave::Vertex > ass_sec = e.associatedVertices ( sim_secondary );

  if ( ass_sec.size() == 0 )
  {
    // no secondary rec vtx?
    missedSecondary ( d, dprim, dist.x(), dist.y(), dist.z() );
    return;
  }

  foundSecondary ( sim_primary, sim_secondary, rec_primary, ass_sec[0] );

  if ( simvtces.size() > 2 )
  {
    for ( unsigned i=2; i< simvtces.size()  ; i++ )
    {
      vertigo::SimVertex simvtx = simvtces[i];
      vector < rave::Vertex > ass_sec = e.associatedVertices ( simvtx );

      rave::Vector3D dist = rec_primary.position() - sim_primary.position();
      float dprim = mu * dist.mag();
      if ( ass_sec.size() == 0 )
      {
        // no secondary rec vtx?
        missedSecondary ( d, dprim, dist.x(), dist.y(), dist.z(), "missed_tertiaries" );
        return;
      } else {
        foundSecondary ( sim_primary, simvtx, rec_primary, ass_sec[0], "tertiaries" );
      }
    }
  }

  vector < rave::Vertex > recvtces = e.vertices();                                                        
  // for all remaining rec vertices, log an extraTertiary FIXME
  for ( vector< rave::Vertex >::const_iterator i=recvtces.begin(); 
        i!=recvtces.end() ; ++i )
  {
    if ( e.associatedVertices ( *i ).size() == 0 )
    {
      extraTertiary ( *i );
    }
  }
}

void FindingObserver::process ( const std::exception & e, const std::string & type )
{
  except ( e.what(), -1 );
  missedPrimary ( -1. );
}

void FindingObserver::except ( const std::string & e, int id )
{
  Tuple data ( "exceptions" );
  data["what"]=e;
  data["id"]=id;
  dataharvester::Writer::file ( theFileName ) << data;
}

void FindingObserver::extraTertiary ( const rave::Vertex & rec_secondary ) const
{
  Tuple tuple("extra_tertiaries");
  tuple.describe ( "a tertiary rec vtx, with no associatable sim vertex" );
  tuple["id"]=rec_secondary.id();
  tuple.fill();
  dataharvester::Writer::file ( theFileName ) << tuple;
}

#include "Analysis/Observer/interface/ObserverBuilder.h"
namespace {
  ObserverBuilder<FindingObserver> t( "FindingObserver", 
      "Special observer for the finding sensitivity paper", false );
}


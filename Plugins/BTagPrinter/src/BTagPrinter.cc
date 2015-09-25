#include "BTagPrinter.h"
#include <rave/impl/RaveBase/Converters/interface/RaveStreamers.h>
#include "Environment/VertigoObjects/interface/VertigoJet.h"
#include "Environment/Utilities/interface/Console.h"                                                                                           
#include "Vertigo/Event/interface/Event.h"
#include "VertigoHarvest/interface/ObserverTuples.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Utilities/interface/Exception.h"
#include <stdexcept>
#include <cmath>

#include <dataharvester/Tuple.h>

using namespace std;
using namespace vertigo;
using namespace dataharvester;
using namespace rave;

namespace {
  vertigo::Console console;

  const SimVertex & getPrimary ( const vector < SimVertex > & vtces )
  {
    // get the primary vertex
    if ( vtces.empty() ) throw cms::Exception ( "BTagPrinter, No SimVertex, cannot find primary");
    for ( vector< SimVertex >::const_iterator i=vtces.begin(); i!=vtces.end() ; ++i )
    {
      if ( i->isPrimary() ) 
      {
        return (*i);
      }
    }

    throw cms::Exception ( "BTagPrinter, No SimVertex, cannot find primary");
  }
  
  SimVertex getBVertex ( const vector < SimVertex > & vtces )
  {
    // get the B Meson
    if ( vtces.size() < 2 ) 
      throw cms::Exception ( "BTagPrinter::getBVertex" ) << "BTagPrinter,fewer than 2 SimVertices, cannot find bmeson";
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
    edm::LogWarning("BTagPrinter") 
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
}

BTagPrinter::BTagPrinter() 
{}

void BTagPrinter::process ( const Event & e )
{
  cout << endl
       << "-------------------------------------------------" << endl
       << "[BTagPrinter] Event #" << e.id();
  if ( e.tag().size() && e.tag()!="undef" )                                                                                                    
  {                                                                                                                                            
    cout << " (" << e.tag() << ")";                                                                                                            
  }                
  cout << endl << endl;
  const vector < SimVertex > & simvtces = e.simVertices();
  try {
    SimVertex primary = getPrimary ( simvtces );
    SimVertex bmeson = getBVertex ( simvtces );
    if ( bmeson.isValid() && primary.isValid() )
    {
      printBFlightPath ( primary, bmeson );
      printRecoVertices ( e, primary, bmeson );
      cout << endl;
      return;
    }
  } catch ( cms::Exception & ep ) { 
    cout << "[BTagPrinter] " << ep.what() << endl;

  }
  cout << "Couldnt find MC vertices!"
       << " (Event contains " << e.simVertices().size() << " MC vertices)." << endl;
  cout << endl;
}

void BTagPrinter::printFlightPath ( const Vertex & primrec,
    const SimVertex & primsim,
    const Vertex & bmesonrec,
    const SimVertex & bmesonsim ) const
{
  float mc_gamma = atof ( bmesonsim.tag().c_str() );
  cout << "MC gamma = " << mc_gamma << endl;
  float mcfp = getDistance ( primsim, bmesonsim );
  float mcfp_eigen = mcfp / mc_gamma;

  cout << "Found an associated b-meson vertex at " << bmesonrec.position() << endl;
  float mcdist = getDistance ( bmesonrec, bmesonsim );
  cout << "Distance ( MC b-meson, rec b-meson ) =	" << console.prettyPrint ( mcdist )
       << "m" << endl;
  float rfp = getDistance ( primrec, bmesonrec );
  cout << "Distance ( rec primary, rec b-meson ) =	" << console.prettyPrint ( rfp )
       << "m" << endl;
  float eigenpath = -1.;
  if ( finite(mc_gamma) && mc_gamma >= 1. )
  {
    eigenpath = rfp / mc_gamma;
  }
  cout << "Eigendistance ( rec primary, rec b-meson ) =	" << console.prettyPrint ( eigenpath )
       << "m" << endl;
  float delta = rfp-mcfp;
  cout << "Delta d, lab   =	" << console.prettyPrint ( delta ) << "m" << endl;
  float delta_eigen = eigenpath - mcfp_eigen;
  cout << "Delta d, eigen =	" << console.prettyPrint ( delta_eigen ) << "m" << endl;
}

void BTagPrinter::printRecoVertices ( const Event & e,
    const SimVertex & primary, const SimVertex & bmeson ) const
{
  // const vector < Vertex > & vtces = e.vertices();
  const vector < Vertex > & rec_prims = e.associatedVertices( primary );
  const vector < Vertex > & rec_bs = e.associatedVertices( bmeson );

  if ( rec_prims.size() )
  {
    cout << "Found an associated rec primary vertex at " << rec_prims[0].position() << endl;
    float dist = getDistance ( rec_prims[0], primary );
    cout << "        Distance ( rec primary, MC primary ) =	" << console.prettyPrint ( dist )
         << "m" << endl;
  } else {
    cout << "Found no associatable rec primary vertex!" << endl;
  }

  if ( rec_prims.size() )
  {
    if ( rec_bs.size() )
    {
      printFlightPath ( rec_prims[0], primary, rec_bs[0], bmeson );
    } else {
      cout <<  "Found no associatable rec b-meson vertex!" << endl;
      // describeFailure ( (rec_prims.size()>0), (rec_bs.size()>0) );
    }
  }
}

void BTagPrinter::printBFlightPath ( const SimVertex & primary,
    const SimVertex & bmeson ) const
{
  cout << "Primary SimVertex is at " << primary.position() << endl;
  cout << "BMeson  SimVertex is at " << bmeson.position() << endl;
  cout << "BMeson pid=" << bmeson.pid() << endl;
  float lab_d = getDistance ( primary, bmeson );
  cout << "      Distance ( MC primary, MC b-meson ) =	" << console.prettyPrint ( lab_d ) << "m" << endl;
  float gamma = atof ( bmeson.tag().c_str() );
  float eigen_d = -1.;
  if ( finite(gamma) && gamma >= 1. )
  {
    eigen_d = lab_d / gamma;
  }
  cout << " Eigendistance ( MC primary, MC b-meson ) =	" << console.prettyPrint (eigen_d) << "m" << endl;
}

void BTagPrinter::process ( const exception & e, const string & type )
{
  cout << "[BTagPrinter] Exception: " << e.what() << " type " << type << endl; 
}

void BTagPrinter::configure ( const string & conf )
{
}

BTagPrinter::~BTagPrinter() {}

BTagPrinter * BTagPrinter::clone() const
{
  return new BTagPrinter ( *this );
}

#include "Analysis/Observer/interface/ObserverBuilder.h"
namespace {
  ObserverBuilder<BTagPrinter> t( "BTagPrinter", "Prints info about rave's b-tags.", false );
}


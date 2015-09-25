#include "EventPrinter.h"
#include "ObjectPrinter.h"
#include <rave/impl/RaveBase/Converters/interface/RaveStreamers.h>
#include "Environment/Configuration/interface/SimpleConfigurable.h"
#include "Vertigo/Converters/interface/VertigoToCmsObjects.h"
#include "Vertigo/Event/interface/Event.h"
#include "Environment/MCObjects/interface/SimVertex.h"
#include "Environment/MCObjects/interface/PDGCodes.h"
#include "Environment/Utilities/interface/Console.h"

#include <vector>
#include <set>
#include <iostream>
#include <iomanip>

using namespace std;
using namespace vertigo;

/*
 * perigee: curv, theta, phi, tip, lip
 */

namespace {
  set < rave::Track > recTracks ( const SimVertex & vtx,
     const Event & e )
  {
    set < rave::Track > ret;
    vector < SimTrack > trks = vtx.tracks();
    for ( vector< SimTrack >::const_iterator j=trks.begin(); j!=trks.end() ; ++j )
    {
      vector < rave::Track > tmp  = e.associatedTracks ( *j );
      for ( vector< rave::Track >::const_iterator t=tmp.begin(); t!=tmp.end() ; ++t )
      {
        ret.insert ( *t );
      }
    }
    return ret;
  }

}

void EventPrinter::printRecVertex ( const rave::Vertex & vtx,
                     const Event & e, bool Short ) const
{
  Console console;
  cout << console.set ( Console::kRed )
       << "XX Vertex #" << vtx.id() << " at " << vtx.position()
       << " chi2=" << setprecision(1) << vtx.chiSquared() 
       << " ndf=" << vtx.ndf();
  const vector < pair < float, rave::Vertex > >  & comps = vtx.components();

  if ( comps.size() )
  {
    cout << " ncomps=" << comps.size();
  }

  cout << console.reset() << endl;

  set < rave::Track > simtrks;
  
  if ( vtx.hasRefittedTracks() )
  {
    cout << "    `-- has refitted tracks: ";
    cout << vtx.refittedTracks().size();
    cout << endl;
  }
  for ( vector< pair < float, rave::Vertex > >::const_iterator i=comps.begin(); 
        i!=comps.end() ; ++i )
  {
    cout << "     `-- component: " << i->first << ", " << i->second.position() << endl;
  }
  


  // float simtrkssize=simtrks.size();

  float mistagging=0.;
  int hard_mistagging=0;

  vector < pair < float, rave::Track > > wtrks = vtx.weightedTracks();

  if ( Short )
  {
    cout << console.set ( Console::kGreen )
         << wtrks.size() << " associated tracks."
         << console.reset () << endl;
  }

  for ( vector< pair < float, rave::Track > >::const_iterator j=wtrks.begin();
        j!=wtrks.end() ; ++j )
  {
    if ( simtrks.count ( j->second ) )
    {
      // this one is in
      simtrks.erase ( j->second );
      if ( !Short )
      {
        cout << console.set ( Console::kGreen );
        printRecTrack ( j->second, false, "", j->first, true );
        cout << console.reset () << endl;
      };
      mistagging+=fabs(1-j->first);
      if ( j->first < 0.5 ) hard_mistagging++;
    } else {
      mistagging+=j->first;
      if ( j->first > 0.5 ) hard_mistagging++;
      if ( !Short )
      {
        printRecTrack ( j->second, false, "", j->first, true );
        cout << endl;
      }
    }

  }

  if ( simtrks.size() )
  {
    cout << console.set ( Console::kBrown );
    for ( set < rave::Track >::const_iterator i=simtrks.begin(); i!=simtrks.end() ; ++i )
    {
      if ( !Short )
      {
        printRecTrack ( *i, false, " missing!" );
        cout << endl;
      };
      mistagging+=1.;
      hard_mistagging++;
    }
    console.reset();
  }
  if (!Short)
  {
    cout << "     -- misassociated tracks " << mistagging << " (hard: "
         << hard_mistagging << ")"
         << endl;
  };
}

  
void EventPrinter::printRecVertex ( const rave::Vertex & vtx,
                     const SimVertex & pr,
                     const vector < SimVertex > & ass,
                     const Event & e, bool Short ) const
{
  Console console;
  cout << console.set ( Console::kRed )
       << "Vertex #" << vtx.id() << " at " << vtx.position()
       << " chi2=" << setprecision(1) << vtx.chiSquared() 
       << " ndf=" << vtx.ndf();
  const vector < pair < float, rave::Vertex > >  & comps = vtx.components();

  if ( comps.size() )
  {
    cout << " ncomps=" << comps.size();
  }
  if ( vtx.id() != pr.id() )
  {
    rave::Vector3D diff = vtx.position() - pr.position();
    cout << " [" << console.prettyPrint ( diff.mag() ) << "m]";
  }

  cout << console.reset() << endl;

  set < rave::Track > simtrks;
  if ( ass.size() )
  {
    rave::Vector3D diff = vtx.position()-ass[0].position();
    // rave::Vector3D diff = vtx.position();
    // diff -= ass[0].position();
    float d2=-1.;
    try {
      VertigoToCmsObjects converter;
      rave::Covariance3D cov = vtx.error();
      AlgebraicVector vdiff = converter.convert ( diff );
      int fail=0;
      AlgebraicSymMatrix vcov = converter.convert ( cov );
      vcov.invert ( fail );
      if ( !fail )
      {
        d2 = sqrt ( vcov.similarity ( vdiff ) );
      }
    } catch ( ... ) {};
    cout << "    `-- associated with SimVertex #" << ass[0].id()
         << " [" << console.prettyPrint ( diff.mag() ) << "m, " << d2 << "s off]"
         << endl;
    simtrks=recTracks ( ass[0], e );
  }
  
  if ( vtx.hasRefittedTracks() )
  {
    cout << "    `-- has refitted tracks: ";
    cout << vtx.refittedTracks().size();
    cout << endl;
    if ( (!Short) && ass.size() ) printRefittedTracks ( vtx, e );
  }
  for ( vector< pair < float, rave::Vertex > >::const_iterator i=comps.begin(); 
        i!=comps.end() ; ++i )
  {
    cout << "     `-- component: " << i->first << ", " << i->second.position() << endl;
  }
  


  // float simtrkssize=simtrks.size();

  float mistagging=0.;
  int hard_mistagging=0;

  vector < pair < float, rave::Track > > wtrks = vtx.weightedTracks();

  if ( Short )
  {
    cout << console.set ( Console::kGreen )
         << wtrks.size() << " associated tracks."
         << console.reset () << endl;
  }

  for ( vector< pair < float, rave::Track > >::const_iterator j=wtrks.begin();
        j!=wtrks.end() ; ++j )
  {
    if ( simtrks.count ( j->second ) )
    {
      // this one is in
      simtrks.erase ( j->second );
      if ( !Short )
      {
        cout << console.set ( Console::kGreen );
        printRecTrack ( j->second, false, "", j->first, true );
        cout << console.reset () << endl;
      };
      mistagging+=fabs(1-j->first);
      if ( j->first < 0.5 ) hard_mistagging++;
    } else {
      mistagging+=j->first;
      if ( j->first > 0.5 ) hard_mistagging++;
      if ( !Short )
      {
        printRecTrack ( j->second, false, "", j->first, true );
        cout << endl;
      }
    }

  }

  if ( simtrks.size() )
  {
    cout << console.set ( Console::kBrown );
    for ( set < rave::Track >::const_iterator i=simtrks.begin(); i!=simtrks.end() ; ++i )
    {
      if ( !Short )
      {
        printRecTrack ( *i, false, " missing!" );
        cout << endl;
      };
      mistagging+=1.;
      hard_mistagging++;
    }
    console.reset();
  }
  if (!Short)
  {
    cout << "     -- misassociated tracks " << mistagging << " (hard: "
         << hard_mistagging << ")"
         << endl;
  };
}

vector < rave::Track > EventPrinter::printSimVertex ( const SimVertex & vtx,
                const SimVertex & pr, const Event & e, bool Short ) const
{
  Console console;
  vector < rave::Track > printed_rectracks;
  cout << console.set ( Console::kBlue, Console::kHilight )
       << "SimVertex #" << vtx.id();

  if ( vtx.isPrimary() ) cout << " <primary>";
  if ( vtx.tag().size() )
  {
    cout << " \"" << vtx.tag() << "\" ";
  }

  if ( vtx.pid() )
  {
    cout << " " << PDGCodes::name ( vtx.pid() );
  }
  cout << " at " << vtx.position();
  if ( vtx.id() != pr.id() )
  {
    rave::Vector3D diff = vtx.position() - pr.position();
    cout << " [" << console.prettyPrint ( diff.mag() ) << "m]";
  }

  if ( e.isReconstructible ( vtx ) )
  {
    cout << "  (rectible)";
  }
  cout << console.reset() << endl;
  vector < SimTrack > trks = vtx.tracks();
  for ( vector< SimTrack >::const_iterator j=trks.begin(); j!=trks.end() ; ++j )
  {
    vector < rave::Track > assrtrks = e.associatedTracks ( *j );
    if  (!Short )
    {
      vector < rave::Track > tmp  = printSimTrack ( *j, assrtrks );
      for ( vector< rave::Track >::const_iterator t=tmp.begin(); t!=tmp.end() ; ++t )
      {
        printed_rectracks.push_back ( *t );
      }
    }
  }
  return printed_rectracks;
}

void EventPrinter::printRecTrack ( const rave::Track & trk, bool print_params,
    const string & comment, float weight, bool print_weight ) const
{
    ObjectPrinter::print ( trk, printEuclidean, printPerigee, "    `-- Track #", comment, weight, print_weight );
    // ObjectPrinter::printMultiComponent ( trk, printEuclidean, printPerigee, "    `-- Track #", comment, weight, print_weight );
}

void EventPrinter::printRefittedRecTrack ( const rave::Track & trk, const SimTrack & sim,
    bool print_params, const string & comment, float weight, bool print_weight ) const
{
  ObjectPrinter::print ( trk, printEuclidean, printPerigee, "    `-- Refitted #", comment, weight, print_weight );
  cout << " simid=" << sim.id() << endl;
}

void EventPrinter::printKinematicParticle ( const rave::KinematicParticle & p, bool print_params,
                                  const string & comment, float weight, bool print_weight ) const
{
  ObjectPrinter::print ( p, printEuclidean, printPerigee, "    `-- Particle #", comment, weight, print_weight );
}

vector < rave::Track > EventPrinter::printSimTrack ( const SimTrack & trk,
   const vector < rave::Track > & iids ) const
{
    ObjectPrinter::print ( trk, printEuclidean, printPerigee );
    cout << endl;
    vector < rave::Track > printed_rectracks;
    for ( vector < rave::Track >::const_iterator i=iids.begin(); i!=iids.end() ; ++i )
    {
      ObjectPrinter::print ( *i, printEuclidean, printPerigee,  "           Track #" );
      cout << endl;
      printed_rectracks.push_back ( *i );
    }
    return printed_rectracks;
}

/// Give us the refitted tracks.
void EventPrinter::printRefittedTracks ( const rave::Vertex & vtx, const Event & e ) const
{
  vector < rave::Track > refits = vtx.refittedTracks();
  
  vector < pair < float, rave::Track > > otracks = vtx.weightedTracks();
  vector < pair < float, rave::Track > >::const_iterator oit=otracks.begin();

  for ( vector< rave::Track >::const_iterator i=refits.begin(); 
        i!=refits.end() ; ++i )
  {
    vector < SimTrack > ass = e.associatedTracks ( oit->second );
    if ( ass.size() )
    {
      printRefittedRecTrack ( *i, ass[0], true, "", oit->first, true );
    }
    oit++;
  }
}


void EventPrinter::printJets ( const Event & e ) const
{
  Console console;
  cout << endl;
  vector < Jet > jets = e.jets();

  for ( vector< Jet >::const_iterator i=jets.begin(); i!=jets.end() ; ++i )
  {
    rave::Track ghost = i->ghost();
    cout << "Jetaxis ( " << ghost << endl;

    cout << console.set ( Console::kRed ) << "Jet " << i->id() << " ("
         << i->getMCTag() << "/" << i->getMCPhysicsTag() << "): " << console.reset();
    vector < rave::Track > trks = i->tracks();
    for ( vector< rave::Track >::const_iterator t=trks.begin(); t!=trks.end() ; ++t )
    {
      cout << t->id() << " ";
    }

    vector < rave::Vertex > vertices = i->vertices();
    if ( vertices.size() )
    {
      cout << "  Vtces ";
      for ( vector< rave::Vertex >::const_iterator v=vertices.begin(); v!=vertices.end() ; ++v )
      {
        cout << v->id() << " ";
      }
    }
    cout << endl;
  }
}

void EventPrinter::printParticles ( const Event & e ) const
{
  Console console;
  cout << endl;
  vector < SimParticle > sps = e.simParticles();
  for ( vector< SimParticle >::const_iterator sp = sps.begin(); sp != sps.end() ; ++sp )
  {
    cout << console.set ( Console::kRed ) << "SimParticle " << std::setw(3) << sp->id();
    if ( sp->tag().size() ) 
      cout << " (" << sp->tag() << ")";
    cout << ": " << setprecision ( 3 ) 
         << sp->state() << " [" << PDGCodes::name ( sp->pid() ) 
         << "]" << console.reset();
    vector < rave::KinematicParticle > ps = e.associatedParticles( *sp );
    for ( vector< rave::KinematicParticle >::const_iterator p = ps.begin(); p != ps.end() ; ++p )
    {
      cout << " -> Particle " << p->id() << ", ";
    }
    cout << endl;
  }
  vector < rave::KinematicParticle > ps = e.particles();
  for ( vector< rave::KinematicParticle >::const_iterator p = ps.begin(); p != ps.end() ; ++p )
  {
    cout << console.set ( Console::kRed ) << "Particle " << p->id() << " ("
        << p->tag() << "): " << p->fullstate() << console.reset();
    vector < SimParticle > sps = e.associatedParticles( *p );
    for ( vector< SimParticle >::const_iterator sp = sps.begin(); sp != sps.end() ; ++sp )
    {
      cout << " -> SimParticle " << sp->id() << ", ";
    }
    cout << endl;
  }
}

EventPrinter::~EventPrinter()
{}

EventPrinter::EventPrinter() : theShort ( false ),
printEuclidean (true), printPerigee ( false ) {}

EventPrinter * EventPrinter::clone() const
{
  return new EventPrinter ( *this );
}

void EventPrinter::configure( const string & s )
{
  if ( s=="short" )
  {
    theShort=true;
  }
}

void EventPrinter::process ( const exception & e, const string & type )
{
  cout << endl
       << "-----------------------------------------" << endl
       << "[EventPrinter] Exception: " << e.what() << " of type " << type << endl
       << "-----------------------------------------" << endl
       << endl;
}

void EventPrinter::printHeader( const Event & e ) const
{
  Console console;
  cout << endl << console.spaces(14)
       << console.set ( Console::kGreen )
       << "Rave Event  " << e.id();
  if ( e.tag().size() && e.tag()!="undef" )
  {
    cout << " (" << e.tag() << ")";
  }
  cout << "  [" << setprecision(1) << floor( e.time()*1000 ) << " ms]";
  cout << endl << console.spaces(5)
       << console.topSeparator( Console::kGreen, 58 )
       << console.set ( Console::kWhite, Console::kNormal )
       << endl;

  int njettracks=0;
  const vector< Jet > & jets=e.jets();
  for ( vector< Jet >::const_iterator i=jets.begin(); i!=jets.end() ; ++i )
  {
    njettracks+=i->tracks().size();
  }

  cout << "Contains: " << e.simTracks().size() << " simtracks, "
       << e.simVertices().size() << " simvertices, "
       << e.simParticles().size() << " simparticles, "
       << e.tracks().size() << " tracks, "
       << e.vertices().size() << " vertices, " << endl
       << "          " << e.particles().size() << " particles, "
       << jets.size() << " jets (containing a total of " << njettracks << " tracks)."
       << endl << endl;
}

void EventPrinter::process ( const Event & e )
{
  Console console;
  theScore ( e );
  printHeader ( e );
  if ( e.jets().size() )
  {
    printJets(e);
  }
  vector < SimVertex > simvtces = e.simVertices();
  if ( simvtces.size() == 0 )
  {
    cout << "[EventPrinter] no sim vertices defined!" << endl;
    cout << endl;
    vector < rave::Track > trks = e.tracks();
    if ( theShort )
    {
      cout << "[EventPrinter] " << trks.size() << " RecTracks, " << e.vertices().size() << " RecVertices." << endl;
    } else {
      for ( vector < rave::Track >::const_iterator i = trks.begin(); i!=trks.end(); i++ )
      {
         printRecTrack ( *i );
         cout << endl;
      }
    }
  };

  set < rave::Track > printed_rectracks;

  vector< SimVertex >::const_iterator last = simvtces.end();

  // static const int vtx_max = 10;
  static const unsigned vtx_max = SimpleConfigurable<int>(10,"EventPrinter:MaxNVtces").value();
  if ( simvtces.size() > vtx_max )
  {
    last = simvtces.begin() + vtx_max-1;
  }

  vector < SimVertex >::const_iterator primary = simvtces.begin();

  for ( vector< SimVertex >::const_iterator i=simvtces.begin();
        i!=last ; ++i )
  {
    if ( i->isPrimary() ) primary = i;
  }

  for ( vector< SimVertex >::const_iterator i=simvtces.begin();
        i!=last ; ++i )

  {
    vector < rave::Track > tmp = printSimVertex ( *i, *(primary), e, theShort );
    for ( vector< rave::Track >::const_iterator t=tmp.begin(); t!=tmp.end() ; ++t )
    {
      printed_rectracks.insert ( *t );
      // printed_rectracks.push_back ( *t );
    }
  }

  if ( simvtces.size() > vtx_max )
  {
    cout << console.set ( Console::kBlue, Console::kHilight ) << "..." 
         << console.reset() << endl;
  }

  if (!theShort)
  {
    cout << endl
         << console.set ( Console::kBlue, Console::kHilight )
         << "Unassociated RecTracks:" << console.reset() << endl;
    // now print remaining RecTracks
    vector < rave::Track > rectracks=e.tracks();
    for ( vector < rave::Track >::const_iterator i=rectracks.begin();
          i!=rectracks.end() ; ++i )

    {
      if ( printed_rectracks.find ( *i )==printed_rectracks.end() )
      {
        printRecTrack ( *i );
        cout << endl;
      }
    }
  }

  if ( false )
  {
    cout << endl
         << console.set ( Console::kBlue, Console::kHilight )
         << "All SimTracks:" << console.reset() << endl;
    // now print remaining RecTracks
    vector < SimTrack > simtracks=e.simTracks();
    for ( vector < SimTrack >::const_iterator i=simtracks.begin();
          i!=simtracks.end() ; ++i )

    {
      if ( true ) // printed_rectracks.find ( *i )==printed_rectracks.end() )
      {
        printSimTrack ( *i, vector < rave::Track > () );
        // cout << endl;
      }
    }
  }

  // printJets ( e );
  printParticles ( e );

  cout << endl
       << console.set ( Console::kRed )
       << "Reconstructed vertices:" << endl
       << "-----------------------" << endl
       << console.reset() << endl;

  vector < rave::Vertex > vtces=e.vertices();
  if ( vtces.size() == 0 )
  {
    cout << "   none defined." << endl;
  } else {
    for ( vector< rave::Vertex >::const_iterator i=vtces.begin();
          i!=vtces.end() ; ++i )
    {
      vector < SimVertex > sims = e.associatedVertices ( *i );
      if ( simvtces.size() )
      {
        printRecVertex ( *i, *(primary), sims, e, theShort );
      } else {
        printRecVertex ( *i, e, theShort );
      }
    }
  }

  printJetVertices ( e, *primary );

  cout << console.reset () << endl;

  bool score = SimpleConfigurable<bool> (false,"EventPrinter:PrintScore").value();
  if (score)
  {
    cout << endl
         << "[SimpleScore] eff=" << setprecision(1)
         << theScore.efficiency().value() << " +/- " 
         << theScore.efficiency().error() << ", fake="
         << theScore.fake().value() << " +/- "
         << theScore.fake().error() << endl;
  }
}

void EventPrinter::printJetVertices ( const Event & e, const SimVertex & primary ) const
{
  vector < Jet > jets = e.jets();
  Console console;
  vector < SimVertex > simvtces = e.simVertices();
  bool once=true;
  for ( vector< Jet >::const_iterator i=jets.begin(); i!=jets.end() ; ++i )
  {
    vector < rave::Vertex > vtces = i->vertices();
    if ( vtces.size() && once )
    {
      cout << endl
           << console.set ( Console::kRed )
           << "Per-Jet Vertices:" << endl
           << "-----------------" << endl;
      once=false;
    }
    for ( vector< rave::Vertex >::const_iterator v=vtces.begin();
          v!=vtces.end() ; ++v )
    {
      cout << console.reset() << "In Jet " << i->id() << ": ";
      vector < SimVertex > sims = e.associatedVertices ( *v );
      printRecVertex ( *v, primary, sims, e, theShort );
    }
  }
}

#include "Analysis/Observer/interface/ObserverBuilder.h"
namespace {
  ObserverBuilder<EventPrinter> t( "EventPrinter", "Prints a fancy event description to stdout", false );
}

#include "Vertigo/EventLoop/interface/LoopThread.h"
#include "Vertigo/EventLoop/interface/LoopSettings.h"
#include "Analysis/Observer/interface/ObserverManager.h"
#include <dataharvester/Timer.h>
#include "Environment/Configuration/interface/SimpleConfigurable.h"
#include <rave/impl/RaveBase/RaveEngine/interface/RaveId.h>
#include <rave/impl/RaveBase/Converters/interface/RaveStreamers.h>
#include <rave/impl/FWCore/MessageLogger/interface/MessageLogger.h>
#include <rave/impl/FWCore/Utilities/interface/Exception.h>
#include <rave/impl/RecoVertex/VertexPrimitives/interface/VertexException.h>
#include <iostream>
#include <climits>
#include <sstream>
#include <stdexcept>

using namespace std;

namespace {
  int timer( int n )
  {
    if (n<1) return 0;
    static int cum = 0;
    static time_t last = time ( 0 );
    time_t cur = time ( 0 );
    cum += cur;
    cum -= last;
    // int spEv = int ( floor ( (float) cum / (float) (n-1) * 10000 ) / 10. );
    int spEv = cum * 1000 / n;
    last = cur;
    return spEv;
  }

  string pretty_print_ms ( int ms )
  {
    // pretty print milliseconds
    string unit="ms";
    int n=ms;
    if ( n > 5000 ) // > 5 seconds?
    {
      n=ms/1000;
      unit="s";
      // seconds
    }

    if ( n > 5*60*1000 ) // > 5 minutes?
    {
      n=ms/60000;
      unit="m";
    }

    if ( n > 5*60*60*1000 ) // > 5 hours?
    {
      n=ms/(60*60*1000);
      unit="h";
    }

    ostringstream o;
    o << n << " " << unit;
    return o.str();
  }
  
  /*
  rave::Vector3D sum ( const vector < rave::Track > & t )
  {
    double px=0.;
    double py=0.;
    double pz=0.;
    for ( vector< rave::Track >::const_iterator i=t.begin(); i!=t.end() ; ++i )
    {
      px+=i->state().momentum().x();
      py+=i->state().momentum().y();
      pz+=i->state().momentum().z();
    }
    return rave::Vector3D ( px, py, pz );
  }*/

  void fitVerticesWithAllTracks (  vertigo::Event & e, int n, int spev )
  {
    // cout << "LoopThread anfang methode fitVerticesWithAllTracks!" << endl;
    /*
     * use rave factory to fit vertices.
     * write result back into event object
     */
    vector < rave::Track > trks = e.tracks();
    /*
    if ( LoopSettings::Instance().verbosity() > 1 )
    {
      cout << endl
          << "[LoopThread] read event #" << n << " [id:" << e.id();
      if ( e.tag()!="undef" ) cout << "; " << e.tag();

      cout << "] (" << trks.size() << " trks)   [";
      if ( spev )
      {
        cout << spev;
      } else {
        cout << "???";
      };
      cout << " ms/ev]" << endl;
    }*/

    double t=0.;
    vector < rave::Vertex > vtces;

    if ( LoopSettings::Instance().method()!="none" )
    {
      dataharvester::Timer::start_counter();
      rave::Track ghost_track;
      // cout << "LoopThread after defining dummy ghost track" << endl;
      vtces = LoopSettings::Instance().vertexFactory()->create ( trks, LoopSettings::Instance().useBeamSpot() );
      // cout << "LoopThread after calling function vtces" << endl;
      /*
      std::vector < vertigo::Jet > jets(e.jets());
      cout << "LoopThreaad before defining ghost track!" << endl;
      for ( vector< vertigo::Jet >::const_iterator i=jets.begin(); i!=jets.end() ; ++i)
	{
	  cout << "inside loop" << endl;
	  rave::Track ghost_track (i->ghost());
	  vtces = LoopSettings::Instance().vertexFactory()->create ( trks, ghost_track, LoopSettings::Instance().useBeamSpot() );
	  cout << "inside loop: iterator i =" << i->ghost() << endl;
	}
      cout << "LoopThread after calling function vtces" << endl;
      */
      t=dataharvester::Timer::lap();
    }
    // cout << "Loop Thread after if" << endl;
    if ( t < 0. )
    {
      edm::LogWarning ("LoopThread") << "Error, negative time: " << t;
      t=0.;
    }

    // cout << "[LoopThread] factory returned " << vtces.size() << " vertices." << endl;
    for ( vector< rave::Vertex >::const_iterator i=vtces.begin();
          i!=vtces.end() ; ++i )
    {
      e.add ( *i );
    }

    e.setTime(t);
    // cout << "LoopThread ende von methode fitVerticesWithAllTracks" << endl;
  }

  void fitPerJetVertices ( vertigo::Event & e, int n, int spev )
  {
    /*
     * Fit Vertices Jet per Jet.
     * FIXME remove duplicates _everywhere_!!!
     */
    vector < vertigo::Jet > & jets = e.jetRefs();
    // cout << "LoopThread anfang methode fitPerJetVertices" << endl;
    for ( vector< vertigo::Jet >::iterator i=jets.begin(); i!=jets.end() ; ++i )
    {
      bool ghosthaserror = i->ghostHasError();

      if (ghosthaserror)
      {
	vector < rave::Track > jettracks = i->tracks();
	rave::Track ghost_track = i->ghost();
	// collect all non_secondary tracks
	set < rave::Track > non_secondaries;
	for ( vector< vertigo::Jet >::iterator j=jets.begin(); j!=jets.end() ; ++j )
	  {
	    // non_secondaries.reserve ( non_secondaries.size() + j->tracks().size()+1 );
	    if ( i!=j )
	      {
		vector < rave::Track > trks = j->tracks();
		// copy ( trks.begin(), trks.end(), inserter ( non_secondaries, non_secondaries.begin() ) );
		for ( vector< rave::Track >::const_iterator i=trks.begin(); i!=trks.end() ; ++i )
		  {
		    // if it's not yet in jettracks, we add it here!
		    if ( find ( jettracks.begin(), jettracks.end(), *i ) == jettracks.end() )
		      {
			non_secondaries.insert ( *i );
		      }
		  }
	      }
	  }

	// cout << "[LoopThread] " << non_secondaries.size() << " non-secs!" << endl;

	vector < rave::Track > non_secs;
	non_secs.reserve ( non_secondaries.size() );
	copy ( non_secondaries.begin(), non_secondaries.end(), back_inserter ( non_secs ) );
      
	double t=0.;
	vector < rave::Vertex > vtces;

	if ( LoopSettings::Instance().method()!="none" && jettracks.size() )
	  {
      dataharvester::Timer::start_counter();
	    vtces = LoopSettings::Instance().vertexFactory()->create ( non_secs, jettracks, LoopSettings::Instance().useBeamSpot() );
	    t=dataharvester::Timer::lap();
	  }
	if ( t < 0. )
	  {
	    cout << "[LoopThread] Error, negative time: " << t << endl;
	    t=0.;
	  }

	// cout << "[LoopThread::fitPerJet] factory returned " << vtces.size() << " vertices." << endl;
	for ( vector< rave::Vertex >::const_iterator v=vtces.begin();
            v!=vtces.end() ; ++v )
	  {
	    i->add ( *v );
	  }

	i->setTime(t);

      }
      else
      {
	vector < rave::Track > jettracks = i->tracks();
      
	// collect all non_secondary tracks
	set < rave::Track > non_secondaries;
	for ( vector< vertigo::Jet >::iterator j=jets.begin(); j!=jets.end() ; ++j )
	  {
	    // non_secondaries.reserve ( non_secondaries.size() + j->tracks().size()+1 );
	    if ( i!=j )
	      {
		vector < rave::Track > trks = j->tracks();
		// copy ( trks.begin(), trks.end(), inserter ( non_secondaries, non_secondaries.begin() ) );
		for ( vector< rave::Track >::const_iterator i=trks.begin(); i!=trks.end() ; ++i )
		  {
		    // if it's not yet in jettracks, we add it here!
		    if ( find ( jettracks.begin(), jettracks.end(), *i ) == jettracks.end() )
		      {
			non_secondaries.insert ( *i );
		      }
		  }
	      }
	  }

	// cout << "[LoopThread] " << non_secondaries.size() << " non-secs!" << endl;

	vector < rave::Track > non_secs;
	non_secs.reserve ( non_secondaries.size() );
	copy ( non_secondaries.begin(), non_secondaries.end(), back_inserter ( non_secs ) );
      
	double t=0.;
	vector < rave::Vertex > vtces;

	if ( LoopSettings::Instance().method()!="none" && jettracks.size() )
	  {
      dataharvester::Timer::start_counter();
	    vtces = LoopSettings::Instance().vertexFactory()->create ( non_secs, jettracks, LoopSettings::Instance().useBeamSpot() );
	    t=dataharvester::Timer::lap();
	  }
	if ( t < 0. )
	  {
	    cout << "[LoopThread] Error, negative time: " << t << endl;
	    t=0.;
	  }

	// cout << "[LoopThread::fitPerJet] factory returned " << vtces.size() << " vertices." << endl;
	for ( vector< rave::Vertex >::const_iterator v=vtces.begin();
            v!=vtces.end() ; ++v )
	  {
	    i->add ( *v );
	  }

	i->setTime(t);
      }
    }
  }

  void fitVertices( vertigo::Event & e, int n, int spev )
  {
    bool fit_all = SimpleConfigurable<bool>(true,"LoopThread:FitWithAllTracks").value();
    if ( fit_all ) fitVerticesWithAllTracks ( e, n, spev );
    bool fit_per_jet = SimpleConfigurable<bool>(false,"LoopThread:FitPerJet").value();
    // cout << "[LoopThread] fitVertices!" << endl;
    if ( fit_per_jet ) fitPerJetVertices ( e, n, spev );
  }
}


LoopThread::LoopThread ( int first, signed n, bool has_method ) :
  theFirst ( first ), theN ( n ), theHasMethod ( has_method ),
  theLastMark(0), theStart ( time(0) ), theMarker('|')
{ }

LoopThread::LoopThread ( ) : theFirst ( 0 ), theN ( 0 ),
  theHasMethod ( false ), theLastMark(0),
  theStart( time(0)), theMarker('|')
{ }

void LoopThread::nextMarker()
{
  switch ( theMarker )
  {
    case '|': { theMarker='/'; break; };
    case '/': { theMarker='-'; break; };
    case '-': { theMarker='\\'; break; };
    default:
      theMarker='|';
  }
}

void LoopThread::operator() ()
{
  for ( int i=1; i<= theN || theN == -1 ; i++ )
  {
    // cout << "[LoopThread] looping: " << i << endl;
    bool hasevent = processEvent ( theFirst + i );
    if (!hasevent )
    {
      if ( LoopSettings::Instance().verbosity()>0 )
        edm::LogInfo ( "LoopThread" ) << "No more events.";
      break;
    }
  }
}

bool LoopThread::processEvent( int n )
{
  // if ( LoopSettings::Instance().verbosity() > 20 )
  edm::LogInfo("LoopThread") << "Start with event " << n;
  vertigo::Event e;
  try {
    RaveId::reset();

    int spev=timer( n - LoopSettings::Instance().min() );

    try {
      e = LoopSettings::Instance().eventFactory().next();

      if ( theFirst == 0 && n == 1 )
      {
        const rave::Ellipsoid3D & spot = LoopSettings::Instance().vertexFactory()->getBeamspot();
        if ( spot.isValid() )
        {
          edm::LogInfo ( "LoopThread" ) << "Beamspot is at (" << spot.point().x() << "+/-"
            << sqrt( spot.covariance().dxx()) << ", " << spot.point().y() << "+/-"
            << sqrt( spot.covariance().dyy()) << ", " << spot.point().z() << "+/-"
            << sqrt( spot.covariance().dzz()) << ") [cm]";
        } else {
          edm::LogInfo ( "LoopThread" ) << "no beamspot constraint defined.";
        }
      }
    } catch (...) {
      // no next event?
      return false;
    }
    if ( theHasMethod )
    {
      fitVertices ( e, n, spev );
    }

    if ( LoopSettings::Instance().performFlavorTagging () )
    {
      tagJets ( e, n, spev );
    }

    e.associateVertices();
    if ( LoopSettings::Instance().vertexFactory() ) 
      e.setAlgorithm ( LoopSettings::Instance().vertexFactory()->method() );

    // now analyse
    ObserverManager::Instance().process( e );
  }
  catch ( VertexException & v ) {
      edm::LogError ("LoopThread" ) << "EXCEPTION CAUGHT: " << v.what();
    ObserverManager::Instance().process ( v, "VertexException" );
  } catch ( cms::Exception & e ) {
    edm::LogError ( "LoopThread" ) << "EXCEPTION CAUGHT: cms::Exception " << e.what();
    ObserverManager::Instance().process ( runtime_error ( e.what() ), "CmsException" );
  } catch ( exception & e ) {
    edm::LogError ( "LoopThread" ) << "EXCEPTION CAUGHT: " << e.what();
    ObserverManager::Instance().process ( e, "std" );
  }

  mark ( n );

  // delete simtracks -> simvertex references, so that
  // reference counting can work.
  e.unReference();
  return true;
}

void LoopThread::mark ( int n )
{
  bool show_marks = ( LoopSettings::Instance().verbosity()==1 );
  if ( LoopSettings::Instance().verbosity() > 0 && LoopSettings::Instance().logToFile () )
  {
    show_marks=true;
  }

  // static bool simple = SimpleConfigurable<bool>(false,"LoopThread:ForceShowMarks").value();
  // if ( simple ) show_marks = true;
  if ( !show_marks ) return;
  // write a "." or a "-" to cout
  signed myN=theN;
  if ( myN == -1 || myN == INT_MAX ) myN=LoopSettings::Instance().expect();
  bool fancy_counter = LoopSettings::Instance().fancyCounter();
  if ( myN == -1 || myN == 0 ) fancy_counter=false; //have to turn off fancy counter
  // if we dont know what to expec
  if ( fancy_counter )
  {
    // advance every ntot / 50 events
    float every = myN / float(LoopSettings::Instance().barLength() );
    if ( n > theLastMark * every )
    {
      time_t t_t =time ( 0 );
      int delta = t_t - theStart;
      // int pos = LoopSettings::Instance().barLength()+10;
      int eta=int ( delta * ( float(myN-n) / float(n)) );
      cout << "\033[1G\033[0;32m[\033[;42;30m";
      for ( int i=0; i< theLastMark ; i++ )
      {
        cout << " ";
      }
      cout << "\033[s\033[0;32m";
      for ( int i=theLastMark; i< LoopSettings::Instance().barLength(); i++ )
      {
        cout << ".";
      }
      cout << "]\033[;0m           ";
      /*
      cout << "\033[;42;30m";
      if ( theLastMark > 0 ) cout << " ";
      cout << theMarker << "\033[;0m\033[s\033[" << pos << "G ";
      */
      if ( theLastMark > 1 ) cout << "ETA " << eta << " secs ";
      cout << "\033[u" << flush;
      theLastMark++;
    }
    // every second event changer marker!
    if ( n>0 /* && ( fmod ( float(n) , 2 ) == 0 ) */ ) {
      nextMarker();
      cout << "\033[s\033[;42;30m" << theMarker << "\033[;0m\033[u" << flush;
    }
  } else {
    if ( fmod ( (float) n, 1000 ) == 0 ) {
      cout << (int) n / 1000 << flush;
    } else if ( fmod ( (float) n, 100 ) == 0 ) {
      cout << "-" << flush;
    } else {
      cout << "." << flush;
    }
  }
}

void LoopThread::tagJets( vertigo::Event & e, int n, int spev )
{
  /*
   * use rave factory to tag jets
   * FIXME currently it's not jet-by-jet
   */
  vector < vertigo::Jet > jets = e.jets();
  // get primary vertex, get jet axis. FIXME
  /*
  if ( e.vertices().size()==0 )
  {
    edm::LogError ( "LoopThread" ) << "no vertex in event - cannot b-tag!";
    return;
  }
  rave::Vertex pv = e.vertices()[0];
   */

  for ( vector< vertigo::Jet >::iterator i=jets.begin(); i!=jets.end() ; ++i )
  {
    float tag=-1.;
    float t=0.;

    if ( i->vertices().size() > 0 )
    {
      rave::Vertex pv = i->vertices()[0];
      dataharvester::Timer::start_counter();
      tag = LoopSettings::Instance().flavorTagFactory()->tag ( i->tracks(), pv, i->momentum() );
      // double tag = LoopSettings::Instance().flavorTagFactory()->tag ( i->tracks() );
      t=dataharvester::Timer::lap();
      i->setTime(t);
      LogDebug ("LoopThread") << "Jet " << i->id() << " b-tag=" << tag << "  t=" << t;
    }
    e.tag ( *i, tag ); // make sure jet tag is in the event as well
  };
}



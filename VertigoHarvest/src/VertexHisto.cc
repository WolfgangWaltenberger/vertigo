#ifdef WITH_HARVESTER
#include <rave/impl/RaveBase/Converters/interface/RaveStreamers.h>
#include "Environment/MCObjects/interface/AssociationManager.h"
#include "Environment/MCObjects/interface/SimTrack.h"
#include "Vertigo/EventLoop/interface/EventLoop.h"
#include "VertigoHarvest/interface/VertexHisto.h"
#include "VertigoHarvest/interface/ObserverTuples.h"

#include <dataharvester/Writer.h>
#include <dataharvester/SystemWriter.h>
#include <sstream>

using namespace dataharvester;

using namespace std;

namespace {
  bool checkFiniteness ( const rave::Vertex & r )
  {
    bool ret  = finite ( r.position().x() ) && finite ( r.position().z() );
    /*
    if (!ret)
    {
      cout << "[VertexHisto] huh? non-finite result?" << endl;
      cout << "   sim=" << s.position() << "   ret=" << r.position() << endl;
    }*/
    return ret;
  }

  int isTrackInVertex ( const vertigo::SimTrack & s , const vertigo::SimVertex & vtx )
  {
    vector < vertigo::SimTrack > trks = vtx.tracks();
    for ( vector< vertigo::SimTrack >::const_iterator
          i=trks.begin(); i!=trks.end() ; ++i )
    {
      if ( s.id() == (*i).id() ) return 1;
    };
    return 0;
  }

  struct Sorter
  {
    bool operator() ( const vertigo::SimTrack & s1, const vertigo::SimTrack & s2 )
    {
      return ( s1.id() < s2.id() );
    };
  };

  void hasSimTrack ( const rave::Track & vtrk, const vector < vertigo::SimTrack > & lstrks,
                     const vertigo::SimVertex & sv, Tuple & mp,
                     map < const vertigo::SimTrack, bool, Sorter > & takenIntoAccount )
  {
    // if we find an associatable simtrack, 
    // then we store the simtrackid, and simweight=1.
    // else simtrackid=-1, simweight=0
    mp["simtrackid"]=-1;
    mp["simtrackpid"]=-1;
    mp["simweight"]=0.;
    // RecTrack rtrk = trkFromVertexTrack ( vtrk );
    // vector < vertigo::SimTrack > allstrks = sv.tracks();
    // ConfigurableTrackAssociator ass ( allstrks );
    if ( lstrks.size() )
    {
      vertigo::SimTrack front=lstrks[0];
      mp["simweight"]=(float) (isTrackInVertex ( front, sv ));
      mp["simtrackid"]=front.id();
      mp["simtrackpid"]=front.pid();
      takenIntoAccount[front]=true;
    }
    /*
    vector < const vertigo::SimTrack * > strks = man.simTracks ( rtrk );
    if ( strks.size() > 0 )
    {
      const vertigo::SimTrack * strk = *(strks.begin());
      if ( strk )
      {
        mp["simweight"]=(float) (isTrackInVertex ( *strk, sv ));
        mp["simtrackid"]=strk->id();
        takenIntoAccount[front]=true;
      };
    };*/
  }
}

VertexHisto::VertexHisto( const std::string & str ) :
  theFileName ( str  ), theNEvents ( 0 ), theNSimEvents(0),
  hasStamped(false)
{}

void VertexHisto::tick()
{
  theNSimEvents++;
}

void VertexHisto::event ( const vertigo::Event & e )
{
  if (!hasStamped)
  {
    Tuple t ("Algo");
    t["name"]=e.algorithm();
    save ( t );
  }
  Tuple t ( "event" );
  t["id"]=e.id();
  t["run"]=e.run();
  t["tag"]=((string) ("_"))+e.tag();
  t["simtrks"]=e.simTracks().size();
  t["rectrks"]=e.tracks().size();
  const vector < vertigo::SimVertex > & svtces = e.simVertices();
  t["simvtces"]=svtces.size();
  int primaries=0;
  for ( vector< vertigo::SimVertex >::const_iterator i=svtces.begin(); 
        i!=svtces.end() ; ++i )
  {
    if (i->isPrimary()) primaries++;
  }
  t["primary_simvtces"]=primaries;
  t["vtces"]=e.vertices().size();
  t.fill();
  save ( t );
  t.setName ( "Event" );
  save ( t );
  theEvent=e;
}

void VertexHisto::stamp()
{
  if ( hasStamped ) return;
  SystemWriter ( theFileName ).save ();
  hasStamped=true;
}

void VertexHisto::add ( const vertigo::SimVertex & sv, float time,
                        Tuple & vtx, int evctr, int vtxnr )
{
  theNEvents++;
  ObserverTuples::addSimVertex ( sv, vtx );
  vtx["n_asstracks (number of associated rec tracks)"]=theEvent.nRecTracks ( sv );
  addDataMembers ( vtx );
  vtx["event"]=evctr;
  vtx["vtxnr"]=vtxnr;
  vtx["time"]=time;
}

void VertexHisto::addDataMembers( Tuple & t ) const
{
  t["simeventnr"]=theNSimEvents;
  t["eventnr"]=theNEvents;
}

void VertexHisto::add ( const rave::Vertex & rv, float time,
                        Tuple & vtx, int evctr, int vtxnr, float w )
{
  theNEvents++;
  ObserverTuples::addVertex ( rv, vtx );
  addDataMembers ( vtx );
  vtx["event"]=evctr;
  vtx["vtxnr"]=vtxnr;
  vtx["time"]=time;
  vtx["w"]=w;
}


void VertexHisto::add ( const vertigo::SimVertex & sv, const rave::Vertex & cv,
          float time, Tuple & vtx, int evctr, int vtxnr )
{
  // add ( sv, time, vtx, evctr, vtxnr );
  addDataMembers ( vtx );
  vtx["event"]=evctr;
  vtx["vtxnr"]=vtxnr;
  vtx["time"]=time;
  ObserverTuples::addAssociatedVertex ( sv, cv, vtx );
  discussTracks ( cv, sv, evctr );
}

void VertexHisto::discussTracks ( const rave::Vertex & cv,
                     const vertigo::SimVertex & sv, int evnr )
{
  /*
  cout << "[VertexHisto] discussing tracks for sv " << sv.id()
       << endl;
       */
  // ok. this method here must produce the TkVtx tuple.
  // it should see if the tracks of cv are in sv.

  vector < pair < float, rave::Track > > trks = cv.weightedTracks();
  float totweight=0.;
  for ( vector< pair < float, rave::Track > >::const_iterator i=
        trks.begin(); i!=trks.end() ; ++i )
  {
    totweight+=i->first;
  };

  map < const vertigo::SimTrack, bool, Sorter > takenIntoAccount;

  for ( vector< pair < float, rave::Track > >::const_iterator i=
        trks.begin(); i!=trks.end() ; ++i )
  {
    Tuple weights ( "TkVtx", "Track weights" );
    weights["weight"]=i->first; // (float) ( i->first );
    weights["totalweight"]=totweight; // sum of all track weights
    weights["accounted"]=1;
    addDataMembers ( weights );
    weights["event"]=evnr;
    weights["simvtxid"]=sv.id();
    weights["simweight"]=-23.;

    vector < vertigo::SimTrack > lstrks = theEvent.associatedTracks ( i->second );

    // find out if the associated SimTrack belongs to the SimVertex sv
    hasSimTrack ( i->second, lstrks, sv, weights, takenIntoAccount );

    save ( weights );
  };

  vector < vertigo::SimTrack > simtrks = sv.tracks();
  for ( vector< vertigo::SimTrack >::const_iterator i=simtrks.begin();
        i!=simtrks.end() ; ++i )
  {
    if ( !(takenIntoAccount[*i]) )
    {
      Tuple weights ("UnassTkVtx", "Unassociated Track weights" );
      weights["weight"]=-1.; // these dont have associated rave::Tracks!
      weights["accounted"]=0;
      weights["totalweight"]=totweight;
      addDataMembers ( weights );
      weights["event"]=evnr;
      weights["simvtxid"]=sv.id();
      weights["simtrackid"]=(*i).id();
      weights["simtrackpid"]=-1;
      weights["simweight"]=1.;
      save ( weights );
    };
  };
}

void VertexHisto::fill( const vertigo::SimVertex & sv, 
                        float time, int evctr, int vtxnr )
{
  stamp();
  Tuple vtx ( "Vtx" );
  add ( sv, time, vtx, evctr, vtxnr );
  save ( vtx );
}

void VertexHisto::fillMissing( const vertigo::SimVertex & sv, 
                               float time, int evctr, int vtxnr )
{
  stamp();
  Tuple vtx ( "MissingVtx" );
  add ( sv, time, vtx, evctr, vtxnr );
  save ( vtx );
}

void VertexHisto::fillExtra( const rave::Vertex & rv, 
                               float time, int evctr, int vtxnr )
{
  stamp();
  Tuple vtx ( "UnassociatedVtx" );
  add ( rv, time, vtx, evctr, vtxnr );
  save ( vtx );
  const vector < pair < float, rave::Vertex > > & comps = rv.components();
  // vtx["ncomps"]= comps.size();
  if ( comps.size () )
  {
    int ctr=0;
    for ( vector< pair < float, rave::Vertex > >::const_iterator i=comps.begin(); 
          i!=comps.end() ; ++i )
    {
      ctr++;
      stringstream name;
      name << vtx.getName() << "_comp" << ctr;
      Tuple comptuple(name.str());
      ObserverTuples::addVertex ( i->second, comptuple, i->first );
      addDataMembers ( comptuple );
      save ( comptuple );
    }
  }
}


void VertexHisto::fill( 
    const vertigo::SimVertex & sv, const rave::Vertex & cv, float time, int evctr,
    int vtxnr )
{
  stamp();
  Tuple vtx ( "no" );
  fill ( sv, cv, time, vtx, evctr, vtxnr );
}

void VertexHisto::fill( 
    const vertigo::SimVertex & sv, const rave::Vertex & cv, float time,
    Tuple & extras, int evctr, int vtxnr )
{
  stamp();
  Tuple vtx = extras;
  if ( !checkFiniteness ( cv ) )
  {
    except ( "rec vertex not finite", "not_finite", evctr );
  } else {
    add ( sv, cv, time, vtx, evctr,vtxnr );
    save ( vtx );

    // FIXME need sth _right_!!!
    if ( sv.isPrimary() ) //  /* vtxnr==1 || */ sv.pid()==2212 )
    {
      vtx.setName ( "Primaries" );
    } else if ( vtxnr==2 ) {
      vtx.setName ( "Secondaries" );
    } else {
      vtx.setName ( "Tertiaries" );
    }
    save ( vtx );

    const vector < pair < float, rave::Vertex > > & comps = cv.components();
    // vtx["ncomps"]= comps.size();
    if ( comps.size () )
    {
      int ctr=0;
      for ( vector< pair < float, rave::Vertex > >::const_iterator i=comps.begin(); 
            i!=comps.end() ; ++i )
      {
        ctr++;
        stringstream name;
        name << vtx.getName() << "_comp" << ctr;
        Tuple comptuple(name.str());
        // ObserverTuples::addVertex ( i->second, comptuple, i->first );
        add ( sv, i->second, time, comptuple, evctr, vtxnr );
        comptuple["w"]=i->first;
        addDataMembers ( comptuple );
        save ( comptuple );
      }
    }
  }
}

void VertexHisto::writeSource( const string & ss ) const
{
  Tuple s ( "Datasource" );
  s["source"]=ss;
  save ( s );
}

void VertexHisto::save ( const Tuple & tuple ) const
{
  if ( theFileName != "" ) dataharvester::Writer::file( theFileName ) << tuple;
}


void VertexHisto::except ( const std::string & e, const std::string & type, int evctr )
{
  stamp();
  Tuple what ( "failed" );
  what["what"]=e;
  what["type"]=type;
  what["event"]=evctr;
  what["simeventnr"]=theNSimEvents;
  save ( what );
}

void VertexHisto::close()
{
//  if ( theFileName != "" ) dataharvester::Writer::file ( theFileName )->close();
}

void VertexHisto::configure ( const string & f )
{
  // cout << "[VertexHisto] setting filename to " << f << endl;
  theFileName = f;
}

const string & VertexHisto::fileName() const
{
  return theFileName;
}
#endif

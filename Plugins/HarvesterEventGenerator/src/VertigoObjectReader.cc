#include "VertigoObjectReader.h"
#include "DelphiToRaveObjects.h"
#include <dataharvester/Tuple.h>
#include <dataharvester/Reader.h>
#include <rave/Track.h>
#include <rave/Vertex.h>
#include <rave/Vector6D.h>
#include <rave/Vector3D.h>
#include <rave/Vector7D.h>
#include <rave/Covariance3D.h>
#include <rave/Covariance6D.h>
#include <rave/Covariance7D.h>
#include <rave/impl/RaveBase/RaveEngine/interface/RaveId.h>
#include "Environment/MCObjects/interface/SimTrack.h"
#include "Environment/MCObjects/interface/SimVertex.h"
#include "Vertigo/EventLoop/interface/LoopSettings.h"
#include "Vertigo/Event/interface/Event.h"
#include <rave/impl/DataFormats/TrajectoryState/interface/TrackCharge.h>
#include <rave/impl/RaveBase/Converters/interface/RaveStreamers.h>
#include <rave/TransientTrackKinematicParticle.h>
#include <rave/impl/RaveTools/Converters/interface/MagneticFieldSingleton.h>
#include <rave/impl/FWCore/MessageLogger/interface/MessageLogger.h>
#include "Environment/Configuration/interface/SimpleConfigurable.h"
#include <sstream>
#ifdef WITH_THREADS
#include <boost/thread/mutex.hpp>                                                                       
#endif

using namespace rave;
using namespace std;
using namespace dataharvester;

namespace {
  #ifdef WITH_THREADS
  boost::mutex readermutex;                                                                             
  #endif

  void print ( map < string, MultiType > & data )
  {
    cout << "[VertigoObjectReader] data: " << endl;
    for ( map < string, MultiType >::iterator i=data.begin(); i!=data.end() ; ++i )
    {
      cout << "  +- " << i->first << ": " << i->second.asString() << endl;
    }
  }
  void addVertex ( vertigo::Event &e, map < string, MultiType > & data )
  {
    rave::Vector3D s( data["x"],  data["y"],  data["z"]  );
    rave::Covariance3D err ( data["dxx"], data["dxy"], data["dxz"],
                           data["dyy"], data["dyz"], data["dzz"] );
    vector < pair < float, rave::Track > > trks;
    rave::Vertex vtx ( s, err, trks, 0., 0., vector < pair < float, rave::Vertex > > ()  );
    e.add ( vtx );
  }
  
  void def ( map < string, MultiType > & data, const string & name , 
             const MultiType & value )
  {
    if ( !data.count(name) || data[name].asString()=="undef" )
    {
      data[name]=value;
    }
  }
  
  // rave::Track createTrack ( map < string, MultiType > & data )
  rave::Track createTrack ( const TupleRow & row, float & weight )
  {
    weight=0.;

    map < string, Tuple * > comp = row.getNestedData();
    if ( comp.size() )
    {
      vector < pair < float, rave::Track > > comps;
      for ( map < string, Tuple * >::const_iterator i=comp.begin(); i!=comp.end() ; ++i )
      {
        if ( i->first == "component" )
        {
          // cout << "[VertigoObjectReader] we have a track component here!" << endl;
          float weight=0.;
          vector < TupleRow > rows = i->second->getTupleRows();
          for ( vector< TupleRow >::const_iterator i=rows.begin(); i!=rows.end() ; ++i )
          {
            rave::Track trk = createTrack ( *i, weight );
            comps.push_back ( pair < float, rave::Track > ( weight, trk ) );
          }
        } else {
          cout << "[VertigoObjectReader] unknown nested structure ``" << i->first << "''" << endl;
        }
      }
      if ( comps.size() )
      {
        map < string, MultiType > data = row.getSimpleData();
        /*
        for ( map < string, MultiType >::const_iterator i=data.begin(); 
              i!=data.end() ; ++i )
        {
          cout << "    `-- " << i->first << endl;
        }*/
        if ( data.count("id")>0 )
        {
          // cout << "[VertigoObjectReader] returning track with " << comps.size() << " components." << data["id"] << endl;
          return rave::Track ( data["id"], comps );
        }
        return rave::Track ( comps );
      }
    }

    map < string, MultiType > data = row.getSimpleData();
    if ( data.count("w")>0 )
    {
      weight=data["w"];
    }


    if ( data["params"].asString() == "perigee" )
    {
      cout << "[VertigoObjectReader] perigee tracks disabled!" << endl;
      exit(0);
      // return perigeeTrack ( data );
    }
    if ( data["params"].asString() == "delphi" )
    {
      edm::LogWarning("VertigoObjectReader") << "FIXME magnetic field hard coded to 4 Tesla!!!";
      vertigo::DelphiToRaveObjects converter (4.0 );
      dataharvester::Tuple track ( "track" );
      for ( map < string, MultiType >::const_iterator i=data.begin(); i!=data.end() ; ++i )
      {
        track[i->first]=i->second;
      }
      return converter.toTrack ( track );
      // cout << "[VertigoObjectReader] perigee tracks disabled!" << endl;
      // exit(0);
      // return perigeeTrack ( data );
    }
    rave::Vector6D s( data["x"],  data["y"],  data["z"],
                    data["px"], data["py"], data["pz"] );
    rave::Covariance6D err( data["dxx"], data["dxy"], data["dxz"],
                          data["dyy"], data["dyz"], data["dzz"],
                          data["dxpx"], data["dxpy"], data["dxpz"],
                          data["dypx"], data["dypy"], data["dypz"],
                          data["dzpx"], data["dzpy"], data["dzpz"],
                          data["dpxpx"], data["dpxpy"], data["dpxpz"],
                          data["dpypy"], data["dpypz"], data["dpzpz"] );
    int nhitspixel = data["nhitspixel"].asInt();
    int nhits = data["nhitstotal"].asInt();
    // cout << "[VertigoObjectReader] nhitspixel=" << nhitspixel << " total=" << nhits << endl;
    bool tag_is_filter = SimpleConfigurable<bool>(true,"VertigoObjectReader:TagIsFilter").value();

    std::string tag = "0";

    if ( tag_is_filter )
    {
      if ( nhitspixel >=2 && nhits >=8 )
      {
        tag="1";
      }

      if ( nhitspixel ==0 && nhits == 0 )
      {
        // evidently the information isnt there
        tag="1";
      }
    } else {
      tag=data["name"].asString();
    }
    rave::Track trk;

    if ( data.count("id") && data["id"].asInt()!=0 )
    {
      // ok, there is an "id" field, but maybe its not an integer!
      trk = rave::Track ( data["id"].asInt(), s, err, data["q"].asInt(), data["chi2"].asDouble(), 
                          data["ndof"].asDouble(), 0, tag );
    } else { 
      trk = rave::Track ( s, err, data["q"].asInt(), data["chi2"].asDouble(), 
                          data["ndof"].asDouble(), 0, tag );
    }


    return trk;
  }

  rave::KinematicParticle createParticle ( map < string, MultiType > & data )
  {
    if ( data["params"].asString() == "perigee" )
    {
      cout << "[VertigoObjectReader] perigee particles disabled!" << endl;
      exit(0);
      // return perigeeTrack ( data );
    }
    rave::Vector6D s( data["x"],  data["y"],  data["z"],
                      data["px"], data["py"], data["pz"] );
    rave::Covariance6D err( data["dxx"], data["dxy"], data["dxz"],
                            data["dyy"], data["dyz"], data["dzz"],
                            data["dxpx"], data["dxpy"], data["dxpz"],
                            data["dypx"], data["dypy"], data["dypz"],
                            data["dzpx"], data["dzpy"], data["dzpz"],
                            data["dpxpx"], data["dpxpy"], data["dpxpz"],
                            data["dpypy"], data["dpypz"], data["dpzpz"] );
    int nhitspixel = data["nhitspixel"].asInt();
    int nhits = data["nhitstotal"].asInt();
    // cout << "[VertigoObjectReader] nhitspixel=" << nhitspixel << " total=" << nhits << endl;

    std::string tag = "0";

    if ( nhitspixel >=2 && nhits >=8 )
    {
      tag="1";
    }

    if ( nhitspixel ==0 && nhits == 0 )
    {
      // evidently the information isnt there
      tag="1";
    }

    // cout << "[VertigoObjectReader] reading " << tag << endl;
    // tag+="_"+data["id"];
    rave::Track trk ( s, err, data["q"].asInt(), data["chi2"].asDouble(), 
                      data["ndof"].asDouble(), 0, tag );
    rave::TransientTrackKinematicParticle p ( trk, data["m"], data["dmm"]);
    return p;
  }

  rave::Ellipsoid3D createBeamspot ( Tuple & t )
  {
    float x=0., y=0., z=0., dxx=1., dxy=0., dxz=0., dyy=1., dyz=0., dzz=1.;
    x=t["x"].asDouble();
    y=t["y"].asDouble();
    z=t["z"].asDouble();
    dxx=t["dxx"].asDouble();
    dxy=t["dxy"].asDouble();
    dxz=t["dxz"].asDouble();
    dyy=t["dyy"].asDouble();
    dyz=t["dyz"].asDouble();
    dzz=t["dzz"].asDouble();
    rave::Point3D p ( x, y, z );
    rave::Covariance3D cov ( dxx, dxy, dxz, dyy, dyz, dzz );
    cout << "[VertigoObjectReader] beamspot at (" << p.x() << "+-" 
         << sqrt ( cov.dxx() ) << ", " << p.y() << "+-"
         << sqrt ( cov.dyy() ) << ", " << p.z() << "+-" 
         << sqrt ( cov.dzz() ) << ") " << endl;
    // edm::LogInfo ( "VertigoObjectReader" ) << "Created BeamSpot at " << p;
    return rave::Ellipsoid3D ( p, cov );
  }
}

void VertigoObjectReader::keepAssociation ( const vertigo::SimTrack & t,
                       const string & vtxid )
{
  theAss[vtxid].push_back ( t );
}

void VertigoObjectReader::keepTrackAssociation ( const vertigo::SimTrack & t, const string & trkid )
{
  theTrkAss[t].push_back ( trkid );
}

void VertigoObjectReader::keepParticleAssociation ( const vertigo::SimParticle & t, const string & trkid )
{
  theParticleAss[t].push_back ( trkid );
}

void VertigoObjectReader::clear()
{
  theAss.clear();
  theVtces.clear();
  theTrkAss.clear();
  theParticleAss.clear();
  theTrks.clear();
  theHasTrackAssociations=false;
}

void VertigoObjectReader::implementAssociations ( vertigo::Event & e )
{
  // cout << "[VertigoObjectReader] implementing unresolved associations!" << endl;
  for ( map < string, vector < vertigo::SimTrack > >::const_iterator 
        i=theAss.begin(); i!=theAss.end() ; ++i )
  {
    // cout << "    `--- " << i->first << " valid:" << theVtces[i->first].isValid() << endl;
    if (theVtces[i->first].isValid())
    {
      vector < vertigo::SimTrack > trks = i->second;
      for ( vector< vertigo::SimTrack >::iterator t=trks.begin(); 
            t!=trks.end() ; ++t )
      {
        theVtces[i->first].add ( *t );
        t->setVertex ( &(theVtces[i->first]) );
        // cout << "          `--- adding track " << t->id() << " vtx=" << endl;
      }
    }
  }

  for ( map < vertigo::SimTrack, vector < string > >::const_iterator s=theTrkAss.begin(); 
        s!=theTrkAss.end() ; ++s )
  {
    vector < string > tags = s->second;
    /* for ( map < string, const rave::Track * >::const_iterator r=theTrks.begin(); 
          r!=theTrks.end() ; ++r ) */
    for ( vector < string >::const_iterator t=tags.begin(); 
          t!=tags.end() ; ++t )
    {
      if ( *t == "undef" ) continue;
      theHasTrackAssociations=true;
      rave::Track trk = theTrks[*t];
      e.associate ( s->first, trk );
    }
  }
  
  // cout << "[VertigoObjectReader] implementing associations: "
  //     << theParticleAss.size() << endl;
  for ( map < vertigo::SimParticle, vector < string > >::const_iterator s=theParticleAss.begin(); 
        s!=theParticleAss.end() ; ++s )
  {
    vector < string > tags = s->second;
    /* for ( map < string, const rave::Track * >::const_iterator r=theTrks.begin(); 
          r!=theTrks.end() ; ++r ) */
    for ( vector < string >::const_iterator t=tags.begin(); 
          t!=tags.end() ; ++t )
    {
      // cout << "  -- " << *t << ": " << flush;
      if ( *t == "undef" ) continue;
      theHasTrackAssociations=true;
      rave::KinematicParticle trk = theParticles[*t];
      // cout << trk.id() << endl;
      e.associate ( s->first, trk );
    }
  }
}


void VertigoObjectReader::addTrack ( vertigo::Event & e, const TupleRow & tuple )
{
  map < string, MultiType > data = tuple.getSimpleData();
  float weight=0.;
  rave::Track trk = createTrack ( tuple, weight );
  // const rave::Track * ptr = e.add ( trk );
  e.add ( trk );
  theTrks[data["id"].asString()]=trk;
}

void VertigoObjectReader::addSimTrack ( vertigo::Event &e, map < string, MultiType > & data )
{
  rave::Vector6D s( data["x"],  data["y"],  data["z"],
                  data["px"], data["py"], data["pz"] );
  const vertigo::SimVertex * vtx=0;
  /*
  cout << "[VertigoObjectReader] adding sim track: " << data["id"]
       << " to " << data["vtxid"] << endl;
  cout << "            `- " << theVtces.count ( data["vtxid"] ) << endl;
  */
  if ( theVtces.count ( data["vtxid"] ) )
  {
    vtx=&(theVtces[data["vtxid"]] );
  }
  vertigo::SimTrack trk;
  int pid=0;
  if ( data.count("pdg") )
  {
    pid=data["pdg"];
  }
  if ( data.count("pid") )
  {
    pid=data["pid"];
  }
  if ( data.count("id") )
  {
    trk = vertigo::SimTrack ( data["id"].asInt(), s, data["q"].asInt(), 
                              0 /* original track */, pid, "", vtx );
  } else {
    trk = vertigo::SimTrack ( s, data["q"].asInt(), 
                              0 /* original track */, pid, "", vtx );
  }
  e.add( trk );
  keepAssociation ( trk, data["vtxid"].asString() );
  keepTrackAssociation ( trk, data["rid"].asString() );
}

void VertigoObjectReader::addSimParticle ( vertigo::Event &e, map < string, MultiType > & data )
{
  rave::Vector7D s( data["x"],  data["y"],  data["z"],
                  data["px"], data["py"], data["pz"], data["m"] );
  const vertigo::SimVertex * vtx=0;
  if ( theVtces.count ( data["vtxid"] ) )
  {
    vtx=&(theVtces[data["vtxid"]] );
  }
  vertigo::SimParticle trk ( s, data["q"].asInt(), 0 /* original track */, /* pid */ 0, data["id"], vtx );
  e.add( trk );
  // keepAssociation ( trk, data["vtxid"].asString() );
  keepParticleAssociation ( trk, data["rid"].asString() );
}

void VertigoObjectReader::addSimVertex ( vertigo::Event &e, map < string, MultiType > & data )
{
  rave::Vector3D s( data["x"],  data["y"],  data["z"]  );
  int pid=0;
  bool isprimary=false;
  if ( data.count("pdg")) pid=data["pdg"];

  //FIXME remove!!
  if ( data.count("pmE"))
  {
    float pmE=data["pmE"];
    if ( pmE>6500. ) isprimary=true;
  }

  if ( data.count("primary")) isprimary=data["primary"];
  string tag="";
  if ( data.count("tag") )
  {
    tag=data["tag"].asString();
  }
  vector < vertigo::SimTrack > trks;
  vertigo::SimVertex vtx= vertigo::SimVertex ( s, trks, pid, tag, isprimary );
  e.add ( vtx );
  theVtces[data["id"].asString()]= vtx;
  //cout << "[VertigoObjectReader] adding sim vertex: " << data["id"] 
  //     << " primary=" << isprimary << endl;
}

void VertigoObjectReader::addTracks ( vertigo::Event & e, Tuple & t )
{
  vector < TupleRow > rows = t.getTupleRows();
  for ( vector < TupleRow >::const_iterator i=rows.begin(); 
        i!=rows.end() ; ++i )
  {
    addTrack ( e, *i );
  }
}

void VertigoObjectReader::addParticle ( vertigo::Event &e, map < string, MultiType > & data )
{
  /*
  cout << "[VertigoObjectReader] dropping prtcl" << endl;
  return;*/
  rave::KinematicParticle p = createParticle ( data );
  e.add ( p );
  theParticles[data["id"].asString()] = p;
}

void VertigoObjectReader::addParticles ( vertigo::Event & e, Tuple & t )
{
  vector < TupleRow > rows = t.getTupleRows();
  for ( vector < TupleRow >::const_iterator i = rows.begin();
        i != rows.end() ; ++i )
  {
    map < string, MultiType >  data= i->getSimpleData();
    addParticle ( e, data );
  }
}

void VertigoObjectReader::addJet ( vertigo::Event & e, dataharvester::Tuple & t )
{
  /*
  cout << "[VertigoObjectReader] dropping jet" << endl;
  return;*/
  vector < TupleRow > rows=t.getTupleRows();

  for ( vector < TupleRow >::const_iterator r=rows.begin(); 
        r!=rows.end() ; ++r )
  {
    vertigo::Jet jet;
    map < string, MultiType > flat = r->getSimpleData();
    if ( flat.count("flavor") )
    {
      jet.mcTag ( flat["flavor"].asString()[0] );
    }
    
    if ( flat.count("mctag") )
    {
      jet.mcTag ( flat["mctag"].asString()[0] );
    }
    
    if ( flat.count("physicsflavor") )
    {
      jet.mcPhysicsTag ( flat["physicsflavor"].asString()[0] );
    }

    map < string, dataharvester::Tuple * > data= r->getNestedData();
    for ( map < string, dataharvester::Tuple * >::const_iterator t=data.begin(); t!=data.end() ; ++t )
    {
      if ( t->second->getName() == "track" )
      {
        vector < TupleRow > trks = t->second->getTupleRows();
        for ( vector < TupleRow >::const_iterator x=trks.begin(); 
              x!=trks.end() ; ++x )
        {
          map < string, MultiType > data = x->getSimpleData();
          /*
          map < string,  Tuple * > comps = x->getNestedData();
          // cout << "[VertigoObjectReader] components: " << comps.size() << endl;
          if ( comps.size() )
          {
            cout << "[VertigoObjectReader] ha! component tracks in jets! Implement me!" << endl;
          }
          */
          float weight=1.;
          rave::Track trk = createTrack ( *x, weight );
          jet.add ( trk );
          theTrks[data["id"].asString()]=trk;
        }
      } else {
        if ( LoopSettings::Instance().verbosity() > 5 )
          cout << "[VertigoObjectReader] ignoring jet tuple object named \"" << t->second->getName() << "\"??"
               << endl;
      }
    }
    bool add = SimpleConfigurable<bool>(true,"VertigoObjectReader:AddJetTracks").value();
    e.add ( jet, add /* do or do not add tracks! */ );
  }
}
  
void VertigoObjectReader::addVtces ( vertigo::Event & e, Tuple & t )
{
  cout << "[VertigoObjectReader] dropping vtces" << endl;
  return;
  vector < TupleRow > rows=t.getTupleRows();
  for ( vector < TupleRow >::const_iterator i=rows.begin(); 
        i!=rows.end() ; ++i )
  {
    map < string, MultiType >  data= i->getSimpleData();
    addVertex ( e, data );
  }
}
  
void VertigoObjectReader::addSimVtces ( vertigo::Event & e, Tuple & t )
{
  vector < TupleRow > rows=t.getTupleRows();
  for ( vector < TupleRow >::const_iterator i=rows.begin(); 
        i!=rows.end() ; ++i )
  {
    map < string, MultiType >  data= i->getSimpleData();
    addSimVertex ( e, data );
  }
}

void VertigoObjectReader::addSimParticles ( vertigo::Event & e, Tuple & t )
{
  vector < TupleRow > rows=t.getTupleRows();
  for ( vector < TupleRow >::const_iterator i=rows.begin(); 
        i!=rows.end() ; ++i )
  {
    map < string, MultiType >  data= i->getSimpleData();
    addSimParticle ( e, data );
  }
}

void VertigoObjectReader::addSimTracks ( vertigo::Event & e, Tuple & t )
{
  vector < TupleRow > rows=t.getTupleRows();
  for ( vector < TupleRow >::const_iterator i=rows.begin(); 
        i!=rows.end() ; ++i )
  {
    map < string, MultiType >  data= i->getSimpleData();
    addSimTrack ( e, data );
  }
}

VertigoObjectReader::VertigoObjectReader( const string & filename ) :
    theFileName ( filename ), theHasTrackAssociations ( false ),
    theHasBeamSpot ( false )
{
}

bool VertigoObjectReader::hasBeamSpot() const
{
  return theHasBeamSpot;
}

const rave::Ellipsoid3D & VertigoObjectReader::beamspot() const
{
  return theBeamspot;
}

void VertigoObjectReader::nextTuple() const
{
  theCurrentTuple = Reader::file ( theFileName ).next();
  vector < TupleRow > rows=theCurrentTuple.getTupleRows();
}

void VertigoObjectReader::skip ( int n ) const
{
  int i=0;
  while ( true )
  {
    Tuple t=Reader::file ( theFileName ).next();
    if ( t.getName() == "event" )
    {
      i++;
      if ( i>=n ) break;
    };
  };
}

void VertigoObjectReader::getRunInformation ( Tuple & t )
{
  // cout << "[VertigoObjectReader] getRunInformation" << endl;
  if ( t.getNumberOfRows() > 0 )
  {
    TupleRow row = t.getRow(0);
    map < string, Tuple * > nested = row.getNestedData ();
    if ( nested.count ("beamspot" ) )
    {
      //cout << "[VertigoObjectReader] set beamspot: " 
      //     << (void *) nested["beamspot"] << endl;
      theBeamspot=createBeamspot( *(nested["beamspot"]) );
      theHasBeamSpot=true;
      LoopSettings::Instance().nonConstVertexFactory()->setBeamSpot ( theBeamspot );
      if ( LoopSettings::Instance().flavorTagFactory() )
          LoopSettings::Instance().flavorTagFactory()->resetBeamSpot();
    }
  }
}

void VertigoObjectReader::reset()
{
  // reset the reader!
  Reader::file ( theFileName ).reset();
}

vertigo::Event VertigoObjectReader::next()
{
  //cout << "[VertigoObjectReader] dropping obj" << endl;
  //return vertigo::Event();
  #ifdef WITH_THREADS
  boost::mutex::scoped_lock lock(readermutex);
  #endif
  clear();
  RaveId::reset();
  // next();
  Tuple t=Reader::file ( theFileName ).next();

  if ( t.getName() == "run" )
  {
    getRunInformation( t );
    t=Reader::file ( theFileName ).next();
    // return next();
  }

  if ( t.getName() != "event" )
  {
    return next();
  }

  vector < TupleRow > rows=t.getTupleRows();
  
  if ( rows.size() == 0 ) { return next(); };
  if ( rows.size()>1 )
  {
    cout << "[VertigoObjectReader] warning. tuple consists of more rows."
         << " all but the first one are ignored." << endl; 
  }
  map < string, MultiType > simple=rows[0].getSimpleData();
  map < string, Tuple * > nested=rows[0].getNestedData();
  // cout << "[VertigoObjectReader] got " << simple["id"] << endl;
  def ( simple, "tag", theFileName );
  def ( simple, "run", 0 );
  def ( simple, "id", 0 );

  vertigo::Event e ( simple["id"], simple["run"], simple["tag"] );

  for ( map < string, Tuple * >::const_iterator i=nested.begin(); 
        i!=nested.end() ; ++i )
  {
    if ( i->first == "track" )
    {
      addTracks ( e, *(i->second) );
    } else if ( i->first == "particle" )
    {
      addParticles ( e, *(i->second) );
    } else if ( i->first == "simparticle" )
    {
      addSimParticles ( e, *(i->second) );
    } else if ( i->first == "simvtx" )
    {
      addSimVtces ( e, *(i->second) );
    } else if ( i->first == "simtrack" )
    {
      addSimTracks ( e, *(i->second) );
    } else if ( i->first == "vtx" )
    {
      addVtces ( e, *(i->second) );
    } else if ( i->first == "jet" )
    {
      addJet ( e, *(i->second) );
    } else {
    }
  }

  implementAssociations ( e );

  if ( !theHasTrackAssociations )
  {
    if ( LoopSettings::Instance().verbosity() > 10 )
    {
      static bool once=true;
      if (once)
      {
        edm::LogInfo ( "VertigoObjectReader" ) << "no track associations stored. will associate by pulls";
        once=false;
      }
    }
    e.associateTracks();
  } else {
    if ( LoopSettings::Instance().verbosity() > 10 )
    {
      edm::LogInfo ( "VertigoObjectReader" ) << "exploit stored track associations.";
    }
  }

  return e;
}

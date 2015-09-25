#ifdef WITH_HARVESTER
#include "VertigoHarvest/interface/EventToTuple.h"
#include <rave/Track.h>
#include <rave/Vertex.h>
#include <rave/Vector6D.h>
#include "Vertigo/Event/interface/Event.h"
#include <rave/Covariance3D.h>
#include <rave/Covariance6D.h>
#include <rave/impl/TrackingTools/TrajectoryState/interface/TrajectoryStateClosestToPoint.h>
// #include <rave/impl/TrackingTools/TrajectoryState/interface/PerigeeConversions.h>
#include <rave/impl/TrackingTools/TransientTrack/interface/TransientTrack.h>
#include <rave/impl/TrackingTools/TrajectoryState/interface/FreeTrajectoryState.h>
#include <rave/impl/RaveBase/Converters/interface/CmsToRaveObjects.h>
#include <rave/impl/RaveBase/Converters/interface/RaveToCmsObjects.h>
#include <rave/impl/RaveBase/Converters/interface/MagneticFieldWrapper.h>
#include <rave/impl/RaveBase/Converters/interface/RaveStreamers.h>
#include <rave/impl/TrackingTools/PatternTools/interface/TSCPBuilderNoMaterial.h>
#include "Environment/Configuration/interface/SimpleConfigurable.h"

using namespace rave;
using namespace vertigo;
using namespace dataharvester;
using namespace std;

namespace {
  string saveString ( const string & r )
  {
    string ret = r;
    string::size_type pos = ret.find(":");
    if ( pos != string::npos )
    {
      ret.replace(pos,1,"_" );
    }
    return "s" + ret;
  }
  
  void addEventHeader ( const vertigo::Event & e, Tuple & t )
  {
    t["id"]=e.id();
    t["run"]=e.run();
    t["tag"]=saveString(e.tag());
  }

  signed vtxIndex ( const vertigo::SimVertex & s, const vector < vertigo::SimVertex > & v )
  {
    for ( vector< vertigo::SimVertex>::const_iterator i=v.begin(); i!=v.end() ; ++i )
    {
      if ( s == *i ) return (signed) (i-v.begin());
    }
    return -1;
  }

  string color ( signed vtxIndex )
  {
    switch (vtxIndex)
    {
      case 0:
        return "cyan";
      case 1:
        return "purple";
      case 2:
        return "darkgreen";
      default:
        return "blue";
    }
    
    return "blue";
  }

  void addComponent ( Tuple & t, float w, const rave::Track & rtrk, const string & pref,
                      int ctr, int simid, int vtxid, int pid, const string & tag )
  {
    string pre=pref+":";
    // cout << "[addComponent] adding to " << pre << endl;
    rave::Vector6D st=rtrk.state();
    rave::Covariance6D err=rtrk.error();
    t[pre + "id"]=rtrk.id();
    t[pre + "compnr"]=ctr;
    t[pre + "name"]="RT";
    t[pre+"w"]=w;
    t[pre+"simid"]=simid;
    t[pre+"vtxid"]=vtxid;
    t[pre+"pid"]=pid;
    t[pre+"x"]=st.x();
    t[pre+"y"]=st.y();
    t[pre+"z"]=st.z();
    t[pre+"px"]=st.px();
    t[pre+"py"]=st.py();
    t[pre+"pz"]=st.pz();
    
    t[pre+"dxx"]=err.dxx();
    t[pre+"dxy"]=err.dxy();
    t[pre+"dxz"]=err.dxz();
    t[pre+"dyy"]=err.dyy();
    t[pre+"dyz"]=err.dyz();
    t[pre+"dzz"]=err.dzz();
    
    t[pre+"dxpx"]=err.dxpx();
    t[pre+"dxpy"]=err.dxpy();
    t[pre+"dxpz"]=err.dxpz();
    t[pre+"dypx"]=err.dypx();
    t[pre+"dypy"]=err.dypy();
    t[pre+"dypz"]=err.dypz();
    t[pre+"dzpx"]=err.dzpx();
    t[pre+"dzpy"]=err.dzpy();
    t[pre+"dzpz"]=err.dzpz();
    
    t[pre+"dpxpx"]=err.dpxpx();
    t[pre+"dpxpy"]=err.dpxpy();
    t[pre+"dpxpz"]=err.dpxpz();
    t[pre+"dpypy"]=err.dpypy();
    t[pre+"dpypz"]=err.dpypz();
    t[pre+"dpzpz"]=err.dpzpz();
    
    t[pre+"q"]=rtrk.charge();
    t.fill(pref);
  }

  void addComponents ( Tuple & t, const rave::Track & track, const string & pref,
     int simid, int vtxid, int pid, const string & tag )
  {
    if ( track.components().size() == 0 ) return;
    const vector< pair < float, rave::Track > > & comps = track.components();
    int ctr=0;
    for ( vector< pair < float, rave::Track > >::const_iterator i=comps.begin(); 
          i!=comps.end() ; ++i )
    {
      ctr++;
      addComponent ( t, i->first, i->second, pref+":component", ctr,
                     simid, vtxid, pid, tag );
    }
  }
}

Tuple EventToTuple::create ( const vertigo::Event & event, bool propagate, bool colors )
{
  Tuple t("event");
  
  addEventHeader ( event, t );
  
  vector < vertigo::SimVertex > svtces = event.simVertices();
  vector < vertigo::Jet > jets=event.jets();
  // cout << "[EventToTuple] add jets" << endl;
  for ( vector< vertigo::Jet >::const_iterator i=jets.begin(); i!=jets.end() ; ++i )
  {
    map < string, MultiType > attrs;
    add ( t, *i, attrs );
  }

  // cout << "[EventToTuple] add tracks" << endl;
  vector < rave::Track > trks = event.tracks();
  for ( vector< rave::Track >::const_iterator i=trks.begin(); i!=trks.end() ; ++i )
  {
    map < string, MultiType > attrs;
    vector < vertigo::SimTrack > simtracks = event.associatedTracks ( *i );
    vertigo::SimVertex vtx;
    if ( simtracks.size() )
    {
      if ( simtracks[0].vertex() )
      {
        vtx=*(simtracks[0].vertex ()); 
      };
    }
    if ( colors )
    {
      attrs["color"]=color( vtxIndex ( vtx, svtces ) );
    }
    add ( t, *i, simtracks, vtx, attrs, propagate );
  }
  
  vector < rave::KinematicParticle > kps = event.particles();
  for ( vector< rave::KinematicParticle >::const_iterator i=kps.begin(); i!=kps.end() ; ++i )
  {
    map < string, MultiType > attrs;
    vector < vertigo::SimParticle > simparticles = event.associatedParticles ( *i );
    vertigo::SimVertex vtx;
    if ( simparticles.size() )
    {
      if ( simparticles[0].vertex() )
      {
        vtx=*(simparticles[0].vertex ()); 
      };
    }

    add ( t, *i, simparticles, vtx, attrs );
  }


  static bool addRecVertices=
    SimpleConfigurable<bool>(false,"EventToTuple:AddRecVertices").value();

  if (addRecVertices)
  {
    vector < rave::Vertex > vtces=event.vertices();
    for ( vector< rave::Vertex >::const_iterator i=vtces.begin(); i!=vtces.end() ; ++i )
    {
      map < string, MultiType > attrs;
      add ( t, *i, attrs );
    }
  }

  for ( vector< vertigo::SimVertex >::const_iterator i=svtces.begin(); i!=svtces.end() ; ++i )
  {
    map < string, MultiType > attrs;
    add ( t, *i, attrs, i==svtces.begin() );
  }
  
  vector < vertigo::SimTrack > strks=event.simTracks();
  for ( vector< vertigo::SimTrack >::const_iterator i=strks.begin(); i!=strks.end() ; ++i )
  {
    map < string, MultiType > attrs;
    add ( t, *i, attrs );
  }
  
  vector < vertigo::SimParticle > sps = event.simParticles();
  for ( vector< vertigo::SimParticle >::const_iterator i=sps.begin(); i!=sps.end() ; ++i )
  {
    map < string, MultiType > attrs;
    attrs["rid"]="undef";
    vector < rave::KinematicParticle > rprts = event.associatedParticles ( *i );
    if ( rprts.size () ) attrs["rid"]=rprts[0].id();
    add ( t, *i, attrs );
  }

  /*
  cout << "[EventToTuple] saving: " << trks.size() << " tracks, "
       << vtces.size() << " vertices, "
       << strks.size() << " simtracks, " << svtces.size() << " simvertices."
       << endl;
       */
  t.fill();
  return t;
}

void EventToTuple::add (  Tuple & t,
    const rave::Track & rtrk, const vector < vertigo::SimTrack > & simtracks,
    const vertigo::SimVertex & vtx,
    const map < string, MultiType > & attrs, bool propagate,
    const std::string & prefix )
{
  if ( hasAdded_[rtrk.id()] ) return;
  hasAdded_[rtrk.id()]=true;
  string pref="track";
  if ( prefix.size() ) pref=prefix+":"+pref;
  string pre=pref+":";

  for ( map < string, MultiType >::const_iterator i=attrs.begin(); 
        i!=attrs.end() ; ++i )
  {
    t[ pre +i->first]=i->second;
  }
  
  rave::Vector6D st=rtrk.state();
  rave::Covariance6D err=rtrk.error();
  int simid=-1;
  int vtxid=-1;
  int pid=-1;
  string tag="";

  if ( simtracks.size() )
  {
    simid=simtracks.front().id();
    vtxid=vtx.id();
    pid=simtracks.front().pid();
    tag=saveString(simtracks.front().tag());

    if (vtx.isValid() && propagate)
    {
      GlobalPoint ref = RaveToCmsObjects().convert ( vtx.position() );
      GlobalTrajectoryParameters gtp = RaveToCmsObjects().convert ( rtrk );
      CartesianTrajectoryError gte = RaveToCmsObjects().convert ( err );
      FreeTrajectoryState fts ( gtp, gte );
      TrajectoryStateClosestToPoint tscp = TSCPBuilderNoMaterial()(fts, ref );
      FreeTrajectoryState newstate = tscp.theState();
      st=CmsToRaveObjects().convert ( newstate.parameters() );
      err=CmsToRaveObjects().convert ( newstate.cartesianError() );
      // cout << "[EventToTuple] propagating " << gtp << " to " << st << endl;
    }
  }
  
  t[pre+"simid"]=simid;
  t[pre+"vtxid"]=vtxid;
  t[pre+"pid"]=pid;
  t[pre+"tag"]=tag;
  t[pre+"id"]=rtrk.id();
  t[pre+"name"]="RT";
  t[pre+"x"]=st.x();
  t[pre+"y"]=st.y();
  t[pre+"z"]=st.z();
  t[pre+"px"]=st.px();
  t[pre+"py"]=st.py();
  t[pre+"pz"]=st.pz();
  
  t[pre+"dxx"]=err.dxx();
  t[pre+"dxy"]=err.dxy();
  t[pre+"dxz"]=err.dxz();
  t[pre+"dyy"]=err.dyy();
  t[pre+"dyz"]=err.dyz();
  t[pre+"dzz"]=err.dzz();
  
  t[pre+"dxpx"]=err.dxpx();
  t[pre+"dxpy"]=err.dxpy();
  t[pre+"dxpz"]=err.dxpz();
  t[pre+"dypx"]=err.dypx();
  t[pre+"dypy"]=err.dypy();
  t[pre+"dypz"]=err.dypz();
  t[pre+"dzpx"]=err.dzpx();
  t[pre+"dzpy"]=err.dzpy();
  t[pre+"dzpz"]=err.dzpz();
  
  t[pre+"dpxpx"]=err.dpxpx();
  t[pre+"dpxpy"]=err.dpxpy();
  t[pre+"dpxpz"]=err.dpxpz();
  t[pre+"dpypy"]=err.dpypy();
  t[pre+"dpypz"]=err.dpypz();
  t[pre+"dpzpz"]=err.dpzpz();
  
  t[pre+"q"]=rtrk.charge();
  addComponents ( t, rtrk, pref, simid, vtxid, pid, tag ); 
  t.fill(pref);
}


void EventToTuple::add (  Tuple & t,
                                 const rave::KinematicParticle & kp, 
                                 const vector < vertigo::SimParticle > & simparticles,
                                 const vertigo::SimVertex & vtx,
                                 const map < string, MultiType > & attrs,
                                 const std::string & prefix )
{
  if ( hasAddedP_[kp.id()] ) return;
  hasAddedP_[kp.id()] = true;
  
  string pre = "particle:";
  if ( prefix.size() ) pre = prefix+":" + pre;

  t[pre + "id"]   = kp.id();
  t[pre + "name"] = "RaveKinematicParticle";
  
  for ( map < string, MultiType >::const_iterator i = attrs.begin();
        i != attrs.end() ; ++i )
  {
    t[ pre + i->first] = i->second;
  }
  
  rave::Vector7D st = kp.fullstate();
  rave::Covariance7D err = kp.fullerror();

  if ( simparticles.size() )
  {
    t[pre+"vtxid"] = vtx.id();
    t[pre+"simid"] = simparticles.front().id();
    t[pre+"pid"]   = simparticles.front().pid();
    t[pre+"tag"]   = saveString(simparticles.front().tag());
  }
  
  t[pre+"x"]  = st.x();
  t[pre+"y"]  = st.y();
  t[pre+"z"]  = st.z();
  t[pre+"px"] = st.px();
  t[pre+"py"] = st.py();
  t[pre+"pz"] = st.pz();
  t[pre+"m"]  = st.m();
  
  t[pre+"dxx"]=err.dxx();
  t[pre+"dxy"]=err.dxy();
  t[pre+"dxz"]=err.dxz();
  t[pre+"dyy"]=err.dyy();
  t[pre+"dyz"]=err.dyz();
  t[pre+"dzz"]=err.dzz();
  
  t[pre+"dxpx"]=err.dxpx();
  t[pre+"dxpy"]=err.dxpy();
  t[pre+"dxpz"]=err.dxpz();
  t[pre+"dypx"]=err.dypx();
  t[pre+"dypy"]=err.dypy();
  t[pre+"dypz"]=err.dypz();
  t[pre+"dzpx"]=err.dzpx();
  t[pre+"dzpy"]=err.dzpy();
  t[pre+"dzpz"]=err.dzpz();
  
  t[pre+"dpxpx"]=err.dpxpx();
  t[pre+"dpxpy"]=err.dpxpy();
  t[pre+"dpxpz"]=err.dpxpz();
  t[pre+"dpypy"]=err.dpypy();
  t[pre+"dpypz"]=err.dpypz();
  t[pre+"dpzpz"]=err.dpzpz();
  
  t[pre+"dxm"]  = err.dxm();
  t[pre+"dym"]  = err.dym();
  t[pre+"dzm"]  = err.dzm();
  t[pre+"dpxm"] = err.dpxm();
  t[pre+"dpym"] = err.dpym();
  t[pre+"dpzm"] = err.dpzm();
  t[pre+"dmm"]  = err.dmm();
  
  t[pre+"q"] = kp.charge();
  
  t.fill("particle");
}

void EventToTuple::add (  Tuple & t,
    const rave::Vertex & vtx, const map < string, MultiType > & attrs )
{
  vector < pair < float, rave::Track > > trks = vtx.weightedTracks();
  for ( vector< pair < float, rave::Track > >::const_iterator i=trks.begin(); 
        i!=trks.end() ; ++i )
  {
    int id = i->second.id();
    double w = i->first;
    t["vtx:asstrk:id"]=id;
    t["vtx:asstrk:w"]=w;
    t.fill("vtx:asstrk");
  }
  
  t["vtx:id"]=vtx.id();
  for ( map < string, MultiType >::const_iterator i=attrs.begin(); 
        i!=attrs.end() ; ++i )
  {
    t["vtx:"+i->first]=i->second;
  }
  rave::Vector3D st=vtx.position();
  rave::Covariance3D err=vtx.error();
  t["vtx:x"]=st.x();
  t["vtx:y"]=st.y();
  t["vtx:z"]=st.z();
  t["vtx:dxx"]=err.dxx();
  t["vtx:dxy"]=err.dxy();
  t["vtx:dxz"]=err.dxz();
  t["vtx:dyy"]=err.dyy();
  t["vtx:dyz"]=err.dyz();
  t["vtx:dzz"]=err.dzz();
  t.fill("vtx");
}

void EventToTuple::add (  Tuple & t,
    const vertigo::SimTrack & rtrk, const map < string, MultiType > & attrs )
{
  t["simtrack:id"]=rtrk.id();
  t["simtrack:pid"]=rtrk.pid();
  t["simtrack:tag"]=saveString(rtrk.tag());
  for ( map < string, MultiType >::const_iterator i=attrs.begin(); 
        i!=attrs.end() ; ++i )
  {
    t["simtrack:"+i->first]=i->second;
  }
  rave::Vector6D st=rtrk.state();
  int vtxid=-1;
  if ( rtrk.vertex() )
  {
    vtxid=rtrk.vertex()->id();
  }
  t["simtrack:vtxid"]=vtxid;
  t["simtrack:x"]=st.x();
  t["simtrack:y"]=st.y();
  t["simtrack:z"]=st.z();
  t["simtrack:px"]=st.px();
  t["simtrack:py"]=st.py();
  t["simtrack:pz"]=st.pz();
  t["simtrack:q"]=rtrk.charge();
  t.fill("simtrack");
}

void EventToTuple::add (  Tuple & t,
                                 const vertigo::SimParticle & sp, const map < string, MultiType > & attrs )
{
  t["simparticle:id"]  = sp.id();
  t["simparticle:pid"] = sp.pid();
  t["simparticle:tag"] = saveString(sp.tag());
  
  for ( map < string, MultiType >::const_iterator i = attrs.begin(); 
        i != attrs.end() ; ++i )
  {
    t["simparticle:"+i->first] = i->second;
  }
  
  rave::Vector7D st = sp.state();
  t["simparticle:x"]  = st.x();
  t["simparticle:y"]  = st.y();
  t["simparticle:z"]  = st.z();
  t["simparticle:px"] = st.px();
  t["simparticle:py"] = st.py();
  t["simparticle:pz"] = st.pz();
  t["simparticle:m"]  = st.m();
  
  t["simparticle:q"]  = sp.charge();
  
  t.fill("simparticle");
}

void EventToTuple::add ( Tuple & t,
    const vertigo::Jet & jet, const map < string, MultiType > & attrs )
{
  t["jet:id"]=jet.id();
  for ( map < string, MultiType >::const_iterator i=attrs.begin(); 
        i!=attrs.end() ; ++i )
  {
    t["jet:"+i->first]=i->second;
  }
  try {
    t["jet:x"]=jet.position().x();
    t["jet:y"]=jet.position().y();
    t["jet:z"]=jet.position().z();
    t["jet:dx"]=jet.momentum().x();
    t["jet:dy"]=jet.momentum().y();
    t["jet:dz"]=jet.momentum().z();
    t["jet:height"]=jet.momentum().mag() / 20.;
  } catch ( std::exception & e ) {
    t["jet:x"]=NAN;
    t["jet:y"]=NAN;
    t["jet:z"]=NAN;
    t["jet:dx"]=NAN;
    t["jet:dy"]=NAN;
    t["jet:dz"]=NAN;
    t["jet:height"]=NAN;
  }
  string tag="";
  tag+=jet.getMCTag();
  t["jet:mctag"]=tag;
  string mcphystag="";
  mcphystag+=jet.getMCPhysicsTag();
  t["jet:mcphystag"]=mcphystag;
  t["jet:angle"]=jet.angle();
  if ( jet.isTagged() )
  {
    t["jet:tag"]=jet.getTag();
  }

  vector < rave::Track > trks = jet.tracks();
  for ( vector< rave::Track >::const_iterator i=trks.begin(); i!=trks.end() ; ++i )
  {
    t["jet:asstrk:id"]=i->id();
    t["jet:asstrk:w"]=1.;
    add ( t, *i, vector < SimTrack > (), SimVertex(), attrs, true, "jet" );
    t.fill("jet:asstrk");
  }

  t.fill("jet");
}

void EventToTuple::add (  Tuple & t,
    const vertigo::SimVertex & vtx, const map < string, MultiType > & attrs, bool first )
{
  t["simvtx:id"]=vtx.id();
  t["simvtx:tag"]=saveString(vtx.tag());
  // cout << "SimVertex pid=" << vtx.pid() << endl;
  if ( vtx.pid() )
  {
    t["simvtx:pid"]=vtx.pid();
    t["simvtx:pdg"]=vtx.pid();
  }
  t["simvtx:primary"]=vtx.isPrimary();
  if ( first )
  {
    t["simvtx:name"]="primary";
  } else {
    t["simvtx:name"]="secondary";
  }
  for ( map < string, MultiType >::const_iterator i=attrs.begin(); 
        i!=attrs.end() ; ++i )
  {
    t["simvtx:"+i->first]=i->second;
  }
  rave::Vector3D st=vtx.position();
  t["simvtx:x"]=st.x();
  t["simvtx:y"]=st.y();
  t["simvtx:z"]=st.z();
  t.fill("simvtx");
}

#endif /* WITH_HARVESTER */

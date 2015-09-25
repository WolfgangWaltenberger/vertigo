#include "TrackParameterHistogrammer.h"
#include "CLHEP/Matrix/Vector.h"
#include <sstream>

#include <rave/impl/TrackingTools/TransientTrack/interface/TransientTrack.h>
#include <rave/impl/TrackingTools/TrajectoryState/interface/TrajectoryStateOnSurface.h>
#include <rave/impl/TrackingTools/PatternTools/interface/TransverseImpactPointExtrapolator.h>
#include <rave/impl/DataFormats/GeometrySurface/interface/Surface.h>
#include <rave/impl/RaveTools/Converters/interface/MagneticFieldSingleton.h>
#include <rave/impl/RaveBase/Converters/interface/RaveToCmsObjects.h>

#include "Math/LorentzVector.h"
#include "Math/GenVector/PxPyPzM4D.h"

#include <dataharvester/Writer.h>
#include <dataharvester/SystemWriter.h>

#include "Environment/MCObjects/interface/SimVertex.h"
#include "Vertigo/Event/interface/Event.h"
#include "Vertigo/Converters/interface/VertigoToCmsObjects.h"
#include "Environment/Utilities/interface/VertigoStreamers.h"

// temporary only!
#include "tools/interface/LHEHelpers.h"

using namespace dataharvester;
using namespace std;

namespace {

  float eta ( const rave::Vector4D & v )
  {
    ROOT::Math::LorentzVector<ROOT::Math::PxPyPzM4D<double> > l
       ( v.x(), v.y(), v.z(), v.m() );
    return l.Rapidity();
    // return l.eta();
  }

  void checkSurface ( const TrajectoryStateOnSurface & simip,
                      const TrajectoryStateOnSurface & recip )
  {
    LocalPoint localp ( 1., 2. );
    GlobalPoint recglobal = recip.surface().toGlobal ( localp );
    LocalPoint local2p ( 1., 2. );
    GlobalPoint simglobal = simip.surface().toGlobal ( local2p );
    if ( ( recglobal - simglobal ).mag() > 1e-6 )
    {
      cout << "[TrackParameterHistogrammer] ERROR: points differ. Diff="
           << ( recglobal - simglobal ).mag()
           << endl
           << "[TrackParameterHistogrammer] rec=" << recglobal << "  sim="
           << simglobal << endl;
      exit(0);
    };
  }

  void add ( const rave::Track & trk, Tuple & params, float weight=1.0 )
  {
    RaveToCmsObjects converter;
    reco::TransientTrack tt ( converter.tTrack( trk) );
    float pRec = tt.initialFreeState().momentum().mag();
    params["rec_p"]=pRec;
    params["rec_x"]=trk.state().x();
    params["rec_y"]=trk.state().y();
    params["rec_z"]=trk.state().z();
    params["rec_px"]=trk.state().px();
    params["rec_py"]=trk.state().py();
    params["rec_pz"]=trk.state().pz();
    params["chi2"]=trk.chi2();
    params["w"]=weight;
    params["ncomponents"]=trk.components().size();
    /*
    TrajectoryStateOnSurface recip = tt.impactPointState();
    fillCorrelations ( recip );
    */
  }

  void add ( const vertigo::SimTrack & simtrk, Tuple & params,
             std::string prefix="sim_" )
  {
    params[prefix+"id"]=simtrk.id();
    params[prefix+"charge"]=simtrk.charge();
    params[prefix+"x"]=simtrk.state().x();
    params[prefix+"y"]=simtrk.state().y();
    params[prefix+"z"]=simtrk.state().z();
    params[prefix+"pid"]=simtrk.pid();
    rave::Vector4D p ( simtrk.state().momentum(), 0. );
    params[prefix+"eta"]=eta ( p );
    params[prefix+"px"]=simtrk.state().px();
    params[prefix+"py"]=simtrk.state().py();
    params[prefix+"pz"]=simtrk.state().pz();
    params[prefix+"p"]=simtrk.state().momentum().mag();
    const vertigo::SimVertex * vtx = simtrk.vertex();
    // params["simvtx_ptr"]=vtx;
    params["simvtx_id"]=-1;
    if ( vtx )
    {
      params["simvtx_id"]=vtx->id();
    };
  }

  void add ( const vertigo::SimParticle & simp, Tuple & params,
             std::string prefix="sim_" )
  {
    params[prefix+"id"]=simp.id();
    params[prefix+"charge"]=simp.charge();
    params[prefix+"x"]=simp.state().x();
    params[prefix+"y"]=simp.state().y();
    params[prefix+"z"]=simp.state().z();
    params[prefix+"pid"]=simp.pid();
    params[prefix+"eta"]=eta ( simp.state().p4() );
    params[prefix+"px"]=simp.state().px();
    params[prefix+"py"]=simp.state().py();
    params[prefix+"pz"]=simp.state().pz();
    params[prefix+"p"]=simp.state().momentum().mag();
    const vertigo::SimVertex * vtx = simp.vertex();
    // params["simvtx_ptr"]=vtx;
    params["simvtx_id"]=-1;
    if ( vtx )
    {
      params["simvtx_id"]=vtx->id();
    };
  }

  void add ( Tuple & t, const map < string, float > & m )
  {
    for ( map < string, float >::const_iterator i=m.begin(); i!=m.end() ; ++i )
    {
      t[i->first]=i->second;
    }
  }

  string perigeeName ( int i )
  {
    switch ( i )
    {
      case 0:
        return "curv";
      case 1:
        return "theta";
      case 2:
        return "phi";
      case 3:
        return "tip";
      case 4:
        return "lip";
      default:
        return "undefined";
    }
  }

  void addDifference ( Tuple & params,
                       const TrajectoryStateOnSurface & im_rec,
                       const TrajectoryStateOnSurface & im_sim )
  {
    // cout << "tsos=" << im_rec.transverseCurvature() << endl;
    params["d_px"]=im_rec.globalMomentum().x() - im_sim.globalMomentum().x();
    params["d_py"]=im_rec.globalMomentum().y() - im_sim.globalMomentum().y();
    params["d_pz"]=im_rec.globalMomentum().z() - im_sim.globalMomentum().z();
    params["d_p"]=im_rec.globalMomentum().mag() - im_sim.globalMomentum().mag();
    params["d_x"]=im_rec.globalPosition().x() - im_sim.globalPosition().x();
    params["d_y"]=im_rec.globalPosition().y() - im_sim.globalPosition().y();
    params["d_z"]=im_rec.globalPosition().z() - im_sim.globalPosition().z();
    params["sigma_px"]=sqrt(im_rec.cartesianError().matrix()(3,3));
    params["sigma_py"]=sqrt(im_rec.cartesianError().matrix()(4,4));
    params["sigma_pz"]=sqrt(im_rec.cartesianError().matrix()(5,5));
    params["sigma_x"]=sqrt(im_rec.cartesianError().matrix()(0,0));
    params["sigma_y"]=sqrt(im_rec.cartesianError().matrix()(1,1));
    params["sigma_z"]=sqrt(im_rec.cartesianError().matrix()(2,2));
    params["st_px"]=params["d_px"].asDouble() / params["sigma_px"].asDouble();
    params["st_py"]=params["d_py"].asDouble() / params["sigma_py"].asDouble();
    params["st_pz"]=params["d_pz"].asDouble() / params["sigma_pz"].asDouble();
    params["st_x"]=params["d_x"].asDouble() / params["sigma_x"].asDouble();
    params["st_y"]=params["d_y"].asDouble() / params["sigma_y"].asDouble();
    params["st_z"]=params["d_z"].asDouble() / params["sigma_z"].asDouble();
  }

  void add ( const FreeTrajectoryState & sim,
             const FreeTrajectoryState & rec, bool refitted,
             Tuple & params )
  {
    // add the "real" thing!
    TransverseImpactPointExtrapolator tipe ( MagneticFieldSingleton::Instance() );
    PerigeeConversions converter;
    double pt; // not used
    GlobalPoint refpt = GlobalPoint (0.,0.,0.); // sim.position();
    TrajectoryStateOnSurface im_sim = tipe.extrapolate ( sim, refpt );
    // cout << "extrapolate rec=" << rec << endl;
    // cout << rec.transverseCurvature() << endl;
    TrajectoryStateOnSurface im_rec = tipe.extrapolate ( rec, refpt );
    addDifference ( params, im_rec, im_sim );

    PerigeeTrajectoryParameters sim_p=converter.ftsToPerigeeParameters
      ( *(im_sim.freeState()),  refpt, pt );
    PerigeeTrajectoryParameters rec_p=converter.ftsToPerigeeParameters
      ( *(im_rec.freeState()), refpt, pt );
    PerigeeTrajectoryError rec_err = converter.ftsToPerigeeError ( *(im_rec.freeState()) );
    // AlgebraicVector diff(5), stres(5);

    for ( int i=0; i< 5 ; i++ )
    {
      float delta=rec_p.vector()[i]-sim_p.vector()[i];
      // diff[i]=delta;
      // stres[i]=delta/sqrt(rec_err.covarianceMatrix[i][i]);
      params["sim_"+perigeeName(i)]=sim_p.vector()[i];
      params["rec_"+perigeeName(i)]=rec_p.vector()[i];
      params["d_"+perigeeName(i)]=delta;
      params["sigma_"+perigeeName(i)]= sqrt(rec_err.covarianceMatrix()(i,i) );
      params["fabs_d_"+perigeeName(i)]=fabs(delta);
      params["st_"+perigeeName(i)]=delta/sqrt(rec_err.covarianceMatrix()(i,i));
    }
  }
}

void TrackParameterHistogrammer::saveSimTracks ( const vertigo::Event & e )
{
  map < string, float > masses = LHEHelpers::getSMSMasses ( e.tag() );
  std::vector < vertigo::SimTrack > strks = e.simTracks();
  for ( vector< vertigo::SimTrack >::const_iterator
        i=strks.begin(); i!=strks.end() ; ++i )

  {
    dataharvester::Tuple t ( "SimTrack" );
    add ( t, masses );
    t["eventid"]=e.id();
    add ( *i, t, "" );
    dataharvester::Writer::file ( theFileName ) << t;
  }
}

void TrackParameterHistogrammer::saveSimParticles ( const vertigo::Event & e )
{
  map < string, float > masses = LHEHelpers::getSMSMasses ( e.tag() );
  std::vector < vertigo::SimParticle > strks = e.simParticles();
  for ( vector< vertigo::SimParticle >::const_iterator
        i=strks.begin(); i!=strks.end() ; ++i )

  {
    dataharvester::Tuple t ( "SimParticle" );
    t["eventid"]=e.id();
    add ( t, masses );
    add ( *i, t, "" );
    dataharvester::Writer::file ( theFileName ) << t;
  }
}

TrackParameterHistogrammer::TrackParameterHistogrammer( const string & str ) :
  theFileName ( str ), theTrackNr( 0 )
{
}

TrackParameterHistogrammer::TrackParameterHistogrammer() :
  theFileName ( "tracks.root" ), theTrackNr ( 0 )
{
}

void TrackParameterHistogrammer::configure ( const string & s )
{
  theFileName=s;
  stamp();
}

TrackParameterHistogrammer * TrackParameterHistogrammer::clone() const
{
  return new TrackParameterHistogrammer ( *this );
}

// get Tracks parameters and add Track in hist

void TrackParameterHistogrammer::stamp()
{
  dataharvester::SystemWriter ( theFileName ).save();
}

void TrackParameterHistogrammer::fillCorrelations ( const TrajectoryStateOnSurface & surf )
{
  const CurvilinearTrajectoryError & curv = surf.curvilinearError();
  const LocalTrajectoryError & lerr = surf.localError();
  AlgebraicSymMatrix matrix = curv.matrix_old();
  AlgebraicSymMatrix lmatrix = lerr.matrix_old();
  dataharvester::Tuple corrs ( "TkCorrs" );

  corrs["invp_lambda"] = matrix(1,2) / sqrt ( matrix(1,1) * matrix(2,2) );
  corrs["invp_phi"] = matrix(1,3) / sqrt ( matrix(1,1) * matrix(3,3) );
  corrs["invp_xt"] = matrix(1,4) / sqrt ( matrix(1,1) * matrix(4,4) );
  corrs["invp_yt"] = matrix(1,5) / sqrt ( matrix(1,1) * matrix(5,5) );
  corrs["lambda_phi"] = matrix(2,3) / sqrt ( matrix(2,2) * matrix(3,3) );
  corrs["lambda_xt"] = matrix(2,4) / sqrt ( matrix(2,2) * matrix(4,4) );
  corrs["lambda_yt"] = matrix(2,5) / sqrt ( matrix(2,2) * matrix(5,5) );
  corrs["phi_xt"] = matrix(3,4) / sqrt ( matrix(3,3) * matrix(4,4) );
  corrs["phi_yt"] = matrix(3,5) / sqrt ( matrix(3,3) * matrix(5,5) );
  corrs["xt_yt"] = matrix(4,5) / sqrt ( matrix(4,4) * matrix(5,5) );

  corrs["invp_dxdir"] = lmatrix(1,2) / sqrt ( lmatrix(1,1) * lmatrix(2,2) );
  corrs["invp_dydir"] = lmatrix(1,3) / sqrt ( lmatrix(1,1) * lmatrix(3,3) );
  corrs["invp_dx"] = lmatrix(1,4) / sqrt ( lmatrix(1,1) * lmatrix(4,4) );
  corrs["invp_dy"] = lmatrix(1,5) / sqrt ( lmatrix(1,1) * lmatrix(5,5) );
  corrs["dxdir_dydir"] = lmatrix(2,3) / sqrt ( lmatrix(2,2) * lmatrix(3,3) );
  corrs["dxdir_dx"] = lmatrix(2,4) / sqrt ( lmatrix(2,2) * lmatrix(4,4) );
  corrs["dxdir_dy"] = lmatrix(2,5) / sqrt ( lmatrix(2,2) * lmatrix(5,5) );
  corrs["dydir_dx"] = lmatrix(3,4) / sqrt ( lmatrix(3,3) * lmatrix(4,4) );
  corrs["dydir_dy"] = lmatrix(3,5) / sqrt ( lmatrix(3,3) * lmatrix(5,5) );
  corrs["dx_dy"] = lmatrix(4,5) / sqrt ( lmatrix(4,4) * lmatrix(5,5) );

  dataharvester::Writer::file( theFileName ) << corrs;
}

void TrackParameterHistogrammer::process ( const vertigo::Event & e )
{
  saveSimParticles ( e );

  std::vector < rave::Track > trks = e.tracks();
  for ( std::vector< rave::Track >::const_iterator i=trks.begin();
        i!=trks.end() ; ++i )
  {
    std::vector < vertigo::SimTrack > ass = e.associatedTracks ( *i );
    if ( ass.size() )
    {
      fill ( ass[0], *i, "AllTk", -1. );
    } else {
      fill ( *i, "AllUnassociatedTk", -1. );
    }
  }

  std::vector < rave::Vertex > vtces = e.vertices();
  for ( vector< rave::Vertex >::const_iterator i=vtces.begin();
        i!=vtces.end() ; ++i )
  {
    saveVertexTracks ( *i, e );
  }
}

void TrackParameterHistogrammer::saveVertexTracks ( const rave::Vertex & v,
    const vertigo::Event & e )
{
  // cout << "[TrackParameterHistogrammer] has refitted tracks." << endl;
  vector < pair < float, rave::Track > > otracks = v.weightedTracks();
  vector< pair < float, rave::Track > >::const_iterator oit=otracks.begin();

  for ( ; oit!=otracks.end() ; ++oit )
  {
    vector < vertigo::SimTrack > ass = e.associatedTracks ( oit->second );
    if ( ass.size() )
    {
      fill ( ass[0], oit->second, "VtxTk", false, oit->first );
    } else {
      fill ( oit->second, "NonAssVtxTk", false, oit->first );
    }

  }
  oit=otracks.begin(); // reset for smoothed tracks.

  if ( v.hasRefittedTracks() )
  {
    vector < rave::Track > rtracks = v.refittedTracks();
    for ( vector< rave::Track >::const_iterator t=rtracks.begin();
          t!=rtracks.end() ; ++t )
    {
      // get the associated tracks from the associations
      // with the original tracks.
      vector < vertigo::SimTrack > ass = e.associatedTracks ( oit->second );
      if ( ass.size() )
      {
        fill ( ass[0], *t, "RefittedVtxTk", true, oit->first );
      } else {
        fill ( *t, "RefittedNonAssVtxTk", true, oit->first );
      }
      oit++;
    }
  };
}

void TrackParameterHistogrammer::process ( const exception & e, const string & type )
{
  Tuple data ( "Exceptions" );
  data["what"]=e.what();
  data["type"]=type;
  dataharvester::Writer::file ( theFileName ) << data;
}

void TrackParameterHistogrammer::fill ( const rave::Track & trk, const string & name,
    bool refitted, double w )
{
  theTrackNr++;
  Tuple params ( name );
  params["track_id"]=theTrackNr;
  add ( trk, params );
  dataharvester::Writer::file( theFileName ) << params;
  // cout << "[TrackParameterHistogrammer] saving comps! " << trk.components().size() << endl;
  if ( trk.components().size() )
  {
    const vector < pair < float, rave::Track > > & components = trk.components();
    int ctr=0;
    for ( vector< pair < float, rave::Track > >::const_iterator i=components.begin();
          i!=components.end() ; ++i )
    {
      ctr++;
      ostringstream compname;
      compname << name << "_comp" << ctr;
      Tuple cparams ( compname.str() );
      add ( i->second, cparams, i->first );
      dataharvester::Writer::file( theFileName ) << cparams;
    }
  }
}

void TrackParameterHistogrammer::fill(const vertigo::SimTrack & simtrk,
       const rave::Track & trk, const string & name, bool refitted, double w )
{
  theTrackNr++;
  dataharvester::Tuple params ( name );
  add ( trk, params, w );
  add ( simtrk, params );

  RaveToCmsObjects rconverter;
  VertigoToCmsObjects vconverter;
  reco::TransientTrack tt ( rconverter.tTrack ( trk ) );

  // TrajectoryStateOnSurface recip = tt.impactPointState();

  // This should propagate the simtrack to the rectrack's surface
  // cout << "[TrackParameterHistogrammer] debug: simtrk.px=" << simtrk << endl;
  FreeTrajectoryState fts = vconverter.freeState ( simtrk );
  /* cout << "[TrackParameterHistogrammer] debug: sim gtp.px=" << fts.parameters()
       << endl; */
  // TrajectoryStateOnSurface simip ( fts.parameters(), recip.surface() );

  // checkSurface ( simip, recip );
  // add ( simip, recip, refitted, params );

  params["track_id"]=theTrackNr;
  // params["w"]=w;

 //  cout << "[TrackParameterHistogrammer] saving comps! " << trk.components().size() << endl;
  if ( trk.components().size() )
  {
    const vector < pair < float, rave::Track > > & components = trk.components();
    int ctr=0;
    for ( vector< pair < float, rave::Track > >::const_iterator i=components.begin();
          i!=components.end() ; ++i )
    {
      ctr++;
      ostringstream compname;
      compname << name << "_comp" << ctr;
      Tuple cparams ( compname.str() );
      add ( i->second, cparams, i->first );
      add ( simtrk, cparams );
      reco::TransientTrack ctt ( rconverter.tTrack ( i->second ) );
      add ( fts, *(ctt.impactPointState().freeState()), refitted, cparams );
      dataharvester::Writer::file( theFileName ) << cparams;
    }
  }

  add ( fts, *(tt.impactPointState().freeState()), refitted, params );
  /*
  cout << "[TrackParameterHistogrammer] filling stphi=" << params["stphi"].asString()
       << " for "  << name << endl;
       */
  dataharvester::Writer::file( theFileName ) << params;
}

void TrackParameterHistogrammer::write()
{
//  dataharvester::Writer::file(theFileName)->close();
}

TrackParameterHistogrammer::~TrackParameterHistogrammer() {}

#include "Analysis/Observer/interface/ObserverBuilder.h"
namespace {
  ObserverBuilder<TrackParameterHistogrammer> t( "TrackParameterHistogrammer",
      "Histograms track parameter", false );
}


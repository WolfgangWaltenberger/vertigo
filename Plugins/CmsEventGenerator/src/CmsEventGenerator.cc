/** @file
 * This implements the CMS event generator used for reading data from CMSSW
 * (*.root) files.
 */

#include "CmsEventGenerator.h"
#include "CmsBeamSpotBuilder.h"
#include "SecondaryVertexTagInfoPrinter.h"
#include "Environment/Configuration/interface/SimpleConfigurable.h"
#include <iostream>
#include <vector>
#include <rave/impl/FWCore/MessageLogger/interface/MessageLogger.h>
#include <rave/Track.h>
#include <rave/Units.h>
#include <rave/impl/RaveBase/Converters/interface/RaveStreamers.h>
#include <rave/impl/TrackingTools/TrajectoryParametrization/interface/CurvilinearTrajectoryError.h>
#include <rave/impl/TrackingTools/TrajectoryParametrization/interface/CartesianTrajectoryError.h>
#include <rave/impl/TrackingTools/AnalyticalJacobians/interface/JacobianCurvilinearToCartesian.h>
#include <rave/impl/DataFormats/CLHEP/interface/AlgebraicObjects.h>
#include <rave/impl/DataFormats/GeometryVector/interface/GlobalVector.h>
#include <rave/impl/TrackingTools/TrajectoryParametrization/interface/GlobalTrajectoryParameters.h>
#include <rave/impl/RaveTools/Converters/interface/MagneticFieldSingleton.h>

#include "TEnv.h"
#include "TLeaf.h"
#ifdef WITH_THREADS
#include <boost/thread/mutex.hpp>
#endif

using namespace std;
using namespace vertigo;

namespace {
  #ifdef WITH_THREADS
  boost::mutex mutex;
  #endif

  int covIndex( int i, int j )  {
    int a = ( i <= j ? i : j ), b = ( i <= j ? j : i );
    return b * ( b + 1 ) / 2 + a;
  }

  rave::Covariance6D createCovMatrix ( TLeaf * l_cov, const rave::Vector6D & vec, signed charge )
  {
     enum { i_qoverp = 0 , i_lambda, i_phi, i_dxy, i_dsz };

     AlgebraicSymMatrix55 curvmat;

     curvmat(i_qoverp,i_qoverp) = l_cov->GetValue ( covIndex ( i_qoverp, i_qoverp ) );
     curvmat(i_qoverp,i_lambda) = l_cov->GetValue ( covIndex ( i_qoverp, i_lambda ) );
     curvmat(i_qoverp,i_phi) = l_cov->GetValue ( covIndex ( i_qoverp, i_phi ) );
     curvmat(i_qoverp,i_dxy) = l_cov->GetValue ( covIndex ( i_qoverp, i_dxy ) );
     curvmat(i_qoverp,i_dsz) = l_cov->GetValue ( covIndex ( i_qoverp, i_dsz ) );
     curvmat(i_lambda,i_lambda) = l_cov->GetValue ( covIndex ( i_lambda, i_lambda ) );
     curvmat(i_lambda,i_phi) = l_cov->GetValue ( covIndex ( i_lambda, i_phi ) );
     curvmat(i_lambda,i_dxy) = l_cov->GetValue ( covIndex ( i_lambda, i_dxy ) );
     curvmat(i_lambda,i_dsz) = l_cov->GetValue ( covIndex ( i_lambda, i_dsz ) );
     curvmat(i_phi,i_phi) = l_cov->GetValue ( covIndex ( i_phi, i_phi ) );
     curvmat(i_phi,i_dxy) = l_cov->GetValue ( covIndex ( i_phi, i_dxy ) );
     curvmat(i_phi,i_dsz) = l_cov->GetValue ( covIndex ( i_phi, i_dsz ) );
     curvmat(i_dxy,i_dxy) = l_cov->GetValue ( covIndex ( i_dxy, i_dxy ) );
     curvmat(i_dxy,i_dsz) = l_cov->GetValue ( covIndex ( i_dxy, i_dsz ) );
     curvmat(i_dsz,i_dsz) = l_cov->GetValue ( covIndex ( i_dsz, i_dsz ) );

     CurvilinearTrajectoryError err ( curvmat ); // this one is ok!!! Compared with cmssw!!!

     GlobalPoint point ( vec.position().x(), vec.position().y(), vec.position().z() ) ;
     GlobalVector momentum ( vec.momentum().x(), vec.momentum().y(), vec.momentum().z() );
     GlobalTrajectoryParameters globalPars ( point, momentum, charge, MagneticFieldSingleton::Instance() );


     JacobianCurvilinearToCartesian curv2Cart( globalPars );
     const AlgebraicMatrix65& jac = curv2Cart.jacobian();
     CartesianTrajectoryError carterr = ROOT::Math::Similarity(jac, curvmat); // this one deviates from cmssw?
     const AlgebraicMatrix66 & m = carterr.matrix();

     rave::Covariance6D cov ( m(0,0), m(0,1), m(0,2), m(1,1), m(1,2), m(2,2), m(0,3), m(0,4), m(0,5),
                              m(1,3), m(1,4), m(1,5), m(2,3), m(2,4), m(2,5), m(3,3),
                              m(3,4), m(3,5), m(4,4), m(4,5), m(5,5) );

     static bool first=false;
     if ( first)
     {
       edm::LogError("CmsEventGenerator") << "point=" << point << endl;
       edm::LogError("CmsEventGenerator") << "momentum=" << momentum << endl;
       edm::LogError("CmsEventGenerator") << "charge=" << charge << endl;
       edm::LogError("CmsEventGenerator") << "cov=" << endl << cov;
       edm::LogError("CmsEventGenerator") << "carterr=" << endl << carterr.matrix_old();
       edm::LogError("CmsEventGenerator") << "curverr=" << endl << err.matrix();
       first=false;
     }

     return cov;
  }
}

string CmsEventGenerator::describe() const
{
  return "CmsEventGenerator";
}

CmsEventGenerator::CmsEventGenerator() : theFileName("undef"), theId(0),
  theTree(0), theHasBeamSpot(false)
{
  gEnv->SetValue("Root.ErrorIgnoreLevel","Error");
  initIdentifier();
}

void CmsEventGenerator::reset()
{
  source ( theFileName );
}

void CmsEventGenerator::source ( const string & filename )
{
  // edm::LogWarning("CmsEventGenerator") << "::source " << filename;
  theFileName=filename;
  theFile=TFile::Open ( theFileName.c_str() );
  if ( !theFile )
  {
    edm::LogError( "CmsEventGenerator" ) << "fatal: " << theFileName << " not found!";
    exit(0);
  }
  theTree = dynamic_cast < TTree * > (theFile->Get("Events"));
  if ( !theTree )
  {
    edm::LogError ( "CmsEventGenerator" ) << "fatal: no Events in file found!";
    exit(0);
  }

  int nevents = theTree->GetEntries(); // This is necessary!!!
  LogDebug("") << "\"Events\" TTree reports " << nevents << " events.";
  createBeamSpot();
  /*
  // theTree->SetBranchStatus("*",0); // dont read nothing???
  TBranch * main = theTree->GetBranch ( theRecTrackIdentifier.c_str() );
  main->GetEntry(0);
  TLeaf * l_charge = theTree->GetLeaf("recoTracks_ctfWithMaterialTracks__ALL.obj.charge_");
  LogDebug ("") << l_charge->GetLen() << " tracks!";
  */
}

void CmsEventGenerator::initIdentifier()
{
  // before 180pre8, "ALL" used to be "Rec"
  string coll=SimpleConfigurable<string>("ALL","CmsEventGenerator:Collection").value();

  theRecTrackIdentifier="recoTracks_ctfWithMaterialTracks__" + coll + ".";
  // theSimTrackIdentifier="TrackingParticles_trackingtruthprod__ALL.";
  theSimTrackIdentifier="SimTracks_g4SimHits__" + coll + ".";
  // theSimVtxIdentifier="SimVertexs_g4SimHits__" + coll + ".";
  theSimVtxIdentifier="TrackingVertexs_trackingtruthprod__" + coll + ".";
}

CmsEventGenerator::CmsEventGenerator( const string & filename ) :
    theId(0), theTree(0), theHasBeamSpot(false)
{
  initIdentifier();
  source ( filename );
}

CmsEventGenerator::~CmsEventGenerator()
{
  theFile->Close();
}


bool CmsEventGenerator::VtxSort::operator() ( const rave::Point3D & r1, const rave::Point3D & r2 ) const
{
  float dx = r1.x() - r2.x();
  float dy = r1.y() - r2.y();
  float dz = r1.z() - r2.z();
  float dist = ( dx*dx + dy*dy + dz * dz );
  if ( dist < 1.e-5 ) return false;
  return dz < 0.;
}

/**
 * Fetches the next event found in the CMS file.
 * @return The event as Vertigo structure
 */
Event CmsEventGenerator::next() const
{
  #ifdef WITH_THREADS
  boost::mutex::scoped_lock lock(mutex);
  #endif
  theSimVertices.clear();
  // theSimVertexId=0;
  Event e ( theId, 0, "CmsEvent");
  LogDebug("") << "Trying to access " << theRecTrackIdentifier << " event #" << theId;
  TBranch * tmpbrnch = theTree->GetBranch ( theRecTrackIdentifier.c_str() );
  if ( !tmpbrnch )
  {
    edm::LogError("CmsEventGenerator" )
      << "Didnt find any RecTracks in " << theRecTrackIdentifier;
    throw string ("No rectracks found.");
  }

  tmpbrnch->GetEntry(theId);
  //tmpbrnch=theTree->GetBranch("recoJetedmRefToBaseProdrecoTracksrecoTrackrecoTracksTorecoTrackedmrefhelperFindUsingAdvanceedmRefVectorsAssociationVector_ic5JetTracksAssociatorAtVertex__ALL.");
  // if ( tmpbrnch ) tmpbrnch->GetEntry(theId);
  createTracks( e );
  // createSimVertices ( e );
  createSimTracks ( e );
  e.associateTracks();
  createJets ( e );
  SecondaryVertexTagInfoPrinter::print ( theTree, theId );
  theId++;
  return e;
}

void CmsEventGenerator::createSimVertices( Event & e ) const
{
  // LogDebug("" ) << "create SimVertices for event " << theId;

  // string prefix="obj.theVertex.fCoordinates.f";
  string prefix="obj.position_.fCoordinates.f";
  TLeaf * l_vx = theTree->GetLeaf ( ( theSimVtxIdentifier + prefix + "X" ).c_str() );
  if ( l_vx )
  {
    l_vx->GetBranch()->GetEntry(theId);
    TLeaf * l_vy = theTree->GetLeaf ( ( theSimVtxIdentifier + prefix + "Y" ).c_str() );
    TLeaf * l_vz = theTree->GetLeaf ( ( theSimVtxIdentifier + prefix + "Z" ).c_str() );

    // for the tag?
    // TLeaf * l_vt = theTree->GetLeaf ( ( theSimVtxIdentifier + "obj.itrack" ).c_str() );
    TLeaf * l_vt = theTree->GetLeaf ( ( theSimVtxIdentifier + prefix + "T" ).c_str() );
    // obj.daughterTracks_.refVector_.items_
    // obj.sourceTracks_.refVector_.items_
    int nsimvtces = l_vx->GetLen();
    LogDebug ( "" ) << nsimvtces << " simvertices found.";
    for ( int i=0; i< nsimvtces ; i++ )
    {
       l_vx->GetBranch()->GetEntry(theId);
       float vx = l_vx->GetValue(i);
       l_vy->GetBranch()->GetEntry(theId);
       float vy = l_vy->GetValue(i);
       l_vz->GetBranch()->GetEntry(theId);
       float vz = l_vz->GetValue(i);

       // for the tag?
       l_vt->GetBranch()->GetEntry(theId);
       float vt = l_vt->GetValue(i);
       ostringstream o;
       o << vt;
       rave::Vector3D pos ( vx, vy, vz );
       bool isprimary=false;
       if ( fabs(vt) < 1e-22 ) isprimary=true;
       SimVertex vtx ( pos, vector < SimTrack > (), 0, o.str(), isprimary );
       e.add ( vtx );
    }
  } else {
    edm::LogError ( "CmsEventGenerator" )
      << "could not find " << theSimVtxIdentifier << prefix << "X - No SimVertices!";
  }
}

const SimVertex * CmsEventGenerator::addSimVertexFromTrack( const rave::Point3D & p, Event & e ) const
{
  if ( theSimVertices[p]!=0 ) return theSimVertices[p];
  // theSimVertexId++;
  bool isprimary=false;
  int pid=0;
  string name="";
  SimVertex m ( p, vector < SimTrack > (), pid, name, isprimary );
  e.add ( m );
  theSimVertices[p]=(&m);
  return (&m);
}

void CmsEventGenerator::createSimTracks( Event & e ) const
{
  // LogDebug("" ) << "create SimTracks for event " << theId;
  TLeaf * l_charge = theTree->GetLeaf ( ( "TrackingParticles_trackingtruthprod__ALL.obj.qx3_" ) );

  if ( l_charge )
  {
    l_charge->GetBranch()->GetEntry(theId);
    // TLeaf * l_pdg = theTree->GetLeaf ( ( theSimTrackIdentifier + "obj.pdgId_" ).c_str() );
    TLeaf * l_pdg = theTree->GetLeaf ( ( theSimTrackIdentifier + "obj.thePID" ).c_str() );
    // TLeaf * l_vert = theTree->GetLeaf ( ( theSimTrackIdentifier + "obj.ivert" ).c_str() );
    // TLeaf * l_genpart = theTree->GetLeaf ( ( theSimTrackIdentifier + "obj.igenpart" ).c_str() );
    // TLeaf * l_mass = theTree->GetLeaf ( ( theRecTrackIdentifier + "obj.mass_" ).c_str() );
    TLeaf * l_vx = theTree->GetLeaf ( ( theSimTrackIdentifier + "obj.tkposition.fCoordinates.fX" ).c_str() );
    TLeaf * l_vy = theTree->GetLeaf ( ( theSimTrackIdentifier + "obj.tkposition.fCoordinates.fY" ).c_str() );
    TLeaf * l_vz = theTree->GetLeaf ( ( theSimTrackIdentifier + "obj.tkposition.fCoordinates.fZ" ).c_str() );
    TLeaf * l_px = theTree->GetLeaf ( ( theSimTrackIdentifier + "obj.tkmomentum.fCoordinates.fX" ).c_str() );
    TLeaf * l_py = theTree->GetLeaf ( ( theSimTrackIdentifier + "obj.tkmomentum.fCoordinates.fY" ).c_str() );
    TLeaf * l_pz = theTree->GetLeaf ( ( theSimTrackIdentifier + "obj.tkmomentum.fCoordinates.fZ" ).c_str() );
    /*
    TLeaf * l_vx = theTree->GetLeaf ( ( theSimTrackIdentifier + "obj.vertex_.fCoordinates.fX" ).c_str() );
    TLeaf * l_vy = theTree->GetLeaf ( ( theSimTrackIdentifier + "obj.vertex_.fCoordinates.fY" ).c_str() );
    TLeaf * l_vz = theTree->GetLeaf ( ( theSimTrackIdentifier + "obj.vertex_.fCoordinates.fZ" ).c_str() );
    TLeaf * l_px = theTree->GetLeaf ( ( theSimTrackIdentifier + "obj.g4Tracks_.tkmomentum.fCoordinates.fX" ).c_str() );
    TLeaf * l_py = theTree->GetLeaf ( ( theSimTrackIdentifier + "obj.g4Tracks_.tkmomentum.fCoordinates.fY" ).c_str() );
    TLeaf * l_pz = theTree->GetLeaf ( ( theSimTrackIdentifier + "obj.g4Tracks_.tkmomentum.fCoordinates.fZ" ).c_str() );
    */
    if ( (!l_px) || (!l_vx) )
    {
      edm::LogError ("CmsEventGenerator" ) << "px/vx is not there! Skipping SimTrack creation!";
      return;
    }
    // obj.daughterTracks_.refVector_.items_
    // obj.sourceTracks_.refVector_.items_
    int ntracks = l_charge->GetLen();
    LogDebug ( "" ) << ntracks << " simtracks found.";
    for ( int i=0; i< ntracks ; i++ )
    {
      l_vx->GetBranch()->GetEntry(theId);
      float vx = l_vx->GetValue(i);
      l_vy->GetBranch()->GetEntry(theId);
      float vy = l_vy->GetValue(i);
      l_vz->GetBranch()->GetEntry(theId);
      float vz = l_vz->GetValue(i);
      l_px->GetBranch()->GetEntry(theId);
      float px = l_px->GetValue(i);
      l_py->GetBranch()->GetEntry(theId);
      float py = l_py->GetValue(i);
      l_pz->GetBranch()->GetEntry(theId);
      float pz = l_pz->GetValue(i);
      l_charge->GetBranch()->GetEntry(theId);
      int q=(int) (l_charge->GetValue(i)/3);

      // skip 0,0,0 tracks
      if ( px*px + py*py + pz*pz < 1e-13 ) continue;
      rave::Point3D p ( vx, vy, vz );
      const SimVertex * vtx = addSimVertexFromTrack ( p, e );
      rave::Vector6D state ( vx, vy, vz, px, py, pz );
      l_pdg->GetBranch()->GetEntry(theId);
      int pdg=(int) (l_pdg->GetValue(i)/3);

      /*
      int ntracks2=l_pdg->GetLen();
      if ( ntracks != ntracks2 )
      {
        edm::LogError("CmsEventGenerator" ) << "Argh! ntracks(" << ntracks << ")!=ntracks2(" << ntracks2
                                            << ")";
      } */
      SimTrack trk ( state, q, 0, pdg, "", vtx );
      e.add ( trk );
    }
  }
}

void CmsEventGenerator::createJets ( Event & e ) const
{
  string id="recoJetedmRefToBaseProdrecoTracksrecoTrackrecoTracksTorecoTrackedmrefhelperFindUsingAdvanceedmRefVectorsAssociationVector_ic5JetTracksAssociatorAtVertex__ALL.";
  // TBranch * b = theTree->GetBranch(id.c_str());
  // b->GetEntry(theId);
  TBranch * b = theTree->GetBranch( ( id + "present" ).c_str() );
  bool value;
  if ( !b ) return;
  b->SetAddress ( &value );
  b->GetEntry(theId);
  cout << "[CmsEventGenerator] createJets: present=" << value << endl;

  TBranch * bitems=theTree->GetBranch( ( id + "obj.data_.refVector_.items_" ).c_str() );
  if ( !bitems ) return;
  pair < const void *, vector < const void * > > pointers;
  bitems->SetAddress ( &pointers );
  // bitems->GetEntry(theId);
  // int njets = bitems->GetEntries();
  TLeaf * leaf=theTree->GetLeaf( ( id + "obj.data_.refVector_.items_" ).c_str() );
  int ntracks = leaf->GetLen();
  cout << "[CmsEventGenerator] I see " << ntracks << " tracks in jet!" << endl;
  // l->GetBranch()->GetEntry(theId);
}

void CmsEventGenerator::createTracks( Event & e ) const
{
  // LogDebug("" ) << "create Tracks for event " << theId;
  TLeaf * l_charge = theTree->GetLeaf ( ( theRecTrackIdentifier + "obj.charge_" ).c_str() );

  if ( l_charge )
  {
    int ntracks = l_charge->GetLen();
    l_charge->GetBranch()->GetEntry(theId);

    TLeaf * l_chi2 = theTree->GetLeaf ( ( theRecTrackIdentifier + "obj.chi2_" ).c_str() );
    TLeaf * l_ndf = theTree->GetLeaf ( ( theRecTrackIdentifier + "obj.ndof_" ).c_str() );
    TLeaf * l_vx = theTree->GetLeaf ( ( theRecTrackIdentifier + "obj.vertex_.fCoordinates.fX" ).c_str() );
    TLeaf * l_vy = theTree->GetLeaf ( ( theRecTrackIdentifier + "obj.vertex_.fCoordinates.fY" ).c_str() );
    TLeaf * l_vz = theTree->GetLeaf ( ( theRecTrackIdentifier + "obj.vertex_.fCoordinates.fZ" ).c_str() );
    TLeaf * l_px = theTree->GetLeaf ( ( theRecTrackIdentifier + "obj.momentum_.fCoordinates.fX" ).c_str() );
    TLeaf * l_py = theTree->GetLeaf ( ( theRecTrackIdentifier + "obj.momentum_.fCoordinates.fY" ).c_str() );
    TLeaf * l_pz = theTree->GetLeaf ( ( theRecTrackIdentifier + "obj.momentum_.fCoordinates.fZ" ).c_str() );
    TLeaf * l_cov = theTree->GetLeaf ( ( theRecTrackIdentifier + "obj.covariance_" ).c_str() );
    LogDebug ( "" ) << ntracks << " tracks found.";
    for ( int i=0; i< ntracks ; i++ )
    {
       // LogDebug("") << "create track #" << i;
       l_px->GetBranch()->GetEntry(theId);
       float px = l_px->GetValue(i);
       l_py->GetBranch()->GetEntry(theId);
       float py = l_py->GetValue(i);
       l_pz->GetBranch()->GetEntry(theId);
       float pz = l_pz->GetValue(i);
       l_vx->GetBranch()->GetEntry(theId);
       float vx = l_vx->GetValue(i);
       l_vy->GetBranch()->GetEntry(theId);
       float vy = l_vy->GetValue(i);
       l_vz->GetBranch()->GetEntry(theId);
       float vz = l_vz->GetValue(i);
       l_chi2->GetBranch()->GetEntry(theId);
       float chi2=l_chi2->GetValue(i);
       float ndf = l_ndf->GetValue(i);
       l_charge->GetBranch()->GetEntry(theId);
       int q=(int) (l_charge->GetValue(i));
       rave::Vector6D state ( vx, vy, vz, px, py, pz );
       rave::Covariance6D cov = createCovMatrix ( l_cov, state, q );
       rave::Track t ( state, cov, q, chi2, ndf, 0, "" );
       e.add ( t );
    }
  }
}

/**
 * Tell the CMS Reader to skip a given number of events.
 * @param n The number of events to skip
 */
int CmsEventGenerator::skip ( int n ) const
{
  theId += n;
  return n;
}

void CmsEventGenerator::createBeamSpot()
{
  theBeamSpot = CmsBeamSpotBuilder::build ( theTree );
  if ( theBeamSpot.isValid() )
  {
    theHasBeamSpot=true;
  } else {
    theHasBeamSpot=false;
  }
}

bool CmsEventGenerator::hasBeamSpot() const
{
  return theHasBeamSpot;
}

rave::Ellipsoid3D CmsEventGenerator::beamSpot() const
{
  return theBeamSpot;
}

#include "Vertigo/EventGeneration/interface/GeneratorBuilder.h"
namespace {
    GeneratorBuilder<CmsEventGenerator> t( "cms", "CMS File Reader" );
}


#include "CmsBeamSpotBuilder.h"
#include <string>
#include "TBranch.h"
#include "TLeaf.h"
#include <rave/impl/FWCore/MessageLogger/interface/MessageLogger.h>
#include <rave/impl/RaveBase/Converters/interface/RaveStreamers.h>
#include <rave/impl/DataFormats/BeamSpot/interface/BeamSpot.h>

using namespace std;

rave::Ellipsoid3D CmsBeamSpotBuilder::build( TTree * tree )
{
  string spot="recoBeamSpot_offlineBeamSpot__ALL.obj.";
  TBranch * b= tree->GetBranch ( "recoBeamSpot_offlineBeamSpot__ALL.obj" );
  b->GetEntries();
  // b->GetEntry(0);

  string loc=spot+"position_.fCoordinates.f";
  TLeaf * l_x=tree->GetLeaf( (loc + "X").c_str() );
  TLeaf * l_y=tree->GetLeaf( (loc + "Y").c_str() );
  TLeaf * l_z=tree->GetLeaf( (loc + "Z").c_str() );

  if ( !l_x )
  {
    edm::LogError("CmsBeamSpotBuilder") << "No BeamSpot information accessible?";
    return rave::Ellipsoid3D();
  }
  l_x->GetBranch()->GetEntry(0);
  l_y->GetBranch()->GetEntry(0);
  l_z->GetBranch()->GetEntry(0);
  float x=l_x->GetValue(0);
  float y=l_y->GetValue(0);
  float z=l_z->GetValue(0);

  reco::BeamSpot::Point p ( x, y, z );
  reco::BeamSpot::CovarianceMatrix error;

  float f[28];
  TBranch * b_array = tree->GetBranch( ( spot + "error_.fRep.fArray[28]" ).c_str() );
  b_array->SetAddress ( f );
  b_array->GetEntries();
  b_array->GetEntry(0);
  TLeaf * l_array = tree->GetLeaf( ( spot + "error_.fRep.fArray" ).c_str() );
  l_array->GetLen();
  // l_array->GetBranch()->GetEntry(0);

  error(0,0)=l_array->GetValue(0);
  error(1,0)=l_array->GetValue(1); 
  error(1,1)=l_array->GetValue(2); 
  error(2,0)=l_array->GetValue(3);
  error(2,1)=l_array->GetValue(4);
  error(2,2)=l_array->GetValue(5);
  error(3,0)=l_array->GetValue(6);
  error(3,1)=l_array->GetValue(7);
  error(3,2)=l_array->GetValue(8);
  error(3,3)=l_array->GetValue(9);
  error(4,0)=l_array->GetValue(10);
  error(4,1)=l_array->GetValue(11);
  error(4,2)=l_array->GetValue(12);
  error(4,3)=l_array->GetValue(13);
  error(4,4)=l_array->GetValue(14);
  error(5,0)=l_array->GetValue(15);
  error(5,1)=l_array->GetValue(16);
  error(5,2)=l_array->GetValue(17);
  error(5,3)=l_array->GetValue(18);
  error(5,4)=l_array->GetValue(19);
  error(5,5)=l_array->GetValue(20);
  error(6,0)=l_array->GetValue(21);
  error(6,1)=l_array->GetValue(22);
  error(6,2)=l_array->GetValue(23);
  error(6,3)=l_array->GetValue(24);
  error(6,4)=l_array->GetValue(25);
  error(6,5)=l_array->GetValue(26);
  error(6,6)=l_array->GetValue(27);
  
  TLeaf * l_width= tree->GetLeaf( ( spot + "BeamWidth_" ).c_str() );
  l_width->GetBranch()->GetEntry(0);
  float BeamWidth = l_width->GetValue(0);

  TLeaf * l_length= tree->GetLeaf( ( spot + "sigmaZ_" ).c_str() );
  l_length->GetBranch()->GetEntry(0);
  float sigmaZ = l_length->GetValue(0);

  TLeaf * l_dxdz = tree->GetLeaf( ( spot + "dxdz_" ).c_str() );
  l_dxdz->GetBranch()->GetEntry(0);
  float dxdz =  l_dxdz->GetValue(0);
  
  TLeaf * l_dydz = tree->GetLeaf( ( spot + "dydz_" ).c_str() );
  l_dydz->GetBranch()->GetEntry(0);
  float dydz =  l_dydz->GetValue(0);
  

  reco::BeamSpot tmpspot ( p, sigmaZ, dxdz, dydz, BeamWidth, error );
  /*
  cout << " -- dxdz=" << tmpspot.dxdz() << endl;
  cout << " -- dydz=" << tmpspot.dydz() << endl;
  for ( int i=0; i< 7 ; i++ )
    for ( int j=i; j< 7 ; j++ )
      cout << " -- error(" << i << "," << j << ")=" << tmpspot.covariance(i,j) << endl;
      */

  // reco::BeamSpot::Covariance3DMatrix nonrotated=tmpspot.covariance3D();
  // cout << "[CmsBeamSpotBuilder] cov=" << nonrotated << endl;

  reco::BeamSpot::Covariance3DMatrix rotated=tmpspot.rotatedCovariance3D();

  // cout << " --- rotated: cxy=" << rotated(0,1);

  rave::Point3D ravep ( x,y,z);
  rave::Covariance3D ravecov ( rotated(0,0), rotated(0,1), rotated(0,2), rotated(1,1), rotated(1,2), rotated(2,2) );

  
  return rave::Ellipsoid3D(ravep, ravecov );
}

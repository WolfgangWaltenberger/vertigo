#include "JetVertexGun.h"
#include "Vertigo/TrackSmearing/interface/GaussianRecTrackSmearer.h"
#include "DirectionalVertexGun.h"
#include "MultipleVertexGun.h"
#include "Environment/VertigoObjects/interface/VertigoJet.h"
#include "Vertigo/Event/interface/EventCombiner.h"
#include "CLHEP/Random/RandFlat.h"
#include "CLHEP/Random/RandGauss.h"
#include <vector>


using namespace std;
using namespace raveunits;

string JetVertexGun::name() const
{
  return "JetVertexGun";
}

vertigo::Event JetVertexGun::shoot()
{
  MultipleVertexGun gun ( false, "jetgun" );

  // z-coord of primary vertex is within [ -4, 4 ] cm. 
  float zp = CLHEP::RandFlat::shoot ( - 5.3 * cm , 5.3 * cm );
  float xp = CLHEP::RandFlat::shoot ( - 15.0 * um , 15.0 * um );
  float yp = CLHEP::RandFlat::shoot ( - 15.0 * um , 15.0 * um );

  // primary vertex
  GlobalPoint primvtx ( xp, yp, zp );

  float eta = CLHEP::RandFlat::shoot ( -2.4, 2.4 );
  float theta = 2 * atan ( exp ( - eta ) );

  // phi is complete 2 pi
  float phi = CLHEP::RandFlat::shoot ( 0. , 2 * M_PI * rad );

  // distance primary - secondary is flat.
  float r= CLHEP::RandFlat::shoot ( theMinFlightDistance, theMaxFlightDistance );

  float zc = zp + r * cos ( theta );
  float xc = xp + r * sin ( theta ) * sin ( phi );
  float yc = yp + r * sin ( theta ) * cos ( phi );

  // secondary vertex
  GlobalPoint secvtx ( xc , yc , zc );

  GlobalVector distvec ( secvtx.x() - primvtx.x(),
                         secvtx.y() - primvtx.y(),
                         secvtx.z() - primvtx.z() );

  // cout << "[JetVertexGun] d=" << distvec.mag() / um << endl;
  if ( fabs ( distvec.mag() - r ) > 1e-5 * cm )
  {
    cout << "[JetVertexGun] error: r=" << r << ", d=" << distvec.mag()
         << endl;
  }

  GaussianRecTrackSmearer smear ( 15. * um, .2 * millirad, 
                                  1. * MeV );

  float pr= CLHEP::RandFlat::shoot ( 20. * GeV, 40. * GeV );

  // jet vector has roughly the magnitude of pr, plus some "noise".
  // jet direction is roughly distvec.
  float noise = 3. * GeV;
  float px = pr * distvec.x() / distvec.mag() + CLHEP::RandFlat::shoot ( - noise, noise );
  float py = pr * distvec.y() / distvec.mag() + CLHEP::RandFlat::shoot ( - noise, noise );
  float pz = pr * distvec.z() / distvec.mag() + CLHEP::RandFlat::shoot ( - noise, noise );
  
  DirectionalVertexGun primary ( 9 , 11 , primvtx,
      GlobalPoint ( 0. * mm, 0. * mm , 0.*mm ), 
                    GlobalVector ( px, py, pz ) , 
                    1.3 * rad , smear,
      "JetVertexGun", true  );
  
  // secondary vertex jet has a total of 10-20 GeV
  float gev= 10. * GeV + CLHEP::RandFlat::shoot ( 0., 10. * GeV );
  GlobalVector jet ( gev / distvec.mag() * distvec );
  
  // secondary vertex has 3-5 tracks attached
  DirectionalVertexGun secondary ( 3, 5, secvtx,
      GlobalPoint ( 0. * nm, 0. * nm , 0. * nm ), jet , 
      262. * mrad, smear,
      "Secondary Vertex", false );
  gun.add ( primary );
  gun.add ( secondary );

  if ( theNOutliers > 0 )
  {
    // add outliers somewhere
    float o_r = CLHEP::RandFlat::shoot ( 5. * cm, 10. * cm );
    float ophi = CLHEP::RandFlat::shoot ( 0., 2 * M_PI );
    float otheta = CLHEP::RandFlat::shoot ( - M_PI, M_PI );
    float ox = o_r * sin ( otheta ) * cos ( ophi );
    float oy = o_r * sin ( otheta ) * sin ( ophi );
    float oz = o_r * cos ( otheta );
    for ( int i=0; i< theNOutliers ; i++ )
    {
      DirectionalVertexGun outlier ( 1, 1, primvtx,
          GlobalPoint ( 5. * mm, 5. * mm, 5. * mm ), GlobalVector ( ox, oy, oz ), 1.3 * rad, 
          GaussianRecTrackSmearer ( 230. * um, 3. * mrad, 1.7 * MeV ), "Outlier", false );

      gun.add ( outlier );
    }
  }

  vertigo::Event e = gun.shoot();
  // cout << "[JetVertexGun] we have " << e.tracks().size() << " tracks ... " << endl;
  // e.clearTracks(); // otherwise we have them twice in
  //vertigo::Jet rjet ( e.tracks() );
  // now put it all into a jet.
  /*
  vertigo::Event ret;
  ret.add(rjet);
  cout << "[JetVertexGun] adding " << e.simVertices().size() << " sim vertices." << endl;
  ret.add(e.simVertices());
  ret.add(e.simTracks());
  return ret;
  */
  
   rave::Point3D point( primvtx.x(), primvtx.y(), primvtx.z() );
   rave::Vector3D momentum ( distvec.x(), distvec.y(), distvec.z() );
   rave::Covariance6D error(1., 0., 0.,
			    1., 0., 1.,
			    0., 0., 0.,
			    0., 0., 0.,
			    0., 0., 0.,
			    1., 0., 0.,
			    1., 0., 1.);//FIXME!! how to create error??
  vertigo::Jet rjet(e.tracks(), point, momentum, error);
   

  e.add ( rjet, false );
  if ( theSecondJet )
  {
    float pt=0.;
    float px2, py2, pz2;
    while ( pt < 1. )
    {
      px2 = CLHEP::RandFlat::shoot ( -10. * GeV, 10. * GeV );
      py2 = CLHEP::RandFlat::shoot ( -10. * GeV, 10. * GeV );
      pz2 = CLHEP::RandFlat::shoot ( -10. * GeV, 10. * GeV );
      pt= px2*px2+py2*py2;
    }
  
    // a poor smearer here!
    GaussianRecTrackSmearer smear2 ( 50. * um, .2 * millirad, 
                                      1. * MeV );


    DirectionalVertexGun sjet ( 9 , 11 , primvtx,
        GlobalPoint ( 1. * um, 1. * um , 2.*um ), 
                      GlobalVector ( px2, py2, pz2 ) , 
                      1.3 * rad , smear2, "SecondJet", true );
    vertigo::Event sjevent = sjet.shoot();
    vector < rave::Track > rtrks = sjevent.tracks();
    /*
    vertigo::EventCombiner combiner;
    combiner.add ( e, sjevent );
    */
    vertigo::Jet secondjet ( rtrks );
    e.add ( secondjet, false );
    vector < vertigo::SimTrack > trk = sjevent.simTracks();
    for ( vector< vertigo::SimTrack >::iterator i=trk.begin(); i!=trk.end() ; ++i )
    {
      vertigo::SimVertex & primary = e.simVertices()[0];
      i->setVertex ( &primary );
      // cout << "[JVG] setting " << i->id() << " vtx to " << primary.id() << endl;
      primary.add ( *i );
      e.add ( *i );
    }

    for ( vector< rave::Track >::const_iterator i=rtrks.begin(); i!=rtrks.end() ; ++i )
    {
      e.add ( *i );
      vector < vertigo::SimTrack > ass = sjevent.associatedTracks ( *i ) ;
      if ( ass.size() )
      {
        e.associate ( ass[0],*i );
        // cout << "[JVG] assiating " << ass[0].id() << " with " << i->id() << endl;
      }
    }

  }
  // e.clearTracks();
  return e;
}

JetVertexGun * JetVertexGun::clone() const
{
  return new JetVertexGun ( *this );
}

JetVertexGun::JetVertexGun( float minflightdistance,
    float maxflightdistance, int noutliers, bool secondjet ) : theId ( 0 ),
  theMinFlightDistance ( minflightdistance ), theMaxFlightDistance ( maxflightdistance ),
  theNOutliers ( noutliers ), theSecondJet ( secondjet )
{}


#include "ConfigurableVertexGun.h"
#include "Vertigo/TrackSmearing/interface/NComponentRecTrackSmearer.h"
#include "Vertigo/TrackSmearing/interface/ElectronSmearer.h"
#include "DirectionalVertexGun.h"
#include "MultipleVertexGun.h"
#include "JetVertexGun.h"
#include "KinematicsVertexGun.h"

using namespace std;

ConfigurableVertexGun::ConfigurableVertexGun( const std::string & s ) :
  theName ( s ), theGun (0)
{
  setup();
}

ConfigurableVertexGun::ConfigurableVertexGun() : theName(""),theGun(0)
{
  init();
}

const map < string, string > & ConfigurableVertexGun::registered() const
{
  return theRegisteredGuns;
}

void ConfigurableVertexGun::init()
{
  theRegisteredGuns.clear();
  theRegisteredGuns["simple"]="ideal vertex with 10 tracks, angular spread of 0.5";
  theRegisteredGuns["simple_z"]="ideal vertex with 10 tracks, angular spread of 0.0001, jet momentum = (10,0,50)";
  theRegisteredGuns["simple_d"]="ideal vertex with 10 tracks, angular spread of 0.0001, jet momentum = (100,0,100 )";
  theRegisteredGuns["simple_T"]="ideal vertex with 10 tracks, angular spread of 0.0001, jet momentum = (100,0,0)";
  theRegisteredGuns["50"]="50 ideal tracks, attached to one vertex.";
  theRegisteredGuns["bjet"]="primary vertex and bjet-like secondary vertex, with some contamination";
  theRegisteredGuns["twojets"]="primary vertex and bjet-like secondary vertex, with some contamination. And a second jet at the primary vertex.";
  theRegisteredGuns["simplebjet"]="primary vertex and bjet-like secondary vertex, no contamination, distance between vertices is between 2 and 3 mm";
  theRegisteredGuns["dirty_primary"]="primary vertex with contamination";
  theRegisteredGuns["kinematics"] = "Z - mU mU with full kinematics";
  theRegisteredGuns["gauss"] = "two-component tracks for the gsf";
  theRegisteredGuns["complicated"] = "A complicated topology, CMS without much filtering";
  theRegisteredGuns["simplegauss"] = "simple testbed for the gsf";
  theRegisteredGuns["ilc"] = "simple gun for ilc -- pv at 0,0,0  sv anywhere";
  theRegisteredGuns["time"] = "A gun to study complexities -- 3 to 50 tracks";
}

void ConfigurableVertexGun::setup()
{
  if ( theGun ) delete theGun;
  // this time no fancy registration mechanism. this will only be
  // implemented, when needed.
  if ( theName == "50" )
  {
    theGun = new DirectionalVertexGun(50,50);
  } else if ( theName == "simple" )
  {
    theGun = new DirectionalVertexGun( 10, 10, GlobalPoint(0.,0.,0.), 
          GlobalPoint ( 1e-5,1e-5,.5), GlobalVector(20,0,0), .5,
         GaussianRecTrackSmearer(), "PrimaryGun", true );
  } else if ( theName == "time" )
  {
    theGun = new DirectionalVertexGun( 3, 100, GlobalPoint(0.,0.,0.), 
          GlobalPoint ( 1e-5,1e-5,.5), GlobalVector(20,0,0), .5,
         GaussianRecTrackSmearer(), "PrimaryGun", true );
  } else if ( theName == "simplegauss" )
  {
    vector < pair < float, const RecTrackSmearer * > > comps;
    GaussianRecTrackSmearer s1;
    GaussianRecTrackSmearer s2 ( .0076, .0012,.0042) ;
    comps.push_back ( pair< float, const RecTrackSmearer * > ( 0.9, &s1 ) );
    comps.push_back ( pair< float, const RecTrackSmearer * > ( 0.1, &s2 ) );
    theGun = new DirectionalVertexGun( 10, 10, GlobalPoint(0.,0.,0.), 
          GlobalPoint ( 1e-5,1e-5,.5), GlobalVector(20,0,0), .5,
         NComponentRecTrackSmearer( comps ), "PrimaryGun", true );
  } else if ( theName == "gauss" )
  {
    vector < pair < float, const RecTrackSmearer * > > comps;
    GaussianRecTrackSmearer s1 ( .002, .0003, .001 );
    GaussianRecTrackSmearer s2 ( .01 , .0015, .005 );
    comps.push_back ( pair< float, const RecTrackSmearer * > ( 0.7, &s1 ) );
    comps.push_back ( pair< float, const RecTrackSmearer * > ( 0.3, &s2 ) );
    theGun = new DirectionalVertexGun( 4, 4, GlobalPoint(0.,0.,0.), 
          GlobalPoint ( 1e-5,1e-5,.5), GlobalVector(20,0,0), .5,
         NComponentRecTrackSmearer( comps ), "PrimaryGun", true );
  } else if ( theName == "gauss10" )
  {
    vector < pair < float, const RecTrackSmearer * > > comps;
    GaussianRecTrackSmearer s1 ( .002, .0003, .001 );
    GaussianRecTrackSmearer s2 ( .01 , .0015, .005 );
    comps.push_back ( pair< float, const RecTrackSmearer * > ( .9, &s1 ) );
    comps.push_back ( pair< float, const RecTrackSmearer * > ( .1, &s2 ) );
    theGun = new DirectionalVertexGun( 10, 10, GlobalPoint(0.,0.,0.), 
          GlobalPoint ( 1e-5,1e-5,.5), GlobalVector(20,0,0), .5,
         NComponentRecTrackSmearer( comps ), "PrimaryGun", true );
  } else if ( theName == "electrons" )
  {
    vector < pair < float, const RecTrackSmearer * > > comps;
    GaussianRecTrackSmearer s1 ( .002, .0003, .001 );
    ElectronSmearer s2 ( 1., .01 , .0015, .005 );
    comps.push_back ( pair< float, const RecTrackSmearer * > ( .9, &s1 ) );
    comps.push_back ( pair< float, const RecTrackSmearer * > ( .1, &s2 ) );
    theGun = new DirectionalVertexGun( 4, 4, GlobalPoint(0.,0.,0.), 
          GlobalPoint ( 1e-5,1e-5,.5), GlobalVector(20,0,0), .5,
         NComponentRecTrackSmearer( comps ), "PrimaryGun", true );
  } else if ( theName == "electrons2" )
  {
    vector < pair < float, const RecTrackSmearer * > > comps;
    GaussianRecTrackSmearer s1 ( .003, .0003, .001 );
    ElectronSmearer s2 ( 20., .025, .0025, .008 );
    comps.push_back ( pair< float, const RecTrackSmearer * > ( .7, &s1 ) );
    comps.push_back ( pair< float, const RecTrackSmearer * > ( .3, &s2 ) );
    theGun = new DirectionalVertexGun( 4, 4, GlobalPoint(0.,0.,0.), 
          GlobalPoint ( 1e-5,1e-5,.5), GlobalVector(20,10,20), 1.3,
         NComponentRecTrackSmearer( comps, false ), "PrimaryGun", true );
  } else if ( theName == "radiatingelectrons" )
  {
    vector < pair < float, const RecTrackSmearer * > > comps;
    GaussianRecTrackSmearer s1 ( .002, .0003, .001 );
    ElectronSmearer s2 ( 1., .01 , .0015, .005 );
    comps.push_back ( pair< float, const RecTrackSmearer * > ( .7, &s1 ) );
    comps.push_back ( pair< float, const RecTrackSmearer * > ( .3, &s2 ) );
    theGun = new DirectionalVertexGun( 4, 4, GlobalPoint(0.,0.,0.), 
          GlobalPoint ( 1e-5,1e-5,.5), GlobalVector(20,0,0), .5,
         NComponentRecTrackSmearer( comps ), "PrimaryGun", true );
  } else if ( theName == "electroncloud" )
  {
    vector < pair < float, const RecTrackSmearer * > > comps;
    GaussianRecTrackSmearer s1 ( .002, .0003, .001 );
    ElectronSmearer s2 ( 10., .01 , .0015, .005 );
    comps.push_back ( pair< float, const RecTrackSmearer * > ( .7, &s1 ) );
    comps.push_back ( pair< float, const RecTrackSmearer * > ( .3, &s2 ) );
    theGun = new DirectionalVertexGun( 4, 4, GlobalPoint(0.,0.,0.), 
          GlobalPoint ( 1e-5,1e-5,.5), GlobalVector(20,0,0), 1.0,
         NComponentRecTrackSmearer( comps, false ), "PrimaryGun", true );
  } else if ( theName == "simple_z" )
  {
    // theGun = new DirectionalVertexGun();
    theGun = new DirectionalVertexGun(10,10, GlobalPoint(0.,0.,0.),
        GlobalPoint(1e-8,1e-8,.1),GlobalVector(10,0,50),.0001 );
  } else if ( theName == "simple_d" )
  {
    // theGun = new DirectionalVertexGun();
    theGun = new DirectionalVertexGun(10,10, GlobalPoint(0.,0.,0.),
        GlobalPoint(1.e-8,1.e-8,1.e-12),GlobalVector(100., 0, 100.),.0001 );
  } else if ( theName == "simple_T" )
  {
    // theGun = new DirectionalVertexGun();
    theGun = new DirectionalVertexGun(10,10, GlobalPoint(0.,0.,0.),
        GlobalPoint(1e-8,1e-8,1e-12),GlobalVector(100,0,0),.0001 );
  } else if ( theName == "dirty_primary" )
  {
    MultipleVertexGun * tmp = new MultipleVertexGun( false, "gun_dirty_primary" );
    tmp->add ( DirectionalVertexGun( 10, 10, GlobalPoint(0.,0.,0.), 
          GlobalPoint ( 1e-5,1e-5,.5), GlobalVector(20,0,0), .5,
         GaussianRecTrackSmearer(), "PrimaryGun", true ) );
    tmp->add ( DirectionalVertexGun ( 2,4, GlobalPoint(0.,0., 0. ),
                  GlobalPoint( 1e-2, 1e-2, 1e-2 ), GlobalVector(5,0,0), 1.0 ) );
    theGun = tmp;
  } else if ( theName == "complicated" )
  {
    MultipleVertexGun * tmp = new MultipleVertexGun( false, "complicated" );
    tmp->add ( DirectionalVertexGun( 20, 40, GlobalPoint(0.,0.,0.), 
          GlobalPoint ( 1e-5,1e-5,.5), GlobalVector(20,0,0), .5,
         GaussianRecTrackSmearer(), "PrimaryGun", true ) );
    tmp->add ( DirectionalVertexGun ( 5,15, GlobalPoint(0.,0., 0. ),
                  GlobalPoint( 1.e-2, 1.e-2, 1.e-2 ), GlobalVector(5.,0.,0.), 1.0 ) );
    tmp->add ( DirectionalVertexGun ( 2,7, GlobalPoint(.1, 0.01, 1.0 ),
                  GlobalPoint( 1.e-2, 1.e-2, 1.e-2 ), GlobalVector(4.,2,4), 1.0 ) );
    tmp->add ( DirectionalVertexGun ( 2,5, GlobalPoint(.05, 0.05, 0. ),
                  GlobalPoint( 1.e-1, 1.e-1, 1.e-1 ), GlobalVector(3.,1,4), 1.0 ) );
    tmp->add ( DirectionalVertexGun ( 2,5, GlobalPoint(.0, 0.0, 0. ),
                  GlobalPoint( 1.e-0, 1.e-0, 1.e-0 ), GlobalVector(1.,10.,15.), 1.0 ) );
    theGun = tmp;
  } else if ( theName == "ilc" )
  {
    MultipleVertexGun * tmp = new MultipleVertexGun( false, "ilc" );
    tmp->add ( DirectionalVertexGun( 20, 40, GlobalPoint(0.,0.,0.), 
          GlobalPoint ( 1e-11,1e-11,1e-11), GlobalVector(20,0,0), .8,
         GaussianRecTrackSmearer(), "PrimaryGun", true ) );
    tmp->add ( DirectionalVertexGun ( 2,7, GlobalPoint(0.0, 0.0, 0.0 ),
                  GlobalPoint( 1.e0, 1.e0, 1.e0 ), GlobalVector(3.,0,0), 0.3 ) );
    theGun = tmp;
  }else if ( theName == "bjet" )
  {
    theGun = new JetVertexGun();
  } else if ( theName == "twojets" )
  {
    theGun = new JetVertexGun( 50. * raveunits::um, .5 * raveunits::mm, 0, true );
  } else if ( theName == "simplebjet" )
  {
    theGun = new JetVertexGun( 2.* raveunits::mm , 3. * raveunits::mm, 0 );
  } else if ( theName.find( "kinematics", 0 ) == 0 )
  {
    KinematicsVertexGun * newgun = new KinematicsVertexGun();
    newgun->configure( theName );
    theGun = newgun;
  } else if ( theName == "time" )
  {
    theGun = new DirectionalVertexGun( 3, 50, GlobalPoint(0.,0.,0.), 
          GlobalPoint ( 1e-5,1e-5,.5), GlobalVector(50,0,0), .5,
         GaussianRecTrackSmearer(), "PrimaryGun", true );
  } else {
    cout << "[ConfigurableVertexGun] Error: unknown gun: " << theName
         << endl;
    exit(0);
  }
}

ConfigurableVertexGun::~ConfigurableVertexGun()
{
  if ( theGun ) delete theGun;
}

vertigo::Event ConfigurableVertexGun::shoot()
{
  /*
  cout << "[ConfigurableVertexGun] shoot:" << theName << ":" << theGun << endl;
  cout << "                        :" << theGun->name() << endl;
   */
  return theGun->shoot();
}

std::string ConfigurableVertexGun::name() const
{
  return theName;
}

VertexGun * ConfigurableVertexGun::clone() const
{
  return new ConfigurableVertexGun ( *this );
}

ConfigurableVertexGun::ConfigurableVertexGun ( const ConfigurableVertexGun & o ) :
  theName ( o.theName), theGun ( 0 )
{
  if ( o.theGun )
  {
    theGun=o.theGun->clone();
  }
}

void ConfigurableVertexGun::skip ( int n ) const
{
  // theId+=n;
  for ( int i=0; i< n ; i++ ) theGun->shoot();
}

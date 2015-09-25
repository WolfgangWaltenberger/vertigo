#include "Vertigo/TrackSmearing/interface/TwoGaussianRecTrackSmearer.h"
#include "Vertigo/TrackSmearing/interface/GaussianRecTrackSmearer.h"
#include "CLHEP/Random/RandFlat.h"

using namespace std;

void TwoGaussianRecTrackSmearer::printDescription() const
{
  cout << "TwoGaussianRecTrackSmearer: " << 100 * theRatio << " % outliers. (";
  if (!theFixed)
  {
    cout << "no ";
  };
  cout << "fixed sequence)." << endl;
  cout << " Inliers: " << endl;
  theFirstSmearer->printDescription();
  cout << "Outliers: " << endl;
  theSecondSmearer->printDescription();
}


TwoGaussianRecTrackSmearer * TwoGaussianRecTrackSmearer::clone () const
{
  return new TwoGaussianRecTrackSmearer ( *this );
}

TwoGaussianRecTrackSmearer::TwoGaussianRecTrackSmearer (
      const RecTrackSmearer & smearer1,
      const RecTrackSmearer & smearer2,
      float ratio, bool fixed_sequence ) : theFixed(fixed_sequence),
  theRatio ( ratio ), theCtr(0),
  theFirstSmearer ( smearer1.clone() ),
  theSecondSmearer( smearer2.clone() )
{}

TwoGaussianRecTrackSmearer::TwoGaussianRecTrackSmearer (
      float sigma_pos, float sigma_dir, float sigma_total_mom,
      float sigma_pos_o, float sigma_dir_o,
      float sigma_total_mom_o, float ratio, bool fixed_sequence ) :
  theFixed(fixed_sequence), theRatio(ratio), theCtr(0),
  theFirstSmearer(new GaussianRecTrackSmearer(sigma_pos, sigma_dir,
        sigma_total_mom, sigma_pos,  sigma_dir, sigma_total_mom ) ),
  theSecondSmearer(new GaussianRecTrackSmearer(sigma_pos_o, sigma_dir_o,
        sigma_total_mom_o, sigma_pos,  sigma_dir, sigma_total_mom ) )
{}

TwoGaussianRecTrackSmearer::TwoGaussianRecTrackSmearer( const TwoGaussianRecTrackSmearer & o )
  : theFixed ( o.theFixed ), theRatio ( o.theRatio ), theCtr ( o.theCtr ),
  theFirstSmearer ( o.theFirstSmearer->clone() ),
  theSecondSmearer ( o.theSecondSmearer->clone() )
{}

TwoGaussianRecTrackSmearer::~TwoGaussianRecTrackSmearer()
{
  delete theFirstSmearer;
  delete theSecondSmearer;
}

rave::Track
TwoGaussianRecTrackSmearer::recTrack ( 
    const GlobalTrajectoryParameters & gp ) const
{
  bool inlier = true;
  if ( theFixed )
  {
    // fixed sequence
    theCtr++;
    float every_nth = 1 / theRatio;
    if ( fabs ( fmod ( theCtr , every_nth ) ) < 1e-5 )
    {
      inlier = false;
    };
  } else {
    // purely random,
    // we have an outlier if rand(0,1) < fraction_outliers
    float rnd = CLHEP::RandFlat::shoot(0.,1.);
    if ( rnd < theRatio ) inlier = false;
  };
  if (inlier)
  {
    return  theFirstSmearer->recTrack ( gp );
  } else {
    return theSecondSmearer->recTrack ( gp );
  };
}

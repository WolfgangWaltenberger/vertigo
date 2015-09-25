#include "Vertigo/TrackSmearing/interface/NComponentRecTrackSmearer.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "RaveBase/Converters/interface/RaveStreamers.h"
#include <iostream>
#include <cstdlib>

// #define DEBUG
#ifdef DEBUG
#include <dataharvester/Writer.h>
#endif

using namespace std;

void NComponentRecTrackSmearer::printDescription() const
{
  cout << "[NComponentRecTrackSmearer] " << theSmearer.size() << "-component smearing: " << endl;
  for ( vector< pair < float, const RecTrackSmearer * > >::const_iterator 
        i=theSmearer.begin(); i!=theSmearer.end() ; ++i )
  {
    i->second->printDescription();
  }
}
  
rave::Track NComponentRecTrackSmearer::recTrack ( const GlobalTrajectoryParameters & s ) const
{
  #ifdef DEBUG
    cout << endl;
    cout << "[NComponentRecTrackSmearer] create a recTrack. SimTrack=" << s.position() << ", "
         << s.momentum() << endl;
  #endif
  vector < pair < float, rave::Track > > components;
  vector < GlobalTrajectoryParameters > deltas;

  for ( vector< pair < float, const RecTrackSmearer * > >::const_iterator i=theSmearer.begin(); 
        i!=theSmearer.end() ; ++i )
  {
    rave::Track smeared = i->second->recTrack ( s );
    // edm::LogError ("NComponentRecTrackSmearer") << "adding " << i->first << " component";
    components.push_back ( pair < float, rave::Track > ( i->first, smeared ) );
    if ( theCorrectStatisticalProperties )
      deltas.push_back ( i->second->delta ( s ) );
    #ifdef DEBUG
      cout << "[NComponentRecTrackSmearer] delta=" << i->second->delta(s).position() << i->second->delta (s).momentum() << endl;
    #endif
  }

  // if we do not want the correct quadratmitteleigenschaften,
  // then we simply return this thing here.
  if ( !theCorrectStatisticalProperties )
    return rave::Track ( components );

  // ok we want correct quadratmitteleigenschaften.
  // so we need to shift to a new mean value.

  /// now choose randomly one of the states (according to the weights),
  /// and set _all_ states to this one.
  double r = drand48();
  const rave::Track * real = &( components.begin()->second );
  for ( vector< pair < float, rave::Track > >::const_iterator i=components.begin(); 
        i!=components.end() ; ++i )
  {
    if ( r < i->first )
    {
      // we choose this one.
      real=&( i->second );
      break;
    } 
    r-=i->first;
    //componentnr++;
  }

  #ifdef DEBUG
    cout << "[NComponentRecTrackSmearer] collapsed=" << *real << endl;
  #endif
  int ctr=0;
  vector < pair < float, rave::Track > > realcomponents;
  for ( vector< pair < float, rave::Track > >::const_iterator i=components.begin(); 
        i!=components.end() ; ++i )
  {
    rave::Vector6D state ( real->state().x() - deltas[ctr].position().x(),
                           real->state().y() - deltas[ctr].position().y(),
                           real->state().z() - deltas[ctr].position().z(),
                           real->state().px() - deltas[ctr].momentum().x(),
                           real->state().py() - deltas[ctr].momentum().y(),
                           real->state().pz() - deltas[ctr].momentum().z() );

     rave::Track trk ( state, i->second.error(), real->charge(), real->chi2(), real->ndof(),
                       real->originalObject(), real->tag() );
    #ifdef DEBUG
      cout << "[NComponentRecTrackSmearer] one component=" << trk.state() << endl;
    #endif
     realcomponents.push_back ( pair < float, rave::Track > ( i->first, trk ) );

     ctr++;
  }

  /// end homogenizing states.
  
  /*
  rave::Track supertrack( realcomponents );
  dataharvester::Tuple t("supertrack");
  t["x"]=supertrack.state().x();
  t["nr"]=componentnr;
  t["sigma_x"]=sqrt( supertrack.error().dxx() );
  t["st_x"]=t["x"].asDouble() / t["sigma_x"].asDouble();
  t["w_0"]=realcomponents[0].first;
  t["x_0"]=realcomponents[0].second.state().x();
  t["sigma_x0"]=sqrt( realcomponents[0].second.error().dxx() );
  t["st_x0"]=t["x0"].asDouble() / t["sigma_x0"].asDouble();
  t["w_1"]=realcomponents[1].first;
  t["x_1"]=realcomponents[1].second.state().x();
  t["sigma_x1"]=sqrt( realcomponents[1].second.error().dxx() );
  t["st_x1"]=t["x1"].asDouble() / t["sigma_x1"].asDouble();
  dataharvester::Writer::file ( "supertrack.root" ) << t;
  */

  return rave::Track( realcomponents );
}

NComponentRecTrackSmearer * NComponentRecTrackSmearer::clone () const
{
  return new NComponentRecTrackSmearer ( *this );
}

NComponentRecTrackSmearer::NComponentRecTrackSmearer ( const vector< pair < float, const RecTrackSmearer * > > & s,
   bool correct_statistical_properties ) : theCorrectStatisticalProperties ( correct_statistical_properties )
{
  // make our own copy of this!
  for ( vector< pair < float, const RecTrackSmearer * > >::const_iterator i=s.begin(); i!=s.end() ; ++i )
  {
    theSmearer.push_back ( pair < float, const RecTrackSmearer * > ( i->first, i->second->clone() ) );
  }
}

NComponentRecTrackSmearer::NComponentRecTrackSmearer ( const NComponentRecTrackSmearer  & other ) :
  theCorrectStatisticalProperties ( other.theCorrectStatisticalProperties )
{
  // make our own copy of this!
  for ( vector< pair < float, const RecTrackSmearer * > >::const_iterator i=other.theSmearer.begin(); 
        i!=other.theSmearer.end() ; ++i )
  {
    theSmearer.push_back ( pair < float, const RecTrackSmearer * > ( i->first, i->second->clone() ) );
  }
}

NComponentRecTrackSmearer::~NComponentRecTrackSmearer()
{
  for ( vector< pair < float, const RecTrackSmearer * > >::const_iterator 
        i=theSmearer.begin(); i!=theSmearer.end() ; ++i )
  {
    delete i->second;
  }
}

#ifdef DEBUG
#undef DEBUG
#endif

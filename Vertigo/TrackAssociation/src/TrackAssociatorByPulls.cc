#include "Vertigo/TrackAssociation/interface/TrackAssociatorByPulls.h"
#include <algorithm>
#include <iostream>

using namespace std;

namespace {
  class compareRecTracks {
  public:
    compareRecTracks( const vertigo::SimTrack & simtrak, TrackDistance * trackDistance) :
         theDistance(trackDistance), sim(simtrak) {}
    bool operator()( const rave::Track & x ) const
    {
      return ( theDistance->distance(sim,x) < 0. );
    }
    bool operator()( const rave::Track & x1, const rave::Track & x2 ) const
    {
      // If the distance is < 0 the tracks are not associated
      return theDistance->distance(sim,x1) < theDistance->distance(sim,x2);
    }
  private:
    TrackDistance * theDistance;
    vertigo::SimTrack sim;
  };

  class compareSimTracks {
  public:

    compareSimTracks();

    compareSimTracks( const rave::Track & rectrak, TrackDistance * trackDistance ) :
      theDistance(trackDistance), rec(rectrak) {}
    bool operator()( const vertigo::SimTrack & x ) const
      {
        // If the distance is < 0 the tracks are not associated
        return theDistance->distance(x,rec) < 0.;
      }
    bool operator()( const vertigo::SimTrack & x1, const vertigo::SimTrack & x2 ) const
      {
        // If the distance is < 0 the tracks are not associated
        return theDistance->distance(x1,rec) < theDistance->distance(x2,rec);
      }
  private:
    TrackDistance * theDistance;
    rave::Track rec;
  };
}

TrackAssociatorByPulls::TrackAssociatorByPulls(const vector < vertigo::SimTrack > & sim,
                                               const vector < rave::Track > & rec,
                                               const TrackDistance & dist ) :
  theSimTracks(sim), theRecTracks(rec), theDistance( dist.clone() )
{ }

TrackAssociatorByPulls::TrackAssociatorByPulls(const vector < vertigo::SimTrack > & sim, 
                                               const TrackDistance & dist ) :
  theSimTracks(sim), theDistance( dist.clone() )
{
}

TrackAssociatorByPulls::TrackAssociatorByPulls(const vector < rave::Track > & rec,
                                               const TrackDistance & dist ) :
  theRecTracks ( rec) ,theDistance( dist.clone() )
{
}

vector < vertigo::SimTrack > TrackAssociatorByPulls::simTracks( const rave::Track & rectrack )
{
  vector < vertigo::SimTrack > newTracks;
  compareSimTracks comparator(rectrack,theDistance);

  for ( vector < vertigo::SimTrack >::iterator i=theSimTracks.begin();
        i!=theSimTracks.end() ; ++i )
  {
    if ( !comparator(*i) )
    {
      newTracks.push_back ( *i );
    };
  };

  /*
  compareSimTracks comparor(rectrack,theDistance);
  newTracks.sort ( comparor );

  // sort ( newTracks.begin(), newTracks.end(), comparor );
   */
  return newTracks;
}

TrackAssociatorByPulls::TrackAssociatorByPulls (
        const TrackAssociatorByPulls & other ) :
    theSimTracks( other.theSimTracks ),
    theRecTracks( other.theRecTracks ),
    theDistance ( other.theDistance->clone() )
{}

TrackAssociatorByPulls::~TrackAssociatorByPulls()
{
  if (theDistance) delete theDistance;
}

vector < rave::Track > TrackAssociatorByPulls::recTracks(const vertigo::SimTrack & simtrack )
{
  vector < rave::Track > newTracks;
  compareRecTracks comparator(simtrack,theDistance);
  for ( vector < rave::Track >::const_iterator i=theRecTracks.begin();
        i!=theRecTracks.end() ; ++i )
  {
    try {
      if ( !comparator(*i)  )
      {
        newTracks.push_back ( *i );
      };
    } catch ( exception & e ) {
      cout << "[TrackAssociatorByPulls] Exception: " << e.what() << endl;
    };
  };
  /*
  sort ( newTracks.begin(), newTracks.end(),
         compareRecTracks(simtrack,theDistance) );
         */
  return newTracks;
}

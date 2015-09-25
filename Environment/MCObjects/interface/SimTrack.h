#ifndef _SimTrack_H_
#define _SimTrack_H_

#include "Environment/MCObjects/interface/BasicSimTrack.h"

/** \class SimTrack Contains the MonteCarlo information
 * of a particle track.
 */

namespace vertigo {

class SimTrack : private BasicSimTrack::Proxy
{
    typedef BasicSimTrack::Proxy Base; 
  public:

    /**
     *  Create a SimTrack from a 6-dimensional euclidean trajectory state
     *  plus covariance matrix, plus charge, plus an optional
     *  pointer to the "original track", whatever that means
     *  
     */
    SimTrack( const rave::Vector6D &, rave::Charge,
               void * originaltrack = 0, int pid=0,
               const std::string & tag="",
               const vertigo::SimVertex * vtx = 0 );
    SimTrack( int id, const rave::Vector6D &, rave::Charge,
               void * originaltrack = 0, int pid=0,
               const std::string & tag="",
               const vertigo::SimVertex * vtx = 0 );
    SimTrack();

    rave::Charge charge() const;
    const rave::Vector6D & state() const;
    const rave::Vector3D & momentum() const;
    const rave::Point3D & position() const;
    float pt() const;
    void * originalObject() const;
    std::string tag() const;
    int id() const; //< the sim track id, should be a unique number
    /// pid the sim track particle id, e.g. 11 for electron, ...
    int pid() const; 
    bool operator< ( const SimTrack & ) const;
    bool operator== ( const SimTrack & ) const;

    void setVertex ( const vertigo::SimVertex * v );
    void setId ( int ); //< set the sim track id

    const vertigo::SimVertex * vertex() const;
};

}

#endif

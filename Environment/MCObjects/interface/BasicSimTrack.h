#ifndef _BasicSimTrack_H_
#define _BasicSimTrack_H_

#include <rave/Vector6D.h>
#include <rave/Covariance6D.h>
#include <rave/Charge.h>
#include <rave/ReferenceCounted.h>
#include <rave/ProxyBase.h>
#include <rave/CopyUsingNew.h>
#include <string>

/** \class BasicSimTrack Contains the MonteCarlo information
 * of a particle track.
 */

namespace vertigo {
  class SimVertex;
  
  class BasicSimTrack : public RaveReferenceCounted
  {
    public:

      typedef RaveProxyBase< BasicSimTrack, CopyUsingNew<BasicSimTrack> > Proxy;

      /**
       *  Create a BasicSimTrack from a 6-dimensional euclidean trajectory state
       *  plus covariance matrix, plus charge, plus an optional
       *  pointer to the "original track", whatever that means
       *  
       *  \paranmae vtx production vertex
       */
      BasicSimTrack( const rave::Vector6D &, rave::Charge,
                 void * originaltrack = 0, int pid=0, const std::string & tag="",
                 const SimVertex * vtx = 0 );
      BasicSimTrack( int id, const rave::Vector6D &, rave::Charge,
                 void * originaltrack = 0, int pid=0, const std::string & tag="",
                 const SimVertex * vtx = 0 );
      BasicSimTrack();
      ~BasicSimTrack();

      int pid() const;
      rave::Charge charge() const;
      const rave::Vector6D & state() const;
      void * originalObject() const;
      std::string tag() const;
      int id() const;
      bool operator< ( const BasicSimTrack & ) const;
      void setVertex ( const SimVertex * );
      void setId ( int );

      const SimVertex * vertex() const;

    private:
      rave::Vector6D theState;
      rave::Charge theCharge;
      void * thePointer;
      int thePid;
      std::string theTag;
      int theId;
      const SimVertex * theVertex;
  };
}

#endif

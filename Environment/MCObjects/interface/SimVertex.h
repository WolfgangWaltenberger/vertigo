#ifndef _SimVertex_H_
#define _SimVertex_H_

#include "Environment/MCObjects/interface/SimTrack.h"
#include "Environment/MCObjects/interface/BasicSimVertex.h"
#include <rave/Point3D.h>
#include <vector>
// #include <string>

namespace vertigo
{
class SimVertex : private BasicSimVertex::Proxy
{
  typedef BasicSimVertex::Proxy Base;
  /**
   *  The Monte Carlo interaction vertex.
   */
  public:
    const rave::Point3D & position() const;
    
    const std::vector < vertigo::SimTrack > & tracks() const;
    const std::vector < vertigo::SimParticle > & particles() const;

    SimVertex( const rave::Point3D &,
               const std::vector < vertigo::SimTrack > &,
               int pid=0,
               const std::string & name="",
               bool isprimary=false );
    SimVertex( const rave::Point3D &,
               const std::vector < vertigo::SimParticle > &,
               int pid=0,
               const std::string & name="",
               bool isprimary=false );
    SimVertex();

    void add ( const vertigo::SimTrack & );
    void add ( const vertigo::SimParticle & );

    std::string tag() const;

    /// the id, should be a unique number
    int id() const;
    void setId ( int );

    /// the particle id of the decaying particle,
    /// e.g. 24 for a W boson
    int pid() const;

    bool isPrimary() const;
    
    bool isValid() const;

    bool operator< ( const SimVertex & o ) const;
    bool operator== ( const SimVertex & o ) const;
};
}

#endif

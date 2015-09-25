#ifndef _BasicSimVertex_H_
#define _BasicSimVertex_H_

#include "Environment/MCObjects/interface/SimTrack.h"
#include "Environment/MCObjects/interface/SimParticle.h"
#include <rave/Point3D.h>
#include <rave/ReferenceCounted.h>
#include <rave/ProxyBase.h>
#include <rave/CopyUsingNew.h>
#include <vector>
// #include <string>

namespace vertigo
{
class BasicSimVertex : public RaveReferenceCounted
{
  /**
   *  The Monte Carlo interaction vertex.
   */
  public:

    typedef RaveProxyBase< BasicSimVertex, CopyUsingNew<BasicSimVertex> > Proxy;
    
    const rave::Point3D & position() const;
    
    const std::vector < vertigo::SimTrack > & tracks() const;
    const std::vector < vertigo::SimParticle > & particles() const;

    BasicSimVertex( const rave::Point3D &,
               const std::vector < vertigo::SimTrack > &,
               int pid=0, const std::string & tag = "",
               bool isprimary=false );

    BasicSimVertex( const rave::Point3D &,
                    const std::vector < vertigo::SimParticle > &,
                    int pid=0, const std::string & tag = "",
                    bool isprimary=false );

    BasicSimVertex();
    ~BasicSimVertex();

    void add ( const vertigo::SimTrack & );
    void add ( const vertigo::SimParticle & );

    std::string tag() const;

    int id() const;
    void setId ( int );
    int pid() const;

    bool isValid() const;
    bool isPrimary() const;

    bool operator< ( const BasicSimVertex & o ) const;

  private:
    rave::Point3D thePoint;
    std::vector < vertigo::SimTrack > theTracks;
    std::vector < vertigo::SimParticle > theParticles;
    int thePid;
    std::string theTag;
    int theId;
    bool theValid;
    bool theIsPrimary;
};
}

#endif

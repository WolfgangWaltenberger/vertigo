#include "Environment/MCObjects/interface/BasicSimVertex.h"
#include <rave/impl/RaveBase/RaveEngine/interface/RaveId.h>

using namespace vertigo;

BasicSimVertex::BasicSimVertex ( const rave::Point3D & p,
                                 const std::vector < SimTrack > & tracks,
                                 int pid, const std::string & tag, 
                                 bool isprimary ) :
    thePoint ( p ), theTracks ( tracks ), thePid ( pid ), theTag ( tag ),
    theId ( RaveId::uniqueId() ), theValid ( true ), theIsPrimary ( isprimary )
{}

BasicSimVertex::BasicSimVertex (
  const rave::Point3D & p,
  const std::vector< vertigo::SimParticle > & particles, int pid,
  const std::string & tag, bool isprimary ) :
    thePoint ( p ), theParticles ( particles ), thePid ( pid ), theTag ( tag ),
    theId ( RaveId::uniqueId() ), theValid ( true ), theIsPrimary ( isprimary )
{}

BasicSimVertex::BasicSimVertex() :
    thePid ( 0 ), theTag ( "" ), theId(-1), theValid ( false ),
    theIsPrimary ( false )
{}

BasicSimVertex::~BasicSimVertex()
{
  theTracks.clear();
  theParticles.clear();
}

std::string BasicSimVertex::tag() const
{
  return theTag;
}

int BasicSimVertex::pid() const
{
  return thePid;
}

const rave::Point3D & BasicSimVertex::position() const
{
  return thePoint;
}

void BasicSimVertex::add ( const vertigo::SimTrack & t )
{
  theTracks.push_back ( t );
}

void BasicSimVertex::add ( const vertigo::SimParticle & p )
{
  theParticles.push_back ( p );
}

const std::vector < SimTrack > & BasicSimVertex::tracks() const
{
  return theTracks;
}

const std::vector < SimParticle > & BasicSimVertex::particles() const
{
  return theParticles;
}

int BasicSimVertex::id() const
{
  return theId;
}

bool BasicSimVertex::isValid() const
{
  return theValid;
}

bool BasicSimVertex::isPrimary() const
{
  return theIsPrimary;
}

bool BasicSimVertex::operator< ( const BasicSimVertex & o ) const
{
  return ( id() < o.id() );
}

void BasicSimVertex::setId ( int id )
{
  theId=id;
}

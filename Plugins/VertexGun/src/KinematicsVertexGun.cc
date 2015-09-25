#include "KinematicsVertexGun.h"
#include "KinematicsParticleGun.h"
#include <rave/impl/FWCore/MessageLogger/interface/MessageLogger.h>
// #include <boost/spirit/core.hpp> // old
// #include <boost/spirit/actor/assign_actor.hpp> // old
// #include <boost/spirit/include/classic_core.hpp> 
// #include <boost/spirit/include/classic_assign_actor.hpp>
#include <iostream>
#include <map>
#include <utility>
#include <string>
#include <cstdlib>

using namespace std;

namespace {
  pair < string, float > decompose ( const string & n )
  {
    string::size_type pos=n.find(":");
    if ( pos==string::npos )
    {
      return pair < string, float > ( n, 0. );
    }
    return pair < string, float > ( n.substr(0,pos), atof ( n.substr(pos+1,string::npos).c_str() ) );
  }
}

KinematicsVertexGun::KinematicsVertexGun (
  const rave::Point3D & vertex, const RecTrackSmearer & smearer,
  const string & name ) : theName ( name ),
  theSmearer ( smearer.clone() )
{
  theGun = boost::shared_ptr<VertexGunFromParticleGun> (
             new VertexGunFromParticleGun ( KinematicsParticleGun() , 
               *theSmearer, theName ) );
}

void KinematicsVertexGun::configure ( const string tmpname )
{
  string name=tmpname;
  map < string, float > tokens;
  bool first=true;
  while ( name.find("-") != string::npos )
  {
    string::size_type pos = name.find("-");
    pair < string, float > token = decompose ( name.substr ( 0, pos ) );
    name = name.substr ( pos+1, string::npos );
    if (!first)
    {
      tokens[token.first]=token.second;
    }
    first=false;
  }
  pair < string, float > token = decompose ( name );
  tokens[token.first]=token.second;
  double mass = 91.1876;
  double width = .0001;
  for ( map < string, float >::const_iterator i=tokens.begin(); 
        i!=tokens.end() ; ++i )
  {
    if ( i->first == "mass" )
    {
      cout << "[KinematicsVertexGun] configuring invariant mass " << i->second
           << endl;
      mass=i->second;
    }

    if ( i->first == "width" )
    {
      cout << "[KinematicsVertexGun] configuring invariant width " << i->second
           << endl;
      width=i->second;
    }
  }
  theGun = boost::shared_ptr<VertexGunFromParticleGun> (
             new VertexGunFromParticleGun ( KinematicsParticleGun( mass, width ) , 
               *theSmearer, theName ) );
}

KinematicsVertexGun::KinematicsVertexGun ( const KinematicsVertexGun & other ) :
    theGun ( other.theGun->clone() ), theName ( other.theName )
{
}

vertigo::Event KinematicsVertexGun::shoot()
{
  return theGun->shoot();
}

string KinematicsVertexGun::name() const
{
  return theName;
}

KinematicsVertexGun * KinematicsVertexGun::clone() const
{
  return new KinematicsVertexGun ( * this );
}

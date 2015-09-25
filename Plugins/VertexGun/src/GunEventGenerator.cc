#include "GunEventGenerator.h"
#include "VertexGunPropagator.h"
#include <rave/impl/RaveBase/Converters/interface/MagneticFieldWrapper.h>
#include "ConfigurableVertexGun.h"
#include <sstream>

using namespace std;


bool GunEventGenerator::hasBeamSpot() const
{
  return false;
}

void GunEventGenerator::reset()
{
  // do nothing.
}

rave::Ellipsoid3D GunEventGenerator::beamSpot() const
{
  return rave::Ellipsoid3D();
}

GunEventGenerator::GunEventGenerator( const string & gunname ) : 
  theGun ( 0 ) //, theId(0)
{
  if ( gunname.size() )
    theGun = new ConfigurableVertexGun ( gunname );
}

GunEventGenerator::~GunEventGenerator()
{
  if (theGun) delete theGun;
}

void GunEventGenerator::source ( const string & gunname )
{
  if ( theGun ) delete theGun;
  theGun=0;
  // cout << "[GunEventGenerator] sourcing " << gunname << endl;
  if ( gunname.size() ) theGun=new ConfigurableVertexGun ( gunname );
  // theId=0;
}

std::string GunEventGenerator::describe() const
{
  ostringstream o;
  o << "Guns:" << endl
    << "=====" << endl;
  map < string, string > reg;
  if (theGun)
  {
    reg = theGun->registered();
  } else {
    ConfigurableVertexGun gun;
    reg = gun.registered();
  }
  for ( map< string, string >::const_iterator i=reg.begin(); i!=reg.end() ; ++i )
  {
    o << "   " << i->first << " (" << i->second << ")" << endl;
  }
  return o.str();
}

GunEventGenerator::GunEventGenerator() :
 theGun ( 0 ) // , theId(0)
{}

vertigo::Event GunEventGenerator::next() const
{
  if ( !theGun ) 
  {
    cout << "[GunEventGenerator] no gun registered!" << endl;
    exit(0);
  }
  // theId++;
  return theGun->shoot();
}

int GunEventGenerator::skip ( int n ) const
{
  if ( !theGun ) 
  {
    cout << "[GunEventGenerator] no gun registered!" << endl;
    exit(0);
  }
  // theId+=n;
 theGun->skip(n);
 return n;
}

#include "Vertigo/EventGeneration/interface/GeneratorBuilder.h"
namespace {
    GeneratorBuilder<GunEventGenerator> t( "gun", "VertexGun event generator" );
}


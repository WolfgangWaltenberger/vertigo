#include "HarvesterEventGenerator.h"
#include "VertigoObjectReader.h"
#include <dataharvester/TerminateException.h>

using namespace std;

bool HarvesterEventGenerator::hasBeamSpot() const
{
  return false;
}

void HarvesterEventGenerator::reset()
{
  if ( theReader ) delete theReader;
  theReader = new VertigoObjectReader ( theFilename );
  theReader->reset();
}

rave::Ellipsoid3D HarvesterEventGenerator::beamSpot() const
{
  return rave::Ellipsoid3D();
}

HarvesterEventGenerator::HarvesterEventGenerator( const string & filename ) :
  theFilename ( filename ), theReader ( 0 )
{
  if ( filename.size() )
    theReader = new VertigoObjectReader ( filename );
}

HarvesterEventGenerator::HarvesterEventGenerator ( const HarvesterEventGenerator&o )
  :  theFilename ( o.theFilename ), theReader ( 0 )
{
  if ( theFilename.size() )
    theReader = new VertigoObjectReader ( theFilename );
}

std::string HarvesterEventGenerator::describe() const
{
  return "HarvesterEventGenerator";
}

HarvesterEventGenerator::~HarvesterEventGenerator()
{
  delete theReader;
}

vertigo::Event HarvesterEventGenerator::next() const
{
  // cout << "[HarvesterEventGenerator] next event." << endl;
  return theReader->next();
}

HarvesterEventGenerator::HarvesterEventGenerator() :
  theFilename(""), theReader ( 0 )
{}

void HarvesterEventGenerator::source ( const std::string & filename )
{
  theFilename=filename;
  if ( theReader ) delete theReader;
  theReader = new VertigoObjectReader ( theFilename );
}

int HarvesterEventGenerator::skip ( int n ) const
{
  theReader->skip (n );
  return n;
}

#include "Vertigo/EventGeneration/interface/GeneratorBuilder.h"
namespace {
    GeneratorBuilder<HarvesterEventGenerator> t( "file", "dataharvester-based event generator" );
}


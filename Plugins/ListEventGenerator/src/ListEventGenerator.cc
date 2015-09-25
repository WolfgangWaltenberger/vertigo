#include "ListEventGenerator.h"
#include "Vertigo/EventGeneration/interface/GeneratorManager.h"
#include <rave/impl/FWCore/MessageLogger/interface/MessageLogger.h>
#include "Vertigo/EventLoop/interface/LoopSettings.h"
#include <dataharvester/TerminateException.h>
#include <dataharvester/Reader.h>

using namespace std;
using namespace dataharvester;

bool ListEventGenerator::hasBeamSpot() const
{
  return false;
}

void ListEventGenerator::reset()
{
  if (theCurGenerator)
  {
    theCurGenerator->reset();
  }
  Reader::file ( theFilename ).reset();
  nextGenerator();
}

rave::Ellipsoid3D ListEventGenerator::beamSpot() const
{
  return rave::Ellipsoid3D();
}

ListEventGenerator::ListEventGenerator( const string & filename ) : 
  theFilename ( filename ), theCurGenerator ( 0 )
{
  nextGenerator();
}

void ListEventGenerator::nextGenerator() const
{
  try {
    Tuple t = Reader::file ( theFilename ).next();
    // cout << "[ListEventGenerator] next: " << t.getName() << endl;
    if ( t.getName() != "source" )
    {
      cout << "[ListEventGenerator] WARNING: tuple \"" << t.getName() << "\" unknown. skipping."
           << endl;
      nextGenerator();
    }
    vector < TupleRow > ros = t.getTupleRows();
    if ( ros.size() != 1 )
    {
      cout << "[ListEventGenerator] Error! One tuple must consist of one and only one tuple row!"
           << endl;
      exit(0);
    }
    map < string, MultiType > data = ros[0].getSimpleData();
    /*
    for ( map < string, MultiType >::const_iterator i=data.begin(); i!=data.end() ; ++i )
    {
      cout << "[ListEventGenerator] " << i->first << "=" << i->second.asString() << endl;
    }*/
    
    if ( (data.count("name")==0) )
    {
      cout << "[ListEventGenerator] error: tuple \"source\" has no \"name\" column." << endl;
      cout << "                     skipping." << endl;
      nextGenerator();
    }

    string type="file";
    if ( data.count("type") ) type=data["type"].asString();
    string coll="";
    if ( data.count("collection") ) coll=string(":")+data["collection"].asString();
    theCurGenerator = GeneratorManager::Instance().get( type );
    if ( !theCurGenerator ) 
    {
      cout << "[ListEventGenerator] error: could not instantiate for type \""
           << type << "\". SKipping." << endl;
      nextGenerator();
    }
    string source=data["name"].asString()+coll;

    int skip=0;
    if ( data.count("skip") )  skip=data["skip"].asInt();
    edm::LogInfo( "ListEventGenerator" ) << "asking for \"" << type << ":" << source << "\"";
    theCurGenerator->source ( source );

    if ( skip!=0 ) theCurGenerator->skip( skip );
  } catch ( TerminateException & t ) {
    throw;
  } catch ( ... ) {
    nextGenerator();
  }
}

std::string ListEventGenerator::describe() const
{
  return "ListEventGenerator";
}

ListEventGenerator::~ListEventGenerator()
{
  // we do not delete. the manager owns.
  // if ( theCurGenerator ) delete theCurGenerator;
}

vertigo::Event ListEventGenerator::next() const
{
  try {
    return theCurGenerator->next();
  } catch ( ... ) {
    nextGenerator();
    return next();
  }
}

ListEventGenerator::ListEventGenerator() :
  theFilename(""), theCurGenerator ( 0 )
{}

void ListEventGenerator::source ( const std::string & filename )
{
  theFilename=filename;
  if ( theCurGenerator )
  {
    // delete theCurGenerator;
    theCurGenerator=0;
  }
  nextGenerator();
}

int ListEventGenerator::skip ( int n ) const
{
  return theCurGenerator->skip (n );
}

#include "Vertigo/EventGeneration/interface/GeneratorBuilder.h"
namespace {
    GeneratorBuilder<ListEventGenerator> t( "list", "A list (dataharvester-file) that contains all source files" );
}


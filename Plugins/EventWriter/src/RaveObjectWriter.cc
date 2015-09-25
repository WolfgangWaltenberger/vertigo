#include "RaveObjectWriter.h"
#include "VertigoHarvest/interface/EventToTuple.h"
#include <dataharvester/Writer.h>
#include <dataharvester/Tuple.h>

using namespace dataharvester;
using namespace rave;
using namespace std;

RaveObjectWriter::RaveObjectWriter( const string & filename ) :
    theFileName ( filename )
{}

RaveObjectWriter::~RaveObjectWriter()
{
}

void RaveObjectWriter::configure ( const std::string & f )
{
  theFileName = f;
}

void RaveObjectWriter::save ( const vertigo::Event & event, bool propagate, bool colors )
{
  Tuple t = vertigo::EventToTuple().create ( event, propagate, colors );
  Writer::file ( theFileName ) << t;
}

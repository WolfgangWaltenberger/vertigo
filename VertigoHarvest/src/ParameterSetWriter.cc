#include "VertigoHarvest/interface/ParameterSetWriter.h"

#ifdef WITH_HARVESTER
#include <dataharvester/TerminateException.h>
#include <dataharvester/HarvestingException.h>
#include <dataharvester/Tuple.h>
#include <dataharvester/Writer.h>
#endif

using namespace std;
using namespace edm;

ParameterSetWriter::ParameterSetWriter ( const string & s ) :
  filename_(s)
{}

ParameterSetWriter::~ParameterSetWriter()
{
  #ifdef WITH_HARVESTER
  dataharvester::Writer::close();
  #endif
}


void ParameterSetWriter::save ( const ParameterSet & c, const string & name )
{
  #ifdef WITH_HARVESTER
  dataharvester::Tuple prod ( name );
  prod["string"]=c.toString();
  dataharvester::Writer::file ( "[ParameterSet]"+filename_ ) << prod;
  #endif
}

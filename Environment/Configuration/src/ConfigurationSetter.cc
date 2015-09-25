#include "Environment/Configuration/interface/ConfigurationSetter.h"
#include "Environment/Configuration/interface/SimpleConfiguration.h"
#include <rave/impl/FWCore/MessageLogger/interface/MessageLogger.h>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <vector>

// forward configuration to dataharvester
#include "VertigoHarvest/interface/HarvesterConfigurationSetter.h"

using namespace std;

namespace {
  int verbose() { return 1; }

  vector < string > split ( string source )
  {
    vector < string > ret;
    while ( source.find(",") != string::npos )
    {
      string sub = source.substr ( 0, source.find(",") );
      if ( sub.size() ) ret.push_back ( source.substr ( 0, source.find(",") ) );
      source.replace ( 0, source.find(",")+1, "" );
      while ( source.size() && source[0]==' ' )
      {
        source.replace ( 0, 1, "" );
      };
    };
    if ( source.size() ) ret.push_back ( source );
    return ret;
  }

  pair < string, string > splitAtEqual ( string source )
  {
    pair < string, string > ret ( "", "" );
    string::size_type p = source.find("=");
    if ( p == string::npos )
      return ret;
    ret.first=source.substr(0,p);
    ret.second=source.substr(p+1,source.size());
    return ret;
  }
}

void ConfigurationSetter::set ( const string & data )
{
  // forward the configuration to the dataharvester
  // FIXME maybe only the parts that are prefixed with e.g. dh:?
  HarvesterConfigurationSetter::set ( data );

  vector < string > tokens = split ( data );
  for ( vector< string >::const_iterator i=tokens.begin(); 
        i!=tokens.end() ; ++i )
  {
    pair < string, string > nameandvalue=splitAtEqual ( *i );
    SimpleConfiguration::current()->modify( nameandvalue.first,
                nameandvalue.second );
    edm::LogPrint ( "ConfigurationSetter" ) << "setting " << nameandvalue.first 
           << " to " << nameandvalue.second;
  };
}

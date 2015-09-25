#include "Environment/Configuration/interface/SimpleConfiguration.h"

#include <map>
#include <iomanip>

using namespace vertigo;

namespace
{
  std::map< std::string, VMultiType > myMap;
  std::map< std::string, bool > myHas;
}

SimpleConfiguration::SimpleConfiguration()
{}

SimpleConfiguration* SimpleConfiguration::current()
{
    static SimpleConfiguration singleton;
    return &singleton;
}

void SimpleConfiguration::status() const
{
    for ( std::map < std::string, VMultiType >::const_iterator i = myMap.begin(); 
          i != myMap.end(); ++i )
    {
        std::cout << std::setw(40) << i->first << " = " << i->second.asString() << std::endl;
    };
}

void SimpleConfiguration::modify( const std::string & name, const VMultiType & value )
{
    myMap[name] = value;
    myHas[name] = true;
}

// setDefault is like modify.
// only that it doesnt override already set values.
void SimpleConfiguration::setDefault( const std::string & name, const VMultiType & value )
{
    if ( !myHas[name] )
    {
        myMap[name] = value;
        myHas[name] = true;
    };
}

VMultiType SimpleConfiguration::value( const std::string & name )
{
    return myMap[name];
}

#ifndef _HarvesterConfigurationSetter_H_
#define _HarvesterConfigurationSetter_H_

#include <string>

class HarvesterConfigurationSetter {
public:
  static void set( const std::string & ); // proxy for dataharvester::ConfigurationSetter::set()
};

#endif

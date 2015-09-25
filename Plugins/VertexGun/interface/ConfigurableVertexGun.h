#ifndef _ConfigurableVertexGun_H_
#define _ConfigurableVertexGun_H_

#include "VertexGun.h"
#include <string>
#include <map>

/**
 *   ConfigurableVertexGun; a gun
 *   that is configurable by a string.
 */

class ConfigurableVertexGun : public VertexGun {
public:
  ConfigurableVertexGun( const std::string & s );
  ConfigurableVertexGun( const ConfigurableVertexGun & );
  ConfigurableVertexGun();
  ~ConfigurableVertexGun();
  vertigo::Event shoot();
  std::string name() const;
  VertexGun * clone() const;
  void setup();
  void skip ( int n ) const;
  const std::map < std::string, std::string > & registered() const;
private:
  void init();
  std::string theName;
  VertexGun * theGun;
  std::map < std::string, std::string > theRegisteredGuns;
};

#endif // _ConfigurableVertexGun_H_

#ifndef RaveObjectWriter_H
#define RaveObjectWriter_H

#include <string>
#include <vector>
#include <map>
#include "Vertigo/Event/interface/Event.h"

class RaveObjectWriter {
public:
  RaveObjectWriter( const std::string & filename = "rave.txt" );
  ~RaveObjectWriter();
  void configure ( const std::string & filename );
  void save ( const vertigo::Event &, bool propagate=false, bool colors=false );
  
private:
  std::string theFileName;
};

#endif

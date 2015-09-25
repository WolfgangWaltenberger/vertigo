#ifndef _GeneratorManager_H_
#define _GeneratorManager_H_

#include "Vertigo/EventGeneration/interface/EventGenerator.h"
#include <map>
#include <string>

/*! \class GeneratorManager
 *  Class that manages all observers 
 *  (eventually they will have to register here)
 */

class GeneratorManager {

public:
  static GeneratorManager & Instance();
  void registerGenerator ( const std::string & name, EventGenerator * o,
                          const std::string & description );

  std::string describe ( const std::string & );
  EventGenerator * get ( const std::string &, bool verbose=true );

  std::map < std::string, EventGenerator * > get (); // get'em all

  ~GeneratorManager();

private:
  GeneratorManager ( const GeneratorManager & );
  GeneratorManager ();
  std::map < std::string, EventGenerator * > theGenerators;
  std::map < std::string, std::string > theDescription;
};

#endif // _GeneratorManager_H_

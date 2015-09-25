#ifndef _Visualiser_H_
#define _Visualiser_H_

#include "Analysis/Observer/interface/Observer.h"

/*! \class Visualiser
 * write event into file
 */

class Visualiser : public Observer {

public:
  Visualiser( const std::string & filename="out.txt" );
  void configure ( const std::string & filename );
  void process ( const vertigo::Event & );
  void process ( const std::exception &, const std::string & );
  Visualiser * clone() const;
private:
  std::string theConfig;
  bool propagate;
  bool firstEvent;
  bool fork_;
};

#endif

#ifndef _EventWriter_H_
#define _EventWriter_H_

#include "Analysis/Observer/interface/Observer.h"
#include "RaveObjectWriter.h"

/*! \class EventWriter
 * write event into file
 */

class EventWriter : public Observer {

public:
  EventWriter( std::string filename="out.txt" );
  void configure ( const std::string & filename );
  void process ( const vertigo::Event & );
  void process ( const std::exception &, const std::string & );
  EventWriter * clone() const;
private:
  std::string theFileName;
  RaveObjectWriter theWriter;
};

#endif // _EventWriter_H_

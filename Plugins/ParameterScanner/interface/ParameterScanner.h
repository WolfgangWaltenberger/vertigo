#ifndef ParameterScanner_H
#define ParameterScanner_H

#include "Analysis/Observer/interface/Observer.h"
#include "Analysis/Score/interface/SimpleScore.h"

#include <string>

class ParameterScanner : public Observer {
public:

  void process ( const vertigo::Event & );
  void process ( const std::exception &, const std::string & );
  void configure ( const std::string & );
  ~ParameterScanner();
  ParameterScanner();
  ParameterScanner * clone() const;
private:
  std::string theConf;
  SimpleScore theScore;
  unsigned theNVertices;
};

#endif

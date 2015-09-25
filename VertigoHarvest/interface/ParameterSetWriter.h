#ifndef ParameterSetWriter_H
#define ParameterSetWriter_H

#include <string>
#include "FWCore/ParameterSet/interface/ParameterSet.h"

/**
 *  \class ParameterSetWriter
 *  Thing that writes down a parameterset.
 */

class ParameterSetWriter {
public:
  ParameterSetWriter ( const std::string & filename );
  void save ( const edm::ParameterSet &, const std::string & name );
  ~ParameterSetWriter();

private:
  std::string filename_;
};

#endif

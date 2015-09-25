#ifndef _MagneticFieldWriter_H_
#define _MagneticFieldWriter_H_

#include "Analysis/Observer/interface/Observer.h"
#include <rave/impl/DataFormats/GeometryVector/interface/GlobalVector.h>
#include <rave/impl/DataFormats/GeometryVector/interface/GlobalPoint.h>
#include <vector>
#include <string>

/*! \class MagneticFieldWriter
 *  Write down the MagneticField!
 */

class MagneticFieldWriter : public Observer {

public:
  MagneticFieldWriter();
  MagneticFieldWriter ( const MagneticFieldWriter & );
  ~MagneticFieldWriter();
  void process ( const vertigo::Event & );
  void process ( const std::exception &, const std::string & );
  void configure ( const std::string & filename );
  MagneticFieldWriter * clone() const;
private:
  void write();
  void writePoint ( const GlobalVector &, const GlobalPoint &, const std::string & ) const;
  void writePoints ( const std::vector < GlobalPoint > &, const std::string & ) const;
  std::string theFileName;
  bool hasProcessed;
};

#endif

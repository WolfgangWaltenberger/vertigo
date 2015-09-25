#ifndef _VertexGun_H_
#define _VertexGun_H_

#include <vector>
#include <string>
#include <rave/Track.h>
#include "Environment/MCObjects/interface/SimTrack.h"
#include "Environment/MCObjects/interface/SimVertex.h"
#include "Vertigo/Event/interface/Event.h"

/*! Abstract base class for all VertexGun classes.
 */

class VertexGun {
public:
  virtual vertigo::Event shoot() = 0;
  virtual std::string name() const = 0;
  virtual VertexGun * clone() const = 0;
  virtual ~VertexGun() {};
};

#endif // _VertexGun_H_

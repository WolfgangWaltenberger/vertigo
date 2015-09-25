#ifndef _ParticleGun_H_
#define _ParticleGun_H_

#include <vector>
#include <string>
#include "Environment/MCObjects/interface/SimTrack.h"
#include "Environment/MCObjects/interface/SimParticle.h"
#include "Environment/MCObjects/interface/SimVertex.h"

/*! Abstract base class for all ParticleGun classes.
 */

class ParticleGun {
public:
  virtual std::vector< vertigo::SimTrack > tracks() const =0;
  virtual std::vector< vertigo::SimVertex > vertices() const =0;
  virtual std::vector< vertigo::SimParticle > particles() const =0;
  virtual void shoot() = 0;
  virtual std::string name() const = 0;
  virtual ParticleGun * clone() const = 0;
  virtual ~ParticleGun() {};
};

#endif // _ParticleGun_H_

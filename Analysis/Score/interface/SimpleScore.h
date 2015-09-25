#ifndef SimpleScore_H
#define SimpleScore_H

#include "Analysis/Score/interface/Score.h"
#include <string>

class SimpleScore : public Score {
public:
  void operator() ( const vertigo::Event & );

  /// What fraction of sim vertices has been reconstructible?
  Measurement1D efficiency() const;

  /// How many fakes do we have per rec vtx?
  Measurement1D fake() const;

  const std::string & algorithm() const;
  SimpleScore();
  ~SimpleScore();
  SimpleScore * clone() const;
private:
  int theFound;
  int theFake;
  int theN;
  std::string theMethod;
};

#endif

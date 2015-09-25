#ifndef HepMCEventGenerator_H
#define HepMCEventGenerator_H

#include <string>
#include "Vertigo/EventGeneration/interface/EventGenerator.h"
#include "Vertigo/TrackSmearing/interface/GaussianRecTrackSmearer.h"
#include "HepMC/IO_HEPEVT.h"
#include "HepMC/GenEvent.h"                                                     
#include "HepMC/IO_GenEvent.h"                                                     

class HepMCEventGenerator : public EventGenerator {
public:
  HepMCEventGenerator( const std::string & filename );
  HepMCEventGenerator();
  HepMCEventGenerator( const HepMCEventGenerator & );
  ~HepMCEventGenerator();
  void source ( const std::string & filename );
  std::string describe() const;
  vertigo::Event next() const;
  int skip ( int n ) const;
  bool hasBeamSpot() const;
  rave::Ellipsoid3D beamSpot() const;

private:
  vertigo::Event fromGenEvent ( HepMC::GenEvent * ) const;
  void add ( vertigo::Event &, const vertigo::SimParticle & ) const;
  void addVertex ( vertigo::Event & event,
      vertigo::SimTrack & ) const;
  void addParticle ( vertigo::Event & event, 
    const HepMC::GenParticle & p ) const;
  std::string config_;
  std::string generator_;
  mutable HepMC::IO_HEPEVT hepevtio_;
  mutable HepMC::IO_GenEvent * genevt_;
  mutable int evtnr_;
  mutable int pid_;
  mutable int vtxid_;
  GaussianRecTrackSmearer smearer_;
  mutable std::vector < vertigo::SimVertex > vertices_;
  bool pythia_;
  bool file_;
};

#endif

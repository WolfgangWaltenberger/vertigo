#ifndef VertigoObjectReader_H
#define VertigoObjectReader_H

#include "Vertigo/Event/interface/Event.h"
#include <rave/Ellipsoid3D.h>
#include <string>
#include <map>
#include <vector>
#include <dataharvester/TupleRow.h>
#include <dataharvester/Tuple.h>

class VertigoObjectReader {
  /**
   *  Can currently not be run in parallel!
   */
public:
  VertigoObjectReader( const std::string & filename = "rave.txt" );
  vertigo::Event next();
  void skip ( int ) const;
  void reset(); // reset the reader!

  /// Is there information on the beamspot constraint in the file?
  bool hasBeamSpot() const;
  const rave::Ellipsoid3D & beamspot() const;

private:
  void getRunInformation ( dataharvester::Tuple & );
  void nextTuple() const;
  void keepAssociation ( const vertigo::SimTrack & t,
       const std::string & vtxid );
  void keepTrackAssociation ( const vertigo::SimTrack & t, const std::string & trkid );
  void keepParticleAssociation ( const vertigo::SimParticle & t, const std::string & trkid );
  void clear();
  void implementAssociations ( vertigo::Event & e );
  void addTrack ( vertigo::Event & e, const dataharvester::TupleRow & );
  void addSimTrack ( vertigo::Event &e, std::map < std::string, dataharvester::MultiType > & data );
  void addSimParticle ( vertigo::Event &e, std::map < std::string, dataharvester::MultiType > & data );
  void addSimVertex ( vertigo::Event &e, std::map < std::string, dataharvester::MultiType > & data );
  void addTracks ( vertigo::Event & e, dataharvester::Tuple & t );
  void addVtces ( vertigo::Event & e, dataharvester::Tuple & t );
  void addSimVtces ( vertigo::Event & e, dataharvester::Tuple & t );
  void addSimParticles ( vertigo::Event & e, dataharvester::Tuple & t );
  void addSimTracks ( vertigo::Event & e, dataharvester::Tuple & t );
  // needs: addJet, addJets?
  void addJet ( vertigo::Event & e, dataharvester::Tuple & t );
  
  void addParticle ( vertigo::Event &e, std::map < std::string, dataharvester::MultiType > & data );
  void addParticles ( vertigo::Event & e, dataharvester::Tuple & t );
private:
  std::string theFileName;
  bool theHasTrackAssociations;
  mutable dataharvester::Tuple theCurrentTuple;

  std::map < std::string, std::vector < vertigo::SimTrack > > theAss;
  std::map < std::string, vertigo::SimVertex > theVtces;
  std::map < vertigo::SimTrack , std::vector < std::string > > theTrkAss;
  std::map < vertigo::SimParticle , std::vector < std::string > > theParticleAss;
  std::map < std::string, rave::Track > theTrks;
  std::map < std::string, rave::KinematicParticle > theParticles;

  bool theHasBeamSpot;
  rave::Ellipsoid3D theBeamspot;
};

#endif

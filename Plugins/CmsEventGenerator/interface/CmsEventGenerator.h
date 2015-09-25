/** @file
 *
 * This defines the cms event generator used for reading
 * track data from cms (*.root) files.
 */

#ifndef CmsEventGenerator_H
#define CmsEventGenerator_H

#include "Vertigo/EventGeneration/interface/EventGenerator.h"
#include <rave/Point3D.h>
#include <string>
#include <map>
#include "TFile.h"
#include "TTree.h"

/**
 * The actual EventGenerator class with the common interface.
 */
class CmsEventGenerator : public EventGenerator
{
public:
  // FIXME implement RecoParticle::getStartVertex SimVertex.
  CmsEventGenerator( const std::string & filename );
  CmsEventGenerator();
  void source ( const std::string & filename );
  void reset();
  std::string describe() const;

  ~CmsEventGenerator();
  vertigo::Event next() const;  //!< Fetches the contained events one by one.
  int skip ( int n ) const;  //!< Skips some events if necessary.
  bool hasBeamSpot() const;
  rave::Ellipsoid3D beamSpot() const;

private:
  void initIdentifier(); //< initialise the identifier strings
  void createTracks( vertigo::Event & e ) const; //< create rave::Tracks
  void createJets ( vertigo::Event & e ) const; //< create vertigo::Jets
  void createSimVertices( vertigo::Event & e ) const; //< create vertigo::SimVertices
  void createSimTracks( vertigo::Event & e ) const; //< create vertigo::SimTracks
  void createBeamSpot( ); //< create BeamSpot constraint
  const vertigo::SimVertex * addSimVertexFromTrack( const rave::Point3D &, vertigo::Event & e ) const; 

private:
  std::string theFileName;  //!< Holds the filename of the Cms Input File
  mutable int theId; //< event id
  TFile * theFile;
  TTree * theTree;
  std::string theRecTrackIdentifier; //< the name of the reco::track tuples
  std::string theSimTrackIdentifier; //< the name of the tracking truth tuples
  std::string theSimVtxIdentifier; //< the name of the vertex truth tuples
  bool theHasBeamSpot;
  rave::Ellipsoid3D theBeamSpot;
  struct VtxSort {
    bool operator() ( const rave::Point3D & r1, const rave::Point3D & r2 ) const;
  };

  mutable std::map < rave::Point3D, const vertigo::SimVertex *, VtxSort > theSimVertices;
  // mutable int theSimVertexId;
};

#endif

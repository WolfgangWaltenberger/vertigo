/** @file
 *
 * This defines the obvious converter class. 
 * Use the included #defines to select either
 * \li the naughty Brahms->Rave or
 * \li the neat L3->Rave conversion.
 */

#ifndef _BrahmsLcioToRaveObjects_H_
#define _BrahmsLcioToRaveObjects_H_

#include "EVENT/Track.h"
#include "EVENT/MCParticle.h"

#include <rave/Track.h>
#include "Environment/MCObjects/interface/SimTrack.h"

/**
 * Converter class to kick the track data read from some
 * LCIO file into usable (=Rave) parametrisation.
 * This is for the Brahms parametrisation
 * which is used by the hacked track fitter
 * (hacked version of Steve Aplin's delphi fitter)
 */

class BrahmsLcioToRaveObjects
{
public:
  rave::Track convert ( const EVENT::Track * ) const ; //!< Converts LCIO Tracks to Rave Tracks
  rave::Point3D convert ( const double * ) const ;     //!< Convert double array to 3D point
  vertigo::SimTrack associate ( const EVENT::MCParticle *, const vertigo::SimVertex & ) const ; //!< Convert a LCIO particle to a track
};

#endif

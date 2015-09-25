/**
 * \file L3LcioToVertigoObjects.h
 */

#ifndef _L3LcioToVertigoObjects_H_
#define _L3LcioToVertigoObjects_H_

#include "EVENT/MCParticle.h"

#include <string>
#include "Environment/MCObjects/interface/SimTrack.h"
#include "Environment/MCObjects/interface/SimParticle.h"
#include <boost/random/linear_congruential.hpp>
#include <boost/random/uniform_real.hpp>
#include <boost/random/normal_distribution.hpp>
#include <boost/random/variate_generator.hpp>

/**
 * Converter class to use MC data read from LCIO files within Vertigo
 *
 * @author Fabian Moser
 * @date   2006-08-07
 */

class L3LcioToVertigoObjects
{
  public:
    /**
     *  @param Bz magnetic field, z-coordinate, in Tesla
     */
    L3LcioToVertigoObjects ();
    ~L3LcioToVertigoObjects();

    /**
     *  A constructor which implements an "offset",
     *  a fake beamspot as long as there isnt a smeared
     *  primary vertex in the lcio data.
     *  the offsets are the sigmas of a Gaussian distribution.
     *  The "real" offsets are created randomly when calling
     *  ::createOffset();
     */
    L3LcioToVertigoObjects ( float xoffset, float yoffset, float zoffset );

    void createOffset();

    vertigo::SimParticle convert( const EVENT::MCParticle * ) const;

    vertigo::SimTrack associate ( const EVENT::MCParticle *,
                                  const vertigo::SimVertex &,
                                  const std::string & tag = "" ) const ; //!< Convert a LCIO particle to a track

    void printOffset() const;
    void defineOffset ( float x, float y, float z );

  private:
    typedef boost::minstd_rand base_generator_type;
    typedef boost::variate_generator< base_generator_type&, boost::normal_distribution< float > >
    normal_generator_type;

    /** \paramname theHasOffset create an offset on all euclidean points.
     *  This implements a fake beamspot, as long as lcio data
     *  dont sport a "smeared" primary.
     **/
    bool theHasOffset;
    base_generator_type theGenerator;
    normal_generator_type * theXDelta;
    normal_generator_type * theYDelta;
    normal_generator_type * theZDelta;
    float theCurX; // the current "delta_x"
    float theCurY; // the current "delta_y"
    float theCurZ; // the current "delta_z"

};

#endif

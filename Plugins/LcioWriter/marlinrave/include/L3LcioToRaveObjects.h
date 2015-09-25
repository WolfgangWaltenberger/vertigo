#ifndef L3LcioToRaveObjects_H
#define L3LcioToRaveObjects_H

#include "EVENT/Track.h"
#include "EVENT/Vertex.h"
#include "EVENT/ReconstructedParticle.h"

#include "rave/Ellipsoid3D.h"
#include "rave/KinematicParticle.h"
#include "rave/Track.h"
#include "rave/Vertex.h"

#include "boost/random/linear_congruential.hpp"
#include "boost/random/uniform_real.hpp"
#include "boost/random/normal_distribution.hpp"
#include "boost/random/variate_generator.hpp"

#include <string>
#include <vector>

namespace marlinrave {

/**
 * @brief Converts LCIO classes to their Rave siblings
 * @ingroup MarlinRave
 * @author Fabian Moser
 *
 * Converter class to translate the data read from some and LCIO database 
 * into the Rave parametrisation.
 */
class L3LcioToRaveObjects
{
  public:
    /**
     * @brief Initializes the instance and uses an optional magnetic field 
     *        strength.
     *
     * @param Bz The z coordinate of the magnetic field in Tesla
     */
    L3LcioToRaveObjects( float Bz = 4.0 );

    /**
     * @brief Constructor with beamspot information.
     *
     * The additional beamspot information is used during conversion of 
     * particles without a defined start vertex. The given beamspot is used 
     * like a default primary vertex.
     *
     * @param beamspot Additional beamspot information
     * @param Bz The z coordinate of the magnetic field in Tesla
     */
    L3LcioToRaveObjects( const rave::Ellipsoid3D & beamspot, float Bz = 4.0 );

    /**
     * @brief Constructor for beamspot simulation.
     *
     * A constructor which implements an "offset", a fake beamspot as long as 
     * there isnt a smeared primary vertex in the lcio data. The offsets are the 
     * sigmas of a Gaussian distribution. The "real" offsets are created 
     * randomly when calling createOffset()
     *
     * @param Bz magnetic field, z-coordinate, in Tesla
     * @param xoffset Sigma of the additional offset on x-coordinate conversions
     * @param yoffset Sigma of the additional offset on y-coordinate conversions
     * @param zoffset Sigma of the additional offset on z-coordinate conversions
     */
    L3LcioToRaveObjects( float Bz, float xoffset, float yoffset, float zoffset );

    ~L3LcioToRaveObjects();

    void createOffset();

    /// Converts LCIO Tracks to Rave Tracks
    rave::Track convert( 
        EVENT::Track *, EVENT::LCObject * origin = 0, 
        const std::string & tag = "" ) const;
    rave::KinematicParticle convert(
        EVENT::ReconstructedParticle * lcioParticle, 
        std::vector< float > cov = std::vector< float >(), 
        const std::string & tag = "") const;
    rave::Point3D convert ( const float * ) const ; //!< Convert float array to 3D point
    rave::Point3D convert ( const double * ) const ; //!< Convert double array to 3D point
    rave::Covariance3D convert ( const std::vector< float > cov ) const; //!< Convert a float vector to a 3D covariance matrix
    rave::Vertex convert ( const EVENT::Vertex * lcioVertex ) const;

    void printOffset() const;
    void defineOffset( float x, float y, float z );
    void defineBz( float );
    void defineBeamspot( const rave::Ellipsoid3D & beamspot );
  private:
    typedef boost::minstd_rand base_generator_type;
    typedef boost::variate_generator< base_generator_type&, boost::normal_distribution< float > >
      normal_generator_type;

    float theBz;
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

    rave::Ellipsoid3D theBeamspot;
    bool theHasBeamspot;

};

} // namespace marlinrave

#endif

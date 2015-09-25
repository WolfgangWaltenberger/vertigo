#ifndef DelphiToRaveObjects_H
#define DelphiToRaveObjects_H

#include "rave/Ellipsoid3D.h"
#include "rave/KinematicParticle.h"
#include "rave/Track.h"
#include "rave/Vertex.h"
#include <dataharvester/Tuple.h>

#include "boost/random/linear_congruential.hpp"
#include "boost/random/uniform_real.hpp"
#include "boost/random/normal_distribution.hpp"
#include "boost/random/variate_generator.hpp"

#include <string>
#include <vector>

namespace vertigo {

/**
 * @brief Converts LCIO classes to their Rave siblings
 * @ingroup vertigo
 * @author Fabian Moser
 *
 * Converter class to translate the data read from some and LCIO database 
 * into the Rave parametrisation.
 */
class DelphiToRaveObjects
{
  public:
    /**
     * @brief Initializes the instance and uses an optional magnetic field 
     *        strength.
     *
     * @param Bz The z coordinate of the magnetic field in Tesla
     */
    DelphiToRaveObjects( float Bz = 4.0 );

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
    DelphiToRaveObjects( const rave::Ellipsoid3D & beamspot, float Bz = 4.0 );

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
    DelphiToRaveObjects( float Bz, float xoffset, float yoffset, float zoffset );

    ~DelphiToRaveObjects();

    void createOffset();

    /// Converts delphi-parametrised tracks to Rave Tracks
    rave::Track toTrack ( dataharvester::Tuple & ) const;
    rave::KinematicParticle toKinParticle( dataharvester::Tuple & ) const;
    rave::Point3D toPoint ( dataharvester::Tuple & ) const ; //!< Convert float array to 3D point
    rave::Covariance3D toCov3D ( dataharvester::Tuple & cov ) const; //!< Convert to a 3D covariance matrix
    rave::Vertex toVertex ( dataharvester::Tuple & ) const;

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

} // namespace vertigo

#endif

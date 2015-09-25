#ifndef RAVETOL3LCIOOBJECTS_H
#define RAVETOL3LCIOOBJECTS_H

#include "rave/Track.h"
#include "rave/Vertex.h"
#include "rave/KinematicTree.h"

#include "IMPL/VertexImpl.h"
#include "IMPL/TrackImpl.h"
#include "IMPL/ReconstructedParticleImpl.h"
#include "IMPL/LCCollectionVec.h"

#include "boost/tuple/tuple.hpp"

namespace marlinrave {

/**
 * @brief Converts Rave classes to their LCIO siblings
 * @ingroup MarlinRave
 * @author Fabian Moser
 *
 * Converter class to translate the data returned from the Rave reconstruction
 * methods into the LCIO parametrisation.
 */

class RaveToL3LcioObjects
{
  public:
    /**
     * @brief Initializes the instance and uses an optional magnetic field 
     *        strength.
     *
     * @param Bz The z coordinate of the magnetic field in Tesla
     */
    RaveToL3LcioObjects( double Bz = 4.0 );

    /**
     * @brief Used for automatical cleanup of boost classes.
     */
    ~RaveToL3LcioObjects();

    /**
     * @brief Converts Rave vertices to LCIO vertices.
     */
    IMPL::VertexImpl* convert( const rave::Vertex & raveVertex ) const;

    /**
     * @brief Converts Rave tracks to LCIO tracks.
     *
     * @param refPoint Reference point for the parameter conversion.
     */
    IMPL::TrackImpl* convert( 
        const rave::Track & raveTrack, const rave::Point3D & refPoint ) const;

    /**
     * @brief Converts Rave particles to LCIO particles.
     *
     * @return The converted particle together with its start vertex
     */
    boost::tuple< IMPL::ReconstructedParticleImpl*, IMPL::VertexImpl* >
    convert (
        const rave::KinematicParticle & raveParticle,
        IMPL::VertexImpl* lcioVertex = 0 );

    /**
     * @brief Converts a decay tree into two LCIO collections, one filled with
     *        the particles and one filled with the vertices contained in the 
     *        tree.
     */
    boost::tuple< IMPL::LCCollectionVec*, IMPL::LCCollectionVec* > convert(
        const rave::KinematicTree & raveTree );

  private:
    double theBz;
};

} // namespace marlinrave

#endif


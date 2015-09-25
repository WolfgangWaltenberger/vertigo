#ifndef LDCMagneticField_H
#define LDCMagneticField_H

#include "rave/MagneticField.h"

namespace marlinrave {

/** 
 * @brief Makes sure we can access the "exterior" magnetic field from within 
 *        Rave.
 * @ingroup MarlinRave
 * @author Fabian Moser
 *
 * This class allows for detailed specification of the magnetic field inside 
 * the detector. Currently it implements a homogenous magnetic field of 4 Tesla
 * in the z direction.
 */

class LDCMagneticField : 
      public rave::MagneticField
{
  public:
	/**
	 * @brief Initializes the magnetic field according to information from the GEAR file
	 *
	 * @param tesla The z coordinate of the magnetic field
	 */
	LDCMagneticField();
	/**
	 * @brief Initializes the magnetic field to the given field strength
	 *
	 * @param tesla The z coordinate of the magnetic field
	 */
	LDCMagneticField( double tesla);

    /**
     * @brief Queries the magnetic field at some given point.
     *
     * @param where The point at which the magnetic field needs to be evaluated
     */
    rave::Vector3D inTesla( const rave::Point3D & where ) const;

    /**
     * @return A copy of the current instance.
     */
    LDCMagneticField * copy() const;

  private:
    double theValue;
};

} // namespace marlinrave

#endif


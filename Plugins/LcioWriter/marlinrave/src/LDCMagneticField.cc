#include "LDCMagneticField.h"

#include <marlin/Global.h>
#include <marlin/Processor.h>
#include <gear/BField.h>
#include <gearimpl/Vector3D.h>

using namespace marlin;

namespace marlinrave {

LDCMagneticField::LDCMagneticField()
{
	try {
		theValue = float(marlin::Global::GEAR->getBField().at( gear::Vector3D( 0., 0., 0.) ).z());
	} catch (gear::UnknownParameterException e) {
		// if bfield is not defined in gear
		// use 1.5 tesla as default
		theValue = 1.5;
		streamlog_out(ERROR4) << "MarlinRave LDCMagneticField: "
				"can not read magnetic field from gear file. Using the default value "
				 << theValue << " as fallback!" << std::endl;
	}
}

LDCMagneticField::LDCMagneticField( double v ) : 
    theValue (v ) 
{
}

rave::Vector3D LDCMagneticField::inTesla( const rave::Point3D & where ) const
{
  return rave::Vector3D ( 0. , 0. , theValue );
}

LDCMagneticField * LDCMagneticField::copy() const
{
  return new LDCMagneticField ( *this );
}

} // namespace marlinrave


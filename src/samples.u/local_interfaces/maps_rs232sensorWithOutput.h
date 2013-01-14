////////////////////////////////
// SDK Programmer samples
////////////////////////////////

#ifndef _MAPSRS232SensorWithOutput_H
#define _MAPSRS232SensorWithOutput_H

#include "maps.hpp"

class MAPSRS232SensorWithOutput : public MAPSComponent 
{
	MAPS_COMPONENT_STANDARD_HEADER_CODE(MAPSRS232SensorWithOutput)

private :
	void InitializeSensor();
};

#endif

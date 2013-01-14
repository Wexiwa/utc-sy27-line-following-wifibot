////////////////////////////////
// RTMaps SDK Sample Component
////////////////////////////////

/* Chapter 7 - Stream.
 * This section demonstrates how to write components that
 * deal with stream data (packets of bytes).
 * Usually, such stream data contain messages that have to be parsed
 * but the packets may not contain entire messages (a packet can start in the
 * middle of a message, and end in the middel of the next one), hence the
 * need for a way to push the packets into an internal ring buffer, and
 * pop data whan a complete message has been exploited.
*/

/*! \ingroup Chapter7
 * \file
 * \brief RS232Sensor
 * This is a sample of RS232 sensor component.
 * This module has one input that must be connected to an RS232 component
 * output.  It receives the data and just display it in the console window
*/

#include "maps_rs232sensor.h"	// Includes the header of this component

MAPS_BEGIN_INPUTS_DEFINITION(MAPSRS232Sensor)
    // Use a Stream8 input to receive data. Stream8 data type is a sequence of 
    // bytes (cf to your programmer reference manual).
    MAPS_INPUT("iStream8",MAPS::FilterStream8,MAPS::FifoReader)
MAPS_END_INPUTS_DEFINITION

// Use the macros to declare your sensor outputs
MAPS_BEGIN_OUTPUTS_DEFINITION(MAPSRS232Sensor)
MAPS_END_OUTPUTS_DEFINITION

// Use the macros to declare the properties
MAPS_BEGIN_PROPERTIES_DEFINITION(MAPSRS232Sensor)
MAPS_END_PROPERTIES_DEFINITION

// Use the macros to declare the actions
MAPS_BEGIN_ACTIONS_DEFINITION(MAPSRS232Sensor)
MAPS_END_ACTIONS_DEFINITION

// Use the macros to declare this component (RS232Sensor) behaviour
MAPS_COMPONENT_DEFINITION(MAPSRS232Sensor,"RS232Sensor","1.0",128,
			  MAPS::Threaded,MAPS::Threaded,1,0,0,0)

void MAPSRS232Sensor::Birth()
{
}

void MAPSRS232Sensor::Core() 
{
	unsigned char *receivedData;
	// Makes an infinite loop
	// Wait for new data from the sensor
	MAPSIOElt *ioElt=StartReading(Input("iStream8"));
	// Check the ioElt returned
	if (ioElt==NULL)
		return;
	// We've got the input/output buffer element : the data is pointed 
	// by Stream8() method
	receivedData=ioElt->Stream8();
	// Prepares to display the data
	MAPSStreamedString str;
	str<<"Received bytes :";
	// ioElt->VectorSize() contains the number of bytes in the stream : for 
	// all bytes do...
	str<<MAPSSManip::hex;
	for(int i=0;i<ioElt->VectorSize();i++)
		// print the data in the display stream
		str<<"0x"<<(int)receivedData[i]<<" ";
	// Reports the string to the console window
	ReportInfo(str);
}

void MAPSRS232Sensor::Death()
{
}

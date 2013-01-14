////////////////////////////////
// RTMaps SDK Sample Component
////////////////////////////////

/*! \ingroup Chapter7
 * \file
 * \brief RS232SensorWithOutput
 * This is a sample of RS232 sensor component.
 * This module has one input that must be connected to an RS232 component
 * output. It also has one output to send bytes streams to the sensor via
 * RS232 component. And it has one more output to transmit sensor decoded 
 * data to other RTMaps consumer components
*/

#include "maps_rs232sensorWithOutput.h"	// Includes the header of this component

#define MAX_OUTPUT_STREAM_SIZE 256
#define MAX_OUTPUT_VECTOR_SIZE 256

MAPS_BEGIN_INPUTS_DEFINITION(MAPSRS232SensorWithOutput)
    // Use a Stream8 input to receive data. Stream8 data type is a sequence of 
    // bytes (cf to your programmer reference manual).
    MAPS_INPUT("iStream8",MAPS::FilterStream8,MAPS::FifoReader)
MAPS_END_INPUTS_DEFINITION

MAPS_BEGIN_OUTPUTS_DEFINITION(MAPSRS232SensorWithOutput)
    // Use a Stream8 output to send data to the serial port. 
    MAPS_OUTPUT("oStream8",MAPS::Stream8,NULL,NULL,MAX_OUTPUT_STREAM_SIZE)
    // Use an integer vector to transmit decoded data (built from input stream)
    MAPS_OUTPUT("oInteger",MAPS::Integer,NULL,NULL,MAX_OUTPUT_VECTOR_SIZE)
MAPS_END_OUTPUTS_DEFINITION

MAPS_BEGIN_PROPERTIES_DEFINITION(MAPSRS232SensorWithOutput)
MAPS_END_PROPERTIES_DEFINITION

MAPS_BEGIN_ACTIONS_DEFINITION(MAPSRS232SensorWithOutput)
MAPS_END_ACTIONS_DEFINITION

MAPS_COMPONENT_DEFINITION(MAPSRS232SensorWithOutput,"RS232SensorWithOutput","1.0",128,
			  // Can be thread or sequential ; start as threaded
			  MAPS::Sequential|MAPS::Threaded,MAPS::Sequential,
			  1,2,0,0)

void MAPSRS232SensorWithOutput::Birth()
{
    InitializeSensor();
}

void MAPSRS232SensorWithOutput::InitializeSensor()
{
    // Initialisation stream to send to the sensor
	unsigned char messageToSend[]={'H','E','L','L','O','\n','\r'};
	// Open the output stream
	MAPSIOElt *ioEltOutputRS232=StartWriting(Output("oStream8"));
	if (ioEltOutputRS232!=NULL)
	{
		// Get the output stream from the buffer element
		unsigned char *outputByteBuffer=ioEltOutputRS232->Stream8();
		// Put the stream to send in the output
		MAPS::Memcpy(outputByteBuffer,messageToSend,sizeof(messageToSend));
		// Set the output stream length
		ioEltOutputRS232->VectorSize()=sizeof(messageToSend);
		// Send the output
		StopWriting(ioEltOutputRS232);
	}
}

void MAPSRS232SensorWithOutput::Core() 
{
	unsigned char *receivedData;

	// Wait for new data from the sensor
	MAPSIOElt *ioEltInput=StartReading(Input("iStream8"));
	// Check the ioElt returned
	if (ioEltInput==NULL)
		return;
	// We've got the input/output buffer element : the data is pointed by Stream8() method
	receivedData=ioEltInput->Stream8();
	// Test if the received buffer is not empty
	if (ioEltInput->VectorSize()>0)
	{
		// Gets the output buffer element
		MAPSIOElt *ioEltOutput=StartWriting(Output("oInteger"));
		// Check the ioElt returned
		if (ioEltOutput==NULL)
			return;
		// It's a vector of integer
		int *tab=&ioEltOutput->Integer();
		// Set the vector size according to number of received bytes and max vector size
		ioEltOutput->VectorSize()=MIN(ioEltInput->VectorSize(),MAX_OUTPUT_VECTOR_SIZE);
		// Fill this vector with the value of each received byte
		for(int i=0;i<ioEltOutput->VectorSize();i++)
			tab[i]=receivedData[i];
		// Send the output
		StopWriting(ioEltOutput);
	}
}

void MAPSRS232SensorWithOutput::Death()
{
}

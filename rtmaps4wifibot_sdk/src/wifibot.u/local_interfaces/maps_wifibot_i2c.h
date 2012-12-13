////////////////////////////////
// SDK Programmers guide - Sample
////////////////////////////////

////////////////////////////////
// Purpose of this module :
////////////////////////////////
// This component is a template for RTMaps 8 bits serial I/O streams generation and processing 
// (RS232 or RS422 sensors for example). This component inherits from the class MAPSStream8IOComponent
// It means that MAPSStream8IOComponent provides to this component its basic behaviour and a set of
// standard inputs, outputs, properties and actions.

#ifndef _MAPSwifibot_i2c_H
#define _MAPSwifibot_i2c_H


#include "maps.hpp"

#include "MAPSStream8IOComponent.h"
#include "maps_i2cserial_interface.h"
#include <math.h>
#include <winsock2.h>

struct SensorData
	{
	int SpeedFront;
	int SpeedRear;
	int IR;
	int IR2;
	long odometry;
	};

class MAPSwifibot_i2c: public MAPSComponent
{
	MAPS_COMPONENT_STANDARD_HEADER_CODE(MAPSwifibot_i2c)
	
	// This is the callback where to place input stream processing
	void	NewDataCallback(MAPSRBRegion &region,const unsigned char *data, const MAPSTimestamp *timestamp, const MAPSTimestamp *timeOfIssue);

	static void GetMotors(MAPSModule *module,int actionNb);
	static void SetMotors(MAPSModule *module,int actionNb);
	static void Stop(MAPSModule *module,int actionNb);
	
	unsigned int normalize(int value);
	struct SensorData GetI2cMotor(MAPSUInt8 adr);

	int nbDataWanted;
	MAPSTimestamp sendTime;
	MAPSInteger _invert;

	void clock();

	MAPSInput*	m_inputs[2];
	MAPSArray<MAPSUInt8> buffer;
	MAPSI2CPortInterface* pI2CCom;

	void SetMotorsInputs();
	void ReceivedIHMStream();

private:
	char buffer_address[sizeof(struct sockaddr)];
	void Dynamic();

	MAPSMutex _mutexIHM;
	bool _firstTime;
	bool _ihmConnected;

};


#endif

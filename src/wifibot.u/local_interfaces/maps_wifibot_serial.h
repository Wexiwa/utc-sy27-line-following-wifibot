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

#ifndef _MAPSwifibot_serial_H
#define _MAPSwifibot_serial_H

#ifndef _MAPS_H
#include "maps.hpp"
#endif

#include "MAPSStream8IOComponent.h"
#define MESSAGE_SIZE 22

class MAPSwifibot_serial: public MAPSStream8IOComponent
{
	MAPS_CHILD_COMPONENT_HEADER_CODE(MAPSwifibot_serial,MAPSStream8IOComponent);
	void Dynamic();
///////////////////////////////

	// This is the callback where to place input stream processing
	void	NewDataCallback(MAPSRBRegion &region,const unsigned char *data, const MAPSTimestamp *timestamp, const MAPSTimestamp *timeOfIssue);
	bool _firstTime;
    unsigned char	_currentPacket[MESSAGE_SIZE + 1];
	MAPSTimestamp _currentTimestamp;

	int _speed_left;
	int _speed_right;
	bool _closed_loop;
	int _input_mode;
	int _pid_speed;
	MAPSDelay _period;
	bool ProcessMessage();
	short Crc16(unsigned char *Adresse_tab , unsigned char Taille_max);

	void ControlThread();
	void SendCommands();
};


#endif

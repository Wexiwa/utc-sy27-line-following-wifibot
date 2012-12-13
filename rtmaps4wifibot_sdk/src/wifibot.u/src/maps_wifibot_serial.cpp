////////////////////////////////
// RTMaps SDK Component
////////////////////////////////

////////////////////////////////
// Purpose of this module :
////////////////////////////////


#include "maps.hpp"
#include "maps_wifibot_serial.h"

// Add here more inputs to your components
MAPS_BEGIN_INPUTS_DEFINITION(MAPSwifibot_serial)
	MAPS_INPUT("ileft_and_right_speeds",MAPS::FilterInteger,MAPS::LastOrNextReader)
	MAPS_INPUT("ileft_speed",MAPS::FilterInteger,MAPS::LastOrNextReader)
	MAPS_INPUT("iright_speed",MAPS::FilterInteger,MAPS::LastOrNextReader)
MAPS_END_INPUTS_DEFINITION

// Add here more outputs to your components
MAPS_BEGIN_OUTPUTS_DEFINITION(MAPSwifibot_serial)
	MAPS_OUTPUT("speeds_left_right",MAPS::Integer,NULL,NULL,2)
	MAPS_OUTPUT("odometers_left_right",MAPS::Integer,NULL,NULL,2)
	MAPS_OUTPUT("infrared_left_right",MAPS::Integer,NULL,NULL,2)
	MAPS_OUTPUT("battery",MAPS::Integer,NULL,NULL,1)
	MAPS_OUTPUT("current",MAPS::Integer,NULL,NULL,1)
	MAPS_OUTPUT("version",MAPS::Integer,NULL,NULL,1)
MAPS_END_OUTPUTS_DEFINITION

// Add here more properties to your components
MAPS_BEGIN_PROPERTIES_DEFINITION(MAPSwifibot_serial)
	MAPS_PROPERTY_ENUM("input_mode","Vectorized|Left/Right separated",0,false,false)
	MAPS_PROPERTY("commands_send_period",10000,false,false)
	MAPS_PROPERTY("closed_loop_speed_control",true,false,false)
	MAPS_PROPERTY_ENUM("pid_speed","10 ms|50 ms",1,false,false)
MAPS_END_PROPERTIES_DEFINITION

// Add here more actions to your components
MAPS_BEGIN_ACTIONS_DEFINITION(MAPSwifibot_serial)
MAPS_END_ACTIONS_DEFINITION

//Version 1.1 : vectorized outputs speed odos and infrared to match wifibot_tcp_server component inputs.

// Define your component name, version, behaviour, etc. Define also its inputs, outputs, properties and actions number.
MAPS_COMPONENT_DEFINITION(MAPSwifibot_serial,"wifibot_serial","1.1",128,MAPS::Sequential|MAPS::Threaded,
			  MAPS::Threaded,0,6,4,0)


void MAPSwifibot_serial::Dynamic()
{
	_input_mode = (int)GetIntegerProperty("input_mode");
	if (_input_mode == 0) {
		NewInput("ileft_and_right_speeds");
	} else {
		NewInput("ileft_speed");
		NewInput("iright_speed");
	}
	MAPSStream8IOComponent::Dynamic();
}

// The Birth() mfethod
void MAPSwifibot_serial::Birth()
{
    // Always call the MAPSStream8IOComponent::Birth() method first (the parent of this component)
    MAPSStream8IOComponent::Birth();
    // Make your own initialisations here...
	_firstTime = true;
	_speed_left = _speed_right = 0;
	_closed_loop = GetBoolProperty("closed_loop_speed_control");
	_period = GetIntegerProperty("commands_send_period");
	_input_mode = (int)GetIntegerProperty("input_mode");
	_pid_speed = (int)GetIntegerProperty("pid_speed");
	CreateThread((MAPSComponentThreadFunction)&MAPSwifibot_serial::ControlThread);
}

// The Core() method
void MAPSwifibot_serial::Core()
{
    // Always call the MAPSStream8IOComponent::Core() method first (the parent of this component)
    MAPSStream8IOComponent::Core();
    // Most of the time, you don't have to put code in the Core() method
}

void MAPSwifibot_serial::ControlThread()
{
	MAPSEvent timeout;
	MAPSInput* my_inputs[2];
	int nb_inputs;
	if (_input_mode == 1) {
		my_inputs[0] = &Input("ileft_speed");
		my_inputs[1] = &Input("iright_speed");
		nb_inputs = 2;
	} else {
		my_inputs[0] = &Input("ileft_and_right_speeds");
		nb_inputs = 1;
	}
	MAPSEvent* my_events[1];
	my_events[0] = &timeout;
	int iTA;
	timeout.SetTrigger(_period);
	while (!IsDying()) {
		MAPSIOElt* ioeltin = StartReading(nb_inputs,my_inputs,&iTA,1,my_events);
		if (_input_mode == 1) { //separated inputs
			switch(iTA) {
				case 0:
					if (ioeltin == NULL)
						continue;
					_speed_left = ioeltin->Integer();
					break;
				case 1:
					if (ioeltin == NULL)
						continue;
					_speed_right = ioeltin->Integer();
					break;
				default:
					timeout.Reset();
					timeout.SetTrigger(_period);
					SendCommands();
					break;
			}
		} else { //vectorized input.
			switch(iTA) {
				case 0:
					if (ioeltin == NULL)
						continue;
					if (ioeltin->VectorSize() < 2)
					{
						ReportError("Input vector too small. It should contain at least 2 elements (left and right speed commands).");
						continue;
					}
					_speed_left = ioeltin->Integer();
					_speed_right = ioeltin->Integer(1);
					break;
				default:
					timeout.Reset();
					timeout.SetTrigger(_period);
					SendCommands();
					break;
			}
		}
	}
}

void MAPSwifibot_serial::SendCommands()
{
	if (_speed_left > 240) {
		_speed_left =  240;
		ReportWarning("Max speed for left motor exceeded. Bounding to 240.");
	}
	if (_speed_left < -240) {
		_speed_left = -240;
		ReportWarning("Max backwards speed for left motor exceeded. Bounding to -240.");
	}
	if (_speed_right > 240) {
		_speed_right =  240;
		ReportWarning("Max speed for right motor exceeded. Bounding to 240.");
	}
	if (_speed_right < -240) {
		_speed_right = -240;
		ReportWarning("Max backwards speed for right motor exceeded. Bounding to -240.");
	}
	unsigned char command[9];
	unsigned int rs = (_speed_right >= 0) ? _speed_right : -_speed_right;
	unsigned int ls = (_speed_left >= 0) ? _speed_left : -_speed_left;
	command[0] = 0xFF;
	command[1] = 0x07;
	command[2] = (unsigned char)(ls & 0xFF);
	command[3] = (unsigned char)((ls >> 8) & 0xFF);
	command[4] = (unsigned char)(rs & 0xFF);
	command[5] = (unsigned char)((rs >> 8) & 0xFF);
	unsigned char flags = 0;
	if (_closed_loop) {
		flags += 128;
		flags += 32;
	}
	if (_speed_left >= 0) {
		flags += 64;
	}
	if (_speed_right >= 0) {
		flags += 16;
	}
	if (_pid_speed == 0) {
		flags += 8;
	}
	flags += 1;
	command[6] = flags;
	short mycrcsend = Crc16(command+1,6);
	command[7] = (unsigned char)(mycrcsend & 0xFF);
	command[8] = (unsigned char)(mycrcsend >> 8);

	SendData(command,9);
}
// The Death() method
void MAPSwifibot_serial::Death()
{
    // Make your own closure here...
    
    // Always call the MAPSStream8IOComponent::Death() method at the end (the parent of this component)
    MAPSStream8IOComponent::Death();
}

// The main function for a RTMaps 8 bits serial I/O streams generation and processing component. This method
// is called each time a stream data  has been read and stacked by the parent class MAPSStream8IOComponent into data parameter. 
// All information concerning data timings are kept into 2 other stacks given in the parameters timestamp and timeOfIssue
// The region parameter contains the region of available data into the ring-buffers data, timestamp and timeOfIssue. A set of method 
// of the class MAPSRBRegion (for MAPS Ring Buffer Region) can be used to access the data. You have to refer to the MAPSRBRegion
// reference documentation in order to get more information on these methods.
void MAPSwifibot_serial::NewDataCallback(MAPSRBRegion &region,const unsigned char *data, const MAPSTimestamp *timestamp, const MAPSTimestamp *timeOfIssue)
{
	//MAPSTimestamp currentTime=MAPS::CurrentTime();
	int offset=0;
	//int offsetStart=0;
	int lastOffset = 0;
	//int maxOffset=region.Size()-1;
	int regionSize = region.Size();
	unsigned char c;
	while(offset<regionSize)
	{
		c = data[region[offset]];
		offset++;
		if (offset-lastOffset > MESSAGE_SIZE*2) {
			lastOffset = offset;
			ReportWarning("Message too long. Discarding data.");
			continue;
		}
		if (c == 0xFF)
		{
			
			if (regionSize - (offset-1) < MESSAGE_SIZE) { //Message too short. Discard.
				break;
			}
			MAPSRBRegion finalMessageSubRegion;
			int res = region.SubRegion(finalMessageSubRegion,MAPSConstRBRegionSubRegionFromBeginning, offset, MESSAGE_SIZE - 1);
			if (!MAPSRBREGION_NO_WARNINGS(res))
			{
				ReportWarning("Error while processing the FIFO");
				continue;
			}
			finalMessageSubRegion.CopyFromRB(data,_currentPacket,finalMessageSubRegion.Size());
			int nbDataInPacket = finalMessageSubRegion.Size();
			_currentPacket[nbDataInPacket] = '\0';
			_currentTimestamp= timestamp[region[offset-1]];
			if(false==ProcessMessage())
			{
				ReportWarning("Error processing a message.");
				lastOffset = offset;
				continue;
			}
			offset += MESSAGE_SIZE -1;
			lastOffset = offset;
		}
	}
	region.Pop(lastOffset);
}

bool MAPSwifibot_serial::ProcessMessage()
{
	short mycrcrcv = (short)((_currentPacket[20] << 8) + _currentPacket[19]);
	short mycrcsend = Crc16(_currentPacket,19);
	if (mycrcrcv!=mycrcsend)
	{
		ReportWarning("Wrong CRC. Discarding...");
		return false;
	}
	MAPSIOElt* ioEltSpeeds = StartWriting(Output(0));
	MAPSIOElt* ioEltOdos = StartWriting(Output(1));
	MAPSIOElt* ioEltInfrared = StartWriting(Output(2));
	MAPSIOElt* ioEltBatt = StartWriting(Output(3));
	MAPSIOElt* ioEltCurrent = StartWriting(Output(4));
	MAPSIOElt* ioEltVersion = StartWriting(Output(5));

	ioEltSpeeds->Timestamp() = _currentTimestamp;
	ioEltOdos->Timestamp() = _currentTimestamp;
	ioEltInfrared->Timestamp() = _currentTimestamp;
	ioEltBatt->Timestamp() = _currentTimestamp;
	ioEltCurrent->Timestamp() = _currentTimestamp;
	ioEltVersion->Timestamp() = _currentTimestamp;

	ioEltSpeeds->Integer(0) = (int)((_currentPacket[1] << 8) + _currentPacket[0]);
	if (ioEltSpeeds->Integer(0) > 32767)
		ioEltSpeeds->Integer(0) -= 65536;
	ioEltSpeeds->Integer(1) = (int)(_currentPacket[10] << 8) + _currentPacket[9];
	if (ioEltSpeeds->Integer(1) > 32767)
		ioEltSpeeds->Integer(1) -= 65536;

	ioEltOdos->Integer(0) = ((((long)_currentPacket[8] << 24))+(((long)_currentPacket[7] << 16))+(((long)_currentPacket[6] << 8))+((long)_currentPacket[5]));
	ioEltOdos->Integer(1) = ((((long)_currentPacket[16] << 24))+(((long)_currentPacket[15] << 16))+(((long)_currentPacket[14] << 8))+((long)_currentPacket[13]));;
	
	ioEltInfrared->Integer() = _currentPacket[3]; //TODO: check infrared sensors decoding and wifibot tcp server infrared handling (units, messages...)
	ioEltInfrared->Integer(1) = _currentPacket[11]; //TODO: check infrared sensors decoding and wifibot tcp server infrared handling (units, messages...)
	//ioEltInfrared->Integer(1) = _currentPacket[4]; //TODO: check infrared sensors decoding and wifibot tcp server infrared handling (units, messages...)
	//ioEltRightADC->Integer() = _currentPacket[11]; //TODO: check infrared sensors decoding and wifibot tcp server infrared handling (units, messages...)
	//ioEltRightADC->Integer(1) = _currentPacket[12]; //TODO: check infrared sensors decoding and wifibot tcp server infrared handling (units, messages...) 
	ioEltBatt->Integer() = _currentPacket[2];
	ioEltCurrent->Integer() = _currentPacket[17];
	ioEltVersion->Integer() = _currentPacket[18];

	StopWriting(ioEltSpeeds);
	StopWriting(ioEltOdos);
	StopWriting(ioEltInfrared);
	StopWriting(ioEltBatt);
	StopWriting(ioEltCurrent);
	StopWriting(ioEltVersion);

	return true;
}
short MAPSwifibot_serial::Crc16(unsigned char *Adresse_tab , unsigned char Taille_max)
{
	unsigned int Crc = 0xFFFF;
	unsigned int Polynome = 0xA001;
	unsigned int CptOctet = 0;
	unsigned int CptBit = 0;
	unsigned int Parity= 0;
	Crc = 0xFFFF;
	Polynome = 0xA001;
	for ( CptOctet= 0 ; CptOctet < Taille_max ; CptOctet++)
	{
		Crc ^= *( Adresse_tab + CptOctet);
		for ( CptBit = 0; CptBit <= 7 ; CptBit++)
		{
			Parity= Crc;
			Crc >>= 1;
			if (Parity%2 > 0) Crc ^= Polynome;
		}
	}
	return(Crc);
}
////////////////////////////////
// RTMaps SDK Component
////////////////////////////////

////////////////////////////////
// Purpose of this module :
////////////////////////////////

#include "maps_wifibot_tcp_client.h"	// Includes the header of this component

// Use the macros to declare the inputs
MAPS_BEGIN_INPUTS_DEFINITION(MAPSwifibot_tcp_client)
MAPS_INPUT("ileft_and_right_speeds",MAPS::FilterInteger,MAPS::SamplingReader)
	MAPS_INPUT("ileft_speed",MAPS::FilterInteger,MAPS::SamplingReader)
	MAPS_INPUT("iright_speed",MAPS::FilterInteger,MAPS::SamplingReader)
MAPS_END_INPUTS_DEFINITION

// Use the macros to declare the outputs
MAPS_BEGIN_OUTPUTS_DEFINITION(MAPSwifibot_tcp_client)
    MAPS_OUTPUT("battery",MAPS::Integer,NULL,"0-255",1)
	MAPS_OUTPUT("odometers",MAPS::Integer,NULL,"ticks per 41 ms",4)
	MAPS_OUTPUT("infrared",MAPS::Integer,NULL,"cm",2)
MAPS_END_OUTPUTS_DEFINITION

// Use the macros to declare the properties
MAPS_BEGIN_PROPERTIES_DEFINITION(MAPSwifibot_tcp_client)
    MAPS_PROPERTY("wifibot_ip_address","127.0.0.1",false,false)
	MAPS_PROPERTY("wifibot_port",15020,false,false)
	MAPS_PROPERTY_ENUM("input_mode","Vectorized|Left/Right separated",0,false,false)
	MAPS_PROPERTY("closed_loop_speed_control",true,false,false)
	MAPS_PROPERTY("send_cmds_period",50000,false,false)
MAPS_END_PROPERTIES_DEFINITION

// Use the macros to declare the actions
MAPS_BEGIN_ACTIONS_DEFINITION(MAPSwifibot_tcp_client)
    //MAPS_ACTION("aName",MAPSwifibot_tcp_client::ActionName)
MAPS_END_ACTIONS_DEFINITION

// Use the macros to declare this component (wifibot_client) behaviour
MAPS_COMPONENT_DEFINITION(MAPSwifibot_tcp_client,"wifibot_tcp_client","1.0",128,
			  MAPS::Threaded,MAPS::Threaded,
			  0, // Nb of inputs
			  3, // Nb of outputs
			  5, // Nb of properties
			  0) // Nb of actions


void MAPSwifibot_tcp_client::Dynamic()
{
	_input_mode = (int)GetIntegerProperty("input_mode");
	if (_input_mode == 0) {
		NewInput("ileft_and_right_speeds");
	} else {
		NewInput("ileft_speed");
		NewInput("iright_speed");
	}
}

void MAPSwifibot_tcp_client::Birth()
{
	_first_time = false;
	_connected = false;

	if (false == _client.Init())
		Error("Could not init TCP socket.");

	MAPSString ip = GetStringProperty("wifibot_ip_address");
	unsigned short port = (unsigned short)GetIntegerProperty("wifibot_port");
	if (false == _client.SetRemoteAddress((const char*)ip,port))
		Error("Could not init TCP socket. Wrong IP address format?");

	_speed_left = _speed_right = 0;
	_closed_loop = GetBoolProperty("closed_loop_speed_control");
	_input_mode = (int)GetIntegerProperty("input_mode");
	_period = (int)GetIntegerProperty("send_cmds_period");


	CreateThread((MAPSComponentThreadFunction)&MAPSwifibot_tcp_client::ReceiveThread);
}

void MAPSwifibot_tcp_client::ReceiveThread()
{
	char buff[7];
	while (!IsDying()) {
		if (false == _connected)
			MAPS::Sleep(1000000);

		timeval tv;
		tv.tv_sec = 0;
		tv.tv_usec = 200000;
		if (_client.Receive(buff,7,tv) != 7)
			continue;

		MAPSTimestamp t = MAPS::CurrentTime();
		MAPSIOElt* ioeltout_batt = StartWriting(Output(0));
		MAPSIOElt* ioeltout_odo = StartWriting(Output(1));
		MAPSIOElt* ioeltout_ir = StartWriting(Output(2));
		ioeltout_batt->Integer() = buff[0];
		ioeltout_odo->Integer() = buff[1];
		ioeltout_odo->Integer(1) = buff[2];
		ioeltout_odo->Integer(2) = buff[3];
		ioeltout_odo->Integer(3) = buff[4];
		ioeltout_ir->Integer() = buff[5];
		ioeltout_ir->Integer(1)= buff[6];
		ioeltout_batt->Timestamp() = t;
		ioeltout_odo->Timestamp() = t;
		ioeltout_ir->Timestamp() = t;
		StopWriting(ioeltout_batt);
		StopWriting(ioeltout_odo);
		StopWriting(ioeltout_ir);
	}
}

void MAPSwifibot_tcp_client::Core() 
{
	while (false == _connected && !IsDying()) {
		int res = _client.Connect(2000000,&isDyingEvent);
		if (res >= 0)
			_connected = true;
	}

	if (_input_mode == 1) {
		MAPSIOElt* ioeltin_left = StartReading(Input(0));
		MAPSIOElt* ioeltin_right = StartReading(Input(1));
		if (ioeltin_left == NULL || ioeltin_right == NULL)
			return;

		_speed_left = ioeltin_left->Integer();
		_speed_right= ioeltin_right->Integer();
	} else {
		MAPSIOElt* ioeltin = StartReading(Input(0));
		if (ioeltin == NULL)
			return;

		if (ioeltin->VectorSize() < 2) {
			ReportError("Vector size too low for left and right speed commands. Must be at least 2.");
			return;
		}

		_speed_left = ioeltin->Integer();
		_speed_right = ioeltin->Integer(1);
	}

	SendCommands();

	MAPS::Sleep(_period);
}

void MAPSwifibot_tcp_client::SendCommands()
{
	if (_speed_left > 60) {
		_speed_left =  60;
		ReportWarning("Max speed for left motor exceeded. Bounding to 63.");
	}
	if (_speed_left < -60) {
		_speed_left = -60;
		ReportWarning("Max backwards speed for left motor exceeded. Bounding to -60.");
	}
	if (_speed_right > 60) {
		_speed_right =  60;
		ReportWarning("Max speed for right motor exceeded. Bounding to 60.");
	}
	if (_speed_right < -60) {
		_speed_right = -60;
		ReportWarning("Max backwards speed for right motor exceeded. Bounding to -60.");
	}
	unsigned char command[2];
	unsigned int rs = (_speed_right >= 0) ? _speed_right : -_speed_right;
	unsigned int ls = (_speed_left >= 0) ? _speed_left : -_speed_left;
	command[0] = 0;
	command[1] = 0;
	if (_closed_loop) {
		command[0] |= 0x01 << 7;
		command[1] |= 0x01 << 7;
	}
	if (_speed_left >= 0) {
		command[0] |= 0x01 << 6;
	}
	if (_speed_right >= 0) {
		command[1] |= 0x01 << 6;
	}
	command[0] |= ls;
	command[1] |= rs;

	int res = _client.Send(command,2,NULL);
	if (res < 0)
		_connected = false;
}

void MAPSwifibot_tcp_client::Death()
{
	_client.Close();
}

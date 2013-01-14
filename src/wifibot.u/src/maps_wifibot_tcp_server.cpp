////////////////////////////////
// RTMaps SDK Component
////////////////////////////////

////////////////////////////////
// Purpose of this module :
////////////////////////////////

#include "maps_wifibot_tcp_server.h"	// Includes the header of this component

// Use the macros to declare the inputs
MAPS_BEGIN_INPUTS_DEFINITION(MAPSwifibot_tcp_server)
	MAPS_INPUT("speeds_left_right",MAPS::FilterInteger,MAPS::FifoReader)
	MAPS_INPUT("odometers_left_right",MAPS::FilterInteger,MAPS::FifoReader)
	MAPS_INPUT("infrared_left_right",MAPS::FilterInteger,MAPS::FifoReader)
	MAPS_INPUT("battery",MAPS::FilterInteger,MAPS::FifoReader)
MAPS_END_INPUTS_DEFINITION

// Use the macros to declare the outputs
MAPS_BEGIN_OUTPUTS_DEFINITION(MAPSwifibot_tcp_server)
	MAPS_OUTPUT("left_and_right_speeds",MAPS::Integer,NULL,NULL,4) // left speed, right speed, closed loop left (0 or 1), closed loop right (0 or 1)
	MAPS_OUTPUT("left_speed",MAPS::Integer,NULL,NULL,2) //left speed, closed loop left (0 or 1)
	MAPS_OUTPUT("right_speed",MAPS::Integer,NULL,NULL,2) //right speed, closed loop right (0 or 1)
MAPS_END_OUTPUTS_DEFINITION

// Use the macros to declare the properties
MAPS_BEGIN_PROPERTIES_DEFINITION(MAPSwifibot_tcp_server)
	MAPS_PROPERTY_ENUM("output_mode","Vectorized|Left/Right separated",0,false,false)
    MAPS_PROPERTY("tcp_listen_port",15020,false,false)
    MAPS_PROPERTY_READ_ONLY("sender_ip","0.0.0.0")
/*REUSEADDR on normal places means you can rebind to the port
right after somebody else has let it go. But REUSEADDR on win32
means to let you bind to the port _even when somebody else
already has it bound_. So, don't do that on Win32.*/
#ifdef WIN32
    MAPS_PROPERTY("reuse_addr",false,false,false)
#else
    MAPS_PROPERTY("reuse_addr",true,false,false)
#endif
MAPS_END_PROPERTIES_DEFINITION

// Use the macros to declare the actions
MAPS_BEGIN_ACTIONS_DEFINITION(MAPSwifibot_tcp_server)
    //MAPS_ACTION("aName",MAPSwifibot_tcp_server::ActionName)
MAPS_END_ACTIONS_DEFINITION

// Use the macros to declare this component (wifibot_tcp_server) behaviour
MAPS_COMPONENT_DEFINITION(MAPSwifibot_tcp_server,"wifibot_tcp_server","1.0",128,
			  MAPS::Threaded,MAPS::Threaded,
			  4, // Nb of inputs
			  0, // Nb of outputs
			  4, // Nb of properties
			  0) // Nb of actions

MAPSwifibot_tcp_server::MAPSwifibot_tcp_server(const char* componentName, MAPSComponentDefinition& cd)
: MAPSComponent(componentName,cd)
{
	m_pserver = NULL;
	m_pclient = NULL;
	// set timetout to 200ms
	m_timeout.tv_sec = 0;
	m_timeout.tv_usec = 200000;
}

MAPSwifibot_tcp_server::~MAPSwifibot_tcp_server()
{
}

void MAPSwifibot_tcp_server::Dynamic()
{
	m_output_mode = (int)GetIntegerProperty("output_mode");
	if (m_output_mode == 0) {
		NewOutput(0);
	} else {
		NewOutput(1);
		NewOutput(2);
	}
}

void MAPSwifibot_tcp_server::Birth()
{
	Reset();

	MAPSComponent::CreateThread((MAPSComponentThreadFunction)&MAPSwifibot_tcp_server::HandleInputsThread);
}

void MAPSwifibot_tcp_server::Core() 
{
	if (m_firsttime) {
		m_firsttime = false;
		while(false == m_pserver->AcceptClient((MAPSTcpClient*)m_pclient, 1000)) { // 1s de timeout
			if(IsDying())
				return;
		}
		m_is_connected = true;
	} // first time

	int result = Receive(2);
	if (result == 0)
		return; //Socket closed or Socket error. Socket has been reset, let's listen for a new client.
	if (result == -1)
		return; //timeout
	
	DecoderBuffer();

	if (m_buffer_size == RECV_BUF_SIZE) {
		ReportWarning("Receive buffer overflow. Discarding current data and resetting buffer.");
		m_buffer_size = 0;
		return;
	}
}

void MAPSwifibot_tcp_server::DecoderBuffer()
{
	if (m_buffer_size < 2)
		return;
	unsigned char left = m_buffer[0];
	unsigned char right = m_buffer[1];
	int speed_left, speed_right;
	speed_left = left & 0x3F;
	speed_right = right & 0x3F;
	if (left & 0x40)
		speed_left = -speed_left;
	if (right & 0x40)
		speed_right = -speed_right;
	bool closed_loop_left = (left & 0x80) >0;
	bool closed_loop_right = (right & 0x80) >0;

	if (m_output_mode == 0) {
		MAPSIOElt* ioeltout = StartWriting(Output(0));
		ioeltout->Integer(0) = speed_left;
		ioeltout->Integer(1) = speed_right;
		ioeltout->Integer(2) = closed_loop_left ? 1 : 0;
		ioeltout->Integer(3) = closed_loop_right ? 1 : 0;
		StopWriting(ioeltout);
	} else {
		MAPSTimestamp t = MAPS::CurrentTime();
		MAPSIOElt* ioeltout_left = StartWriting(Output(0));
		MAPSIOElt* ioeltout_right = StartWriting(Output(1));
		ioeltout_left->Integer() = speed_left;
		ioeltout_left->Integer(1)= closed_loop_left;
		ioeltout_left->Timestamp() = t;
		ioeltout_right->Integer() = speed_right;
		ioeltout_right->Integer(1)= closed_loop_right;
		ioeltout_right->Timestamp() = t;
		StopWriting(ioeltout_right);
		StopWriting(ioeltout_left);
	}

	m_buffer_size = 0;
}

void MAPSwifibot_tcp_server::HandleInputsThread()
{
	MAPSInput* my_inputs[4];
	MAPSIOElt* my_ioelts[4];
	my_inputs[0] = &Input(0);
	my_inputs[1] = &Input(1);
	my_inputs[2] = &Input(2);
	my_inputs[3] = &Input(3);

	while (!IsDying()) {
		MAPSTimestamp t = SynchroStartReading(4,my_inputs,my_ioelts);
		if (t==-1)
			continue;
		if (this->m_is_connected == false)
			continue;
		if (my_ioelts[3]->VectorSize() != 1)
			Error("Unexpected vector size on battery level input. Expecting a scalar integer (battery level).");
		if (my_ioelts[0]->VectorSize() != 2 && my_ioelts[0]->VectorSize() != 4)
			Error("Unexpected vector size on wheel speeds input. Expecting a vector of 2 or 4 integers (left and right speeds or frontleft/rearleft/frontright/rearright speeds).");
		if (my_ioelts[1]->VectorSize() != 2)
			Error("Unexpected vector size on wheel odometers input. Expecting a vector of 2 integers (left and right odometers).");	
		if (my_ioelts[2]->VectorSize() != 2)
			Error("Unexpected vector size on infrared input. Expecting a vector of 2 integers (left and right infrared sensors values).");	

		char buffso_send[15];
		buffso_send[0]=(char)(my_ioelts[3]->Integer()+20);
		if (my_ioelts[0]->VectorSize() == 2) {
			buffso_send[1]=(char)my_ioelts[0]->Integer(0);
			buffso_send[2]=(char)my_ioelts[0]->Integer(0);
			buffso_send[3]=(char)my_ioelts[0]->Integer(1);
			buffso_send[4]=(char)my_ioelts[0]->Integer(1);
		} else {
			buffso_send[1]=(char)my_ioelts[0]->Integer(0);
			buffso_send[2]=(char)my_ioelts[0]->Integer(1);
			buffso_send[3]=(char)my_ioelts[0]->Integer(2);
			buffso_send[4]=(char)my_ioelts[0]->Integer(3);
		}
		buffso_send[5]=(char)my_ioelts[2]->Integer(0);
		buffso_send[6]=(char)my_ioelts[2]->Integer(1);
		MAPSInt32 odoleft = my_ioelts[1]->Integer();
		MAPSInt32 odoright = my_ioelts[1]->Integer(1);
		buffso_send[7]=(char) odoleft;
		buffso_send[8]=(char)(odoleft >> 8);
		buffso_send[9]=(char)(odoleft >> 16);
		buffso_send[10]=(char)(odoleft >> 24);
		buffso_send[11]=(char)odoright;
		buffso_send[12]=(char)(odoright >> 8);
		buffso_send[13]=(char)(odoright >> 16);
		buffso_send[14]=(char)(odoright >> 24);

		if (m_pclient->Send(buffso_send,15,&isDyingEvent) != 15) {
			HandleDeconnection("Socket error: send failed.");
		}
	}
}

void MAPSwifibot_tcp_server::Death()
{
	MAPS_SAFE_DELETE(m_pserver);
	MAPS_SAFE_DELETE(m_pclient);
}

void MAPSwifibot_tcp_server::Reset() 
{
	m_is_connected = false;
	m_firsttime=true;
	m_buffer_size = 0;

	MAPS_SAFE_DELETE(m_pserver);

	m_pserver = new MAPSTcpServer();
	m_pclient = new MAPSTcpClient();
	m_pserver->SetLocalAddress(NULL,(unsigned short)GetIntegerProperty("tcp_listen_port"),AF_INET);

	m_pserver->SetReuseAddr(GetBoolProperty("reuse_addr"));

	if(m_pserver->Init() == false) {
		Error("Cannot initialize connection.");
	}
}

int MAPSwifibot_tcp_server::Receive(int size) 
{
	if (m_buffer_size + size > RECV_BUF_SIZE) {
		size = RECV_BUF_SIZE - m_buffer_size;
	}
	int result = m_pclient->Receive(m_buffer + m_buffer_size,size,m_timeout);
	if (result == MAPS_SOCKET_TIMEOUT) // timeout
		return -1;
	else if (result == 0) { // Connection closed
		HandleDeconnection("Socket closed by peer.");
		return 0;
	} else if (result == MAPS_SOCKET_ERROR) {
		HandleDeconnection("Socket error.");
		return 0;
	}
	m_buffer_size += result;
	return result;
}

void MAPSwifibot_tcp_server::HandleDeconnection(MAPSString s)
{
	MAPSStreamedString sx;
	sx << s << " Trying to reconnect.";
	ReportError(sx);
	Reset();
}

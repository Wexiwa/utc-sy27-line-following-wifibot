////////////////////////////////
// RTMaps SDK Component header
////////////////////////////////

#ifndef _Maps_wifibot_tcp_client_H
#define _Maps_wifibot_tcp_client_H

// Includes maps sdk library header
#include "maps.hpp"
#include "MAPSTcpClient.h"

// Declares a new MAPSComponent child class
class MAPSwifibot_tcp_client : public MAPSComponent 
{
	// Use standard header definition macro
	MAPS_COMPONENT_STANDARD_HEADER_CODE(MAPSwifibot_tcp_client)
	void Dynamic();
private :
	// Place here your specific methods and attributes
	MAPSTcpClient _client;	
	bool	_connected;
	bool	_first_time;
	int		_speed_left;
	int		_speed_right;
	bool	_closed_loop;
	int		_input_mode;
	int		_period;

	void SendCommands();
	void ReceiveThread();

};

#endif

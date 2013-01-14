////////////////////////////////
// RTMaps SDK Component header
////////////////////////////////

#ifndef _Maps_wifibot_tcp_server_H
#define _Maps_wifibot_tcp_server_H

// Includes maps sdk library header
#include "maps.hpp"
#include "MAPSTcpServer.h"
#include "MAPSTcpClient.h"


#define RECV_BUF_SIZE 256

// Declares a new MAPSComponent child class
class MAPSwifibot_tcp_server : public MAPSComponent 
{
	// Use standard header definition macro
	MAPS_COMPONENT_HEADER_CODE_WITHOUT_CONSTRUCTOR(MAPSwifibot_tcp_server)
		
	MAPSwifibot_tcp_server(const char* componentName, MAPSComponentDefinition& cd);
	virtual ~MAPSwifibot_tcp_server();
	void Dynamic();
private :
	// Place here your specific methods and attributes
	MAPSTcpServer*		m_pserver;
	MAPSTcpClient*		m_pclient;
    struct timeval		m_timeout;
    MAPSInetAddr		m_sender_ip;
	bool				m_firsttime;
	MAPSUInt8			m_buffer[RECV_BUF_SIZE];
	int					m_buffer_size;
	int					m_output_mode;
	bool				m_is_connected;


	void	Reset();
	void	HandleDeconnection(MAPSString s);
	int		Receive(int size) ;
	void	DecoderBuffer();
	void	HandleInputsThread();


};

#endif

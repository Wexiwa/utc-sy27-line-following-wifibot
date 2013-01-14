#ifndef MAPSTCPSERVER_H
#define MAPSTCPSERVER_H

#include "MAPSSocketServer.h"
#include "MAPSTcpClient.h"

class MAPSTcpServer : public MAPSSocketConnection
{
protected:
	bool    _listenSucceeded;
	int		_maxClients;

public:
	MAPSTcpServer()
	: MAPSSocketConnection(), _listenSucceeded(false), _maxClients(5) {}
	virtual ~MAPSTcpServer() { Close(); }

	bool Init();
	bool Close();

	// 5 par défaut
	void SetMaxClients(int maxClients) { _maxClients = maxClients; }
	// timeout en ms
	bool AcceptClient(MAPSTcpClient* newClient, int timeout);

	bool CanAccept() { return _listenSucceeded; }
};

#endif

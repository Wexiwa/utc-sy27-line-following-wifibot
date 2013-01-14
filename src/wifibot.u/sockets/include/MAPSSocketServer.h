#ifndef MAPSSOCKETSERVER_H
#define MAPSSOCKETSERVER_H

#include "MAPSSocketConnection.h"
#include "MAPSSocketClient.h"

class MAPSSocketServer : public MAPSSocketConnection
{
protected:
	bool    _listenSucceeded;

public:
	MAPSSocketServer()
	: MAPSSocketConnection(), _listenSucceeded(false) {}
	virtual ~MAPSSocketServer() {}

	virtual bool Init()=0;
	virtual bool Close();
};

#endif

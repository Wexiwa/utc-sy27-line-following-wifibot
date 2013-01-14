#ifndef MAPSTCPCLIENT_H
#define MAPSTCPCLIENT_H

#include "MAPSSocketClient.h"

class MAPSTcpClient : public MAPSSocketClient
{
	friend class MAPSTcpServer;

protected:
	int				_iMaxSendRetries;

public:
	MAPSTcpClient()
	: MAPSSocketClient(),_iMaxSendRetries(5) {}
	virtual ~MAPSTcpClient() {}

	bool Init();

	// Par default, 5
	void SetMaxSendRetries(int maxSendRetries)
	{ _iMaxSendRetries = maxSendRetries; }

    int Send(const void* buffer,unsigned int size,MAPSEvent* cancelEvent);

	void SetTcpNoDelay(bool bVal)
	{ _socket.SetTcpNoDelay(bVal); }
};

#endif

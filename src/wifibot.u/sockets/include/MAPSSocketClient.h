#ifndef MAPSSOCKETCLIENT_H
#define MAPSSOCKETCLIENT_H

#include "MAPSSocketConnection.h"

class MAPSSocketClient : public MAPSSocketConnection
{
protected:
	//bool _connected;
	struct timeval	_sendTimeout;

public:
	MAPSSocketClient()
	: MAPSSocketConnection() { _sendTimeout.tv_sec=1; _sendTimeout.tv_usec=0; }
	virtual ~MAPSSocketClient() { Close(); }

	virtual bool Init()=0;
	virtual bool Close();

	// timeout en ms
	int Connect(MAPSInt64 timeout, MAPSEvent* abortEvent = NULL);

	int Receive(void* buffer,unsigned int size,struct timeval& timeout);
	int ReceiveFrom(void* buffer, unsigned int size, struct timeval& timeout, MAPSInetAddr *from = NULL);

	// Par default, 1s
	void SetSendTimeout(int timeoutMs);

	virtual int Send(const void* buffer,unsigned int size,MAPSEvent* cancelEvent) = 0;
};

#endif

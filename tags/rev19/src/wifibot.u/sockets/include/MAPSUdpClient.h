#ifndef MAPSUDPCLIENT_H
#define MAPSUDPCLIENT_H

#include "MAPSSocketClient.h"

class MAPSUdpClient : public MAPSSocketClient
{
protected:
	unsigned int	_maxMsgSize;

public:
	MAPSUdpClient() 
	: MAPSSocketClient(),_maxMsgSize(0), m_isMCast(false), m_isBCast(false) {}
	virtual ~MAPSUdpClient() {}

	bool Init();

	bool m_isMCast;
	bool m_isBCast;

	// Besoin de faire un connect d'abord
    int Send(const void* buffer,unsigned int size,MAPSEvent* cancelEvent);
	int SendTo(const void* buffer, unsigned int size, MAPSInetAddr const *const to);

	// Specific multicast. Si mcastAddr est null, on prend l'adresse de la connexion
	bool JoinMCastGroup(MAPSInetAddr* mcastAddr = NULL);
	void DropMCastGroup(MAPSInetAddr* mcastAddr = NULL);

	void SetMaxMessageSize(unsigned int requestedMaxMsgSize)
	{ _maxMsgSize = requestedMaxMsgSize; }

	void SetMCastTTL(int ttl);
};

#endif

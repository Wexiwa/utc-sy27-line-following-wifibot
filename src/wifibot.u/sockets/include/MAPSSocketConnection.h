#ifndef MAPSSOCKETCONNECTION_H
#define MAPSSOCKETCONNECTION_H

#include "MAPSSocket.h"

// Classe de base des connexions par socket
class MAPSSocketConnection
{
private:
	MAPSSocketConnection(const MAPSSocketConnection& srcSC) {} // interdit
#ifdef _WIN32
	// At least one initialization per process is required
	static int				_win32NetworkStartedCount;
#endif

protected:
	MAPSSocket		_socket;
	MAPSInetAddr	_sockLocalAddr;
	MAPSInetAddr	_sockRemoteAddr;

public:
	MAPSSocketConnection();
	virtual ~MAPSSocketConnection();

	bool IsValid() const
	{ return _socket.IsValid(); }

	bool SetBlocking(bool block)
	{ if(!_socket.IsValid()) return false; return _socket.SetBlocking(block);}

	unsigned int GetMaxMessageSize()
	{ if(!_socket.IsValid()) return 0; return _socket.GetMaxMsgSize(); }

	/** Sets the address and port of the remote socket.
	 * @param addrString : if null, prepare the socket to be used for listening incoming connections.
	 * Otherwise, can be an IPv4 or IPv6 address or a host name.
	 * @param port : the port
	 * @param ai_family : force resolution to IPv4 (AF_INET), IPv6 (AF_INET6), or try auto resolution, then preferring
	 * IPv4 if both are available (AF_UNSPEC).
	 * @return true if succeeded, false otherwise (can be because of failure of hostname resolution for example).
	 * */
	bool SetRemoteAddress(const char* addrString, unsigned short port, int ai_family = AF_UNSPEC)
	{ if (false == _sockRemoteAddr.SetAddress(addrString,ai_family)) return false; _sockRemoteAddr.SetPort(port); return true;}

	/** Sets the address and port of the local socket.
	 * @param addrString : if null, prepare the socket to be used for listening incoming connections.
	 * Otherwise, can be an IPv4 or IPv6 address or a host name.
	 * @param port : the port
	 * @param ai_family : force resolution to IPv4 (AF_INET), IPv6 (AF_INET6), or try auto resolution, then preferring
	 * IPv4 if both are available (AF_UNSPEC).
	 * @return true if succeeded, false otherwise (can be because of failure of hostname resolution for example).
	 * */
	bool SetLocalAddress(const char* addrString, unsigned short port, int ai_family = AF_UNSPEC)
	{ if (false == _sockLocalAddr.SetAddress(addrString,ai_family)) return false; _sockLocalAddr.SetPort(port); return true;}

	int SetSockOpt(int level, int optname, const void *optval, int optlen)
	{ return _socket.SetOpt(level,optname,optval,optlen); }

	bool SetReuseAddr(bool bVal)
	{ return _socket.SetReuseAddr(bVal); }

	MAPSInetAddr& GetRemoteAddress() { return _sockRemoteAddr; }
	MAPSInetAddr& GetLocalAddress() { return _sockLocalAddr; }
};

#endif

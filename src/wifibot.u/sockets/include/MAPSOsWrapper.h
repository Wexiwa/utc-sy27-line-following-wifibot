#ifndef __MAPSOsWrapper_h__
#define __MAPSOsWrapper_h__

#include "MAPSSystemDefs.h"

//! Wrapper class to OS-dependant functions.
class MAPSOsWrapper
{
public:
    //! @name IO routines.
    //@{
    static int open(const char *fileName, int flags, int mode);
    static int close(int fd);
    static int read(int fd, char *buffer, size_t maxSize);
    static int write(int fd, const char *buffer, size_t numBytes);
    static int ioctl(int fd, int function, void* arg);
    //@}

    //! @name BSD Sockets.
    //@{
    static MAPS_IO_HANDLE socket(int af, int type, int protocol);
    static int close_socket(MAPS_IO_HANDLE sock);
    static int shutdown(MAPS_IO_HANDLE sock, int how);
    static int bind(MAPS_IO_HANDLE sock, const struct sockaddr *addr, int addrLen);
    static int listen(MAPS_IO_HANDLE sock, int backlog);
    static int connect(MAPS_IO_HANDLE sock, const struct sockaddr *addr, int addrLen);
    static MAPS_IO_HANDLE accept(MAPS_IO_HANDLE sock, struct sockaddr *addr, int *addrLen);
    static int send(MAPS_IO_HANDLE sock, const char *buffer, int len, int flags);
    static int recv(MAPS_IO_HANDLE sock, char *buffer, int len, int flags);
    static int sendto(MAPS_IO_HANDLE sock, const char *buffer, int len, int flags,
	const struct sockaddr *toAddr ,int toAddrLen);
    static int recvfrom(MAPS_IO_HANDLE sock, char *buffer, int len, int flags,
    struct sockaddr *fromAddr, int *fromAddrLen);
    static int setsockopt(MAPS_IO_HANDLE sock, int level, int optName,
	const void *optVal, int optLen);
    static int getsockopt(MAPS_IO_HANDLE sock, int level, int optName,
	void *optVal, int *optLen);
    static int getsockname(MAPS_IO_HANDLE sock, struct sockaddr *addr, int *addrLen);
    static unsigned long Htonl(unsigned long hostLong);
    static unsigned long Ntohl(unsigned long netLong);
    static unsigned short Htons(unsigned short hostShort);
    static unsigned short Ntohs(unsigned short netShort);
    static int select(int width, fd_set *readSet, fd_set *writeSet, fd_set *exceptSet,
	const timeval *timeout);
	//! Returns a positive value on success
    //static int inet_pton(int af, const char* src, void* dst);
    //static const char* inet_ntop(int af, const void* src, char* dst, int cnt);
	//static int getaddrinfo(const char *node, const char *service, const struct addrinfo *hints, struct addrinfo **res);
	//static void freeaddrinfo(struct addrinfo *res);
	//static int getnameinfo(const struct sockaddr *sa, socklen_t salen, char *host, size_t hostlen, char *serv, size_t servlen, int flags);
    //@}
};

//////////////////////////////////////////////////////////////////////
////////////////// INLINE IMPLEMENTATIONS ////////////////////////////
//////////////////////////////////////////////////////////////////////

inline
int
MAPSOsWrapper::open(const char *fileName, int flags, int mode)
{
#ifdef WIN32
#ifdef WINCE
	return open(MAPSString(fileName), flags, mode);
#else
    return ::_open(fileName, flags, mode);
#endif //WINCE
#else
    return ::open(fileName, flags, mode);
#endif // WIN32
}

//// -----------------------------------------------------------
inline
int
MAPSOsWrapper::close(int fd)
{
#ifdef WIN32
#ifdef WINCE
	return close(fd);
#else
    return ::_close(fd);
#endif
#else
    return ::close(fd);
#endif // WIN32
}

//// -----------------------------------------------------------
inline
int
MAPSOsWrapper::read(int fd, char *buffer, size_t maxSize)
{

#ifdef WIN32
#ifdef WINCE
	return read(fd,buffer, (unsigned int)maxSize);
#else
    return ::_read(fd, buffer, (unsigned int)maxSize);
#endif
#else
    return ::read(fd, buffer, maxSize);
#endif // WIN32
}

//// -----------------------------------------------------------
inline
int
MAPSOsWrapper::write(int fd, const char *buffer, size_t numBytes)
{
#ifdef WIN32
#ifdef WINCE
	return write(fd,buffer,(unsigned int)numBytes);
#else
    return ::_write(fd, buffer, (unsigned int)numBytes);
#endif //WINCE
#else
    return ::write(fd, (char *)buffer, numBytes);
#endif // WIN32
}

//// -----------------------------------------------------------
inline
int
MAPSOsWrapper::ioctl(int fd, int function, void* arg)
{
#ifdef WIN32
    return ::ioctlsocket(fd, (long)function, (u_long *)arg);
#else
    return ::ioctl(fd, function, (char*)arg);
#endif // WIN32
}

//// -----------------------------------------------------------
inline
MAPS_IO_HANDLE
MAPSOsWrapper::socket(int af, int type, int protocol)
{
#if defined(WIN32)
    return WSASocket(af, type, protocol, NULL, 0, 0);
#else
    return ::socket(af, type, protocol);
#endif
}

//// -----------------------------------------------------------
inline
int
MAPSOsWrapper::close_socket(MAPS_IO_HANDLE sock)
{
#ifdef WIN32
    return ::closesocket(sock);
#else
    return MAPSOsWrapper::close(sock);
#endif // WIN32
}

//// -----------------------------------------------------------
inline
int 
MAPSOsWrapper::shutdown(MAPS_IO_HANDLE sock, int how)
{
    return ::shutdown(sock, how);
}

//// -----------------------------------------------------------
inline
int
MAPSOsWrapper::bind(MAPS_IO_HANDLE sock, const struct sockaddr *addr, int addrLen)
{
#ifdef WIN32
    return ::bind(sock, addr, addrLen);
#else
    return ::bind(sock, (struct sockaddr *)addr, addrLen);
#endif // WIN32
}

//// -----------------------------------------------------------
inline
int
MAPSOsWrapper::listen(MAPS_IO_HANDLE sock, int backlog)
{
    return ::listen(sock, backlog);
}

//// -----------------------------------------------------------
inline
int
MAPSOsWrapper::connect(MAPS_IO_HANDLE sock, const struct sockaddr *addr, int addrLen)
{
#ifdef WIN32
    return ::connect(sock, addr, addrLen);
#else
    return ::connect(sock, (struct sockaddr *)addr, addrLen);
#endif // WIN32
}

//// -----------------------------------------------------------
inline
MAPS_IO_HANDLE
MAPSOsWrapper::accept(MAPS_IO_HANDLE sock, struct sockaddr *addr, int *addrLen)
{
#if defined(WIN32)
    return ::accept(sock, addr, addrLen);
#elif defined(VXWORKS)
    return ::accept(sock, (struct sockaddr *)addr, addrLen);
#elif defined(LINUX)
    return ::accept(sock, addr, (socklen_t*)addrLen);
#endif
}

//// -----------------------------------------------------------
inline
int
MAPSOsWrapper::send(MAPS_IO_HANDLE sock, const char *buffer, int len, int flags)
{
#ifdef WIN32
    return ::send(sock, buffer, len, flags);
#else
    return ::send(sock, (char *)buffer, len, flags);
#endif // WIN32
}

//// -----------------------------------------------------------
inline
int
MAPSOsWrapper::recv(MAPS_IO_HANDLE sock, char *buffer, int expectedBytes, int flags)
{
    int recvBytes = 0;
    int leftBytes = expectedBytes;
    while (recvBytes < expectedBytes) {
	int bytes = ::recv(sock, buffer, leftBytes, flags);
	if (bytes == MAPS_SOCKET_ERROR) {
	    return -1;
	}
	if (bytes == 0)
	    return -2;
	recvBytes += bytes;
	leftBytes -= bytes;
	buffer += bytes;
    }
    return recvBytes;
}

//// -----------------------------------------------------------
inline
int
MAPSOsWrapper::sendto(MAPS_IO_HANDLE sock, const char *buffer, int len, int flags,
	       const struct sockaddr *toAddr ,int toAddrLen)
{
#ifdef WIN32
    return ::sendto(sock, buffer, len, flags, toAddr, toAddrLen);
#else
    return ::sendto(sock, (char *)buffer, len, flags,
	(struct sockaddr *)toAddr, toAddrLen);
#endif // WIN32
}

//// -----------------------------------------------------------
inline
int
MAPSOsWrapper::recvfrom(MAPS_IO_HANDLE sock, char *buffer, int len, int flags,
struct sockaddr *fromAddr, int *fromAddrLen)
{
#ifdef LINUX
    return ::recvfrom(sock, buffer, len, flags, fromAddr, (socklen_t*)fromAddrLen);
#else
    return ::recvfrom(sock, buffer, len, flags, fromAddr, fromAddrLen);
#endif // LINUX
}

//// -----------------------------------------------------------
inline
int
MAPSOsWrapper::setsockopt(MAPS_IO_HANDLE sock, int level, int optName,
		   const void *optVal, int optLen)
{
#if defined(WIN32)
    return ::setsockopt(sock, level, optName, (char*) optVal, optLen);
#elif defined(VXWORKS)
    return ::setsockopt(sock, level, optName, optVal, optLen);
#elif defined(LINUX)
    return ::setsockopt(sock, level, optName, optVal, (socklen_t)optLen);
#endif
}

//// -----------------------------------------------------------
inline
int
MAPSOsWrapper::getsockopt(MAPS_IO_HANDLE sock, int level, int optName,
		   void *optVal, int *optLen)
{
#if defined(WIN32)
    return ::getsockopt(sock, level, optName, (char*)optVal, optLen);
#elif defined(VXWORKS)
    return ::getsockopt(sock, level, optName, optVal, optLen);
#elif defined(LINUX)
    return ::getsockopt(sock, level, optName, optVal, (socklen_t*)optLen);
#endif
}

//// -----------------------------------------------------------
inline
int
MAPSOsWrapper::getsockname(MAPS_IO_HANDLE sock, struct sockaddr *addr, int *addrLen)
{
#ifdef LINUX
    return ::getsockname(sock, addr, (socklen_t*)addrLen);
#else
    return ::getsockname(sock, addr, addrLen);
#endif // LINUX
}

//// -----------------------------------------------------------
inline
unsigned long MAPSOsWrapper::Htonl(unsigned long hostLong)
{
    return htonl(hostLong);
}

//// -----------------------------------------------------------
inline
unsigned long MAPSOsWrapper::Ntohl(unsigned long netLong)
{
    return ntohl(netLong);
}

//// -----------------------------------------------------------
inline
unsigned short MAPSOsWrapper::Htons(unsigned short hostShort)
{
    return htons(hostShort);
}

//// -----------------------------------------------------------
inline
unsigned short MAPSOsWrapper::Ntohs(unsigned short netShort)
{
    return ntohs(netShort);
}

//// -----------------------------------------------------------
inline
int
MAPSOsWrapper::select(int width, fd_set *readSet, fd_set *writeSet, fd_set *exceptSet,
	       const timeval *timeout)
{
#ifdef WIN32
    return ::select(width, readSet, writeSet, exceptSet, timeout);
#else
    return ::select(width, readSet, writeSet, exceptSet, (struct timeval*)timeout);
#endif
}

//// -----------------------------------------------------------
/*
inline
int MAPSOsWrapper::inet_pton(int af, const char* src, void* dst)
{
#if defined(WIN32)
    switch(af) {
	case AF_INET:
	    {
		struct sockaddr_in sa;
		sa.sin_family=af;
		INT size=sizeof(sa);
		int res = WSAStringToAddress((LPTSTR)src, af, NULL,(LPSOCKADDR)&sa,&size);
		if(res == SOCKET_ERROR)
			return 0;
		*((in_addr*)dst) = sa.sin_addr;
		return 1;
	    }
	case AF_INET6:
	    {
		struct sockaddr_in6 sa6;
		sa6.sin6_family=af;
		INT size=sizeof(sa6);
		int res = WSAStringToAddress((LPTSTR)src, af, NULL,(LPSOCKADDR)&sa6,&size);
		if(res == SOCKET_ERROR)
			return 0;
		*((in6_addr*)dst) = sa6.sin6_addr;
		return 1;
	    }
	default:
	    return 0;
    }
#else
    return ::inet_pton(af, src, dst);
#endif
}
*/
/* -----------------------------------------------------------
  
inline
const char* MAPSOsWrapper::inet_ntop(int af, const void* src, char* dst, int cnt)
{
#if defined(WIN32)
    DWORD dwAdLength=(DWORD)cnt;
    switch(af) {
	case AF_INET:
	    {
		struct sockaddr_in sa;
		sa.sin_family=af;
		sa.sin_addr=*((struct in_addr*)src);
		WSAAddressToString((LPSOCKADDR)&sa,sizeof(sa),NULL,(LPTSTR)dst,&dwAdLength);
		return dst;
	    }
	case AF_INET6:
	    {
		struct sockaddr_in6 sa6;
		sa6.sin6_family=af;
		sa6.sin6_addr=*((struct in6_addr*)src);
		WSAAddressToString((LPSOCKADDR)&sa6,sizeof(sa6),NULL,(LPTSTR)dst,&dwAdLength);
		return dst;
	    }
	default:
	    return NULL;
    }
#else
    return ::inet_ntop(af, src, dst, (socklen_t)cnt);
#endif
}*/

/*//// -----------------------------------------------------------
inline
int getaddrinfo(const char *node, const char *service, const struct addrinfo *hints, struct addrinfo **res)
{
	return ::getaddrinfo(node,service,hints,res);
}

//// -----------------------------------------------------------
inline
void freeaddrinfo(struct addrinfo *res)
{
	return ::freeaddrinfo(res);
}*/

//// -----------------------------------------------------------
/*inline
int getnameinfo(const struct sockaddr *sa, socklen_t salen, char *host, size_t hostlen, char *serv, size_t servlen, int flags)
{
	return ::getnameinfo(sa, salen, host, hostlen, serv, servlen, flags);
}*/

#endif // __MAPSOsWrapper_h__
//// End of file

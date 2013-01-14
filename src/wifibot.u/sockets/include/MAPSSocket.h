#ifndef __MAPSSocket_h
#define __MAPSSocket_h

#include "MAPSSystemDefs.h"
#include "MAPSInetAddr.h"
#ifdef LINUX
#include <netinet/tcp.h> // defines TCP_NODELAY
#endif

typedef enum {
    MAPS_SOCKET_TYPE_TCP,
    MAPS_SOCKET_TYPE_UDP,
    MAPS_SOCKET_TYPE_MCAST
} MAPS_SOCKET_TYPE;

//! Class to handle a socket.
class MAPSSocket
{
private:
	// Copy operator interdit. Voir les DuplicateHandle si on en a vraiment besoin sous windows
	MAPSSocket(const MAPSSocket& srcSock) {}
public:
    MAPSSocket()
	:_handle(MAPS_INVALID_IO_HANDLE), _shutdownBeforeClose(true), _reuseaddr(false) {}
    /*!
     * @param type is the transmission type (stream or datagram for ex.)
     * @param addr the connection address
     */
    MAPSSocket(int type, const MAPSInetAddr &addr, bool shutdownBeforeClose= true)
	:_shutdownBeforeClose(shutdownBeforeClose){Open(type, &addr);}

    virtual ~MAPSSocket(){Close();}

    static const char * GetErrorStr(int errorcode);
    static const char * GetLastErrorStr(void);

public:
    /*! Opens a socket with parameters.
     * @param type the transmission type (stream or datagram for ex.)
     * @param addr the domain connection address
     * @param closeValidSocket set this parameter to \c true if you want to close the current
     * @param multicast set this parameter to \c true if you intend to use this socket for multicast
     * opened socket.
     */
    MAPS_IO_HANDLE Open(int type,
						MAPSInetAddr const* pAddr,
						bool closeValidSocket = true,
						bool multicast = false);
    /*! Accepts connection.
     *  @param timeout Timeout in ms
	 *	@param newSocket A new socket with the connection established
     *  @param remoteAddr Optional pointer to a buffer that receives the address of the connecting entity.
     *  @return Return value means:
	 *		0: success,
	 *		MAPS_SOCKET_ERROR: socket error,
	 *		MAPS_SOCKET_TIMEOUT: timeout
	 *	@remark To make the function interruptible, you can loop with small values for timeout 
	 *  while return value is MAPS_SOCKET_TIMEOUT.
     */
	int Accept(MAPSInt64 timeout, MAPSSocket& newSocket, MAPSInetAddr* remoteAddr = NULL);

    //! Detaches the handle.
    /*! @param closeValidHandle If it is set to true, closes the handle first.
     *  @return \c true on success.
     */
    bool Detach(bool closeValidHandle= false);

	//! Connects to a remote address, cancelable either by a timeout and/or an event
	/*! @param remoteAddr
	 *	@param timeout Timeout en ms
	 *	@param cancelEvent
	 *  @return Return value means:
	 *		0: success,
	 *		MAPS_SOCKET_ERROR: socket error,
	 *		MAPS_SOCKET_TIMEOUT: timeout,
	 *		MAPS_SOCKET_CANCELED: cancelEvent set
	 */
	int Connect(const MAPSInetAddr& remoteAddr, MAPSInt64 timeout, MAPSEvent* cancelEvent=NULL);

	//! Puts the socket in listening state
	/*! @param backLog The max size of the pending connection queue.
	 *  @return ?
	 */
	int Listen(int backLog = SOMAXCONN);

public:
    bool operator==(const MAPSSocket &other) const {return _handle == other._handle;}
    bool operator!=(const MAPSSocket &other) const {return !(*this == other);}

    bool operator==(MAPS_IO_HANDLE handle) const {return _handle == handle;}
    bool operator!=(MAPS_IO_HANDLE handle) const {return !(*this == handle);}

    operator MAPS_IO_HANDLE &() {return _handle;}
    operator MAPS_IO_HANDLE() const {return _handle;}

    MAPS_IO_HANDLE& Handle() {return _handle;}
    MAPS_IO_HANDLE Handle() const {return _handle;}
    MAPS_IO_HANDLE Socket() const {return _handle;}

    //! Tells if the handle is valid, or equals to MAPS_INVALID_IO_HANDLE.
    bool IsValid() const {return _handle != MAPS_INVALID_IO_HANDLE;}

public:
    int SetOpt(int level, int optname, const void *optval, int optlen);
    int GetOpt(int level, int optname, void *optval, int *optlen);

public:
    /*! @name Shutdown/Close functions.
     * Different mode are available, depending on linger and blocking parameters. The 
     * subsequent behavior of the \c Close function is the following:
     * \li If \c SetNoLinger is called, \c Close always returns immediately 
     * —connection is gracefully closed in the background.
     * \li If \c SetLinger is called with <tt>timeoutSeconds=0</tt>: it always returns 
     * immediately —connection is reset/terminated.
     * \li If \c SetLinger is called with a nonzero time-out and the blocking mode is
     * enabled, it blocks until all data sent or time-out expires. 
     * \li If \c SetLinger is called with a nonzero time-out and the blocking mode is
     * disabled, it returns immediately indicating failure. 
     */
    //@{
    //bool SetLinger(unsigned int timeoutSeconds);
    //bool SetNoLinger();
    //! Enables blocking mode.
    //! @param yesOrNo Set this parameter to \c false to switch to non-blocking mode.
    bool SetBlocking(bool yesOrNo);
    //! Tries a shutdown on a call to \c Close.
    //! @param yesOrNo Set this parameter to \c false to disable this feature.
    //void SetShutdownBeforeClose(bool yesOrNo) { _shutdownBeforeClose=yesOrNo; }
    //! Tells if a shutdown must be tried before a call to \c Close.
    //bool IsShutdownBeforeClose() const {return _shutdownBeforeClose;}
    bool Shutdown();
    bool Close();
    //@}

	bool SetTcpNoDelay(bool bVal)
	{ int optVal=bVal?1:0; return 0==SetOpt(IPPROTO_TCP,TCP_NODELAY,&optVal,sizeof(int)); }

	bool SetReuseAddr(bool bVal);

	//!@name Send/Receive functions.
    //@{
    int Send(const char *buffer, unsigned int bufferSize, int flags= 0);

    /*! blocking single timeouted operation
     * @param timeout in milliseconds
     */
    int Send(const void* buffer,unsigned int size,const struct timeval& timeout);

	int SendTo(const void* buffer, unsigned int size, MAPSInetAddr const *const to);
	int SendTo(const void* buffer, unsigned int size, const struct timeval& timeout, MAPSInetAddr const *const to);

	/*! Blocking single timeouted operation 
     * @param buffer Will contain the received data
     * @param size size of the buffer
     * @param timeout in milliseconds
     */
    int Receive(void* buffer,unsigned int size,const struct timeval& timeout)
	{ return ReceiveFrom(buffer,size,timeout,NULL); }
    /*! Receive main function. !!! Don't call this function directly as there is no timeout.
     * @param buffer Will contain the received data
     * @param size size of the buffer
     * @param flags It is safe to set it to 0
     * @param from If not null and socket is connectionless, will hold the source address
     */
	int ReceiveFrom(void* buffer, unsigned int size, int flags = 0, MAPSInetAddr *from = NULL);

	/*! Receive main function
     * @param buffer Will contain the received data
     * @param size size of the buffer
     * @param timeout in milliseconds
     * @param from If not null and socket is connectionless, will hold the source address
     */
    int ReceiveFrom(void* buffer, unsigned int size, const struct timeval& timeout, MAPSInetAddr *from = NULL);
    //@}

    /*! Retreive the max size the underlying protocol can handle.
     * This function is only meaningful once the socket is bound and only for
     * message-oriented socket types.
     */
    unsigned int GetMaxMsgSize(void);

public:
    bool GetAddress(MAPSInetAddr *addr);

protected:
	bool _reuseaddr;
    bool _shutdownBeforeClose;
    MAPS_IO_HANDLE _handle; //!< The handle.
};

#endif // __MAPSSocket_h
//// End of file

#ifndef __MAPSInetAddr_h
#define __MAPSInetAddr_h

#include "MAPSSystemDefs.h"
#include "MAPSOsWrapper.h"

#define MAPS_CAST_IPV4(IP) (*(struct sockaddr_in*)&(IP))
#define MAPS_CAST_IPV6(IP) (*(struct sockaddr_in6*)&(IP))

//! Socket address. 
/*! Internally kept as an internet style address \c sockaddr_storage.
 */
class MAPSInetAddr
{
protected:
    //! The internal socket address
    struct sockaddr_storage _inetAddr;

public:
    MAPSInetAddr() {Clean();}
    MAPSInetAddr(unsigned short port, const char* ipString=NULL, int ai_family = AF_UNSPEC) {Clean();SetAddress(ipString,ai_family);SetPort(port);}

    void Set(struct sockaddr_storage& addr);
    void SetPort(unsigned short port);
	/** Sets the address using getaddrinfo (performs host name resolution if needed).
	 * @param ipString : if null, prepare the socket to be used for listening incoming connections.
	 * Otherwise, can be an IPv4 or IPv6 address or a host name.
	 * @param ai_family : force resolution to IPv4 (AF_INET), IPv6 (AF_INET6), or try auto resolution, then preferring
	 * IPv4 if both are available (AF_UNSPEC).
	 * @return true if succeeded, false otherwise (can be because of failure of hostname resolution for example).
	 * */
   bool SetAddress(const char* ipString, int ai_family = AF_UNSPEC);
    /*! Sets the address to the local machine ip.
     *  @param index For machine that have multiple addresses, allows to select one.
     *  @return \c true on success.
     */
    bool GetOwnIP(unsigned int index=0);

    //Obsolete. Superseeded by SetAddress.
//	void SetAddressByName(const char* hostName, int ai_family = AF_UNSPEC);

	short GetFamily();
    //! Returns the port in host byte order.
    unsigned short GetPort() const;
    //! Returns a string representing the address in host byte order.
    MAPSString GetAddressString() const;

	struct sockaddr_storage& sockaddr_storage() { return _inetAddr; }

	//! Tells if it is a multicast address.
    bool IsMulticast();
    //! Tells if it is a broadcast address.
    bool IsBroadcast();

protected:
    void Clean();
};

#endif // __MAPSInetAddr_h
//// End of file

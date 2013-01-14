#ifndef __MAPSSystemDefs_h
#define __MAPSSystemDefs_h

/* To change FD_SETSIZE, define it before including this file. 
 * Default is 64 on windows, 1024 on linux. 
 */

#ifdef QNX
    #define LINUX
#endif

// ----------------------------------------------------------------------------
// QNX
// ----------------------------------------------------------------------------
#ifdef QNX
    #define LINUX
#endif

// ----------------------------------------------------------------------------
// WIN32
// ----------------------------------------------------------------------------
// WIN32 defintions.
#ifdef WIN32
    #pragma warning(disable:4786)

    #define MAPS_USING_WINSOCK2
    // needed by Timer routines
    //#define _WIN32_WINNT  0x0400

    #include <winsock2.h>
    #pragma comment (lib, "ws2_32.lib")
    #include <ws2tcpip.h>

    // Other includes.
#ifndef WINCE
    #include <io.h> // for _open, _close, _read, _write
#else
	#include <wchar.h>
#endif
    #include <iphlpapi.h>
#endif // WIN32

// ----------------------------------------------------------------------------
// VxWorks.
// ----------------------------------------------------------------------------
// VxWorks defintions.
#ifdef VXWORKS
    #include <stdlib.h>
    #include <semLib.h>
    #include <taskLib.h>
    #include <sysLib.h>
    #include <string.h>
    #include <ctype.h>
    #include <fioLib.h>
    #include <intLib.h>
    #include <timers.h>
    #include <sys/times.h>
    #include <netinet/in.h>
    #include <inetLib.h>
    #include <ioLib.h>
    #include <sockLib.h>
    #include <pipeDrv.h>
    #include <errnoLib.h>
    #include <stat.h>
    #include <hostLib.h>
#endif // VXWORKS

// ----------------------------------------------------------------------------
// Linux
// ----------------------------------------------------------------------------
// Linux defintions.
#ifdef LINUX
    #include <signal.h>
    #include <stdlib.h>
    #include <ctype.h>
    #include <sys/times.h>
    #include <sys/types.h>
    #include <fcntl.h>
    #include <unistd.h>
    #include <sys/ioctl.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <errno.h>
    #include <netdb.h>
    #include <sys/timeb.h>
    #include <sys/time.h>
#endif // LINUX

//// -----------------------------------------------------------
// Io.
#ifdef WIN32
    #define MAPS_IO_HANDLE SOCKET
    #define MAPS_INVALID_IO_HANDLE (MAPS_IO_HANDLE)INVALID_SOCKET
#else
    #define MAPS_IO_HANDLE int
    #define MAPS_INVALID_IO_HANDLE -1
#endif // WIN32

//// -----------------------------------------------------------
// Sockets.
#if defined(WIN32)
    #define MAPS_SOCKET_ERROR SOCKET_ERROR
#elif defined(LINUX)
    #define MAPS_SOCKET_ERROR -1
#elif defined(VXWORKS)
    #define MAPS_SOCKET_ERROR ERROR
#endif

#define MAPS_SOCKET_TIMEOUT -2
#define MAPS_SOCKET_CANCELED -3

#endif // __MAPSSystemDefs_h
//// End of file

/* -*- mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/* *************************************************************************
   ***   RTMaps: Real-Time, Multisensor, Advanced Prototyping Software   ***
   *************************************************************************
      1999-2003 : Bruno STEUX - Pierre COULOMBEAU
      2001-2009 : Nicolas DU LAC - Olivier MEUNIER
      Copyright (c) Intempora S.A. 
   ************************************************************************/

/*! \file
 *	\brief The RTMaps engine header file - Version 3.4 build 99
 */
/*! \mainpage notitle
 * \section intro How to use this documentation
 * This documentation is the reference of the RTMaps SDK. Various entries are available, depending on what your are 
 * looking for, and what you know. 
 * \li You know the name of the class: the menus "Class Hierarchy", "Alphabetical List" and "Class List" give a direct access
 * to the documentation of a class. 
 * \li You know the name of the function, define, typedef...: under "Class Members" you can find the documentation of a member 
 * of a class, while "File Members" offers a list of all the other documented items, for example C-style functions, defines or typedefs.
 * \li Under "Related Pages" is the "Deprecated List". It lists all deprecated features that will be removed in the next version.
 * \li An important section, especially for beginners, is called "Modules".
 *
 * <b>This is only a reference documentation. Don't forget to read the Developer's manual!</b>
 * 
 */

#ifndef _MAPS_H
#define _MAPS_H

#define MAPS_KERNEL_BUILD 99
#define MAPS_KERNEL_VERSION "3.4"
#define MAPS_MINOR_VERSION "4" //-> complete version strings will look like 3.4.0

#if (_MSC_VER >= 1300)
    #pragma warning(push)
    #pragma warning(disable: 4995)
#endif

#include <stdio.h>
#if (_MSC_VER < 1300) /* < .NET 2002 */
#undef MAPS_STD_CPP
#else
#define MAPS_STD_CPP 1
#endif /* _MSC_VER */

#if defined WIN64 || defined _WIN64 || defined __LP64__ || defined _LP64
#define MAPS_64 1
#endif

#if defined LINUX
#include <endian.h>
#endif
#if defined QNX
#include <gulliver.h>
#endif

#if (defined __BYTE_ORDER && (__BYTE_ORDER == __BIG_ENDIAN)) || defined __BIGENDIAN__
#define MAPS_BIG_ENDIAN 1
#endif

#ifdef MAPS_BIG_ENDIAN
#  if defined(LINUX)
#    include <byteswap.h>
#    define MAPS_BSWAP_16(x) bswap_16(x)
#    define MAPS_BSWAP_32(x) bswap_32(x)
#    define MAPS_BSWAP_64(x) bswap_64(x)
#  endif
#  if defined QNX
#    define MAPS_BSWAP_16(x) ENDIAN_LE16(x)
#    define MAPS_BSWAP_32(x) ENDIAN_LE32(x)
#    define MAPS_BSWAP_64(x) ENDIAN_LE64(x)
#  endif
#endif
#ifndef MAPS_BSWAP_16
#  define MAPS_BSWAP_16(x) x
#endif
#ifndef MAPS_BSWAP_32
#  define MAPS_BSWAP_32(x) x
#endif
#ifndef MAPS_BSWAP_64
#  define MAPS_BSWAP_64(x) x
#endif

/* Shared C/C++ types and structures definitions */
#if defined _WIN32 || defined _WIN64 /* Windows */
#if (_MSC_VER < 1300) /* < .NET 2002 */
typedef signed __int64 		MAPSInt64;
typedef unsigned __int64	MAPSUInt64;
typedef signed int			MAPSInt32;
typedef unsigned int		MAPSUInt32;
typedef signed short		MAPSInt16;
typedef unsigned short		MAPSUInt16;
typedef signed char			MAPSInt8;
typedef unsigned char		MAPSUInt8;
#else
typedef signed __int64 		MAPSInt64;
typedef unsigned __int64	MAPSUInt64;
typedef signed __int32		MAPSInt32;
typedef unsigned __int32	MAPSUInt32;
typedef signed __int16		MAPSInt16;
typedef unsigned __int16	MAPSUInt16;
typedef signed __int8		MAPSInt8;
typedef unsigned __int8		MAPSUInt8;
#endif /* _MSC_VER */
typedef unsigned long		MAPSThreadId;
#define MAPS_DLL_EXPORT __declspec(dllexport)
#else /* POSIX OS */
/*#include <sys/types.h>
typedef __int64_t 	MAPSInt64;
typedef __uint64_t	MAPSUInt64;
typedef __int32_t	MAPSInt32;
typedef __uint32_t	MAPSUInt32;
typedef __int16_t	MAPSInt16;
typedef __uint16_t	MAPSUInt16;
typedef __int8_t	MAPSInt8;
typedef __uint8_t	MAPSUInt8;*/
/* LSB 3.2 conformance */
#include <stdint.h>
typedef int64_t 	MAPSInt64;
typedef uint64_t	MAPSUInt64;
typedef int32_t		MAPSInt32;
typedef uint32_t	MAPSUInt32;
typedef int16_t		MAPSInt16;
typedef uint16_t	MAPSUInt16;
typedef int8_t		MAPSInt8;
typedef uint8_t		MAPSUInt8;

#include <pthread.h>
typedef pthread_t 	MAPSThreadId;
#if __GNUC__ >= 4
#define MAPS_DLL_EXPORT __attribute__ ((visibility("default")))
#else
#define MAPS_DLL_EXPORT
#endif
#endif /* Windows */

/*! \var typedef MAPSInt32 MAPSInteger
 *	\brief The MAPSInteger should be a 32-bit integer, even on 64-bit architecture
 */
typedef MAPSInt32	MAPSInteger;

typedef double	MAPSFloat64;
typedef float	MAPSFloat32;
/*! \var typedef double MAPSFloat
 *	\brief The MAPSFloat type is a double precision floating point number
 *
 *  RTMaps always uses MAPSFloat (\c double = 64-bit floating point numbers).
 *  Please never use \c float (\c float = 32 bits floating point numbers)
 */
typedef MAPSFloat64	MAPSFloat; 

/*! \var typedef MAPSInt64 MAPSTimestamp
 *	\brief The MAPSTimestamp type is a 64-bit integer
 *
 *  RTMaps always timestamps data with a 64-bit integer representing the
 *  amount of time in microseconds since the last Run command execution (the reference time)
 */
typedef MAPSInt64 MAPSTimestamp;
/*! \var typedef MAPSInt64 MAPSDelay
 *	\brief A 64-bit integer that specifies a delay in microseconds.
 */
typedef MAPSInt64 MAPSDelay;
/*! \var typedef MAPSInt64 MAPSTypeInfoValue
 *	\brief The MAPSTypeInfoValue type is a 64 bit-integer. All types in RTMaps are associated to a 64-bit value.
 */
typedef MAPSInt64 MAPSTypeInfoValue;

/*! \var typedef MAPSInt32 MAPS3States
 *	\brief 3-state value, generally used to tell the state of a thread.
 *
 *	It can take the MAPS::DeadState, MAPS::DyingState and MAPS::LivingState values.
 */
typedef MAPSInt32 MAPS3States;

//! 24-bit integer color representation
/*! Stricly equivalent to the Windows RGB macro.
 *  Please use this one in place of RGB for better portability of the code.
 */
#define MAPS_RGB(r,g,b) ((r)|((g)<<8)|((b)<<16))

//! 24-bit integer color representation + 8-bit alpha value (transparency)
#define MAPS_RGBA(r,g,b,a) ((r)|((g)<<8)|((b)<<16)|((a)<<24))

//! The Pi number
/*! You should use this instead of M_PI, which is not defined on some plateforms.
 */
#define MAPS_PI 3.1415926535897932384626433832795

//! Degrees to radians conversion macro
#define MAPS_DEG2RAD(x) (((x)*MAPS_PI)/180)

#ifdef MAPS_BIG_ENDIAN
#define MAPS_FC( ch0, ch1, ch2, ch3 )                                \
        ( (MAPSUInt32)(unsigned char)(ch3) | ( (MAPSUInt32)(unsigned char)(ch2) << 8 ) |    \
        ( (MAPSUInt32)(unsigned char)(ch1) << 16 ) | ( (MAPSUInt32)(unsigned char)(ch0) << 24 ) )
#else
#define MAPS_FC( ch0, ch1, ch2, ch3 )                                \
        ( (MAPSUInt32)(unsigned char)(ch0) | ( (MAPSUInt32)(unsigned char)(ch1) << 8 ) |    \
        ( (MAPSUInt32)(unsigned char)(ch2) << 16 ) | ( (MAPSUInt32)(unsigned char)(ch3) << 24 ) )
#endif

#define MAPS_COLORMODEL_RGB   MAPS_FC('R','G','B',000)
#define MAPS_COLORMODEL_RGBA  MAPS_FC('R','G','B','A')
#define MAPS_COLORMODEL_YUV   MAPS_FC('Y','U','V',000)
#define MAPS_COLORMODEL_YUVA  MAPS_FC('Y','U','V','A')
#define MAPS_COLORMODEL_GRAY  MAPS_FC('G','R','A','Y')
#define MAPS_CHANNELSEQ_RGB   MAPS_FC('R','G','B',000)
#define MAPS_CHANNELSEQ_BGR   MAPS_FC('B','G','R',000)
#define MAPS_CHANNELSEQ_RGBA  MAPS_FC('R','G','B','A')
#define MAPS_CHANNELSEQ_BGRA  MAPS_FC('B','G','R','A')
#define MAPS_CHANNELSEQ_YUV   MAPS_FC('Y','U','V',000)
#define MAPS_CHANNELSEQ_YUVA  MAPS_FC('Y','U','V','A')
#define MAPS_CHANNELSEQ_GRAY  MAPS_FC('G','R','A','Y')
#define MAPS_IMAGETYPE_MONO   MAPS_FC('M','O','N','O')
#define MAPS_IMAGETYPE_COLOR  MAPS_FC('C','O','L','R')

#define MAPS_IMAGECODING_UNKNOWN MAPS_FC('?','?','?','?') 
#define MAPS_IMAGECODING_555  MAPS_FC('5','5','5',000) /*RGB 555 format : 16 bits per pixel. High order byte : XRRRRRGG. Low order byte : GGGBBBBB*/
#define MAPS_IMAGECODING_565  MAPS_FC('5','6','5',000) /*RGB 565 format : 16 bits per pixel. High order byte : RRRRRGGG. Low order byte : GGGBBBBB*/
#define MAPS_IMAGECODING_RAW  MAPS_FC('R','A','W',000) /* RAW format */
#define MAPS_IMAGECODING_MTRX MAPS_FC('M','T','R','X') /* MATROX LOSSY_INTERLACED format */
#define MAPS_IMAGECODING_JPEG MAPS_FC('J','P','E','G') /* 'JPEG' compressed (raw ) */
#define MAPS_IMAGECODING_JPGE MAPS_FC('J','P','G','E') /* 'JPEG-ELS' compressed  */
#define MAPS_IMAGECODING_JPGL MAPS_FC('J','P','G','L') /* 'JPEG Lossless */
#define MAPS_IMAGECODING_LJPG MAPS_FC('L','J','P','G') /* PIC2 JPEG Lossless */
#define MAPS_IMAGECODING_PJPG MAPS_FC('P','J','P','G') /* 'PJPG' progressive compressed (raw) */
#define MAPS_IMAGECODING_TGA1 MAPS_FC('T','G','A','1') /* 'TGA1' Targa type 1 (cm) uncompressed */
#define MAPS_IMAGECODING_TGA2 MAPS_FC('T','G','A','2') /* 'TGA2' Targa type 2 (bgr) uncompressed */
#define MAPS_IMAGECODING_TGA3 MAPS_FC('T','G','A','3') /* 'TGA3' Targa type 3 (gray) uncompressed */
#define MAPS_IMAGECODING_TGA9 MAPS_FC('T','G','A','9') /* 'TGA9' Targa type 9 (cm) RLE */
#define MAPS_IMAGECODING_TGAA MAPS_FC('T','G','A','A') /* 'TGAA' Targa type 10 (bgr) RLE */
#define MAPS_IMAGECODING_TGAB MAPS_FC('T','G','A','B') /* 'TGAB' Targa type 11 (gray) RLE */
#define MAPS_IMAGECODING_PICJ MAPS_FC('P','I','C','J') /* 'PICJ' compressed (PIC JPEG) */
#define MAPS_IMAGECODING_PICL MAPS_FC('P','I','C','L') /* 'PICL' compressed */
#define MAPS_IMAGECODING_PICP MAPS_FC('P','I','C','P') /* 'PICP' compressed (PIC prog. JPEG) */
#define MAPS_IMAGECODING_PIC1 MAPS_FC('P','I','C','1') /* 'PIC1' compressed */
#define MAPS_IMAGECODING_PIC4 MAPS_FC('P','I','C','4') /* 'PIC4' compressed */
#define MAPS_IMAGECODING_PIC8 MAPS_FC('P','I','C','8') /* 'PIC8' compressed */
#define MAPS_IMAGECODING_PICG MAPS_FC('P','I','C','G') /* 'PICG' compressed */
#define MAPS_IMAGECODING_PICX MAPS_FC('P','I','C','X') /* 'PICX' compressed */
#define MAPS_IMAGECODING_PC2J MAPS_FC('P','C','2','J') /* 'PC2J' compressed (PIC2 JPEG) */
#define MAPS_IMAGECODING_PC2E MAPS_FC('P','C','2','E') /* 'PC2E' compressed (PIC2 JPEG Els) */
#define MAPS_IMAGECODING_PC2L MAPS_FC('P','C','2','L') /* 'PC2L' compressed */
#define MAPS_IMAGECODING_PC2P MAPS_FC('P','C','2','P') /* 'PC2P' compressed (PIC2 prog. JPEG) */
#define MAPS_IMAGECODING_PC2N MAPS_FC('P','C','2','N') /* 'PC2N' compressed (PIC2 prog. JPEG Els) */
#define MAPS_IMAGECODING_PC21 MAPS_FC('P','C','2','1') /* 'PC21' compressed */
#define MAPS_IMAGECODING_PC24 MAPS_FC('P','C','2','4') /* 'PC24' compressed */
#define MAPS_IMAGECODING_PC28 MAPS_FC('P','C','2','8') /* 'PC28' compressed */
#define MAPS_IMAGECODING_PC2G MAPS_FC('P','C','2','G') /* 'PC2G' compressed */
#define MAPS_IMAGECODING_PC2X MAPS_FC('P','C','2','X') /* 'PC2X' compressed */
#define MAPS_IMAGECODING_RGB565 MAPS_FC('R','5','6','5')   /* to recognize rgb 565 format */
#define MAPS_IMAGECODING_BMPO MAPS_FC('B','M','P','O') /* 'BMPO' old-style BMP uncompressed */
                                    /* (OS/2 1.1 and 1.2) */
#define MAPS_IMAGECODING_BMPR MAPS_FC('B','M','P','R') /* 'BMPR' new-style BMP RLE */
#define MAPS_IMAGECODING_PCX1 MAPS_FC('P','C','X','1') /* 'PCX1' PCX 1-bit */
#define MAPS_IMAGECODING_PCX2 MAPS_FC('P','C','X','2') /* 'PCX2' PCX 2-bit ('CGA') (cm) */
#define MAPS_IMAGECODING_PCX3 MAPS_FC('P','C','X','3') /* 'PCX3' PCX 3-bit ('EGA') (cm) */
#define MAPS_IMAGECODING_PCX4 MAPS_FC('P','C','X','4') /* 'PCX4' PCX 4-bit (cm) */
#define MAPS_IMAGECODING_PCX8 MAPS_FC('P','C','X','8') /* 'PCX8' PCX 8-bit (cm) */
#define MAPS_IMAGECODING_PCXT MAPS_FC('P','C','X','T') /* 'PCXT' PCX 24-bit */
#define MAPS_IMAGECODING_DCXZ MAPS_FC('D','C','X','Z') /* 'DCXZ' DCX multi-image */
#define MAPS_IMAGECODING_TIF1 MAPS_FC('T','I','F','1') /* 'TIF1' TIFF 1-bit */
#define MAPS_IMAGECODING_TIFM MAPS_FC('T','I','F','M') /* 'TIFM' TIFF color-map (up to 8-bit) */
#define MAPS_IMAGECODING_TIFG MAPS_FC('T','I','F','G') /* 'TIFG' TIFF gray-scale (up to 8-bit) */
#define MAPS_IMAGECODING_TIFC MAPS_FC('T','I','F','C') /* 'TIFC' TIFF rgb (16,24,32-bit) */
#define MAPS_IMAGECODING_TIFZ MAPS_FC('T','I','F','Z') /* 'TIFZ' TIFF multi-image */
#define MAPS_IMAGECODING_TIFJ MAPS_FC('T','I','F','J') /* 'TIFJ' TIFF Jpeg */
#define MAPS_IMAGECODING_TIFL MAPS_FC('T','I','F','L') /* 'TIFL' TIFF LZW */
#define MAPS_IMAGECODING_TIFK MAPS_FC('T','I','F','K') /* 'TIFK' TIFF CMYK */
#define MAPS_IMAGECODING_TIFu MAPS_FC('T','I','F','u') /* 'TIFu' TIFF unknown */
#define MAPS_IMAGECODING_GIFN MAPS_FC('G','I','F','N') /* 'GIFN' GIF non-interlaced */
#define MAPS_IMAGECODING_GIFI MAPS_FC('G','I','F','I') /* 'GIFI' GIF interlaced */
#define MAPS_IMAGECODING_GIFu MAPS_FC('G','I','F','u') /* 'GIFu' GIF unknown */
#define MAPS_IMAGECODING_PNG  MAPS_FC('P','N','G',' ') /* 'PNG ' PNG all flavors */
#define MAPS_IMAGECODING_OS2  MAPS_FC('O','S','2','U') /* 'OS2U' OS2 2.x+ uncompressed */
#define MAPS_IMAGECODING_TGA  MAPS_FC('T','G','A',' ') /* 'TGA ' Targa generic */
#define MAPS_IMAGECODING_PCX  MAPS_FC('P','C','X',' ') /* 'PCX ' PCX generic */
#define MAPS_IMAGECODING_TIF  MAPS_FC('T','I','F',' ') /* 'TIF ' TIFF generic */
#define MAPS_IMAGECODING_PCD  MAPS_FC('P','C','D',' ') /* 'PCD ' PhotoCD generic */
#define MAPS_IMAGECODING_G3   MAPS_FC('G','3',' ',' ') /* 'G3  ' Raw G3 fax -- 1D */
#define MAPS_IMAGECODING_G32D MAPS_FC('G','3','2','D') /* 'G32D' Raw G3 fax -- 2D */
#define MAPS_IMAGECODING_CMYK MAPS_FC('C','M','Y','K') /* 'CMYK' 4-byte interlaced */
#define MAPS_IMAGECODING_WAVE MAPS_FC('W','A','V','E') /* 'WAVE' wavelet */
#define MAPS_IMAGECODING_WAV2 MAPS_FC('W','A','V','2') /* 'WAV2' wavelet */
#define MAPS_IMAGECODING_WAVP MAPS_FC('W','A','V','P') /* 'WAVP' progressive wavelet */
#define MAPS_IMAGECODING_GR12 MAPS_FC('G','R','1','2') /* 'GR12' 12-bit gray uncompressed,low-order bits in 16-bit field */
#define MAPS_IMAGECODING_DJVU MAPS_FC('D','J','V','U') /* 'DJVU' AT&T DJVU(tm) */
#define MAPS_IMAGECODING_WSQ  MAPS_FC('W','S','Q',' ') /* 'WSQ ' */
/* Packed YUV formats */
#define MAPS_IMAGECODING_UYVY MAPS_FC('U','Y','V','Y') /* 'UYVY' uyvy video subtype : U0 Y0 V0 Y1 U2 Y2 V2 Y3 ... : 16 bpp*/
#define MAPS_IMAGECODING_YUY2 MAPS_FC('Y','U','Y','2') /* 'YUY2' yuy2 video subtype : Y0 U0 Y1 V0 Y2 U2 Y3 V2 ...*: 16 bpp*/
#define MAPS_IMAGECODING_YUYV MAPS_FC('Y','U','Y','V') /* 'YUYV' yuyv video subtype : same as YUY2*/
#define MAPS_IMAGECODING_VYUY MAPS_FC('V','Y','U','Y') /* 'VYUY' vyuy video subtype : V0 Y0 U0 Y1 V2 Y2 U2 Y3 ... : 16 bpp*/
#define MAPS_IMAGECODING_YVYU MAPS_FC('Y','V','Y','U') /* 'YVYU' yvyu video subtype : Y0 V0 Y1 U0 Y2 V2 Y3 U2 ... : 16 bpp*/
#define MAPS_IMAGECODING_Y41P MAPS_FC('Y','4','1','P') /* 'Y41P' y41p video subtype : U0 Y0 V0 Y1 U4 Y2 V4 Y3 Y4 Y5 Y6 Y7 .... : 12 bpp*/
#define MAPS_IMAGECODING_211  MAPS_FC('2','1','1',000) /* 'Y211' y211 video subtype : Y0 U0 Y2 V0 Y4 U4 Y6 V4 ... : 8 bpp*/
/* Planar YUV formats */
#define MAPS_IMAGECODING_422  MAPS_FC('4','2','2',000) /* YUV 422 (Planar) : Y0Y1...... U0... V0... -> 2 pixels : 16 bpp*/
#define MAPS_IMAGECODING_420  MAPS_FC('4','2','0',000) /* YUV 420 (Planar) : Y00Y01 ... U0... V0... -> 4 pixels : 12 bpp
									     Y10Y11 ...   (mainly used for PAL images)*/
#define MAPS_IMAGECODING_I420 MAPS_FC('I','4','2','0') /* Same as MAPS_IMAGE_CODING_420*/
#define MAPS_IMAGECODING_YV12 MAPS_FC('Y','V','1','2') /* YVU 420 (Planar) : same as 420 but U & V planes are switched. : 12 bpp*/
#define MAPS_IMAGECODING_411  MAPS_FC('4','1','1',000) /* YUV 411 (Planar) : Y0Y1Y2Y3.... U0... V0... -> 4 pixels (mainly used for NTSC images) : 12 bpp*/
#define MAPS_IMAGECODING_UYY  MAPS_FC('U','Y','Y',000) /* YUV 411 (Packed) : U0Y0Y1V2Y2Y3... -> 4 pixels : 12 bpp*/
#define MAPS_IMAGECODING_IYU  MAPS_FC('I','Y','U',000) /* Same as UYY just above : see DCAM spec*/
#define MAPS_IMAGECODING_YVU9 MAPS_FC('Y','V','U','9') /* Y00...Y03	U0	V0
							  ...
							  Y30...Y33 : 9 or 10 bpp ?*/
/* Consumer DV formats */
#define MAPS_IMAGECODING_DVSL MAPS_FC('d','v','s','l') /* SD-DVCR (525-60 or 625-50) : DV 12.5 Mbps*/
#define MAPS_IMAGECODING_DVSD MAPS_FC('d','v','s','d') /* SDL-DVCR (525-60 or 625-50) : DV 25 Mbps*/
#define MAPS_IMAGECODING_DVHD MAPS_FC('d','v','h','d') /* HD-DVCR (1125-60 or HD-DVCR 1250-50) : DV 50 Mbps*/ 
/* Professional DV formats */
#define MAPS_IMAGECODING_DV25 MAPS_FC('d','v','2','5') /* DVCPRO 25 (525-60 or 625-50)*/
#define MAPS_IMAGECODING_DV50 MAPS_FC('d','v','5','0') /* DVCPRO 50 (525-60 or 625-50)*/
#define MAPS_IMAGECODING_DVH1 MAPS_FC('d','v','h','1') /* DVCPRO 100 (1080/60i, 1080/50i, or 720/60P)*/

/* The C++ only part */
#ifdef __cplusplus

//! Standard macro for safely deleting a pointer to an array
#define MAPS_SAFE_DELETE(p)			{if(p) { delete   (p); (p) = NULL; }}
//! Standard macro for safely deleting a single-element pointer
#define MAPS_SAFE_DELETE_ARRAY(p)	{if(p) { delete[] (p); (p) = NULL; }}

// C++ classes definitions
class MAPSAction;
class MAPSComponent;
class MAPSCondition;
class MAPSCoreFunctionInterface;
class MAPSEvent;
class MAPSFile;
class MAPSFileHandle;
class MAPSFileWriteHandle;
class MAPSFileReadHandle;
class MAPSFlowVolume;
class MAPSHMIKeysHandler;
class MAPSInput;
class MAPSIOElt;
class MAPSIOMonitor;
class MAPSKernelInterface;
class MAPSModule;
class MAPSOutput;
class MAPSPlayer;
class MAPSProperty;
class MAPSSynchronizer;
template<typename T> class MAPSFastIO;
struct MAPSAbsoluteTime;
struct MAPSComponentDefinition;
struct MAPSOutputDefinition;
typedef void (*MAPSRegisteringFunction)(void);
typedef void (*MAPSUnregisteringFunction)(void);

#define MAPSArray2D MAPSMatrix2
namespace MAPS {
    /*! \name C standard library wrapper
     *	These functions should be called instead of their C counterparts
     *	to ensures easier cross-platform design of components.
     */
    //@{
    //! Ascii to integer conversion
    int
    Atoi(const char* a);
    //! Ascii to integer (64 bits) conversion
    MAPSInt64
    Atoi64(const char* a);
	//! Converts the initial part of the string in s to a MAPSInt32 according to the given base
	MAPSInt32
	Strtol(const char* s,
		   char** endptr,
		   int base);
	//! Converts the initial part of the string in s to a MAPSUInt32 according to the given base
	MAPSUInt32
	Strtoul(const char* s,
			char** endptr,
			int base);
	//! Converts the initial part of the string in s to a MAPSInt64 according to the given base
	MAPSInt64
	Strtoi64(const char* s,
			 char** endptr,
			 int base);
	//! Converts the initial part of the string in s to a MAPSUInt64 according to the given base
	MAPSUInt64
	Strtoui64(const char* s,
			  char** endptr,
			  int base);
    //! Calculates the length of the string \a s
    int Strlen(const char* s);
    //! Finds character \a ch in \a s
	const char*
    Strchr(const char* s,
	   char ch);
	//! Finds character \a ch in \a s
	char*
	Strchr(char* s,
		   char ch);
    //! String comparison
    int
    Strcmp(const char* s1,
	   const char* s2);
    //! Lowercase comparison of strings
    int
    Stricmp(const char* s1,
	    const char* s2);
    //! Compares the first \a size characters of two strings.
    int
    Strncmp(const char* s1,
	    const char* s2,
	    MAPSInt64 size);
    //! Returns a pointer to the first occurrence of a search string \a strSearch in the string \a s
    char*
    Strstr(const char* s,
	   const char* strSearch);
    //! Copies \a strSrc into \a strDest
    char*
    Strcpy(char* strDest,
	   const char* strSrc);
	//! Duplicate strings.
	char*
	Strdup(const char* strSrc);

    //! Integer to Ascii conversion
    char*
    Itoa(int val,
	 char* buf,
	 int radix=10);
    //! Integer to Ascii conversion
    char*
    Itoa(unsigned long val,
	 char* buf,
	 int radix=10);
    //! Integer to Ascii conversion
    char*
    Itoa(MAPSInt64 val,
	 char* buf,
	 int radix=10);
    //! Is the character \a c a space, a tab or a carriage return?
    bool
    IsSpace(char c);
    //! Is the character \a c a digit?
    bool
    IsDigit(char c);
    //! Memory copy
    /*! Should be ALWAYS called instead of memcpy */
    void*
    Memcpy(void* dest,
	   const void* source,
	   MAPSInt64 size,
	   MAPSEvent* event=NULL);
    //! Sets buffers to a specified character \a c, repeated \a size times.
    void*
    Memset(void* dest,
	   int c,
	   MAPSInt64 size);
    //! Moves one buffer to another.
    void*
    Memmove(void* dest,
	    const void* source,
	    MAPSInt64 size);
    //! Splits a floating-point value into fractional and integer parts.
    double
    Modf(double x, double* intptr);
    //! Calculates the absolute value of the floating-point argument.
    double
    Fabs(double x);
    //@}


};

//  // Basic data structures used by RTMaps
// 

#ifndef _BDS
#define _BDS

#ifdef DEFBOOL
typedef char bool
#define true 1
#define false 0
#endif

#ifdef _DEBUG
#define BDS_RUNTIME_CHECK
#endif

/*!
 * \defgroup bds Basic data structures
 *
 * The following classes are the basis for all the data structures used by the RTMaps Engine.
 * They are generally simpler but similar to their STL counterparts.
 * These classes are available to developers using the RTMaps SDK to ensure portable code as
 * they have been especially designed to be fully cross-platform.
 */

 //! Template class for couple data
 /*!
  * \ingroup bds
  */
template<typename T> 
    class MAPSCouple
    {
    protected:
	T t[2];
    public:
	//! Returns a reference to the first element.
	T&
	first() {return t[0];}
	//! Returns a reference to the second element.
	T&
	second() {return t[1];}
	//! Unsafe! Use it at your own risk
	T&
	operator[](int i) {return t[i];}
	MAPSCouple(const T& first,
		   const T& second) 
	{ t[0]=first;t[1]=second; }
	virtual ~MAPSCouple() {}
    };

// Linear lists (bidirectionnal)
class MAPSBasicList;
template<typename T> class MAPSList;

//! The basic list item class.
/*! This is the base element of a MAPSBasicList.
 * \warning Should not be used directly by the end user.
 * \ingroup bds
 */
class MAPSBasicListItem
{
    friend class MAPSListIterator;
#if (_MSC_VER <= 1300) // <= .NET 2002
public :
#else
    template<typename T> friend class MAPSList;
protected :
#endif

    MAPSBasicListItem* predecessor;
    MAPSBasicListItem* successor;

    virtual ~MAPSBasicListItem() {}
public:
    MAPSBasicListItem* Prev() {return predecessor;}
    MAPSBasicListItem* Next() {return successor;}

#ifdef USE_COMPATIBILITY_V2
    MAPSBasicListItem* previous() {return predecessor;}
    MAPSBasicListItem* prev() {return predecessor;}
    MAPSBasicListItem* next() {return successor;}
#endif
};

//! The list iterator class.
/*! Use this iterator in conjunction with MAPSBasicList or MAPSList.
 * \ingroup bds
 */
class MAPSListIterator 
{
    MAPSBasicListItem* ptr;

public:
    operator MAPSBasicListItem*() {return ptr;}
    //! Prefix increment operator. Increments iterator and returns the new iterator
    MAPSListIterator& operator++() {ptr=ptr->successor; return *this;}
    //! Postfix increment operator. Increments iterator and returns the old iterator
    MAPSListIterator operator++(int) {MAPSListIterator tmp=*this; ptr=ptr->successor; return tmp;}
    //! Prefix decrement operator. Decrements iterator and returns the new iterator
    MAPSListIterator& operator--() {ptr=ptr->predecessor; return *this;}
    //! Postfix decrement operator. Decrements iterator and returns the old iterator
    MAPSListIterator operator--(int) {MAPSListIterator tmp=*this; ptr=ptr->predecessor; return tmp;}
    //! Replaces the current list item by \a p
    MAPSListIterator operator=(MAPSBasicListItem* p) {ptr=p; return *this;}
    //! Ckecks if two list items are the same item (not only have the same content)
    bool operator==(MAPSBasicListItem* p) {return (ptr==p);}
    MAPSListIterator Predecessor() {return MAPSListIterator(ptr->predecessor);}
    MAPSListIterator Successor() {return MAPSListIterator(ptr->successor);}
    //! Default constructor
    MAPSListIterator() {ptr=NULL;}
    //! Constructor from a basic list item \a p
    explicit MAPSListIterator(MAPSBasicListItem* p) {ptr=p;}
    //! Initializes the iterator to the first item of a MAPSBasicList or \c NULL if \a list is empty
    explicit MAPSListIterator(MAPSBasicList& list);

#ifdef USE_COMPATIBILITY_V2
    MAPSListIterator predecessor() {return MAPSListIterator(ptr->predecessor);} // \deprecated Use MAPSListIterator#Predecessor
    MAPSListIterator successor() {return MAPSListIterator(ptr->successor);} // \deprecated Use MAPSListIterator#Successor
#endif
};

//! The list item template class
/*! Should be only for internal use.
 * \ingroup bds
 */
template <typename T> 
    class MAPSListItem : public MAPSBasicListItem
    {
	T content;

    public:
	MAPSListItem() {}
	explicit 
	MAPSListItem(const T& value) : content(value) {}
	operator MAPSListIterator() {return MAPSListIterator(this);}
	T& 
	operator*() {return content;}

	virtual ~MAPSListItem() {}
    };

//! The base class for the MAPSList template class
/*! Should not be used directly.
 * \ingroup bds
 */
class MAPSBasicList
{
protected:
    MAPSBasicListItem* first_item;
    MAPSBasicListItem* last_item;
    int l;

public:
    //! Returns \c true if the list is empty
    bool 
    Empty() const {return l==0;}
    //! Returns the length of the list (the number of elements in the list)
    int
    Length() const {return l;}
    //! Returns the number of elements in the list
    int
    Size() const {return l;}
    //! Returns the number of elements in the list
    int
    Dim() const {return l;}

    /*! \name Iteration management
     * @{
     */
    //! Returns an iterator corresponding to the first element in the list (\c NULL if the list is empty)
    MAPSListIterator
    First() const {return MAPSListIterator(first_item);}
    //! Returns an iterator corresponding to the first element in the list (\c NULL if the list is empty)
    MAPSListIterator
    Iterator() const {return MAPSListIterator(first_item);}
    //! Returns an iterator corresponding to the first element in the list (\c NULL if the list is empty)
    MAPSListIterator
    Begin() const {return MAPSListIterator(first_item);}
    //! Returns an iterator corresponding to the last element in the list (\c NULL if the list is empty)
    MAPSListIterator
    Last() const {return MAPSListIterator(last_item);}
    //! Returns a \c NULL iterator
    MAPSListIterator
    End() const {return MAPSListIterator(NULL);}

    //! Increments iterator \a it. Returns the next item in the list.
    MAPSListIterator&
    Next(MAPSListIterator& it) const {if(it!=NULL) ++it; return it;}
    //! Decrements iterator \a it. Returns the previous item in the list.
    MAPSListIterator&
    Previous(MAPSListIterator& it) const {if (it!=NULL) --it; return it;}
    //! Has the iterator \a it reached the end of the list ?
    bool
    Finished(MAPSListIterator it) const {return (it==NULL);}
    //! Does the iterator \a it correspond to the first element in the list?
    bool
    IsFirst(MAPSListIterator it) const {return (it==First());}
    //! Does the iterator \a it correspond to the last element in the list?
    bool
    IsLast(MAPSListIterator it) const {return (it==Last());}
    /*@}*/

    //! Default constructor
    MAPSBasicList() : first_item(NULL),last_item(NULL),l(0) { }

    virtual ~MAPSBasicList() {}

#ifdef USE_COMPATIBILITY_V2
    /*! \name Deprecated members
     * @{
     */
    MAPSListIterator begin() const {return MAPSListIterator(first_item);} //!< \deprecated Use MAPSBasicList::Begin
    MAPSListIterator end() const {return MAPSListIterator(NULL);} //!< \deprecated Use MAPSBasicList::End
    MAPSListIterator first() const {return MAPSListIterator(first_item);} //!< \deprecated Use MAPSBasicList::First
    MAPSListIterator last() const {return MAPSListIterator(last_item);} //!< \deprecated Use MAPSBasicList::Last
    bool empty() {return l==0;} //!< \deprecated Use MAPSBasicList::Empty
    int length() {return l;} //!< \deprecated Use MAPSBasicList::Length
    int size() {return l;} //!< \deprecated Use MAPSBasicList::Length
    int dim() {return l;} //!< \deprecated Use MAPSBasicList::Length
    MAPSListIterator NewIterator() const {return MAPSListIterator(first_item);} //!< \deprecated Use MAPSBasicList::Iterator
    MAPSListIterator new_iterator() const {return MAPSListIterator(first_item);} //!< \deprecated Use MAPSBasicList::Iterator
    MAPSListIterator iterator() const {return MAPSListIterator(first_item);} //!< \deprecated Use MAPSBasicList::Iterator
    MAPSListIterator& next(MAPSListIterator& it) const {if(it!=NULL) ++it;return it;} //!< \deprecated Use MAPSBasicList::Next
    MAPSListIterator& previous(MAPSListIterator& it) const {if(it!=NULL) --it;return it;} //!< \deprecated Use MAPSBasicList::Previous
    bool finished(MAPSListIterator it) const {return (it==NULL);} //!< \deprecated Use MAPSBasicList::Finished
    bool is_first(MAPSListIterator it) const {return (it==first());} //!< \deprecated Use MAPSBasicList::IsFirst
    bool is_last(MAPSListIterator it) const {return (it==last());} //!< \deprecated Use MAPSBasicList::IsLast
    /*@}*/
#endif
};

inline
MAPSListIterator::MAPSListIterator(MAPSBasicList& basicList) {ptr=basicList.First();}

/*! Helper macro to walk through a MAPSBasicList. Typical use is like:
 * \code
 MAPSList<MAPSString> li;
 // ... populate the list ...
 MAPSListIterator iter;
 // Walk through the list:
 MAPSForallItems(iter,li) {
     std::cout << li[iter] << std::endl;
 }
 \endcode
 * \param it a MAPSListIterator
 * \param L a MAPSBasicList
 * \hideinitializer
 * \ingroup bds
 */
#define MAPSForallItems(it,L) for((it)=(L).Iterator();!(L).Finished(it);(L).Next(it))

#ifdef USE_COMPATIBILITY_V2
#define MAPSForeachItem(it,L) MAPSForallItems(it,L) // deprecated
#endif

/*! Helper macro to walk through a MAPSList with an access to the list elements.
 * \code
 MAPSList<MAPSString> li;
 // ... populate li ...
 // Displays li elements:
 MAPSString x;
 MAPSForall(x,li) {
     std::cout << x << std::endl;
 }
 \endcode
 * \param x a \a T object
 * \param L a MAPSList<T>
 * \hideinitializer
 * \ingroup bds
 */
#define MAPSForall(x,L) MAPSListIterator __it##x;__it##x=(L).Iterator();if(__it##x)(x)=(L)[__it##x];for(;(!(L).Finished(__it##x))&&((x)=(L)[__it##x],true);(L).Next(__it##x))

/*! Helper macro to walk through a MAPSList with an access to a pointer to the list elements.
 * \code
 MAPSList<MAPSString> li;
 // ... populate li ...
 // Displays li elements:
 MAPSString* x;
 MAPSForallPtr(x,li) {
     std::cout << (*x) << std::endl;
 }
 \endcode
 * \param x a \a T* object
 * \param L a MAPSList<T>
 * \hideinitializer
 * \ingroup bds
 */
#define MAPSForallPtr(x,L) MAPSListIterator __itq##x;for(__itq##x=(L).Iterator(),((__itq##x)?((x)=&(L)[__itq##x]):NULL);(!(L).Finished(__itq##x))&&((x)=&(L)[__itq##x]);(L).Next(__itq##x))

template<typename TKey, typename TContent, typename H> class MAPSHashTable;

//! The RTMaps double linked list template class
/*! MAPSLists have now a true iterator MAPSList::MAPSIterator. 
 * Future developments should always prefer this iterator to MAPSListIterator when possible.
 * Methods that use MAPSListItem objects should be avoided as direct access to the list item might be removed in the next release.
 * \ingroup bds
 */
template <typename T> 
    class MAPSList : public MAPSBasicList 
    {
#if (_MSC_VER <= 1300) // <= .NET 2002
    public:
#else
	template<typename TKey, typename TContent, typename H> friend class MAPSHashTable;
    protected :
#endif
	// Special functions, not for end users. Used by MAPSHashTable.
	// Special append function : appends an element previously allocated
	void
	appendX(MAPSListIterator itx);
	// Special remove function : removes an element without deallocating it
	void
	removeX(MAPSListIterator itx);

	void
	permutations_recur(MAPSList< MAPSList<T> >& m, 
			   MAPSList<T>& w);

    public:
	class MAPSIterator;
	friend class MAPSIterator;

	//! Iterator on MAPSList objects.
	/*! This is a more efficient alternative to MAPSListIterator. Its usage is close to its STL counterpart.
	 * Following is an example that displays the content of a list of \c int:
	 \code
	 MAPSList<int> li;
	 // ... populate li ...
	 // Get an iterator it on the list li
	 MAPSList<int>::MAPSIterator it(li);
	 // And walk through the list
	 for(;!it.End();++it) {
	    std::cout << (*it) << std::endl;
	 }
	 \endcode
	 * \ingroup bds
	 */
	class MAPSIterator {
	protected:
	    MAPSListItem<T>* itemPtr;
	public:
	    //! Default constructor
	    MAPSIterator() : itemPtr(NULL) {}
	    //! Copy constructor
	    MAPSIterator(const MAPSIterator& iter) { itemPtr=iter.itemPtr; }
	    explicit MAPSIterator(MAPSListItem<T>* item) : itemPtr(item) {}
	    //! Constructor associated to a MAPSList
	    MAPSIterator(const MAPSList<T>& li)
	    { itemPtr = reinterpret_cast<MAPSListItem<T>* >(li.first_item); }

	    MAPSIterator&
	    operator=(const MAPSIterator& iter) { itemPtr=iter.itemPtr; return (*this); }
	    operator MAPSListItem<T>*() { return (itemPtr); }
	    //! Direct access to the data of the list item
	    T& 
	    operator*() { return (**itemPtr); }
	    MAPSIterator& 
	    operator++() { itemPtr=(MAPSListItem<T>*)itemPtr->Next(); return (*this); }
	    MAPSIterator 
	    operator++(int) { MAPSIterator tmp=*this; ++*this; return (tmp); }
	    MAPSIterator& 
	    operator--() { itemPtr=(MAPSListItem<T>*)itemPtr->Prev(); return (*this); }
	    MAPSIterator 
	    operator--(int) { MAPSIterator tmp=*this; --*this; return (tmp); }
	    bool 
	    operator==(const MAPSIterator& right) const { return (itemPtr==right.itemPtr); }
	    bool 
	    operator!=(const MAPSIterator& right) const { return (!(*this==right)); }
	    //! Returns \c true if the iterator has a successor
	    bool 
	    HasNext() { return ((itemPtr!=NULL) && (itemPtr->Next()!=NULL)); }
	    //! Returns \c true if the iterator corresponds to the end of the list
	    bool
	    End() { return (itemPtr==NULL); }
	    //! Returns \c true if the iterator is associated to a real list item
	    bool
	    Initialized() { return itemPtr != NULL; }
	    //! Puts the iterator in an uninitialized state
	    void
	    Reset() { itemPtr = NULL; }
	};

	T&
	operator[](MAPSListItem<T>& it) const {return *it;}
	//! Returns the element corresponding to iterator \a it
	T&
	operator[](MAPSListIterator it) const
	{ return *(*((MAPSListItem<T>*)(MAPSBasicListItem*)it)); }
	T&
	operator[](MAPSIterator& it) const { return (*it); }
	//! Returns the <CODE>n</CODE>th element in the list (starting from 0)
	T&
	operator[](int n) const;

	//! Gets an iterator on the first element of the list
	MAPSIterator
	Iter() { return MAPSIterator(reinterpret_cast<MAPSListItem<T>* >(this->first_item)); }
	//! Gets an iterator on the first element of the list
	MAPSIterator
	IterFirst() { return MAPSIterator(reinterpret_cast<MAPSListItem<T>* >(this->first_item)); }
	//! Gets an iterator on the last element of the list
	MAPSIterator
	IterLast() { return MAPSIterator(reinterpret_cast<MAPSListItem<T>* >(this->last_item)); }
	/*! \name Adds an element to the list
	 * @{
	 */
	//! Inserts a new empty element in the front of the list (at position 0)
	/*! Prefer MAPSList#PushFront() when possible */
	MAPSListItem<T>&
	Insert()
	{ this->PushFront(); return *((MAPSListItem<T>*)first_item); }
	//! Inserts a new empty element in the front of the list (at position 0)
	void
	PushFront();
	//! Inserts element \a elt in the front of the list (at position 0)
	/*! Prefer MAPSList#PushFront(const T&) when possible */
	MAPSListIterator
	Insert(const T& elt)
	{ this->PushFront(elt); return MAPSListIterator(first_item); }
	//! Inserts element \a elt in the front of the list (at position 0)
	void
	PushFront(const T& elt);

	//! Appends a new empty element to the end of the list (the tail)
	/*! Prefer MAPSList#PushBack() when possible */
	MAPSListItem<T>&
	Append()
	{ this->PushBack(); return *((MAPSListItem<T>*)last_item); }
	//! Appends a new empty element to the end of the list (the tail)
	void
	PushBack();
	//! Appends element \a elt to the end of the list (the tail)
	/*! Prefer MAPSList#PushBack(const T&) when possible */
	MAPSListIterator
	Append(const T& elt)
	{ this->PushBack(elt); return MAPSListIterator(last_item); }
	//! Appends element \a elt to the end of the list (the tail)
	void
	PushBack(const T& elt);
	//! Appends element \a elt to the end of the list (the tail)
	MAPSList<T>&
	operator<<(const T& elt)
	{ this->PushBack(elt); return (*this); }
	/*@}*/

	/*! \name Element insertion
	 * @{
	 */
	//! Inserts element \a elt just after the element corresponding to the iterator \a itx
	/*! Prefer MAPSList#InsertAfter(MAPSIterator&, const T&) when possible */
	MAPSListIterator
	InsertAfter(const MAPSListIterator itx,
		    const T& elt)
	{ MAPSListItem<T>& it=Item(itx); return MAPSListIterator(this->InsertAfter(it,elt)); }
	//! Inserts element \a elt just after the element corresponding to the iterator \a itx
	/*! Prefer MAPSList#InsertAfter(MAPSIterator&, const T&) when possible */
	MAPSListItem<T>*
	InsertAfter(MAPSListItem<T>& it,
		    const T& elt);
	//! Inserts element \a elt just after the element corresponding to the iterator \a itx
	void
	InsertAfter(MAPSIterator& it, const T& elt);
	//! Inserts a new empty element just after the element corresponding to the iterator \a itx
	/*! Prefer MAPSList#InsertAfter(MAPSIterator&) when possible */
	MAPSListItem<T>&
	InsertAfter(const MAPSListIterator itx);
	//! Inserts a new empty element just after the element corresponding to the iterator \a itx
	T&
	InsertAfter(MAPSIterator& it);
	//! Inserts element \a elt just before the element corresponding to the iterator \a itx
	/*! Prefer MAPSList#InsertBefore(MAPSIterator&, const T&) when possible */
	MAPSListIterator
	InsertBefore(const MAPSListIterator itx,
		     const T& elt)
	{ MAPSListItem<T>& it=Item(itx); return MAPSListIterator(this->InsertBefore(it,elt)); }
	//! Inserts element \a elt just before the list item \a it
	/*! Prefer MAPSList#InsertBefore(MAPSIterator&, const T&) when possible */
	MAPSListItem<T>*
	InsertBefore(MAPSListItem<T>& it,
		     const T& elt);
	//! Inserts element \a elt just before the element corresponding to the iterator \a itx
	void
	InsertBefore(MAPSIterator& it,
		     const T& elt);
	//! Inserts a new empty element just before the element corresponding to the iterator \a itx
	/*! Prefer MAPSList#InsertBefore(MAPSIterator&) when possible */
	MAPSListItem<T>&
	InsertBefore(MAPSListIterator itx);
	//! Inserts a new empty element just before the element corresponding to the iterator \a itx
	T&
	InsertBefore(MAPSIterator& it);
	/*@}*/

	//! Destroys the content of the list
	void
	Clear();
    	
	//! Access to the MAPSListItem from an iterator. Try to avoid this method.
	MAPSListItem<T>&
	Item(MAPSListIterator it)
	{ return *((MAPSListItem<T>*)(MAPSBasicListItem*)it); }
	//! Access to the MAPSListItem from a MAPSBasicListItem*. Try to avoid this method.
	MAPSListItem<T>&
	Item(MAPSBasicListItem* it)
	{ return *((MAPSListItem<T>*)it); }

	/*! \name Deletion
	 * @{
	 */
	//! Deletes the item corresponding to iterator \a itx
	MAPSList<T>&
	operator-=(MAPSListIterator itx);
	//! Deletes all the items in list \a L
	MAPSList<T>&
	operator-=(MAPSList<MAPSListIterator>& L)
	{MAPSListIterator it; MAPSForall(it,L) (*this)-=it; return (*this);}
	//! Deletes the item corresponding to iterator \a itx
	T
	Delete(MAPSListIterator itx);
	//! Deletes the item corresponding to iterator \a it
	void
	Delete(MAPSIterator& it);
	//! Deletes the item corresponding to iterator \a it
	T
	Remove(MAPSListIterator it)
	{ return Delete(it); }
	//! Deletes all the items in list \a L
	MAPSList<T>&
	Remove(MAPSList<MAPSListIterator>& L)
	{ return (*this)-=L; }
	//! Deletes all the items in list \a L
	MAPSList<T>&
	Delete(MAPSList<MAPSListIterator>& L)
	{ return (*this)-=L; }
	//! Removes and returns the last item in the list (Perl-like Pop)
	T
	Pop() {return Delete(Last());}
	//! Removes and returns the first element in the list (Perl-like Shift)
	T
	Shift() {return Delete(First());}
	/*@}*/

	/*! \name Order manipulation
	 * @{
	 */
	//! Moves an item to the end of the list
	MAPSList<T>&
	Move2End(MAPSListIterator itx);
	//! Moves an item to the end of the list
	void
	Move2End(MAPSIterator& it);
	//! Moves an item to the front of the list
	MAPSList<T>&
	Move2Front(MAPSListIterator itx);
	//! Moves an item to the front of the list
	void
	Move2Front(MAPSIterator& it);
	//! Swaps two items
	MAPSList<T>&
	Swap(MAPSListIterator itx1, 
	    MAPSListIterator itx2);
	//! Swaps two items
	void
	Swap(MAPSIterator& it1, 
	    MAPSIterator& it2);
	/*@}*/

	//! List duplication
	MAPSList<T>&
	operator=(const MAPSList<T>& L)
	{ this->Clear(); (*this) += L; return (*this); }
	//! List concatenation
	MAPSList<T>&
	operator+=(const MAPSList<T>& L) 
	{ for(MAPSIterator it(L);!it.End();++it) this->PushBack(*it); return (*this); }
    	
	//! Computes all the possible permutations in a list
	void
	Permutations(MAPSList< MAPSList<T> >& L)
	{ L.Clear(); permutations_recur(L,(*this)); }
	//! Computes all the couples of elements of the list
	void
	Couples(MAPSList< MAPSCouple<T> >& L);
	//! Bubblesorts the list
	void Bubblesort(int (*compare_function)(T *t1,T *t2));

	// Constructors and Destructor
	//! Default constructor
	MAPSList() {}
	//! Destructor
	virtual ~MAPSList() {Clear();}
	//! Copy constructor
	MAPSList(const MAPSList<T>& L)
	{ (*this) += L; }

#ifdef USE_COMPATIBILITY_V2
	/*! \name Deprecated members
	 * @{
	 */
	T& operator*() const {return (*this)[First()];}	//!< \deprecated Returns the first element in the list. Too confusing. Use <code>li[li.First()]</code> instead.
	T& operator~() const {return (*this)[Last()];}	//!< \deprecated Returns the last element in the list. Too confusing. Use <code>li[li.Last()]</code> instead.
	MAPSListItem<T>& append() { return Append();} //!< \deprecated Use MAPSList#Append()
	MAPSListItem<T>& append(const T& elt) {return Item(Append(elt));} //!< \deprecated Use MAPSList#Append(const T&). Use <code>myList.Item(myList.Append(newElt))</code> to get the old behavior.
	MAPSListItem<T>& insert() { return Insert();} //!< \deprecated Use MAPSList#Insert()
	MAPSListItem<T>& insert(const T& elt) {return Item(Insert(elt));} //!< \deprecated Use MAPSList#Insert(const T&) instead.
	MAPSListIterator Unshift(const T& elt) {return Insert(elt);} //!< \deprecated Use MAPSList#Insert(const T&) instead.
	MAPSListItem<T>& Unshift() {return Insert();} //!< \deprecated Use MAPSList#Insert() instead.
	MAPSListItem<T>& unshift() { return Insert();} //!< \deprecated Use MAPSList#Insert() instead.
	MAPSListItem<T>& unshift(const T& elt) {return Item(Insert(elt));} //!< \deprecated Use MAPSList#Insert(const T&) instead.
	MAPSListItem<T>& push(const T& elt) {return Push(elt);} //!< \deprecated Use MAPSList#PushBack(const T&) instead.
	MAPSList<T>& Push(const T& elt) {
	    MAPSListItem<T>* new_item=new MAPSListItem<T>(elt); new_item->successor=NULL;
	    if (l) { last_item->successor=new_item; new_item->predecessor=last_item; last_item=new_item; ++l;
	    } else { first_item=last_item=new_item; new_item->predecessor=NULL; l=1; }
	    return (*this); }//!< \deprecated Use MAPSList#PushBack(const T&) instead.
	MAPSListItem<T>& insert_after(MAPSListIterator itx) {return InsertAfter(itx);} //!< \deprecated Use MAPSList#InsertAfter(const MAPSListIterator)
	MAPSListItem<T>& insert_after(MAPSListIterator itx,T& elt) {return Item(InsertAfter(itx,elt));} //!< \deprecated Use MAPSList#InsertAfter(const MAPSListIterator,const T&)
	MAPSListItem<T>& insert_before(MAPSListIterator itx) {return InsertAfter(itx);} //!< \deprecated Use MAPSList#InsertBefore(const MAPSListIterator)
	MAPSListItem<T>& insert_before(MAPSListIterator itx,T& elt) {return Item(InsertAfter(itx,elt));} //!< \deprecated Use MAPSList#InsertBefore(const MAPSListIterator,const T&)
	void clear() {Clear();} //!< \deprecated Use MAPSList#Clear()
	MAPSListItem<T>& item(MAPSListIterator it) {return *((MAPSListItem<T>*)(MAPSBasicListItem*)it);} //!< \deprecated Use MAPSList#Item(MAPSListIterator)
	T pop() {return Pop();}//!< \deprecated Use MAPSList#Pop()
	//  M4 protection
	T shift() {return Shift();}//!< \deprecated Use MAPSList#Shift()
	//  M4 protection
	T remove(MAPSListIterator it) {return Delete(it);}//!< \deprecated Use MAPSList::Delete()
	MAPSList<T>& remove(MAPSList<MAPSListIterator>& L) {return (*this)-=L;} //!< \deprecated Removed
	T del_item(MAPSListIterator itx) {return Delete(itx);} //!< \deprecated Use MAPSList#Delete(MAPSListIterator)
	T remove_item(MAPSListIterator itx) {return Delete(itx);} //!< \deprecated Use MAPSList#Delete(MAPSListIterator)
	T delete_item(MAPSListIterator itx) {return Delete(itx);} //!< \deprecated Use MAPSList#Delete(MAPSListIterator)
	MAPSList<T>& remove_items(MAPSList<MAPSListIterator>& L) {return (*this)-=L;} //!< \deprecated Removed
	MAPSList<T>& del_items(MAPSList<MAPSListIterator>& L) {return (*this)-=L;} //!< \deprecated Removed
	MAPSList<T>& delete_items(MAPSList<MAPSListIterator>& L) {return (*this)-=L;} //!< \deprecated Removed
	MAPSList<T>& move2end(MAPSListIterator itx) {return Move2End(itx);} //!< \deprecated Use MAPSList#Move2End(MAPSListIterator)
	MAPSList<T>& move2front(MAPSListIterator itx) {return Move2Front(itx);} //!< \deprecated Use MAPSList#Move2Front(MAPSListIterator)
	MAPSList<T>& swap(MAPSListIterator itx1, MAPSListIterator itx2) {return Swap(itx1,itx2);} //!< \deprecated Use MAPSList#Swap(MAPSListIterator,MAPSListIterator)
	void permutations(MAPSList<MAPSList<T> >& L) {Permutations(L);} //!< \deprecated Use MAPSList#Permutations(MAPSList<MAPSList<T> >&)
	void couples(MAPSList< MAPSCouple<T> >& L) {Couples(L);} //!< \deprecated Use MAPSList#Couples(MAPSList< MAPSCouple<T> >&)
	void bubblesort(int (*compare_function)(T* t1,T* t2)) {Bubblesort(compare_function);} //!< \deprecated Use MAPSList#Bubblesort()
	/*@}*/
#endif
    };

// MAPSList inline methods implementations
template<typename T> 
    inline void 
    MAPSList<T>::permutations_recur(MAPSList< MAPSList<T> >& m, MAPSList<T>& w)
    {
	MAPSList<T> x;
	m.Clear();
	if (w.Length()==1) {
	    x.PushBack(w[w.First()]);
	    m.PushBack(x);
	} else {
	    MAPSIterator it2;
	    typename MAPSList< MAPSList<T> >::MAPSIterator it_tmp;
	    MAPSList< MAPSList<T> > l_tmp;
	    for(MAPSIterator it(w);!it.End();++it) {
		x.Clear();
		for(it2=MAPSIterator(w);!it2.End();++it2)
		    if (it2!=it)
			x.PushBack(*it2);
		permutations_recur(l_tmp,x);
		for(it_tmp=MAPSList< MAPSList<T> >::MAPSIterator(l_tmp);!it_tmp.End();++it_tmp)
		    (*it_tmp).PushFront(*it);
		m+=l_tmp;
	    }
	}
    }
template<typename T>
    inline T&
    MAPSList<T>::operator[](int n) const 
    {
	MAPSIterator iter(*this);
	for(int i=0;i<n;++i,++iter) {
#ifdef BDS_RUNTIME_CHECK
	    if (iter.End()) {
		throw "Index out of bounds";
	    }
#endif
	}
	return *iter;
    }

template<typename T>
    inline void
    MAPSList<T>::PushFront(const T& elt)
    {
	MAPSListItem<T>* new_item=new MAPSListItem<T>(elt);
	new_item->predecessor=NULL;
	if (l) {
	    first_item->predecessor=new_item; new_item->successor=first_item; first_item=new_item; ++l;
	} else {
	    first_item=last_item=new_item; new_item->successor=NULL; l=1;
	}
    }
template<typename T>
    inline void
    MAPSList<T>::PushFront()
    {
	MAPSListItem<T>* new_item=new MAPSListItem<T>;
	new_item->predecessor=NULL;
	if (l) {
	    first_item->predecessor=new_item; new_item->successor=first_item; first_item=new_item; ++l;
	} else {
	    first_item=last_item=new_item; new_item->successor=NULL; l=1;
	}
    }
template<typename T>
    inline void
    MAPSList<T>::PushBack(const T& elt)
    {
	MAPSListItem<T>* new_item=new MAPSListItem<T>(elt);
	new_item->successor=NULL;
	if (l) {
	    last_item->successor=new_item; new_item->predecessor=last_item; last_item=new_item; ++l;
	} else {
	    first_item=last_item=new_item; new_item->predecessor=NULL; l=1;
	}
    }
template<typename T>
    inline void
    MAPSList<T>::PushBack()
    {
	MAPSListItem<T>* new_item=new MAPSListItem<T>;
	new_item->successor=NULL;
	if (l) {
	    last_item->successor=new_item; new_item->predecessor=last_item; last_item=new_item; ++l;
	} else {
	    first_item=last_item=new_item; new_item->predecessor=NULL; l=1;
	}
    }
template<typename T>
    inline MAPSListItem<T>*
    MAPSList<T>::InsertAfter(MAPSListItem<T>& it, const T& elt)
    {
	MAPSListItem<T>* new_item=new MAPSListItem<T>(elt);
	new_item->successor=it.successor; new_item->predecessor=&it;
	if (it.successor) it.successor->predecessor=new_item;
	else last_item=new_item;
	it.successor=new_item; ++l;
	return new_item;
    }
template<typename T>
    inline void
    MAPSList<T>::InsertAfter(MAPSIterator& it, const T& elt)
    {
	MAPSListItem<T>* new_item=new MAPSListItem<T>(elt);
	new_item->successor=((MAPSListItem<T>*)it)->successor; new_item->predecessor=(MAPSListItem<T>*)it;
	if (((MAPSListItem<T>*)it)->successor) ((MAPSListItem<T>*)it)->successor->predecessor=new_item;
	else last_item=new_item;
	((MAPSListItem<T>*)it)->successor=new_item; ++l;
    }
template<typename T>
    inline MAPSListItem<T>&
    MAPSList<T>::InsertAfter(const MAPSListIterator itx)
    {
	MAPSListItem<T>& it=Item(itx); 
	MAPSListItem<T>* new_item=new MAPSListItem<T>;
	new_item->successor=it.successor; new_item->predecessor=&it;
	if (it.successor) it.successor->predecessor=new_item;
	else last_item=new_item;
	it.successor=new_item; ++l;
	return *new_item;
    }
template<typename T>
    inline T&
    MAPSList<T>::InsertAfter(MAPSIterator& it)
    {
	MAPSListItem<T>* new_item=new MAPSListItem<T>;
	new_item->successor=((MAPSListItem<T>*)it)->successor; new_item->predecessor=(MAPSListItem<T>*)it;
	if (((MAPSListItem<T>*)it)->successor) ((MAPSListItem<T>*)it)->successor->predecessor=new_item;
	else last_item=new_item;
	((MAPSListItem<T>*)it)->successor=new_item; ++l;
	return **new_item;
    }
template<typename T>
    inline MAPSListItem<T>*
    MAPSList<T>::InsertBefore(MAPSListItem<T>& it, const T& elt) 
    {
	MAPSListItem<T>* new_item=new MAPSListItem<T>(elt);
	new_item->successor=&it; new_item->predecessor=it.predecessor;
	if (it.predecessor) it.predecessor->successor=new_item;
	else first_item=new_item;
	it.predecessor=new_item; ++l;
	return new_item;
    }
template<typename T>
    inline void
    MAPSList<T>::InsertBefore(MAPSIterator& it, const T& elt) 
    {
	MAPSListItem<T>* new_item=new MAPSListItem<T>(elt);
	new_item->successor=(MAPSListItem<T>*)it; new_item->predecessor=((MAPSListItem<T>*)it)->predecessor;
	if (((MAPSListItem<T>*)it)->predecessor) ((MAPSListItem<T>*)it)->predecessor->successor=new_item;
	else first_item=new_item;
	((MAPSListItem<T>*)it)->predecessor=new_item; ++l;
    }	
template<typename T>
    inline MAPSListItem<T>&
    MAPSList<T>::InsertBefore(MAPSListIterator itx) 
    {
	MAPSListItem<T>& it=Item(itx); 
	MAPSListItem<T>* new_item=new MAPSListItem<T>;
	new_item->successor=&it; new_item->predecessor=it.predecessor;
	if (it.predecessor) it.predecessor->successor=new_item;
	else first_item=new_item;
	it.predecessor=new_item; ++l;
	return *new_item;
    }
template<typename T>
    inline T&
    MAPSList<T>::InsertBefore(MAPSIterator& it) 
    {
	MAPSListItem<T>* new_item=new MAPSListItem<T>;
	new_item->successor=(MAPSListItem<T>*)it; new_item->predecessor=((MAPSListItem<T>*)it)->predecessor;
	if (((MAPSListItem<T>*)it)->predecessor) ((MAPSListItem<T>*)it)->predecessor->successor=new_item;
	else first_item=new_item;
	((MAPSListItem<T>*)it)->predecessor=new_item; ++l;
	return **new_item;
    }
template<typename T>
    inline void
    MAPSList<T>::appendX(MAPSListIterator itx) 
    {
	MAPSListItem<T>& it=Item(itx);
	MAPSListItem<T>* new_item=&it;
	new_item->successor=NULL;
	if (l) {
	    last_item->successor=new_item; new_item->predecessor=last_item; last_item=new_item; ++l;
	} else {
	    first_item=last_item=new_item; new_item->predecessor=NULL; l=1;
	}
    }
template<typename T>
    inline void
    MAPSList<T>::removeX(MAPSListIterator itx) 
    {
	MAPSListItem<T>& it=Item(itx);
	if (it.predecessor==NULL) first_item=it.successor;
	else it.predecessor->successor=it.successor;
	if (it.successor==NULL) last_item=it.predecessor;
	else it.successor->predecessor=it.predecessor;
	--l;
    }
template<typename T>
    inline void
    MAPSList<T>::Clear() 
    {
	MAPSBasicListItem* next_item = first_item;
	MAPSBasicListItem* del_item;
	while(next_item != NULL) {
	    del_item=next_item;
	    next_item=next_item->successor;
	    delete ((MAPSListItem<T>*)del_item);
	}
	l=0;
	first_item=NULL;
	last_item=NULL;
    }
template<typename T>
    inline MAPSList<T>& 
    MAPSList<T>::operator-=(MAPSListIterator itx) 
    {
	MAPSListItem<T>& it=Item(itx);
	if (it.predecessor==NULL) first_item=it.successor;
	else it.predecessor->successor=it.successor;
	if (it.successor==NULL) last_item=it.predecessor;
	else it.successor->predecessor=it.predecessor;
	--l;delete &it;return (*this);
    }
template<typename T>
    inline T
    MAPSList<T>::Delete(MAPSListIterator itx) 
    {
	MAPSListItem<T>& it=Item(itx); T t;
	if (it.predecessor==NULL) first_item=it.successor;
	else it.predecessor->successor=it.successor;
	if (it.successor==NULL) last_item=it.predecessor;
	else it.successor->predecessor=it.predecessor;
	--l;t=*it;delete &it;return t;
    }
template<typename T>
    inline void 
    MAPSList<T>::Delete(MAPSIterator& it)
    {
	if (((MAPSListItem<T>*)it)->predecessor==NULL) first_item=((MAPSListItem<T>*)it)->successor;
	else ((MAPSListItem<T>*)it)->predecessor->successor=((MAPSListItem<T>*)it)->successor;
	if (((MAPSListItem<T>*)it)->successor==NULL) last_item=((MAPSListItem<T>*)it)->predecessor;
	else ((MAPSListItem<T>*)it)->successor->predecessor=((MAPSListItem<T>*)it)->predecessor;
	--l;
	delete ((MAPSListItem<T>*)it);
    }
template<typename T>
    inline MAPSList<T>&
    MAPSList<T>::Move2End(MAPSListIterator itx)
    {
	MAPSListItem<T>& it=Item(itx); 
	if (l>1) {
	    if (it.predecessor==NULL) first_item=it.successor;
	    else it.predecessor->successor=it.successor;
	    if (it.successor==NULL) last_item=it.predecessor;
	    else it.successor->predecessor=it.predecessor;
	    it.successor=NULL;
	    last_item->successor=&it; it.predecessor=last_item; last_item=&it;
	}
	return (*this);
    }
template<typename T>
    inline void
    MAPSList<T>::Move2End(MAPSIterator& it)
    {
	if (l>1) {
	    if (((MAPSListItem<T>*)it)->predecessor==NULL) first_item=((MAPSListItem<T>*)it)->successor;
	    else ((MAPSListItem<T>*)it)->predecessor->successor=((MAPSListItem<T>*)it)->successor;
	    if (((MAPSListItem<T>*)it)->successor==NULL) last_item=((MAPSListItem<T>*)it)->predecessor;
	    else ((MAPSListItem<T>*)it)->successor->predecessor=((MAPSListItem<T>*)it)->predecessor;
	    ((MAPSListItem<T>*)it)->successor=NULL;
	    last_item->successor=(MAPSListItem<T>*)it; ((MAPSListItem<T>*)it)->predecessor=last_item; last_item=(MAPSListItem<T>*)it;
	}
    }
template<typename T>
    inline MAPSList<T>&
    MAPSList<T>::Move2Front(MAPSListIterator itx)
    {
	MAPSListItem<T>& it=Item(itx);
	if (l>1) {
	    if (it.predecessor==NULL) first_item=it.successor;
	    else it.predecessor->successor=it.successor;
	    if (it.successor==NULL) last_item=it.predecessor;
	    else it.successor->predecessor=it.predecessor;
	    it.predecessor=NULL;
	    first_item->predecessor=&it; it.successor=first_item; first_item=&it;
	}
	return (*this);
    }
template<typename T>
    inline void
    MAPSList<T>::Move2Front(MAPSIterator& it)
    {
	if (l>1) {
	    if (((MAPSListItem<T>*)it)->predecessor==NULL) first_item=((MAPSListItem<T>*)it)->successor;
	    else ((MAPSListItem<T>*)it)->predecessor->successor=((MAPSListItem<T>*)it)->successor;
	    if (((MAPSListItem<T>*)it)->successor==NULL) last_item=((MAPSListItem<T>*)it)->predecessor;
	    else ((MAPSListItem<T>*)it)->successor->predecessor=((MAPSListItem<T>*)it)->predecessor;
	    ((MAPSListItem<T>*)it)->predecessor=NULL;
	    first_item->predecessor=(MAPSListItem<T>*)it; it.successor=first_item; first_item=(MAPSListItem<T>*)it;
	}
    }
template<typename T>
    inline MAPSList<T>&
    MAPSList<T>::Swap(MAPSListIterator itx1, 
		      MAPSListIterator itx2)
    {
	MAPSBasicListItem *temp1, *temp2, it1=&(MAPSBasicListItem*)itx1, it2=&(MAPSBasicListItem*)itx2;
	temp1=it1.predecessor;
	temp2=it1.successor;
	it1.predecessor=it2.predecessor;
	it1.successor=it2.successor;
	it2.predecessor=temp1;
	it2.successor=temp2;
	return (*this);
    }
template<typename T>
    inline void
    MAPSList<T>::Swap(MAPSIterator& it1, 
		      MAPSIterator& it2) 
    {
	MAPSBasicListItem *temp1, *temp2;
	temp1=((MAPSListItem<T>*)it1)->predecessor;
	temp2=((MAPSListItem<T>*)it1)->successor;
	((MAPSListItem<T>*)it1)->predecessor=((MAPSListItem<T>*)it2)->predecessor;
	((MAPSListItem<T>*)it1)->successor=((MAPSListItem<T>*)it2)->successor;
	((MAPSListItem<T>*)it2)->predecessor=temp1;
	((MAPSListItem<T>*)it2)->successor=temp2;
    }
template<typename T>
    inline void
    MAPSList<T>::Couples(MAPSList< MAPSCouple<T> >& L)
    {
    	L.Clear();
	MAPSIterator it1(*this);
	MAPSIterator it2;
	for(MAPSIterator it1(*this);!it1.End();++it1) {
	    it2=it1;
	    ++it2;
	    while(!it2.End()) {
		L.PushBack(MAPSCouple<T>(*it1,*it2));
		++it2;
	    }
	}
    }

template<typename T>
    inline void
    MAPSList<T>::Bubblesort(int (*compare_function)(T* t1,T* t2)) 
    {
	T buf;
	int i,j;
	for (i=0;i<l-1;i++) {
	    MAPSIterator it(*this);
	    MAPSIterator itNext(*this);
	    ++itNext;
	    for (j=0;j<l-i-1;j++) {
		if (compare_function(&(*it),&(*itNext))>0) {
		    buf = *it;
		    *it = *itNext;
		    *itNext = buf;
		}
		++it;
		++itNext;
	    }
	}
    }

class MAPSRegExp;

// Strings
template<typename T> class MAPSArray;

//! The RTMaps flexible and fast string template class
/*! This template class is a very efficient and exhaustive string template class.
 *	In this template class, the parameter \a BUFFER_SIZE gives the preallocated size in
 *	bytes for the string. The memory is preallocated in the stack, which avoids
 *	any slow call to malloc (and any memory fragmentation due to heap usage).
 *	Note that if the string actually contains more characters than the \a BUFFER_SIZE
 *	template parameter, the buffer is automatically moved to the heap via malloc.
 *
 *	A common value for \a BUFFER_SIZE is 40. The usual MAPSString class is defined
 *  as MAPSStackedString<40>.
 *
 *  Among other features, the MAPSStackedString class provides :
 *		- string formatting features, based on the Microsoft familiar formatting features found 
 *	in Visual Basic or Excel.
 *		- operators and functions for basic string manipulation (concatenation, extraction, find, substitution)
 *		- tokenization functions (strtok like)
 *
 * \sa MAPSString MAPSStreamedString 
 * \ingroup bds
 */
template<int BUFFER_SIZE> 
    class MAPSStackedString 
    {
    int l;		// Length of the string
    int res;	// Number of bytes reserved in heap (via new operator)
    int tpos;	// Current token position
    char *str;	// Pointer to the string itself
    char buf[BUFFER_SIZE]; // Initial buffer stored in stack
#ifdef UNICODE
	MAPSUInt16* unicode_str;
	MAPSUInt16 unicode_buff[BUFFER_SIZE];
#endif
    // Allocates \a lx bytes, initializes with \a s (of size \a lx, not 0 terminated)
    // If \a s is not \c NULL, then the size of the string is \a lx, otherwise it's set to 0.
    void 
    init(int lx, const char* s);
#ifdef UNICODE
	void
    destroy() { /*Memory free*/ if (str!=buf) delete[] str; if (unicode_str != unicode_buff) delete [] unicode_str;}
#else
    void
    destroy() { /*Memory free*/ if (str!=buf) delete[] str; }
#endif //UNICODE
    void
    concat(int lx, const char* s);
    // Useful little function for formatting characters
    static char
    tfx(char c, char transform) 
    { switch (transform) {
	case '<': /*Forces lowercase*/ if ((c>='A')&&(c<='Z')) c+='a'-'A'; return c;
	case '>': /*Forces uppercase*/ if ((c>='a')&&(c<='z')) c-='a'-'A'; return c;
      } return c; }
    static char
    digit(int n, int radix=10)
    { int m=n%radix; if (m<10) { return '0'+m; } else { return 'a'+m-10; } }
    MAPSStackedString&
    formatX(const char* format_string, 
	    int integer, 
	    double decimal);
    void
    InitNextChar(bool left2right=true)
    { if (left2right) tpos=0; else tpos=l; }
    char
    NextChar(bool left2right=true,
	     bool interpret_special=false,
	     bool* backslashed=NULL);
    int
    CharPos(bool left2right=true)
    { if (left2right) return tpos-1; else if (str[tpos]=='\\') return tpos+1; return tpos; }	

public:
    //! Returns the length of the string.
    int
    Length() const {return l;}
    //! Returns the length of the string.
    int
    Len() const {return l;}

    // Operators
    //! Gets/Sets the character at position \a pos
    char& 
    operator[](int pos) const {
#ifdef BDS_RUNTIME_CHECK
	if ((pos<0)||(pos>l)) { throw "Index out of bounds"; }
#endif
	return str[pos]; }
    //! Automatic cast to <CODE>const char*</CODE>
    operator const char*() const {return str;}
#ifdef UNICODE
#ifdef _WIN32_WCE
	//! Automatic cast to <CODE>LPCWSTR</CODE>
	operator const wchar_t*() {mbstowcs((wchar_t*)unicode_str,str,l+1); return (wchar_t*)unicode_str;}
#endif //_WIN32_WCE
#endif //UNICODE
    /*! \name Strings comparison functions
     * @{
     */
    //! String comparaison, equivalent to \c strcmp, i.e. case-sensitive.
    bool
    operator==(const MAPSStackedString& s) const
    { if (s.Length()!=l) return false; for (int i=0;i<l;++i) if (str[i]!=s[i]) return false; return true; }
    bool
    operator==(MAPSStackedString& s) const
	{ return operator==((const MAPSStackedString&)s); }
    //! String comparaison, equivalent to \c strcmp, i.e. case-sensitive.
    bool
    operator!=(const MAPSStackedString& s) const
    { return !(operator==(s)); }
    //! String comparaison, equivalent to \c strcmp, i.e. case-sensitive.
    bool
    operator==(const char* s) const
    { if(s==NULL) return l==0; int i; for (i=0;i<l;++i) {if (str[i]!=s[i]) return false;} if (s[i]!=0) return false; return true; }
    //! String comparaison, equivalent to \c strcmp, i.e. case-sensitive.
    bool
    operator!=(const char* s) const
    { return !(operator==(s)); }
    //! Simple string order, case sensitive.
    bool
    operator<(const MAPSStackedString& s) const
    { int x=(s.Length()<l)?s.Length():l; for(int i=0;i<x;++i) { if(str[i]<s[i]) return true;} return (l<s.Length()); }
    //! Simple string order, case sensitive.
    bool
    operator>(const MAPSStackedString& s) const
    { int x=(s.Length()<l)?s.Length():l;for(int i=0;i<x;++i) { if(str[i]>s[i]) return true;} return (l>s.Length()); }
    //! String comparison, equivalent to \c strcmp, i.e. case-sensitive
    bool
    Equals(const MAPSStackedString& s) const 
    { return (*this)==s; }
    //! String comparaison, equivalent to \c stricmp, i.e. lowercase comparison of strings.
    bool
    Iequals(const MAPSStackedString& s) const
    { if (s.Length()!=l) return false; for (int i=0;i<l;++i) { char s1,s2; s1=str[i]; s2=s[i]; if ((s1>='a')&&(s1<='z')) s1-='a'-'A'; if ((s2>='a')&&(s2<='z')) s2-='a'-'A'; if (s1!=s2) return false;} return true;}
    /*@}*/

    //! String copy operator. Returns a reference to the current string (<code>*this</code>).
    MAPSStackedString&
    operator=(const MAPSStackedString& s)
    { this->destroy();this->init(s.Length(),s);tpos=s.tpos;return(*this);}
    //! String copy operator. Returns a reference to the current string (<code>*this</code>).
    MAPSStackedString&
    operator=(const char *s)
    { this->destroy();if (s) this->init((int)MAPS::Strlen(s),s); else this->init(0,NULL); return(*this);}
#ifdef UNICODE
#ifdef _WIN32_WCE
	MAPSStackedString&
	operator=(const wchar_t* s)
	{ this->destroy(); if (s) {BOOL defaultCharUsed; int reqLen = WideCharToMultiByte(0,0,s,-1,NULL,0,NULL,&defaultCharUsed); char* temp = new char[reqLen]; WideCharToMultiByte(0,0,s,-1,temp,reqLen,NULL,&defaultCharUsed); this->init((int)MAPS::Strlen(temp),temp); delete [] temp;} else {this->init(0,NULL);} return (*this);}
#endif //_WIN32_WCE
#endif //UNICODE
    //! Match against a regular expression
    MAPSRegExp&
    operator=(MAPSRegExp& regexp);

    /*! \name Strings concatenation functions
     * @{
     */
    //! String concatenation operator. Returns a reference to the current string (<code>*this</code>).
    MAPSStackedString&
    operator+=(const char *s)
    { if(s) this->concat((int)MAPS::Strlen(s),s);return(*this); }
    //! String concatenation operator. Returns a reference to the current string (<code>*this</code>).
    MAPSStackedString&
    operator+=(unsigned char *s)
    { if(s) this->concat((int)MAPS::Strlen((const char*)s),(const char*)s);return(*this); }
    //! String concatenation operator. Returns a reference to the current string (<code>*this</code>).
    MAPSStackedString&
    operator+=(const MAPSStackedString& s)
    { this->concat(s.Length(),s);return(*this); }
    //! Concatenates character \a c to the string. Returns a reference to the current string (<code>*this</code>).
    MAPSStackedString&
    operator+=(const char c)
    { this->concat(1,&c);return(*this); }
    /*@}*/

    // Useful functions
    //! Returns a pointer to the beginning of the string.
    const char*
    Beginning() const {return str;}
    //! Returns the position corresponding to a pointer to a character of the string
    int
    Pos(const char* ptr) const
    { return ptr-str; }

    /*! \name Substring extraction functions
     * All of these functions return a new string, or an array of strings.
     * @{
     */
    //! Extracts a part of the string.
    /*! Returns all the characters between positions \a start and \a stop (included).
     */
    MAPSStackedString 
    operator()(int start, 
	       int stop) const 
    { if (start>l-1) start=l; if (stop>l-1) stop=l-1; MAPSStackedString temp(stop-start+1,true); for (int i=start,j=0;i<=stop;++i,++j) {temp[j]=str[i];} return temp; }
    //! Returns the part of the string up to the first occurence of character \a s
    /*! Note : When \a from_beginning is \c false, i.e. when we are seeking the first occurence of character \a s in the string 
     *  beginning from the end of the string, if there is no such character, then the returned string is empty.
     *	On the other hand, when \a from_beginning is \c true, i.e. when we are seeking the first occurence of character \a s in the
     *  string beginning from the first character of the string, if there is no such character, then the whole string is returned.
     */
    MAPSStackedString
    UpTo(char s, bool from_beginning=true) const
    { int i; if (from_beginning) { for (i=0;i<l;++i) if (str[i]==s) break; } else { for (i=l-1;i>=0;i--) if (str[i]==s) break; } if (i<0) i=0; MAPSStackedString temp(i,true); for (int j=0;j<i;j++) temp[j]=str[j]; return temp; }
    //! Returns the part of the string up to the first occurence of character \a s
    /*! \copydoc UpTo(char,bool) const
     */
    MAPSStackedString
    Up2(char s, 
	bool from_beginning=true) const
    { return UpTo(s,from_beginning); }
    //! Returns the characters starting from the beginning of the string to <i>m</i>th character, counting from the beginning or the end of the string.
    MAPSStackedString
    UpTo(int m,
	 bool from_beginning=true) const
    { int n; if (from_beginning) n=m; else {n=Length()-m; if (n<0) n=0;} MAPSStackedString temp(n,true); for (int i=0;i<n;++i) temp[i]=str[i]; return temp;} 
    //! Returns the characters starting from the beginning of the string to <i>m</i>th character, counting from the beginning or the end of the string.
    MAPSStackedString
    Up2(int m,
	bool from_beginning=true) const
    { int n; if (from_beginning) n=m; else {n=Length()-m; if (n<0) n=0;} MAPSStackedString temp(n,true); for (int i=0;i<n;++i) temp[i]=str[i]; return temp;} 
    //! Returns the characters starting from the <i>m</i>th character, counting from the beginning or the end of the string, to the last character.
    MAPSStackedString
    Tail(int m,
	 bool from_beginning=true) const
    { int n; if (from_beginning) n=m; else {n=Length()-m; if (n<0) n=0;} MAPSStackedString temp(l-n,true); for (int i=n,j=0;i<Length();++i,++j) temp[j]=str[i]; return temp;}
    //! Returns the characters found after the first occurence of character \a s, starting from the beginning or the end of the string.
    MAPSStackedString
    Tail(char s,
	 bool from_beginning=true) const
    { int i; if (from_beginning) { for (i=0;i<l;++i) if (str[i]==s) break; } else { for (i=l-1;i>=0;i--) if (str[i]==s) break; } MAPSStackedString temp(Length()-i-1,true); for (int j=i+1,k=0;j<Length();++j,++k) temp[k]=str[j]; return temp; }
    //! Extracts a part of the string.
    /*! Returns all the characters between positions \a start and \a stop (included).
     */
    MAPSStackedString
    Part(int start,
	 int stop) const
    { return (*this)(start,stop); }
    //! Extracts the left part of the string
    /*! Similar to Visual Basic \c Left$ function. 
     * \param length Indicates how many characters to return. If 0, a zero-length string ("") is returned. 
     *         If greater than or equal to the number of characters in the string, the entire string is returned.
     */
    MAPSStackedString
    Left(int length) const
    { int stop=(length>l)?l:length; MAPSStackedString temp(stop,true); for (int i=0;i<stop;++i) temp[i]=str[i]; return temp; } 
    //! Extracts the right part of the string
    /*! Similar to Visual Basic \c Right$ function.
     * \param length Indicates how many characters to return. If 0, a zero-length string ("") is returned. 
     *         If greater than or equal to the number of characters in the string, the entire string is returned.
     */
    MAPSStackedString
    Right(int length) const 
    { int start=(length>l)?0:l-length; MAPSStackedString temp(l-start,true); for (int i=start,j=0;i<l;++i,++j) temp[j]=str[i]; return temp; }
    //! Extracts a part of the string
    /*! Similar to Visual Basic \c Mid$ function.
     * \param start Character position in the string at which the part to be taken begins. If \a start is greater than 
     *         the number of characters in the string, Mid returns a zero-length string ("").
     *	\param length (optional) Number of characters to return. If omitted or if there are fewer than \a length characters in the 
     *         text (including the character at start), all characters from the start position to the end of the string are returned.
     */
    MAPSStackedString
    Mid(int start,
	int length=-1) const
    { if (start>l-1) start=l; int stop=((length<0)||(start+length>l))?l-1:start+length-1;MAPSStackedString temp(stop-start+1,true);for (int i=start,j=0;i<=stop;++i,++j) temp[j]=str[i];return temp; }
    //! Splits the string into several strings according to the specified separation character.
    MAPSArray<MAPSStackedString>
    Split(const char c) const;
    /*@}*/

    /*! \name String tokenization functions
     * @{
     */
    //! Resets the token search. Returns a reference to the current string (<code>*this</code>).
    MAPSStackedString&
    TokenReset() {tpos=0; return *this;}
    //! Returns the next token.
    /*! \param delim The set of characters in \a delim specifies possible delimiters of the token to be found in the string on the current call
     *  \param s Holds the substring
     *	\return Returns \c false when no more tokens are found.
     */
    bool
    Token(const char* delim,
	  MAPSStackedString& s);
    //! Equivalent to standard C library \c strtok.
    const char*
    Strtok(const char* delim);
    /*@}*/

    /*! \name Find functions
     * @{
     */
    //! Finds character \a c in the string, starting at position \a start.
    /*! Returns \c true if found, and updates the \a pos variable.
     */
    bool
    Find(char c,
	 int& pos,
	 int start=0) const
    { for (int j=start;j<l;++j) if (str[j]==c) {pos=j; return true;} return false; }
    //! Finds one of the characters in \a s in the string, starting at position \a start.
    /*! Returns \c true if found, and updates the \a pos variable.
     * \warning This function does not look for the whole string \a s. If this is what you want,
     *          have a look at FindStr functions.
     */
    bool
    Find(const char *s,
	 int& pos,
	 int start=0) const
    { int n=(int)MAPS::Strlen(s); for (int j=start;j<Length();++j) for (int i=0;i<n;++i) if (str[j]==s[i]) {pos=j; return true;} return false; }
    //! Finds the substring \a s in the string, starting at position \a start.
    int
    FindStr(const MAPSStackedString& s,
	    int start=0) const;
    //! Finds the substring \a s in the string, starting at position \a start.
    int
    FindStr(const char* s,
	    int start=0) const;
    /*@}*/

    //! Reduces the length of the string to \a pos.
    /*! Puts a 0 (end of string) in position \a pos */
    MAPSStackedString&
    Shorten(int pos)
    { if (pos<l) {l=pos;str[pos]=0;} return *this; }
    //! Clears the content of the string.
    void
    Clear()
	{ l=0;str[0]=0; }
    //! Updates the length of the string by finding the first 0 character in the string buffer.
    MAPSStackedString&
    UpdateLength() {l=0; while(str[l]) ++l; return *this;}
    //! Removes the trailing <CODE>'\\n'</CODE> or <CODE>'\\r\\n'</CODE> characters.
    MAPSStackedString&
    RemoveCRLF() {if (l>=1) { if (str[l-1]=='\n') { str[l-1]=0; --l; if (l>=1) {if (str[l-1]=='\r') { str[l-1]=0; --l; } } } } return *this;}
    //! Removes spaces, tabs or carriage returns found at the beginning and at the end of the string.
    MAPSStackedString&
    RemoveSpaces();
    //! Sets the string to uppercase ('abc'->'ABC'). Returns a reference to the current string (<code>*this</code>).
    MAPSStackedString&
    Uppercase() { for (int i=0;i<Length();i++) if ((str[i]>='a')&&(str[i]<='z')) str[i]-='a'-'A'; return (*this);}
    //! Sets the first character of the string to uppercase
    MAPSStackedString&
    FirstUppercase() { if ((str[0]>='a')&&(str[0]<='z')) str[0]-='a'-'A'; return (*this); }
    //! Substitutes all \a s characters with \a d. Returns a reference to the current string (<code>*this</code>).
    MAPSStackedString&
    Substitute(char s,
	       char d) 
    { for (int i=0;i<l;i++) if (str[i]==s) str[i]=d; return (*this); }
    //! Sets the current string to the reverse of string \a s. Returns a reference to the current string (<code>*this</code>).
    MAPSStackedString&
    Reverse(const MAPSStackedString& s);

    /*! \name Formatting
     * @{
     */
    //! Formats a string.
    MAPSStackedString&
    Format(const char* format_string,
	   const char* s);
	
    //! Formats an integer.
    MAPSStackedString&
    Format(const char* format_string,
	   int m)
    { return this->formatX(format_string,m,0); }
    //! Formats a floating point number.
    MAPSStackedString&
    Format(const char* format_string,
	   double x)
    { return this->formatX(format_string,0,x); }
    /*@}*/

    /*! \name Constructors
     * @{
     */
    //! Formatting constructor
    MAPSStackedString(const char* string_format,
		      const char* s)
    { this->init(0,NULL);this->Format(string_format,s); }
    //! Formatting constructor
    MAPSStackedString(const char* string_format,
		      int n)
    { this->init(0,NULL);this->Format(string_format,n); }
    //! Formatting constructor
    MAPSStackedString(const char* string_format,
		      double x)
    { this->init(0,NULL);this->Format(string_format,x); }
    //! Constructs a string from another string in C format.
    MAPSStackedString(const char* s)
    { if(s) this->init((int)MAPS::Strlen(s),s); else this->init(0,NULL); }
    //! Constructs a string containing a single character
    explicit
    MAPSStackedString(const char c)
    { this->init(1,&c); }
    //! Copy constructor
    MAPSStackedString(const MAPSStackedString& s)
    { this->init(s.Length(),s);tpos=s.tpos;}
    //! Preallocating constructor. By default, sets the length to 0. Optionally sets the length to \a n, setting the <i>n</i>th character to 0 (End of string)
    explicit
    MAPSStackedString(int n,
		      bool set_length=false)
    { if (n<0) n=0;this->init(n,NULL);if (set_length) {l=n;str[l]=0;} }
    //! Simple integer formatting. Fills with 0 if the number \a n to format is too short.
    MAPSStackedString(int n,
		      int nbdigits)
    { this->init(nbdigits,NULL);l=nbdigits;for (int i=0,j=n;i<nbdigits;i++) {str[nbdigits-i-1]=(char)((j%10)+'0'); j/=10;} }
    //! Simple constructor.
    MAPSStackedString() {this->init(0,NULL);}
    /*@}*/

    // Destructor
    virtual
    ~MAPSStackedString() {this->destroy();}

#ifdef USE_COMPATIBILITY_V2
    /*! \name Deprecated members
     * @{
     */
    int length() const {return l;} //!< \deprecated Use MAPSStackedString#Length() instead.
    //  M4 protection
    int len() const {return l;} //!< \deprecated Use MAPSStackedString#Length() instead.
    //  M4 protection
    const char* beginning() const {return str;} //!< \deprecated Use MAPSStackedString#Beginning() instead.
    int pos(const char* ptr) const {return ptr-str;} //!< \deprecated Use MAPSStackedString#Pos(const char*) const instead.
    bool operator||(const MAPSStackedString& s) const { return this->Iequals(s); } //!< \deprecated Was too ambigous. Use MAPSStackedString#Iequals(const MAPSStackedString&) const instead.
    bool EqualsLower(const MAPSStackedString& s) const { return this->Iequals(s); } //!< \deprecated Use MAPSStackedString#Iequals(const MAPSStackedString&) const instead.
    MAPSStackedString& substitute(char s,char d) {return Substitute(s,d);} //!< \deprecated Use MAPSStackedString#Substitute(char,char) instead.
    bool equals(const MAPSStackedString& s) const {return (*this)==s;} //!< \deprecated Use MAPSStackedString#Equals(const MAPSStackedString&) const instead.
    bool iequals(const MAPSStackedString& s) const {return (*this)||s;} //!< \deprecated Use MAPSStackedString#Iequals(const MAPSStackedString&) const instead.
    bool equals_lower(const MAPSStackedString& s) const {return (*this)||s;} //!< \deprecated Use MAPSStackedString#Iequals(const MAPSStackedString&) const instead.
    MAPSStackedString& reverse(const MAPSStackedString& s) {return Reverse(s);} //!< \deprecated Use MAPSStackedString#Reverse(const MAPSStackedString&) instead.
    MAPSStackedString up2(char s, bool from_beginning=true) const {return Up2(s,from_beginning);} //!< \deprecated Use MAPSStackedString#Up2(char,bool) const instead.
    MAPSStackedString up_to(char s, bool from_beginning=true) const {return Up2(s,from_beginning);} //!< \deprecated Use MAPSStackedString#Up2(char,bool) const instead.
    MAPSStackedString up2(int m, bool from_beginning=true) const {return Up2(m,from_beginning);} //!< \deprecated Use MAPSStackedString#Up2(int,bool) const instead.
    MAPSStackedString up_to(int m, bool from_beginning=true) const {return Up2(m,from_beginning);} //!< \deprecated Use MAPSStackedString#Up2(int,bool) const instead.
    MAPSStackedString tail(char s, bool from_beginning=true) const {return Up2(s,from_beginning);} //!< \deprecated Use MAPSStackedString#Up2(char,bool) const instead.
    MAPSStackedString tail(int m, bool from_beginning=true) const {return Up2(m,from_beginning);} //!< \deprecated Use MAPSStackedString#Up2(int,bool) const instead.
    MAPSStackedString part(int start, int stop) const {return (*this)(start,stop);} //!< \deprecated Use MAPSStackedString#Part(int,int) const instead.
    MAPSStackedString left(int length) const {return Left(length);} //!< \deprecated Use MAPSStackedString#Left(int) const instead.
    MAPSStackedString right(int length) const {return Right(length);} //!< \deprecated Use MAPSStackedString#Right(int) const instead.
    MAPSStackedString mid(int start,int length=-1) {return Mid(start,length);} //!< \deprecated Use MAPSStackedString#Mid(int,int) const instead.
    MAPSArray<MAPSStackedString> split(const char c) const {return Split(c);} //!< \deprecated Use MAPSStackedString#Split(const char) const instead.
    MAPSStackedString& uppercase() {return Uppercase();} //!< \deprecated Use MAPSStackedString#Uppercase() instead.
    MAPSStackedString firstUppercase() {return FirstUppercase();} //!< \deprecated Use MAPSStackedString#FirstUppercase() instead.
    const char* strtok(const char* delim) {return Strtok(delim);} //!< \deprecated Use MAPSStackedString#Strtok(const char*) instead.
    MAPSStackedString& token_reset() {tpos=0;return *this;} //!< \deprecated Use MAPSStackedString#TokenReset() instead.
    bool token(const char* delim,MAPSStackedString& s) {return Token(delim,s);} //!< \deprecated Use MAPSStackedString#Token(const char*,MAPSStackedString&) instead.
    bool find(char c,int& pos) const {return Find(c,pos);} //!< \deprecated Use MAPSStackedString#Find(char,int&,int) const instead.
    bool find(char c,int& pos,int start) const {return Find(c,pos,start);} //!< \deprecated Use MAPSStackedString#Find(char,int&,int) const instead.
    bool find(const char* s,int& pos) const {return Find(s,pos);} //!< \deprecated Use MAPSStackedString#Find(const char*,int&,int) const instead.
    bool find(const char* s,int& pos,int start) const {return Find(s,pos,start);} //!< \deprecated Use MAPSStackedString#Find(const char*,int&,int) const instead.
    const char* find(char c) const {char* p=(char*)str;while(*p){if(*p==c) return p;p++;}return NULL;} //!< \deprecated Use MAPSStackedString#Find(char,int&,int) const instead.
    const char* find(char c,const char* start) const {const char* p=start;while(*p){if(*p==c) return p;p++;}return NULL;} //!< \deprecated Use MAPSStackedString#Find(char,int&,int) const instead.
    const char* find(const char* s) const {int n=MAPS::Strlen(s);char* p=(char*)str;while(*p){for(int i=0;i<n;i++) if(*p==s[i]) return p;p++;}return NULL;} //!< \deprecated Use MAPSStackedString#Find(const char*,int&,int) const instead.
    const char* find(const char* s,const char* start) const {int n=MAPS::Strlen(s);const char* p=start;while(*p){for(int i=0;i<n;i++) if(*p==s[i]) return p;p++;}return NULL;} //!< \deprecated Use MAPSStackedString#Find(const char*,int&,int) const instead.
    MAPSStackedString& shorten(int pos) {return Shorten(pos);} //!< \deprecated Use MAPSStackedString#Shorten(int) instead.
    MAPSStackedString& update_length() {return UpdateLength();} //!< \deprecated Use MAPSStackedString#UpdateLength() instead.
    MAPSStackedString& remove_crlf() {return RemoveCRLF();} //!< \deprecated Use MAPSStackedString#RemoveCRLF() instead.
    MAPSStackedString& removespaces() {return RemoveSpaces();} //!< \deprecated Use MAPSStackedString#RemoveSpaces() instead.
    ///  M4 protection
    MAPSStackedString& format(const char* format_string, const char* s) {return Format(format_string,s);} //!< \deprecated Use MAPSStackedString#Format(const char*,const char*) instead.
    MAPSStackedString& format(const char* format_string, int m) {return formatX(format_string,m,0);} //!< \deprecated Use MAPSStackedString#Format(const char*,int) instead.
    MAPSStackedString& format(const char* format_string, double x) {return formatX(format_string,0,x);} //!< \deprecated Use MAPSStackedString#Format(const char*,double) instead.
    ///  M4 protection
    /*@}*/
#endif
};

// MAPSStackedString inline methods implementation
template<int BUFFER_SIZE> 
    inline void 
    MAPSStackedString<BUFFER_SIZE>::init(int lx, const char* s) 
    {
	tpos=0;
	if (lx>=BUFFER_SIZE) {
	    // Memory reservation
	    res=((lx+1)/BUFFER_SIZE+1)*BUFFER_SIZE;
	    str=new char[res];
#ifdef UNICODE
		unicode_str = new MAPSUInt16[res];
#endif
	} else {
	    // Use the stack
	    res=BUFFER_SIZE;
	    str=buf;
#ifdef UNICODE
		unicode_str = unicode_buff;
#endif
	}
	// String initialization
	if (s) {
	    l=lx;
	    MAPS::Memcpy(str,s,lx);
	    str[lx]='\0';
	} else {
	    str[0]='\0';
	    l=0;
	}
    }
template<int BUFFER_SIZE>
    inline void
    MAPSStackedString<BUFFER_SIZE>::concat(int lx, const char* s)
    {
	if (l+lx>=res) {
	    char *strx=str;
	    // Memory reservation
	    res=((l+lx+1)/BUFFER_SIZE+1)*BUFFER_SIZE;
	    str=new char[res];
#ifdef UNICODE
		MAPSUInt16* unicode_strx = unicode_str;
		unicode_str = new MAPSUInt16[res];
#endif
	    // String copy
	    MAPS::Memcpy(str,strx,l);
	    // Memory free
	    if (strx!=buf) delete[] strx;
#ifdef UNICODE
		if (unicode_strx != unicode_buff) delete [] unicode_strx;
#endif
	}
	// The concatenation itself
	MAPS::Memcpy(str+l,s,lx);
	str[l+lx]='\0'; // 0 termination
	l+=lx;
    }
template<int BUFFER_SIZE>
    inline MAPSStackedString<BUFFER_SIZE>&
    MAPSStackedString<BUFFER_SIZE>::formatX(const char* format_string, 
					    int integer, 
					    double decimal)
    {
	int sign,leftnumber,nb,nbofdigits;
	bool foundStar=false;
	bool wrongNumber=false;
	bool addMinus=false;
	// Is this an integer to format, or a double?
	if (integer) {
	    if (integer>0) {
		sign=1;
		leftnumber=integer;
	    } else if (integer<0) {
		sign=-1;
		leftnumber=-integer;
	    } else {
		sign=0;
		leftnumber=0;
	    }
	} else {
	    if (decimal>0) {
		sign=1;
	    } else if (decimal<0) {
		sign=-1;
		decimal=-decimal;
	    } else {
		sign=0;
	    }
	}

	// Is there one, two or three sections in the format string ?
	MAPSStackedString fx(format_string);
	MAPSStackedString section1,section2,section3;
	MAPSStackedString result;
	int section=0;
	if (fx.Token(";",section1)) {
	    // When there is one section only, the format expression applies to all values.
	    // When there are two sections, the first section applies to positive values and zeros, the second to negative values.
	    // When there are three sections, the first section applies to positive values, the second to negative values, and the third to zeros.
	    if (fx.Token(";",section2)) {
		if (fx.Token(";",section3)) {
		    switch (sign) {
		    case -1: section=1; break;
		    case  0: section=2; break;
		    case +1: section=0; break;
		    }
		} else {
		    switch (sign) {
		    case -1: section=1; break;
		    case  0: section=0; break;
		    case +1: section=0; break;
		    }
		}
	    }
	    MAPSStackedString f,rightpart;
	    switch (section) {
	    case 0:
		f=section1;
		if (sign==-1) {
		    addMinus=true;
		}
		break;
	    case 1:
		f=section2;
		break;
	    case 2:
		f=section3;
		break;
	    }
	    if (f=="") {
		f=section1;
		if (sign==-1) {
		    addMinus=true;
		}
	    }

	    // Well, now we have the right format string (f)
	    // Look for a % character
	    int pos,start=0;
	    while (f.Find('%',pos,start)) {
		if ((pos==0)||(f[pos-1]!='\\')) {
		    if (!integer) {
			decimal*=100;
		    }
		    break;
		}
		start=pos+1;
	    }

	    // Look for the presence of the . character, the decimal separator
	    bool found_decimal_separator=false;
	    start=0;
	    while (f.Find('.',pos,start)) {
		if ((pos==0)||(f[pos-1]=='#')||(f[pos-1]=='0')||(f[pos-1]=='_')) {
		    found_decimal_separator=true;
		    break;
		}
		start=pos+1;
	    }
	    if (found_decimal_separator) {
		rightpart=f.Tail(pos+1);
		f.Shorten(pos);
					
		if (!integer) {
		    // Get the fractionnal part of the decimal number
		    double dec2,dummy;
		    dec2=MAPS::Modf(decimal,&dummy);
					
		    // Find out the number of #, 0 or _ character in the format string
		    nb=0;
		    start=0;
		    while (rightpart.Find("#0_",pos,start)) {
			if ((pos==0)||(f[pos-1]!='\\')) {
			    nb++;
			}
			start=pos+1;
		    }
					
		    // Find out the number of digits to the right of the decimal point in the decimal number
		    double x=dec2;
		    if (MAPS::Fabs(x)<1e-10) {
			nbofdigits=0;
		    } else {
			double precision=1e-12;
			for (nbofdigits=1;nbofdigits<10;nbofdigits++) { // 10 is the max number of digits
			    x*=10;
			    x-=(int)x;
			    if (MAPS::Fabs(x)<precision) break;
			    if (1-MAPS::Fabs(x)<precision) break;
			    precision*=10;
			}
		    }
					
		    x=dec2;
		    foundStar=rightpart.Find('*',pos);
		    if ((!foundStar)&&(nbofdigits>nb)) {
			// Round the number
			int c;
			for (c=0;c<nb;c++) x*=10;
			double remaining=MAPS::Modf(x,&dummy);
			if (remaining>=0.5) x+=1;
			for (c=0;c<nb;c++) x/=10;
		    }
		    // Temporary solution
		    if (decimal>2e10) {
			leftnumber=-2;
		    } else {
			leftnumber=(int)decimal;
		    }
		    if (x>=1) leftnumber++;
		} else {
		    decimal=0;
		}
	    } else {
		if (!integer) {
		    // Round the number
		    leftnumber=(int)(decimal+0.5);
		}
	    }
	    if (leftnumber<0) {
		wrongNumber=true;
		leftnumber=0;
		nbofdigits=0;
	    }

	    // Look for an amperand (&), special character that sets to hexa
	    start=0;
	    int radix=10;
	    while (f.Find('&',pos,start)) {
		if ((pos==0)||(f[pos-1]!='\\')) {
		    if (integer) {
			radix=16;
		    }
		    break;
		}
		start=pos+1;
	    }

	    // Look for the first #,0,_ or ! character
	    pos=0;
	    f.Find("#0_!",pos);

	    // Now, let's process the left part, right to left
	    f.InitNextChar(false); // Right to left
	    bool backslashed;
	    bool thousand_separator=false;
	    char cf,previous=0;
	    int count=0; // character counter
	    char csx;
	    cf=f.NextChar(false,true,&backslashed);
	    while (cf) {
		bool next=true;
		if (!backslashed) {
		    switch (cf) {
		    // '0' : Digit placeholder. Displays a digit or a zero.
		    //
		    // If the expression has a digit in the position where the 0 appears in the 
		    // format string, it displays the digit. Otherwise, it displays a zero in that
		    // position. 
		    //
		    // If the number has fewer digits than there are zeros (on either side of the 
		    // decimal) in the format expression, it displays leading or trailing zeros. 
		    //
		    // If the number has more digits to the right of the decimal separator than 
		    // there are zeros to the right of the decimal separator in the format 
		    // expression, it rounds the number to as many decimal places as there are 
		    // zeros. 
		    //
		    // If the number has more digits to the left of the decimal separator than 
		    // there are zeros to the left of the decimal separator in the format 
		    // expression, it displays the extra digits without modification.
		    //
		    case '0':
			if (thousand_separator) {
			    if (count&&!(count%3)) {
				result+=',';
			    }
			}
			if (wrongNumber) {
			    result+='#';
			} else {
			    if ((!addMinus)||(f.CharPos(false)!=pos)) {
				csx=MAPSStackedString::digit(leftnumber,radix); leftnumber/=radix;
				result+=csx;
			    } else count--;
			}
			count++;
			// If this is the last 0 (the leftmost), write the remaining digits
			if (f.CharPos(false)==pos) {
			    while (leftnumber) {
				if (thousand_separator) {
				    if (count&&!(count%3)) {
					result+=',';
				    }
				}
				csx=MAPSStackedString::digit(leftnumber,radix); leftnumber/=radix;
				result+=csx;
				count++;
			    }
			    if (addMinus) {
				result+='-';
				count++;
			    }
			}
			break;
		    // '#' : Digit placeholder. Displays a digit or nothing. If the expression has a digit
		    // in the position where the # appears in the format string, it displays the 
		    // digit. Otherwise, it displays nothing in that position. This symbol works 
		    // like the 0 digit placeholder except that leading and trailing zeros are not 
		    // displayed if the number has the same or fewer digits than there are # 
		    // characters on either side of the decimal separator in the format expression.
		    case '#':
			if ((leftnumber)||((count==0)&&(!addMinus)&&found_decimal_separator)) {
			    if (thousand_separator) {
				if (count&&!(count%3)) {
				    result+=',';
				}
			    }
			    csx=MAPSStackedString::digit(leftnumber,radix); leftnumber/=radix;
			    result+=csx;
			}
			count++;
			// If this is the last # (the leftmost), write the remaining digits
			if (f.CharPos(false)==pos) {
			    while (leftnumber) {
				if (thousand_separator) {
				    if (!(count%3)) {
					result+=',';
				    }
				}
				csx=MAPSStackedString::digit(leftnumber,radix); leftnumber/=radix;
				result+=csx;
				count++;
			    }
			    if (addMinus) {
				result+='-';
				count++;
			    }
			}
			break;
		    // '_' : Digit placeholder. Displays a digit or a space. If the expression has a digit
		    // in the position where the _ appears in the format string, it displays the 
		    // digit. Otherwise, it displays a space in that position. This symbol works much
		    // like the 0 digit placeholder.
		    case '_':
			if ((leftnumber)||(!count)) {
			    if (thousand_separator) {
				if (count&&!(count%3)) {
				    result+=',';
				}
			    }
			    csx=MAPSStackedString::digit(leftnumber,radix); leftnumber/=radix;
			    result+=csx;
			} else {
			    if (wrongNumber) {
				result+='#';
			    } else {
				if (addMinus) {
				    result+='-';
				    addMinus=false;
				} else {
				    result+=' ';
				}
			    }
			}
			count++;
			// If this is the last # (the leftmost), write the remaining digits
			if (f.CharPos(false)==pos) {
			    while (leftnumber) {
				if (thousand_separator) {
				    if (!(count%3)) {
					result+=',';
				    }
				}
				csx=MAPSStackedString::digit(leftnumber,radix); leftnumber/=radix;
				result+=csx;
				count++;
			    }
			    if (addMinus) {
				result+='-';
				count++;
			    }
			}
			break;
		    // ',' : Thousand separator
		    case ',':
			if ((previous=='_')||(previous=='0')||(previous=='#')) {
			    // Read the next character
			    previous=cf;
			    cf=f.NextChar(false,true,&backslashed);
			    next=false;
			    if ((cf=='_')||(cf=='0')||(cf=='#')||(cf==',')) {
				// The comma is surrounded by '0' or '#' characters
				thousand_separator=true;
			    } else {
				result+=previous;
			    }
			} else {
			    if (count==0) {
				if (integer) {
				    leftnumber/=1000;	
				} else {
				    decimal/=1000;
				    if (found_decimal_separator ) {
					// Get the fractionnal part of the decimal number
					double dec2,dummy;
					dec2=MAPS::Modf(decimal,&dummy);
										
					// Find out the number of digits to the right of the decimal point in the decimal number
					double x=dec2;
					if (MAPS::Fabs(x)<1e-10) {
					    nbofdigits=0;
					} else {
					    double precision=1e-12;
					    for (nbofdigits=1;nbofdigits<10;nbofdigits++) { // 10 is the max number of digits
						x*=10;
						x-=(int)x;
						if (MAPS::Fabs(x)<precision) break;
						if (1-MAPS::Fabs(x)<precision) break;
						precision*=10;
					    }
					}
										
					x=dec2;
					if ((!foundStar)&&(nbofdigits>nb)) {
					    // Round the number
					    int c;
					    for (c=0;c<nb;c++) x*=10;
					    double remaining=MAPS::Modf(x,&dummy);
					    if (remaining>=0.5) x+=1;
					    for (c=0;c<nb;c++) x/=10;
					}
					leftnumber=(int)decimal;
					if (x>=1) leftnumber++;										
				    } else {
					// Round the number
					leftnumber=(int)(decimal+0.5);
				    }
				}

			    } else if (previous==',') {
				thousand_separator=true;
			    } else {
				result+=cf;
			    }
			}
			break;
		    case '!':
			break;
		    case '&':
			break;
		    default :
			result+=cf;
		    }
		} else {
		    result+=cf;
		}
		if (next) {
		    previous=cf;
		    cf=f.NextChar(false,true,&backslashed);
		}
	    }
	    Reverse(result);
	    if (found_decimal_separator) {
		result="";
		double x,dummy;
		if (integer) {
		    x=0;
		    nbofdigits=0;
		} else {
		    // Get the fractionnal part of the decimal number
		    x=MAPS::Modf(decimal,&dummy);
		    if ((!foundStar)&&(nbofdigits>nb)) {
			// Round the number
			int c;
			for (c=0;c<nb;c++) x*=10;
			double remaining=MAPS::Modf(x,&dummy);
			if (remaining>=0.5) x+=1;
			for (c=0;c<nb;c++) x/=10;
		    }
		    if (x>=1) x-=1;
		}
				
		bool dot=false;
		cf=rightpart.NextChar(true,true,&backslashed);
		nb=0;
		while (cf) {
		    if (!backslashed) {
			switch (cf) {
			case '0':
			    if (!dot) {
				dot=true;
				result+='.';
			    }
			    nb++;
			    if (nb<=nbofdigits) {
				x*=10;
				if (nb==nbofdigits) {
				    if (MAPS::Modf(x,&dummy)>0.5) {
					int n=(int)(((int)x)+0.5)+1;
					if (n==10) {
					    result+='9';
					} else {
					    result+=(char)('0'+n);
					}
					break;
				    }
				}
				result+=(char)('0'+(int)(((int)x)+0.5));
				x=MAPS::Modf(x,&dummy);
			    } else {
				result+=(wrongNumber)?'#':'0';
			    }
			    break;
			case '#':
			    nb++;
			    if (nb<=nbofdigits) {
				if (!dot) {
				    dot=true;
				    result+='.';
				}
				x*=10;
				if (nb==nbofdigits) {
				    if (MAPS::Modf(x,&dummy)>0.5) {
					int n=(int)(((int)x)+0.5)+1;
					if (n==10) {
					    result+='9';
					} else {
					    result+=(char)('0'+n);
					}
					break;
				    }
				}
				result+=(char)('0'+(int)(((int)x)+0.5));
				x=MAPS::Modf(x,&dummy);
			    }
			    break;
			case '_':
			    if (!dot) {
				dot=true;
				result+='.';
			    }
			    nb++;
			    if (nb<=nbofdigits) {
				x*=10;
				if (nb==nbofdigits) {
				    if (MAPS::Modf(x,&dummy)>0.5) {
					int n=(int)(((int)x)+0.5)+1;
					if (n==10) {
					    result+='9';
					} else {
					    result+=(char)('0'+n);
					}
					break;
				    }
				}
				result+=(char)('0'+(int)(((int)x)+0.5));
				x=MAPS::Modf(x,&dummy);
			    } else {
				result+=(wrongNumber)?'#':' ';
			    }
			    break;
			case '*':
			    while (nb<nbofdigits) {
				if (!dot) {
				    dot=true;
				    result+='.';
				}
				nb++;
				x*=10;
				if (nb==nbofdigits) {
				    if (MAPS::Modf(x,&dummy)>0.5) {
					int n=(int)(((int)x)+0.5)+1;
					if (n==10) {
					    result+='9';
					} else {
					    result+=(char)('0'+n);
					}
					break;
				    }
				}
				result+=(char)('0'+(int)(((int)x)+0.5));
				x=MAPS::Modf(x,&dummy);
			    }
			    break;
			default :
			    result+=cf;
			}
		    } else {
			result+=cf;
		    }
		    cf=rightpart.NextChar(true,true,&backslashed);
		}
		*this+=result;
	    }
	}
	return *this;
    }
template< int BUFFER_SIZE>
    inline int
    MAPSStackedString<BUFFER_SIZE>::FindStr(const MAPSStackedString& s,
					    int start/*=0*/) const
    {
	int n=s.Length();
	if(n==0)
	    return -1;
	for(int i=start;i<Length()-n+1;++i) {
	    if(str[i]==s[0]) {
		int j=i,k=0;
		while(k<n) {
		    if(str[j]!=s[k])
			k=n;
		    ++k;
		    ++j;
		}
		if(k==n)
		    return i;
	    }
	}
	return -1;
    }
template< int BUFFER_SIZE>
    inline int
    MAPSStackedString<BUFFER_SIZE>::FindStr(const char* s,
					    int start/*=0*/) const
    {
	int n=MAPS::Strlen(s);
	if(n==0)
	    return -1;
	for(int i=start;i<Length()-n+1;++i) {
	    if(str[i]==s[0]) {
		int j=i,k=0;
		while(k<n) {
		    if(str[j]!=s[k])
			k=n;
		    ++k;
		    ++j;
		}
		if(k==n)
		    return i;
	    }
	}
	return -1;
    }
template<int BUFFER_SIZE>
    inline MAPSArray< MAPSStackedString<BUFFER_SIZE> >
    MAPSStackedString<BUFFER_SIZE>::Split(const char c) const {
	MAPSArray<MAPSStackedString> result;
	if(l==0) {
	    result.Push("");
	    return result;
	}
	int i=0, j=0;
	char* msg = new char[l+1];
	msg[0] = '\0';
	while(i<l) {
	    if(str[i] == c) {
		if(j > 0)
		    msg[j++] = '\0';
		result.Push(msg);
		j=0;
		msg[0] = '\0';
	    }
	    else
		msg[j++]=str[i];
	    ++i;
	}
	msg[j]='\0';
	result.Push(msg);
	delete[] msg;
	return result;
    }
template<int BUFFER_SIZE>
    inline MAPSStackedString<BUFFER_SIZE>&
    MAPSStackedString<BUFFER_SIZE>::Reverse(const MAPSStackedString& s) {
	MAPSStackedString<40> temp(s);
	this->destroy(); 
	this->init(temp.Length(), NULL); 
	l=temp.Length();
	str[l]=0;		
	for (int i=0;i<l;++i)
	    str[i]=temp[l-i-1];
	return *this;
    }
template<int BUFFER_SIZE>
    inline bool
    MAPSStackedString<BUFFER_SIZE>::Token(const char* delim,
	  MAPSStackedString& s) {
	if (tpos==l) return false;
	int start=tpos;
	int n;
	if (delim) n=MAPS::Strlen(delim); else n=0;
	while (tpos!=l) {
	    for (int i=0;i<n;++i) {
		if (str[tpos]==delim[i]) {
		    s=Part(start,tpos-1);
		    ++tpos;
		    return true;
		}
	    }
	    ++tpos;
	}
	s=Tail(start);
	return true;
    }
template<int BUFFER_SIZE>
    inline const char*
    MAPSStackedString<BUFFER_SIZE>::Strtok(const char* delim) {
	if (!str[tpos]) return NULL;
	const char* start=str+tpos;
	int n;
	if (delim) n=(int)MAPS::Strlen(delim); else n=0;
	while (str[tpos]) {
	    for (int i=0;i<n;++i) {
		if (str[tpos]==delim[i]) {
		    str[tpos]=0;
		    ++tpos;
		    return start;
		}
	    }
	    ++tpos;
	}
	return start;		
    }
template<int BUFFER_SIZE>
    inline MAPSStackedString<BUFFER_SIZE>&
    MAPSStackedString<BUFFER_SIZE>::RemoveSpaces() 
    {
	char c;
	int i;
	// Leading spaces
	for (i=0;i<l;++i)
	{
	    c=str[i];
	    if ((c!=' ')&&(c!='\t')&&(c!='\n'))
		break;
	}
	MAPSStackedString s(this->Tail(i));
	// Trailing spaces
	for (i=s.Length()-1;i>=0;--i)
	{
	    c=s[i];
	    if ((c!=' ')&&(c!='\t')&&(c!='\n'))
		break;
	}
	s.Shorten(i+1);
	*this=s;
	return *this;
    }
template<int BUFFER_SIZE>
    inline char
    MAPSStackedString<BUFFER_SIZE>::NextChar(bool left2right/*=true*/,
					     bool interpret_special/*=false*/,
					     bool *backslashed/*=NULL*/)
    {
	if (left2right) {
	    if (str[tpos]=='\\') {
		++tpos;
		if (interpret_special) {
		    char c=str[tpos];
		    char special;
		    switch (c) {
		    case 'n': special='\n'; break;
		    case 't': special='\t'; break;
		    case 'r': special='\r'; break;
		    case '\\': special='\\'; break;
		    default : special=0; break;
		    }
		    if (special) {
			++tpos;
			if (backslashed) {
			    *backslashed=false;
			}
			return special;
		    }
		}
		if (backslashed) {
		    *backslashed=true;
		    char c=str[tpos];
		    ++tpos;
		    return c;
		} else {
		    return '\\';
		}
	    } else {
		if (backslashed)
		    *backslashed=false;
		char c=str[tpos];
		if (c)
		    ++tpos;
		return c;
	    }
	} else {
	    if (!tpos) 
		return 0;
	    if ((tpos>1)&&(str[tpos-2]=='\\')) {
		if (interpret_special) {
		    char c=str[tpos-1];
		    char special;
		    switch (c) {
		    case 'n': special='\n'; break;
		    case 't': special='\t'; break;
		    case 'r': special='\r'; break;
		    case '\\': special='\\'; break;
		    default : special=0; break;
		    }
		    if (special) {
			tpos-=2;
			if (backslashed)
			    *backslashed=false;
			return special;
		    }
		}
		if (backslashed) {
		    *backslashed=true;
		    char c=str[tpos-1];
		    tpos-=2;
		    return c;
		} else {
		    char c=str[tpos-1];
		    --tpos;
		    return c;
		}
	    } else {
		if (backslashed)
		    *backslashed=false;
		char c=str[tpos-1];
		--tpos;
		return c;
	    }
	}
    }
template<int BUFFER_SIZE>
    inline MAPSStackedString<BUFFER_SIZE>&
    MAPSStackedString<BUFFER_SIZE>::Format(const char *format_string,
					   const char *s) {
	// Is there one or two sections in the format string ?
	MAPSStackedString sx(s);
	MAPSStackedString fx(format_string);
	MAPSStackedString section1,section2;
	MAPSStackedString result;
	int section=0;
	if (fx.Token(";",section1)) {
	    // When there is one section only, the format applies to all string data
	    // When there are two sections, the first section applies to string data,
	    // the second to NULL values and zero-length strings ("")
	    if (fx.Token(";",section2)) {
		if ((s==NULL)||(s[0]==0)) {
		    section=1;
		}
	    }
	    MAPSStackedString& f=(section)?section2:section1;
	    // Well, now we have the right format string (f)
	    // Look for the presence of a ! character, which forces left-to-right fill of placeholders (the default is to fill placeholders from right to left)
	    int pos;
	    char transform=0;
	    bool left2right=false;
	    int start=0;
	    while (f.Find('!',pos,start)) {
		if ((pos==0)||(f[pos-1]!='\\')) {
		    left2right=true;
		    break;
		}
		start=pos+1;
	    }
	    start=0;
	    while (f.Find('<',pos,start)) {
		if ((pos==0)||(f[pos-1]!='\\')) {
		    transform='<';
		    break;
		}
		start=pos+1;
	    }
	    start=0;
	    while (f.Find('>',pos,start)) {
		if ((pos==0)||(f[pos-1]!='\\')) {
		    transform='>';
		    break;
		}
		start=pos+1;
	    }
	    // Well, now we know in which direction to go
	    f.InitNextChar(left2right);
	    sx.InitNextChar(left2right);
	    bool backslashed;
	    char cf,csx=sx.NextChar(left2right);
	    while ('\0'!=(cf=f.NextChar(left2right,true,&backslashed))) {
		if (!backslashed) {
		    switch (cf) {
		    // '@' : Character placeholder. Displays a character or a space. If the string
		    // has a character in the position where the at symbol (@) appears in the format
		    // string, it displays the character. Otherwise, it displays a space in that
		    // position. Placeholders are filled from right to left unless there is an
		    // exclamation point character (!) in the format string.
		    case '@':
			result+=(csx)?MAPSStackedString::tfx(csx,transform):' ';
			csx=sx.NextChar(left2right);
			break;
		    // '&' : Character placeholder. Displays a character or nothing. If the string has a 
		    // character in the position where the ampersand (&) appears, it displays the 
		    // character. Otherwise, it displays nothing. Placeholders are filled from right to
		    // left unless there is an exclamation point character (!) in the format string.
		    case '&':
			if (csx) {
			    result+=MAPSStackedString::tfx(csx,transform);
			}
			csx=sx.NextChar(left2right);
			break;
		    // '<' : Forces lowercase. Displays all characters in lowercase format.
		    case '<':
		    // '>' : Forces uppercase. Displays all characters in uppercase format.
		    case '>':
		    // '!' : Forces left-to-right fill of placeholders. (The default is to fill placeholders from right to left.)
		    case '!':
			break;
	    	    // '*' : Place all remaining characters
		    case '*':
			while (csx) {
			    result+=MAPSStackedString::tfx(csx,transform);
			    csx=sx.NextChar(left2right);
			}
			break;
		    default :
			result+=cf;
		    }
		} else {
		    result+=cf;
		}
	    }
	    if (left2right) {
		*this=result;
	    } else {
		this->Reverse(result);
	    }
	}
	return *this;
    }

/*!
* \ingroup bds
*/
template<int BUFFER_SIZE>
	inline MAPSStackedString<BUFFER_SIZE>
	operator+(const MAPSStackedString<BUFFER_SIZE>& s1,
	const MAPSStackedString<BUFFER_SIZE>& s2)
	{
		MAPSStackedString<BUFFER_SIZE> s(s1.Length()+s2.Length(),true);
		int i;
		for (i=0;i<s1.Length();++i)
			s[i]=s1[i];
		for (i=0;i<s2.Length();++i)
			s[i+s1.Length()]=s2[i];
		return s;
	}

/*!
* \ingroup bds
*/
template<int BUFFER_SIZE>
	inline MAPSStackedString<BUFFER_SIZE>
	operator+(const MAPSStackedString<BUFFER_SIZE>& s1,
	const char* s2)
	{
		if(s2 == NULL)
			return s1;
		int l1 = s1.Length(), l2 = MAPS::Strlen(s2);
		MAPSStackedString<BUFFER_SIZE> s(l1+l2,true);
		int i;
		for (i=0;i<l1;++i)
			s[i]=s1[i];
		for (i=0;i<l2;++i)
			s[i+l1]=s2[i];
		return s;
	}

/*!
* \ingroup bds
*/
template<int BUFFER_SIZE>
	inline MAPSStackedString<BUFFER_SIZE>
	operator+(const char* s1,
	const MAPSStackedString<BUFFER_SIZE>& s2)
	{
		if(s1 == NULL)
			return s2;
		int l2 = s2.Length(), l1 = MAPS::Strlen(s1);
		MAPSStackedString<BUFFER_SIZE> s(l1+l2,true);
		int i;
		for (i=0;i<l1;++i)
			s[i]=s1[i];
		for (i=0;i<l2;++i)
			s[i+l1]=s2[i];
		return s;
	}

template<int BUFFER_SIZE>
	inline MAPSStackedString<BUFFER_SIZE>
	operator+(MAPSStackedString<BUFFER_SIZE>& s, char c)
	{ MAPSStackedString<BUFFER_SIZE> s2(s); s2 += c; return s2; }

template<int BUFFER_SIZE>
	inline MAPSStackedString<BUFFER_SIZE>
	operator+(char c, MAPSStackedString<BUFFER_SIZE>& s)
	{ return s+c; }

template<int BUFFER_SIZE>
	inline bool
	operator==(const char* s1, MAPSStackedString<BUFFER_SIZE>& s2)
	{ return s2 == s1; }

/*! \var MAPSStackedString<40> MAPSString;
 *  \brief The RTMaps flexible and fast string class.
 *  \sa MAPSStreamedString
 *  \ingroup bds
 */
typedef MAPSStackedString<40> MAPSString;

namespace MAPSSManip
{
    static const unsigned int S_NUMERIC_MASK = 0x000F;
    enum flags
    {
	f_dec,
	f_hex,
	f_oct,
	f_endl = 0x10,
	f_ends
    };
    struct MAPSIOSBase {
	unsigned int m_flag;
	MAPSIOSBase(unsigned int f) {m_flag = f;}
    };
    static const MAPSIOSBase dec(f_dec);
    static const MAPSIOSBase hex(f_hex);
    static const MAPSIOSBase oct(f_oct);
    static const MAPSIOSBase endl(f_endl);
    static const MAPSIOSBase ends(f_ends);
};

//! The RTMaps streamed string class.
/*! The MAPSStreamedString class extends the MAPSString features.
 * The main extension is the operator<< support. Special formatting options are also available, in the namespace \c MAPSSManip:
 *  - \c dec, \c hex, \c oct for integers. For example:
 * \code
 MAPSStreamedString sx("Formatting test:");
 unsigned int i = 15;
 sx << "i=" << i << ", 0" << MAPSSManip::oct << i << ", 0x" << MAPSSManip::hex << i;
 Now sx contains: "Formatting test: i=15, 017, 0xF"
 \endcode
 *  - \c endl: adds just a '\\n' to the end of the string
 *  - \c ends: adds just a '\\0' to the end of the string
 * \note If you use intensively formatting options, you can add 
 * \code using namespace MAPSSManip;\endcode after
 * the inclusion of maps.hpp, but you may have conflicts with STL names.
 * \sa MAPSString MAPSStackedString
 * \ingroup bds
 */
class MAPSStreamedString : public MAPSString
{
private:
    unsigned int m_fmtflags;
    void 
    printHex16(MAPSUInt16 ui16)
    { char _tmp[8]; sprintf(_tmp,"%x",ui16); (*this)+=(const char*)_tmp; }
    void
    printOct16(MAPSUInt16 ui16)
    { char _tmp[8]; sprintf(_tmp,"%o",ui16); (*this)+=(const char*)_tmp; }
    void 
    printHex32(MAPSUInt32 ui32)
    { char _tmp[16]; sprintf(_tmp,"%x",ui32); (*this)+=(const char*)_tmp; }
    void
    printOct32(MAPSUInt32 ui32)
    { char _tmp[16]; sprintf(_tmp,"%o",ui32); (*this)+=(const char*)_tmp; }
    void
    printHex64(MAPSUInt64 ui64)
    {
	char _tmp[24];
	sprintf(_tmp,
#ifdef _WIN32
		"%I64x",
#elif defined MAPS_64
		"%lx",
#else
		"%llx",
#endif
		ui64);
	(*this)+=(const char*)_tmp;
    }
    void
    printOct64(MAPSUInt64 ui64)
    {
	char _tmp[24];
	sprintf(_tmp,
#ifdef _WIN32
		"%I64o",
#elif defined MAPS_64
		"%lo",
#else
		"%llo",
#endif
		ui64);
	(*this)+=(const char*)_tmp;
    }
public:
    MAPSStreamedString() : MAPSString(),m_fmtflags(0) {}
    explicit
    MAPSStreamedString(char* s) : MAPSString(s),m_fmtflags(0) {}
    explicit
    MAPSStreamedString(const char* s) : MAPSString(s),m_fmtflags(0) {}
    explicit
    MAPSStreamedString(MAPSString& s) : MAPSString(s),m_fmtflags(0) {}
    explicit
    MAPSStreamedString(int n) : MAPSString(n),m_fmtflags(0) {}

    virtual ~MAPSStreamedString() {}

    MAPSStreamedString&
    operator<<(const MAPSSManip::MAPSIOSBase& m);
    MAPSStreamedString&
    operator<<(bool b)
    { (*this)+=(b?"1":"0"); return (*this); }
    MAPSStreamedString&
    operator<<(MAPSInt16 i16)
    {
	if(m_fmtflags&MAPSSManip::f_hex) printHex32((MAPSUInt16)i16);
	else if(m_fmtflags&MAPSSManip::f_oct) printOct32((MAPSUInt16)i16);
	else { char _tmp[8]; sprintf(_tmp,"%d",i16); (*this)+=_tmp; }
	return (*this);
    }
    MAPSStreamedString&
    operator<<(MAPSUInt16 ui16)
    {
	if(m_fmtflags&MAPSSManip::f_hex) printHex16(ui16);
	else if(m_fmtflags&MAPSSManip::f_oct) printOct16(ui16);
	else { char _tmp[8]; sprintf(_tmp,"%u",ui16); (*this)+=_tmp; }
	return (*this);
    }
    MAPSStreamedString&
    operator<<(MAPSInt32 i32)
    {
	if(m_fmtflags&MAPSSManip::f_hex) printHex32((MAPSUInt32)i32);
	else if(m_fmtflags&MAPSSManip::f_oct) printOct32((MAPSUInt32)i32);
	else { char _tmp[16]; sprintf(_tmp,"%d",i32); (*this)+=_tmp; }
	return (*this);
    }
    MAPSStreamedString&
    operator<<(MAPSUInt32 ui32)
    {
	if(m_fmtflags&MAPSSManip::f_hex) printHex32(ui32);
	else if(m_fmtflags&MAPSSManip::f_oct) printOct32(ui32);
	else { char _tmp[16]; sprintf(_tmp,"%u",ui32); (*this)+=_tmp; }
	return (*this);
    }
    MAPSStreamedString&
    operator<<(MAPSInt64 i64)
    {
	if(m_fmtflags&MAPSSManip::f_hex) printHex64(i64);
	else if(m_fmtflags&MAPSSManip::f_oct) printOct64(i64);
	else { char _tmp[24];
	    sprintf(_tmp,
#ifdef _WIN32
		    "%I64d",
#elif defined MAPS_64
		    "%ld",
#else
		    "%lld",
#endif
		    i64);
	    (*this)+=_tmp;
	}
	return (*this);
    }
    MAPSStreamedString&
    operator<<(MAPSUInt64 ui64)    {
	if(m_fmtflags&MAPSSManip::f_hex) printHex64(ui64);
	else if(m_fmtflags&MAPSSManip::f_oct) printOct64(ui64);
	else { char _tmp[24];
	    sprintf(_tmp,
#ifdef _WIN32
		    "%I64u",
#elif defined MAPS_64
		    "%lu",
#else
		    "%llu",
#endif
		    ui64);
	    (*this)+=_tmp;
	}
	return (*this);
    }
    MAPSStreamedString&
    operator<<(float f)
    { char _tmp[32]; sprintf(_tmp,"%.10g",(double)f); (*this)+=_tmp; return (*this); }
    MAPSStreamedString&
    operator<<(double d)
    { char _tmp[32]; sprintf(_tmp,"%.10g",d); (*this)+=_tmp; return (*this); }
    MAPSStreamedString&
    operator<<(char c) { (*this)+=c; return (*this); }
    MAPSStreamedString&
    operator<<(const char* s) { (*this)+=s; return (*this); }
    MAPSStreamedString&
    operator<<(unsigned char* s) { (*this)+=s; return (*this);}
    MAPSStreamedString&
    operator<<(MAPSString& s) { (*this)+=s; return (*this); }
};

inline MAPSStreamedString&
MAPSStreamedString::operator<<(const MAPSSManip::MAPSIOSBase& m)
{
    switch(m.m_flag) {
    case (MAPSSManip::f_dec):
    case (MAPSSManip::f_hex):
    case (MAPSSManip::f_oct):
	m_fmtflags &= ~MAPSSManip::S_NUMERIC_MASK;
	m_fmtflags |= m.m_flag;
	break;
    case (MAPSSManip::f_endl):
	(*this)+='\n';
	break;
    case (MAPSSManip::f_ends):
	(*this)+='\0';
	break;
    }
    return (*this);
}

//! The RTMaps array class
/*! This class implements a resizable one-dimensional array. It can also work as a very efficient ring buffer, a queue or even a stack.
 *	Indeed, it acts as a Perl-like array, that is with an offset management.
 *
 *	Using this class is generally faster than using MAPSList, since MAPSArray allocates
 *	objects by packets, and not one by one. MAPSArray is the preferable class to use
 *	when you need to use a queue or a stack.
 *
 *	\note The constructor and the destructor of class \c T are never called.Use MAPSList if you need a systematic
 *	call to content constructors/destructors.
 *
 *	\note The size of the allocated array is always of power of 2, due to an optimization in the
 *	ring buffer behaviour management.
 * \ingroup bds
 */
template <typename T>
    class MAPSArray
	{
    private:
	static int
	compute_array_size(int n)
	{ int m=n-1,p=1; if (n) {while (m) {p<<=1;m>>=1;} return p;} else return 0; }

    protected:
	int s; // s is always a power of 2
	int busy;
	int offset;
	T *t;
	bool owner;

    public:
	// Operators
	//! Retrieves the element with index \a i in the array
	/*! No memory allocation if \a i is superior to the actual size of the array.*/
	T&
	operator[](int i) const
	{
#ifdef BDS_RUNTIME_CHECK
	    if ((i<0)||(i>=busy)) {throw "Index out of bounds";}
#endif
	    return t[(i+offset)&(s-1)];
	}
	//! Retrieves the element with index \a i in the array and resizes the array when necessary.
	T&
	operator()(int i);
	//! Copy operator
	MAPSArray<T>&
	operator=(const MAPSArray<T>& v);
	//! Gets the current size of the array
	int
	Dim() const {return busy;}
	//! Gets the current size of the array
	int
	Size() const {return busy;}
	//! Returns a pointer to the data
	/*! Should be used with care... Especially after the use of Shift() function, it will not give
	* you what you expect, since the array will thus be used as a ring buffer. 
	* Furthermore, the pointer might not be valid for long due to possible reallocations.
	*/
	T*
	Vect() const {return t;}
	//! Fills the array with model \a model
	void
	Fill(const T& model)
	{ for(int i=0;i<busy;++i) (*this)[i]=model; }
	//! Appends an element to the array
	/*! Resizes the array and returns a reference to the new appended element.*/
	T&
	Append() {return (*this)(busy);}
	//! Sets the content of the array. The data is not copied.
	/*! This is not a copy operator. The array will point to the T objects
	* given in arguments. This data will not be destroyed when the object
	* is destroyed (i.e. the array does not own the data).
	*/
	void
	Set(T* ptr,
	    int d)
	{ owner=false; t=ptr; s=compute_array_size(d); busy=d; offset=0; }
	//! Reallocates the data
	/*! Moves previous data to a new location with room for \a d pieces of data.*/
	void
	Realloc(int d);
	//! Allocates some data
	/*! Destroys previously allocated data.*/
	void
	Alloc(int d)
	{ if (owner&&t) delete[] t; s=compute_array_size(d); busy=d; owner=true; offset=0; if (s) t=new T[s]; else t=NULL; }
	//! Sets the size of the array. Moves the data (realloc) if needed. 
	void
	SetSize(int d)
	{if (d<=s) busy=d; else Realloc(d);}
	//! Clears the array (i.e. Resets size to zero)
	void
	Clear() {SetSize(0); offset=0;}
	//! Cast operator. Returns a C-like array.
	/*! \copydoc MAPSArray::Vect()
	*/
	operator T*() {return t;}
	
	//! Shifts an element from the array
	/*! Perl-like operator. Removes an element from the beginning of the array (efficiently, i.e.
	*	without moving all the elements to the left, but through the management of an "offset" variable)
	*  Note that further pointers returned by a Vect() function are not valid any more after a call to Shift().
	*
	*  This function returns a reference, but this reference is only valid up to the next call
	*	to another array related function, since the element can and will certainly be crushed by
	*  a further call to Push() of Unshift().
	*/
	T&
	Shift() {T &z=t[offset]; ++offset; offset&=(s-1); --busy; return z;}
	//! Unshifts an element to the array
	/*! Perl-like operator. It adds an element in front of the array. Inserts an element at the beginning of the array (efficiently, i.e.
	*	without moving all the elements to the right, but through the management of an "offset" variable)
	*  Note that further pointers returned by a Vect() function are not valid any more after a call to Shift().
	*/
	void
	Unshift(const T& z);
	//! Pops an element from the end of the array
	/*! This function returns a reference, but this reference is only valid up to the next call
	* to another array related function, since the element can and will certainly be crushed by
	* a further call to Push() of Unshift().
	*/
	T&
	Pop() {T &z=t[(offset+busy-1)&(s-1)]; --busy; return z;}
	//! Pushes an element to the end of the array
	void
	Push(const T& z) {T& zx=Append(); zx=z;}

	// Constructors and Destructor
	//! Basic constructor
	/*! Allocates nothing. The size of the array is set to 0.
	*/
	MAPSArray()
	: s(0),busy(0),offset(0),t(NULL),owner(true)
	{ }
	//! Allocating constructor
	/*! Allocates an array of size \a d. Indeed, the true allocated size is a power of 2.
	*/
	explicit
	MAPSArray(int d)
	: s(compute_array_size(d)),busy(d),offset(0),owner(true)
	{ if (s) t=new T[s]; else t=NULL; }
	//! Copy constructor
	MAPSArray(const MAPSArray<T>& v)
	: s(v.s),busy(v.busy),offset(0),owner(true)
	{ if (s) t=new T[s]; else t=NULL; for (int i=0;i<busy;++i) t[i]=v[i]; }
	//! Destructor
	virtual ~MAPSArray() {if ((t!=NULL)&&(owner)) delete[] t;}

#ifdef USE_COMPATIBILITY_V2
	/*! \name Deprecated members
	 * @{
	 */
	int dim() const {return busy;} //!< \deprecated Use MAPSArray#Size() const instead.
	int size() const {return busy;} //!< \deprecated Use MAPSArray#Size() const instead.
	T* vect() const {return t;} //!< \deprecated Use MAPSArray#Vect() const instead.
	void fill(const T& model) {Fill(model);} //!< \deprecated Use MAPSArray#Fill(const T&) instead.
	T& append() {return (*this)(busy);} //!< \deprecated Use MAPSArray#Append() instead.
	void set(T* ptr,int d) {Set(ptr,d);} //!< \deprecated Use MAPSArray#Set(T*, int) instead.
	void realloc(int d) {Realloc(d);} //!< \deprecated Use MAPSArray#Realloc(int) instead.
	void alloc(int d) {Alloc(d);} //!< \deprecated Use MAPSArray#Alloc(int) instead.
	void set_size(int d) {SetSize(d);} //!< \deprecated Use MAPSArray#SetSize(int) instead.
	void clear() {Clear();} //!< \deprecated Use MAPSArray#Clear() instead.
	//  M4 protection
	T& shift() {return Shift();} //!< \deprecated Use MAPSArray#Shift() instead.
	//  M4 protection
	void unshift(const T& z) {Unshift(z);} //!< \deprecated Use MAPSArray#Unshift(const T&) instead.
	T& pop() {return Pop();} //!< \deprecated Use MAPSArray#Pop() instead.
	void push(const T& z) {Push(z);} //!< \deprecated Use MAPSArray#Push() instead.
	/*@}*/
#endif
    };

// MAPSArray inline methods implementations
template<typename T>
    inline T&
    MAPSArray<T>::operator()(int i)
    {
	if ((owner)&&(i<s)) {
	    if (i>=busy) busy=i+1;
	    return t[(i+offset)&(s-1)];
	} else if ((owner)||(i>=busy)) {
	    int news=compute_array_size(i+1);
	    T *newt=new T[news];
	    for (int j=0;j<busy;j++) {newt[j]=t[(j+offset)&(s-1)];}
	    if ((t!=NULL)&&(owner)) {delete[] t;}
	    t=newt; s=news; offset=0;
	    owner=true;
	    busy=i+1;		
	    return t[i];
	} else {
	    return t[(i+offset)&(s-1)];	
	}
    }
template<typename T>
    inline MAPSArray<T>&
    MAPSArray<T>::operator=(const MAPSArray<T>& v)
    {
	if (s<v.Size()) {
	    if ((t!=NULL)&&(owner)) delete[] t;
	    owner=true;
	    s=compute_array_size(v.Size());
	    if (s) t=new T[s]; else t=NULL;
	}
	busy=v.busy; offset=0;
	for (int i=0;i<busy;i++) t[i]=v[i];
	return (*this);
    }
template<typename T>
    inline void
    MAPSArray<T>::Realloc(int d)
    {
	if (d<busy) busy=d; // Size down if requested
	int news=compute_array_size(d);
	T* newt=new T[news];
	for (int j=0;j<busy;++j) {newt[j]=t[(j+offset)&(s-1)];}
	if ((t!=NULL)&&(owner)) {delete[] t;}
	t=newt; s=news; offset=0;
	owner=true;
	busy=d;
    }
template<typename T>
    inline void
    MAPSArray<T>::Unshift(const T& z)
    {
	if (busy<s) {
	    --offset; offset&=(s-1);
	    t[offset]=z;
	} else {
	    int news=(s<<1);
	    T *newt=new T[news];
	    for (int j=0;j<busy;++j) {newt[j+1]=t[(j+offset)&(s-1)];}
	    if ((t!=NULL)&&(owner)) {delete[] t;}
	    t=newt; s=news; offset=0;
	    owner=true; t[0]=z;
	}
	++busy;
    }

//! The RTMaps regular expressions class
/*! 
 *
 *     Regular expression support is provided by the PCRE library package,
 *    which is open source software, written by Philip Hazel, and copyright
 *    by the University of Cambridge, England.
 *
 *	You can find some further information on PCRE (including sources) at :
 *
 *    ftp://ftp.csx.cam.ac.uk/pub/software/programming/pcre/
 *
 * \ingroup bds
 */
struct MAPSbaseregexp;

class MAPSRegExp
{
    MAPSbaseregexp* base;

    void
    addref();
    void
    decref();
    void
    newbase();

    void
    compile_pattern(const char* pattern);
    void
    study_pattern();
    bool
    exec(const char* subject,
	 int length);

public:
    static const int OptCaseless;

    //! Changes current options. Will be considered the next time the pattern is changed.
    void
    SetOptions(int o);
    //! Sets the current pattern.
    void
    SetPattern(const char *pattern);
    //! Returns \c true if evrything is ok with this regular expression
    bool
    Ok() const;
    //! Returns a string pointing to the last error description (\c NULL if no error occured).
    const char*
    LastError() const;
    //! Returns the offset in the pattern where the last compilation error occured.
    int
    ErrorOffset() const;
    //! If the pattern is intended to be used several times, it is worth studying it further. This is the goal of this function.
    void
    Study() {study_pattern();}
    //! Matches the string \a s against the current pattern.
    template<int T> bool
    Match(MAPSStackedString<T>& s) { return exec(s,s.Length()); }
    //! Matches the string \a s of length \a l against the current pattern. If parameter \a l is omitted, the string is considered 0 terminated (classical C string).
    bool
    Match(const char* s,
	  int l=0)
    { return exec(s,(l)?l:(int)MAPS::Strlen(s)); }
    //! Returns whether the last match succeeded or not
    bool
    Matched() const
	{ return this->Substrings().Size() != 0; }
    //! Returns the array of matched substrings for the last match
    MAPSArray<MAPSString>&
    Substrings() const;
    //! Returns the <CODE>i</CODE>th matched substring
    MAPSString&
    Substring(int i=0) const
	{ return this->Substrings()[i]; }
    //! Perl-like syntax ($1,$2,etc) for matched substrings
    MAPSString&
    Dollar(int i) const
	{ return this->Substrings()[i]; }
    //! Returns the numbers of captured substrings
    int
    NbSubstrings() const
	{ return this->Substrings().Size(); }
    //! Returns the offset of the <CODE>i</CODE>th matched substring
    int
    Position(int i=0) const;
	
    MAPSRegExp&
    operator~() {return (*this);}
    MAPSRegExp&
    operator=(const MAPSRegExp &regexp);
    
    MAPSRegExp();
    
    explicit
    MAPSRegExp(const char* pattern,
	       int opt=0);
    MAPSRegExp(const MAPSRegExp& regexp);

    virtual ~MAPSRegExp() {decref();}

#ifdef USE_COMPATIBILITY_V2
    /*! \name Deprecated members
     * @{
     */
    void set_options(int o){SetOptions(o);} //!< Use MAPSRegExp#SetOptions(int) instead.
    void set_pattern(const char* pattern){SetPattern(pattern);} //!< Use MAPSRegExp#SetPattern(const char*) instead.
    bool ok() const {return Ok();} //!< Use MAPSRegExp#Ok() const instead.
    const char* last_error() const {return LastError();} //!< Use MAPSRegExp#LastError() const instead.
    int error_offset() const {return ErrorOffset();} //!< Use MAPSRegExp#ErrorOffset() const instead.
    void study() {Study();} //!< Use MAPSRegExp#Study() instead.
    template<int T> bool match(MAPSStackedString<T>& s){return Match(s);} //!< Use MAPSRegExp#Match(MAPSStackedString&) instead.
    bool match(const char* s, int l=0) {return Match(s,l);} //!< Use MAPSRegExp#Match(const char*,int) instead.
    bool matched() const {return Matched();} //!< Use MAPSRegExp#Matched() const instead.
    MAPSArray<MAPSString>& substrings() const {return Substrings();} //!< Use MAPSRegExp#Substrings() const instead.
    MAPSString& substring(int i=0) const {return Substring(i);} //!< Use MAPSRegExp#Substring(int) const instead.
    MAPSString& dollar(int i) const {return Dollar(i);} //!< Use MAPSRegExp#Dollar(int) const instead.
    int nb_substrings() const {return NbSubstrings();} //!< Use MAPSRegExp#NbSubstrings() const instead.
    /*@}*/
#endif
};

template <int BUFFER_SIZE>
    MAPSRegExp&
    MAPSStackedString<BUFFER_SIZE>::operator=(MAPSRegExp& regexp) 
    {
	regexp.Match(*this);
	return regexp;
    }

//! The RTMaps MAPSMatrix2 class, a powerful matrix management class.
/*	This matrix class is fundamentally different from the MAPSMatrix class.
 *	The MAPSMatrix class was designed to be an input/output type for RTMaps
 *	components, and thus it has many limitations. Among these limitations,
 *	MAPSMatrixes can only store complex numbers, and due to MATLAB and Fortran compatibility
 *	issues, MAPSMatrixes store these numbers column-wise.
 *
 *	On the contrary, the MAPSMatrix2 class is a very powerful and efficient class
 *	for doing matrix computations. It is a template class able to store integers, floats,
 *	doubles or complex numbers at will. A lot of operators are defined for this
 *	class, such as addition, multiplication, transpose, inverse, etc.
 * \ingroup bds
 */
template<typename T>
    class MAPSMatrix2 {
	struct Bbasematrix {
	    int nor; // number of rows
	    int noc; // Number of columns
	    MAPSArray<MAPSArray<T> > t;
	    MAPSArray<T> data;
	    int nbrefs;
	    T&
	    operator()(int y,
		       int x)
	    {
#ifdef BDS_RUNTIME_CHECK
		if ((x<0)||(y<0)||(y>=nor)||(x>=noc)) { throw "Index out of bounds"; }
#endif
		return data[y*noc+x];
	    }
	    Bbasematrix(int nr,
			int nc)
	    :nor(nr),noc(nc),t(nr),data(nr*nc),nbrefs(1)
	    { for (int i=0;i<nr;++i) { t[i].Set(&data[i*nc],nc); } }

	    virtual ~Bbasematrix() {}
	};
	Bbasematrix *bm;

    int
    pivot(int row);
    void
    init(int nor,
	 int noc)
    { bm=new Bbasematrix(nor,noc); }
public:
    // Functions
    //! Returns the number of rows
    int
    M() const {return bm->nor;}
    //! Returns the number of columns
    int
    N() const {return bm->noc;}
    //! Returns the number of rows
    int
    NbRows() const {return bm->nor;}
    //! Returns the number of columns
    int
    NbColumns() const {return bm->noc;}
    //! Returns the number of columns
    int
    SizeX() const {return bm->noc;}
    //! Returns the number of rows
    int
    SizeY() const {return bm->nor;}
    //! Returns the number of rows
    int
    Dim1() const {return bm->nor;}
    //! Returns the number of columns
    int
    Dim2() const {return bm->noc;}
    //! Maps to an array of pointers to each row, for handling by C functions.
    void
    To(T *t[]) const
    { for (int i=0;i<M();i++) t[i]=bm->t[i].Vect(); }
    /*! \name Operators
     * @{
     */
    //! Returns element (\a i,\a j) of the matrix.
    /*! Valid values are: 0<= \a i < M(), 0<= \a j < N()
     */
    T&
    operator()(int i,
	       int j) const
    { return (*bm)(i,j); }
    //! Returns row \a i of the matrix. 0<= \a i < M().
    /*! This allows the use of <code>[i][j]</code> combination to retrieve an element of the matrix.
     */
    MAPSArray<T>&
    operator[](int i) const
    { return bm->t[i]; }
    //! Warning! This operator does not make a copy (too slow).
    /*! It is a real mathematical operator = (The MAPSMatrix2 class has a reference counter)
     */
    MAPSMatrix2&
    operator=(const MAPSMatrix2 &v)
    { v.bm->nbrefs++; if ((bm)&&(--bm->nbrefs==0)) delete bm; bm=v.bm; return (*this); }
    //! Fills the matrix with data from C array \a v
    MAPSMatrix2&
    operator=(T* v)
    { int i,j,c; for(i=0,c=0;i<NbRows();++i) { for(j=0;j<NbColumns();++j,++c) {(*bm)(i,j)=v[c];} } return (*this); }
    //! Transposition operator
    MAPSMatrix2<T>
    operator~() const 
    { MAPSMatrix2<T> temp(NbColumns(),NbRows());for(int i=0;i<NbRows();++i) { for(int j=0;j<NbColumns();++j) {temp(j,i)=(*bm)(i,j);} } return temp; }
    //! Matrix inversion operator
    MAPSMatrix2<T>
    operator!() const;
    /*@}*/

    /*! \name Functions
     * @{
     */
    //! Makes a clone copy of the original matrix.
    /*! When you use the operator = (such as in A=B), A is not really a copy of B.
     *	A and B share the same matrix in memory. If you do A.Clone(), the A will
     *	have its own image of the matrix in memory, i.e. any modification of A
     *	will not affect B any longer.
     */
    void
    Clone()
    { bm->nbrefs--; Bbasematrix *newbm=new Bbasematrix(NbRows(),NbColumns()); for (int i=0;i<NbRows();++i) { for (int j=0;j<NbColumns();++j) {(*newbm)(i,j)=(*bm)(i,j);} } bm=newbm; }
    //! Sets all the elements of the matrix to 0.
    void
    Clear(void)
    { for (int i=0;i<NbRows();i++) { for (int j=0;j<NbColumns();++j) {(*bm)(i,j)=T(0);} } }
    //! Sets the current matrix to identity, i.e. all the elements of the matrix will be set to 0, except the diagonal elements that will be set to 1.
    void
    Id() { this->Clear(); for (int i=0;i<NbRows();++i) { (*bm)(i,i)=T(1); } }

    //! Computes the inverse of the matrix.
    MAPSMatrix2<T>
    Inverse() const  { return !(*this); }
    //! Computes the determinant of the matrix.
    T
    Det() const;
    //! Computes the pseudo inverse of the matrix, i.e <CODE>!(~A*A)*~A</CODE>. This is the general formula. There are some optimizations if the matrix has special properties.
    MAPSMatrix2<T>
    PseudoInverse() const
    { MAPSMatrix2<T> &A=*this; return !(~A*A)*~A; }
    //! Returns row \a i, where 0<= \a i < M()
    MAPSArray<T>&
    Row(int i) const
    { return bm->t[i]; }
    //! Returns column \a i, where 0<= \a i < N()
    MAPSArray<T>
    Col(int i) const
    { MAPSArray<T> v(NbRows()); for (int j=0;j<NbRows();++j) v[j]=(*bm)(j,i); return v; }
    /*@}*/

    /*! \name Constructors and destructor
     * @{
     */
    //! Default constructor
    MAPSMatrix2()
    :bm(NULL)
    { }
    //! Constructor with size of the matrix
    MAPSMatrix2(int m,
		int n)
    { init(m,n); }
    //! Copy constructor
    MAPSMatrix2(const MAPSMatrix2 &v)
    { bm=v.bm;bm->nbrefs++; } 
    //! Destructor
    virtual ~MAPSMatrix2() { if ((bm)&&(--bm->nbrefs==0)) delete bm; }
    /*@}*/
#ifdef USE_COMPATIBILITY_V2
    /*! \name Deprecated members
     * @{
     */
    int m() const {return bm->nor;}
    int n() const {return bm->noc;}
    int nor() const {return bm->nor;}
    int noc() const {return bm->noc;}
    int nrow() const {return bm->nor;}
    int ncol() const {return bm->noc;}
    int sx() const {return bm->noc;}
    int sy() const {return bm->nor;}
    int sizex() const {return bm->noc;}
    int sizey() const {return bm->nor;}
    int dim1() const {return bm->nor;}
    int dim2() const {return bm->noc;}
    T& t(int i, int j) const { return (*bm)(i,j); } //!< \deprecated Use MAPSMatrix2::operator()(int,int)
    void to(T* t[]) const {To(t);} //!< \deprecated Use MAPSMatrix2::To
    void map_to(T* t[]) const {To(t);} //!< \deprecated Use MAPSMatrix2::To
    void Map2(T* t[]) const {To(t);} //!< \deprecated Use MAPSMatrix2::To
    void MapTo(T* t[]) const {To(t);} //!< \deprecated Use MAPSMatrix2::To
    void clone() {Clone();}
    void clear() {Clear();}
    void id() {Id();}
    MAPSMatrix2<T> inverse() const {return Inverse();}
    T det() const {return Det();}
    MAPSMatrix2<T> pseudoinverse() const {return PseudoInverse();}
    MAPSArray<T>& row(int i) const {return bm->t[i];}
    MAPSArray<T> col(int i) const {return Col(i);}
    /*@}*/
#endif
};

// MAPSMatrix2 inline methods implementations
template<typename T>
    inline int
    MAPSMatrix2<T>::pivot(int row)
    {
	int k=row;
	double amax=-1,temp;
	for (int i=row; i < NbRows(); i++) {
	    if ( (temp = MAPS::Fabs( (*bm)(i,row))) > amax && temp != 0.0) {
		amax = temp;
		k = i;
	    }
	}
	if ((*bm)(k,row) == T(0))
	    return -1;
	if (k != row)
	{
	    for (int j=0;j<NbColumns();j++) {
		T swap=(*bm)(k,j);
		(*bm)(k,j)=(*bm)(row,j);
		(*bm)(row,j)=swap;
	    }
	    return k;
	}
	return 0;
    }
template<typename T>
    inline MAPSMatrix2<T>
    MAPSMatrix2<T>::operator!() const
    {
	MAPSMatrix2<T> m(*this);
	MAPSMatrix2<T> temp(NbRows(),NbColumns());
	temp.id();
	m.clone();
	int i,j,k;
	T a1,a2,swap;

	if (NbRows()!=NbColumns())
	    throw "MAPSMatrix2: matrix not square. Unable to invert it.";
	for (k=0; k < NbRows(); ++k)
	{
	    int indx = m.pivot(k);
	    if (indx == -1)
		throw "MAPSMatrix2: singular matrix. Unable to invert it.";
	    if (indx != 0) {
		for (j=0;j<NbColumns();++j) {
		    swap=temp(k,j);
		    temp(k,j)=temp(indx,j);
		    temp(indx,j)=swap;
		}
	    }
	    a1 = m(k,k);
	    for (j=0; j < NbRows(); ++j) {
		m(k,j) /= a1;
		temp(k,j) /= a1;
	    }
	    for (i=0; i < NbRows(); ++i) {
		if (i != k) {
		    a2 = m(i,k);
		    for (j=0; j < NbRows(); ++j) {
			m(i,j) -= a2 * m(k,j);
			temp(i,j) -= a2 * temp(k,j);
		    }
		}
	    }
	}
	return temp;
    }
template<typename T>
    inline T
    MAPSMatrix2<T>::Det() const
    {
	int i,j,k;
	T piv,detVal = T(1);
		
	if (NbRows() != NbColumns()) {
	    throw "MAPSMatrix2: matrix not square. Unable to compute its determinant.";
	}
		
	MAPSMatrix2<T> temp(*this);
	temp.Clone();
		
	for (k=0; k <NbRows(); k++)
	{
	    int indx = temp.pivot(k);
	    if (indx == -1)
		return 0;
	    if (indx != 0)
		detVal = - detVal;
	    detVal = detVal * temp(k,k);
	    for (i=k+1; i < NbRows(); i++)
	    {
		piv = temp(i,k) / temp(k,k);
		for (j=k+1; j < NbRows(); j++)
		    temp(i,j) -= piv * temp(k,j);
	    }
	}
	return detVal;
    }

//! MAPSMatrix2 multiplication operator
/*! \relates MAPSMatrix2
 * \ingroup bds
 */
template<typename T>
    inline MAPSMatrix2<T>
    operator*(const MAPSMatrix2<T>& A,
	      const MAPSMatrix2<T>& B)
    {
	MAPSMatrix2<T> temp(A.NbRows(),B.NbColumns());
	for (int i=0; i < A.NbRows(); ++i) {
	    for (int j=0; j < B.NbColumns(); ++j) {
		temp(i,j) = 0;
		for (int k=0; k < A.NbColumns(); ++k) {
		    temp(i,j) += A(i,k) * B(k,j);
		}
	    }
	}
	return temp;
    }

//! MAPSMatrix2 addition operator
/*! \relates MAPSMatrix2
 * \ingroup bds
 */
template<typename T>
    inline MAPSMatrix2<T>
    operator+(const MAPSMatrix2<T>& A,
	      const MAPSMatrix2<T>& B)
    {
	MAPSMatrix2<T> temp(A.NbRows(),A.NbColumns());
	for (int i=0; i < A.NbRows(); ++i) {
	    for (int j=0; j < A.NbColumns(); ++j) {
		temp(i,j) = A(i,j) + B(i,j);
	    }
	}
	return temp;
    }

//! MAPSMatrix2 substraction operator
/*! \relates MAPSMatrix2
 * \ingroup bds
 */
template<typename T>
    inline MAPSMatrix2<T>
    operator-(const MAPSMatrix2<T>& A,
	      const MAPSMatrix2<T>& B)
    {
	MAPSMatrix2<T> temp(A.NbRows(),A.NbColumns());
	for (int i=0; i < A.NbRows(); ++i) {
	    for (int j=0; j < A.NbColumns(); ++j) {
		temp(i,j) = A(i,j) - B(i,j);
	    }
	}
	return temp;
    }

//! Multiplication of a MAPSMatrix2 with a MAPSArray
/*! \relates MAPSArray
 * \ingroup bds
 */
template<typename T>
    inline MAPSArray<T>
    operator*(const MAPSMatrix2<T>& A,
	      const MAPSArray<T>& b)
    {
	MAPSArray<T> temp(A.NbRows());
	for (int i=0; i < A.NbRows(); ++i) {
	    temp[i] = A(i,0)*b[0];
	    for (int j=1; j < A.NbColumns(); ++j) {
		temp[i]+=A(i,j)*b[j];
	    }
	}
	return temp;
    }

//! MAPSArrays addition
/*! \relates MAPSArray
 * \ingroup bds
 */
template<typename T>
    inline MAPSArray<T>
    operator+(const MAPSArray<T>& a,
	      const MAPSArray<T>& b)
    {
	MAPSArray<T> temp(a.Size());
	for (int i=0; i < a.Size(); ++i) {
	    temp[i] = a[i] + b[i];
	}
	return temp;
    }

//! MAPSArrays substraction
/*! \relates MAPSArray
 * \ingroup bds
 */
template<typename T>
    inline MAPSArray<T>
    operator-(const MAPSArray<T>& a,
	      const MAPSArray<T>& b)
    {
	MAPSArray<T> temp(a.Size());
	for (int i=0; i < a.Size(); ++i) {
	    temp[i] = a[i] - b[i];
	}
	return temp;
    }

//! Addition of a MAPSMatrix2 with a MAPSArray
/*! The result is a MAPSArray. Only the first column of the matrix \a a is added to the array \a b.
 * \relates MAPSArray
 * \ingroup bds
 */
template<typename T>
    inline MAPSArray<T>
    operator+(const MAPSMatrix2<T>& a,
	      const MAPSArray<T>& b)
    {
	MAPSArray<T> temp(b.Size());
	for (int i=0; i < b.Size(); ++i) {
	    temp[i] = a(i,0) + b[i];
	}
	return temp;
    }

template<typename T>
    void
    kalman_predict(const MAPSArray<T>& xk,
		   const MAPSArray<T>& uk,
		   const MAPSMatrix2<T>& Pk,
		   MAPSArray<T>& xpkplus1,
		   MAPSMatrix2<T>& Ppkplus1,
		   const MAPSMatrix2<T>& Ad,
		   const MAPSMatrix2<T>& Bd,
		   const MAPSMatrix2<T>& Q)
    {
	xpkplus1=Ad*xk+Bd*uk;
	Ppkplus1=Ad*Pk*~Ad+Q;
    }

template<typename T>
    void 
    kalman_predictX(const MAPSArray<T>& xk,
		    const MAPSMatrix2<T>& Pk,
		    MAPSArray<T>& xpkplus1,
		    MAPSMatrix2<T>& Ppkplus1,
		    const MAPSMatrix2<T>& Ad,
		    const MAPSMatrix2<T>& Q)
    {
	xpkplus1=Ad*xk;
	Ppkplus1=Ad*Pk*~Ad+Q;
    }

template<typename T>
    void 
    kalman_adjust(const MAPSArray<T>& xpkplus1,
		  const MAPSMatrix2<T>& Ppkplus1,
		  const MAPSArray<T>& ykplus1,
		  MAPSArray<T>& xkplus1,
		  MAPSMatrix2<T>& Pkplus1,
		  const MAPSMatrix2<T>& C,
		  const MAPSMatrix2<T>& R)
    {
	MAPSMatrix2<T> In(xpkplus1.Size(),xpkplus1.Size());
	In.Id();
	MAPSMatrix2<T> Lkplus1=Ppkplus1*~C*!(C*Ppkplus1*~C+R);
	xkplus1=xpkplus1+Lkplus1*(ykplus1-C*xpkplus1);
	Pkplus1=(In-Lkplus1*C)*Ppkplus1;
    }
				
//! The RTMaps (key,content) pair template class
/*!
 * \sa MAPSHashTable
 * \ingroup bds
 */
template<typename TKey, typename TContent>
    class MAPSPair
    {
	int hash_val;
	TKey k;
	TContent c;
    public:
	//! Returns the key stored in the pair
	TKey&
	Key() { return k; }
	 //! Returns the content stored in the pair
	TContent&
	Content() { return c; }
	int&
	HashValue() { return hash_val; }
	//! Constructor
	MAPSPair(TKey &kx,
		 TContent &cx)
	{ k=kx; c=cx; }
	//! Default constructor
	MAPSPair() {}

	virtual ~MAPSPair() {}

#ifdef USE_COMPATIBILITY_V2
	TKey& First() { return k; }
	TContent& Second() { return c; }
	TKey& first() {return k;}
	TKey& key() {return k;}
	TContent& second() {return c;}
	TContent& content() {return c;}
	int& hash_value() {return hash_val;}
#endif
    };

class MAPSHashTableIterator;

/*!
 * \ingroup bds
 */
class MAPSBasicHashTable
{
    friend class MAPSHashTableIterator;
    virtual void* 
    nextX(MAPSHashTableIterator& it) const=0;
protected:
    virtual
    ~MAPSBasicHashTable() {}
};

//! The RTMaps hash table iterator
/*!
 * \ingroup bds
 */
class MAPSHashTableIterator
{
#if (_MSC_VER <= 1300) // <= .NET 2002
public :
#else
	template<typename TKey, typename TContent, typename H> friend class MAPSHashTable;
protected :
#endif
    int current_bucket;
    MAPSListIterator current_iterator;
    const MAPSBasicHashTable *associated_table;
    void *content;

public:
    //! The ++ operator : goes to the next element in the associated hash table
    MAPSHashTableIterator&
    operator++()
    { if (associated_table) content=associated_table->nextX(*this); return *this;}
    //! The ++ operator : goes to the next element in the associated hash table
    MAPSHashTableIterator
    operator++(int)
    { MAPSHashTableIterator tmp=*this; ++(*this); return tmp; }
    operator void*() {return content;}
     //! Resets the iterator.
    void
    Reset() {current_bucket=-1; if (associated_table) ++(*this);}
    //! Default constructor. Not that you cannot use the iterator directly. You must associate it to a hash table through a call to MAPSHashTable#Iterator or MAPSHashTable#Next.
    MAPSHashTableIterator() {associated_table=NULL; content=NULL; Reset();}
    //! Another constructor that associates this iterator to a hash table
    explicit
    MAPSHashTableIterator(const MAPSBasicHashTable& hash_table)
    { associated_table=&hash_table; Reset(); }


#ifdef USE_COMPATIBILITY_V2
    void reset() {Reset();}
#endif
};

/*! The RTMaps hash table class.
 * You should not expect any order of the items.
 * Hash table initialization example :
 \code
 MAPSHashTable<MAPSString,int,MAPSStringHashFunction> hash_table(1000);
 \endcode
 * Here is an example of hash table use :
 \code
 MAPSStringHashTable<int> HashTable(500);
 HashTable.Insert("Hello",20);
 HashTable.Insert("World",30);
 HashTable.Insert("!",40);
 int *res=HashTable.Lookup("Hello");
 std::cout<<"res : "<<*res<<std::endl;
 res=HashTable.Lookup("World");
 std::cout<<"res : "<<*res<<std::endl;
 HashTable.Remove("Hello");
 res=HashTable.Lookup("Hello"); // Now res is NULL
 MAPSHashTableIterator it(HashTable);
 while (it) {
     std::cout<<HashTable.Key(it)<<" = "<<HashTable.Content(it)<<std::endl;
     it++;
 }
 \endcode
 * \ingroup bds
 */
template<typename TKey, typename TContent, typename H> 
    class MAPSHashTable : public MAPSBasicHashTable 
    {
	H _hash;
	MAPSArray< MAPSList< MAPSPair<TKey,TContent> > > *hash_array;
	int s; // Size in bits of the buckets array
	unsigned int nb; // Number of elements currently in the hash
	bool auto_adjust; // Automatic adjusment of the size of the hash

	static unsigned int 
	hashsize(int n) { return 1<<n; }
	static unsigned int
	hashmask(int n) { return hashsize(n)-1; }

	virtual void*
	nextX(MAPSHashTableIterator& it) const;

    public:
	//! Sets the auto size adjustment behaviour of the hash table
	void
	SetAutoAdjust(const bool x)
	{ auto_adjust=x; }
	//! Returns the size of the hash table
	int
	HashSize() const {return hashsize(s); }
	//! Returns the number of elements currently in the hash table
	int
	Size() const
	{ return nb; }
	//! Returns the number of elements currently in the hash table
	int
	NbElts() const
	{ return nb; }
	//! Computes the number of buckets per stored element. When <1, the hash table should be resized.
	float
	Efficiency() const
	{ if (nb) {return hashsize(s)/((float)nb);} else return 1; }
	//! Inserts element \a content with key \a key into the hash table.
	/*! \warning This function does not check if an element with the same key exists.
	 * If it happens to be the case, the hash table content will be inconsistent.
	 */
	void
	Insert(const TKey& key, 
	       const TContent& content)
	{ this->Insert(key)=content; }
	//! Inserts a new element with key \a key into the hash table. Returns a reference to the content that you can further set up.
	/*! \warning This function does not check if an element with the same key exists.
	 * If it happens to be the case, the hash table content will be inconsistent.
	 */
	TContent&
	Insert(const TKey& key);
	//! Finds the element with key \a key. Returns \c NULL if that element cannot be found in the hash table
	TContent*
	Lookup(const TKey& key) const;
	//! Returns the element the key of which is \a key and removes it from the hash table.
	TContent
	Remove(const TKey& key);
	//! Removes element the key of which is \a key and returns the hash table itself (faster than Remove())
	MAPSHashTable&
	operator-=(const TKey& key);
	//! Removes element corresponding to iterator \a it from the hash table, and returns its content.
	TContent
	Remove(MAPSHashTableIterator& it) 
	{ MAPSList< MAPSPair<TKey,TContent> >& li=(*hash_array)[it.current_bucket];TContent c=li[it.current_iterator].Content();li-=it.current_iterator;--nb;return c; }
	//! Removes element corresponding to iterator \a it from the hash table. Returns the hash table itself (faster than Remove())
	MAPSHashTable&
	operator-=(MAPSHashTableIterator& it)
	{ MAPSList< MAPSPair<TKey,TContent> >& list=(*hash_array)[it.current_bucket];list-=it.current_iterator;--nb;return *this; }
	//! Returns a new iterator
	MAPSHashTableIterator
	Iterator() const
	{ MAPSHashTableIterator it(*this); return it; }
	//! STL-like End()
	void*
	End() const
	{ return NULL; }
	//! Returns the current key of an iterator
	TKey&
	Key(const MAPSHashTableIterator& it) const
	{ return ((MAPSPair<TKey,TContent> *)it.content)->Key(); }
	//! Returns the current content of an iterator
	TContent&
	Content(const MAPSHashTableIterator& it) const
	{ return ((MAPSPair<TKey,TContent> *)it.content)->Content(); }
	//! Returns the current content of an iterator
	TContent&
	operator[](const MAPSHashTableIterator& it) const
	{ return ((MAPSPair<TKey,TContent> *)it.content)->Content(); }
	//! Returns the current pair (key, content) of an iterator
	MAPSPair<TKey,TContent>&
	Pair(const MAPSHashTableIterator& it) const
	{ return *(MAPSPair<TKey,TContent> *)it.content; }
	//! Iterates...
	/*! Note that the elements in the hash table are NOT sorted.
	*/
	MAPSHashTableIterator&
	Next(MAPSHashTableIterator& it) const
	{ it.associated_table=this; nextX(it); return it; }
	//! Resizes the hash table to \a size (number of buckets)
	void
	Resize(const int size);
	//! Adjusts the size of the hash table if needed (smart function): double the current size of the hash while the efficiency is <1.
	bool
	Adjust()
	{ if (nb>hashsize(s)) { Resize(hashsize(s)*(nb/hashsize(s)+1)); return true; } return false; }
	//! Clears the content of the hash table.
	/*! \param n Same as \a n parameter in the constructor MAPSHashTable#MAPSHashTable(int,bool) 
	*/
	void
	Clear(const int n=16);
	//! Constructor
	/*!
	* \param n must be set so that the number of elements
	* that will be put in the hash table should be less than 3*n, unless the
	* hash table will work but will lose its efficiency.
	* The default value for \a n is 16. We strongly recommend that
	* you provide your own convenient value.
	* \param adjust when set to \c true, the size of the hash table is automatically adjusted when needed 
	* with Adjust() function.
	*/
	explicit
	MAPSHashTable(const int n=16, const bool adjust=true);
	MAPSHashTable(const MAPSHashTable& src);
	MAPSHashTable&
	operator=(const MAPSHashTable& src);
	// Destructor
	virtual ~MAPSHashTable()
	{ delete hash_array; }

#ifdef USE_COMPATIBILITY_V2
	/*! \name Deprecated members
	 * @{
	 */
	TContent* operator[](const TKey& key) const {return Lookup(key);} //!< \deprecated Use MAPSHashTable#Lookup(const TKey&) const instead.
	MAPSHashTableIterator NewIterator() const {return Iterator();} //!< \deprecated Use MAPSHashTable#Iterator() const instead.
	TContent *Find(const TKey& key) const { return Lookup(key); } //!< \deprecated Use MAPSHashTable#Lookup(const TKey&) const instead.
	void set_auto_adjust(bool x) {auto_adjust=x;} //!< \deprecated Use MAPSHashTable#SetAutoAdjust(bool) instead.
	int hash_size() {return hashsize(s);} //!< \deprecated Use MAPSHashTable#HashSize() const instead.
	int size() {return nb;} //!< \deprecated Use MAPSHashTable#Size() const instead.
	int nb_elts() {return nb;} //!< \deprecated Use MAPSHashTable#Size() const instead.
	float efficiency() {return Efficiency();} //!< \deprecated Use MAPSHashTable#Efficiency() const instead.
	TContent& insert(const TKey& key, const TContent& content) {return Insert(key,content);} //!< \deprecated Use MAPSHashTable#Insert(const TKey&,const TContent&) instead.
	TContent& insert(const TKey& key) {return Insert(key);} //!< \deprecated Use MAPSHashTable#Insert(const TKey&) instead.
	TContent* lookup(const TKey& key) const {return Lookup(key);} //!< \deprecated Use MAPSHashTable#Lookup(const TKey&) const instead.
	TContent* find(const TKey& key) const {return Lookup(key);} //!< \deprecated Use MAPSHashTable#Lookup(const TKey&) const instead.
	TContent remove(const TKey& key) {return Remove(key);} //!< \deprecated Use MAPSHashTable#Remove(const TKey&) instead.
	TContent remove(MAPSHashTableIterator& it) {return Remove(it);} //!< \deprecated Use MAPSHashTable#Remove(MAPSHashTableIterator&) instead.
	MAPSHashTableIterator iterator() const {return Iterator();} //!< \deprecated Use MAPSHashTable#Iterator() const instead.
	MAPSHashTableIterator new_iterator() const {return Iterator();} //!< \deprecated Use MAPSHashTable#Iterator() const instead.
	TKey& key(MAPSHashTableIterator& it) const {return Key(it);} //!< \deprecated Use MAPSHashTable#Key(const MAPSHashTableIterator&) const instead.
	TContent& content(MAPSHashTableIterator& it) const {return Content(it);} //!< \deprecated Use MAPSHashTable#Content(const MAPSHashTableIterator&) const instead.
	MAPSPair<TKey,TContent>& pair(MAPSHashTableIterator& it) const {return Pair(it);} //!< \deprecated Use MAPSHashTable#Pair(const MAPSHashTableIterator&) const instead.
	MAPSHashTableIterator& next(MAPSHashTableIterator& it) const {return Next(it);} //!< \deprecated Use MAPSHashTable#Next(MAPSHashTableIterator&) const instead.
	void resize(int new_size) {Resize(new_size);} //!< \deprecated Use MAPSHashTable#Resize(int) instead.
	bool adjust() {return Adjust();} //!< \deprecated Use MAPSHashTable#Adjust() instead.
	void clear() {Clear();} //!< \deprecated Use MAPSHashTable#Clear() instead.
	void* end() const {return NULL;} //!< \deprecated Use MAPSHashTable#End() const instead.
	/*@}*/
#endif
    };

// MAPSHashTable inline implementations
template<typename TKey, typename TContent, typename H> 
    inline /*virtual*/ void*
    MAPSHashTable<TKey,TContent,H>::nextX(MAPSHashTableIterator& it) const
    {
	MAPSArray< MAPSList< MAPSPair<TKey,TContent> > > &buckets=*hash_array;
	if (it.current_bucket==-1) {
	    it.current_bucket=0;
	    it.current_iterator=buckets[0].Iterator();
	} else {
	    buckets[it.current_bucket].Next(it.current_iterator);
	}
	while (it.current_iterator==NULL) {
	    it.current_bucket++;
	    if (it.current_bucket==buckets.Size()) {
		return NULL;
	    }
	    it.current_iterator=buckets[it.current_bucket].Iterator();
	}
	MAPSPair<TKey,TContent> *res=&buckets[it.current_bucket][it.current_iterator];
	return res;
    }

template<typename TKey, typename TContent, typename H> 
    inline TContent&
    MAPSHashTable<TKey,TContent,H>::Insert(const TKey& key)
    {
	unsigned int hash_value=this->_hash((TKey&)key);
	MAPSList< MAPSPair<TKey,TContent> >& bucketList = (*hash_array)[(int)(hash_value&hashmask(s))];
	bucketList.PushBack();
	MAPSPair<TKey,TContent>& pair = bucketList[bucketList.Last()];
	pair.Key()=key;
	pair.HashValue()=hash_value;
	++nb;
	if (auto_adjust)
	    Adjust();
	return pair.Content();
    }

template<typename TKey, typename TContent, typename H> 
    inline TContent*
    MAPSHashTable<TKey,TContent,H>::Lookup(const TKey& key) const 
    {
	MAPSArray< MAPSList< MAPSPair<TKey,TContent> > > &buckets=*hash_array;
	unsigned int hash_value=this->_hash(key)&hashmask(s);
	MAPSListIterator it;
	MAPSList< MAPSPair<TKey,TContent> >& list=buckets[(int)hash_value];
	MAPSForallItems(it,list) {
	    if (list[it].Key()==key)
		break;
	}
	if (!it)
	    return NULL;
	return &list[it].Content();
    }

template<typename TKey, typename TContent, typename H> 
    inline TContent
    MAPSHashTable<TKey,TContent,H>::Remove(const TKey& key)
    {
	TContent c;
	MAPSListIterator it;
	MAPSList< MAPSPair<TKey,TContent> >& li=(*hash_array)[(int)(this->_hash((TKey&)key)&hashmask(s))];
	MAPSForallItems(it,li) {
	    if (li[it].Key()==key) {
		c=li[it].Content();
		li-=it;
		--nb;
		break;
	    }
	}
	return c;
    }

template<typename TKey, typename TContent, typename H> 
    inline MAPSHashTable<TKey,TContent,H>&
    MAPSHashTable<TKey,TContent,H>::operator-=(const TKey& key)
    {
	MAPSListIterator it;
	MAPSList< MAPSPair<TKey,TContent> >& li=(*hash_array)[(int)(this->_hash((TKey&)key)&hashmask(s))];
	MAPSForallItems(it,li) {
	    if (li[it].Key()==key) {
		li-=it;
		--nb;
		break;
	    }
	}
	return *this;
    }

template<typename TKey, typename TContent, typename H> 
    inline void
    MAPSHashTable<TKey,TContent,H>::Resize(int size)
    {
	int m=size-1,p=1;
	int new_size=0;
	while (m) {
	    p<<=1;
	    ++new_size;
	    m>>=1;
	}
	MAPSArray< MAPSList< MAPSPair<TKey,TContent> > > *new_hash=new MAPSArray< MAPSList< MAPSPair<TKey,TContent> > >(hashsize(new_size));
	MAPSHashTableIterator it(*this);
	MAPSHashTableIterator itx;
	while (it) {
	    itx=it;
	    ++it;
	    (*hash_array)[itx.current_bucket].removeX(itx.current_iterator);
	    (*new_hash)[(int)(((MAPSPair<TKey,TContent> *)itx.content)->HashValue()&hashmask(new_size))].appendX(itx.current_iterator);
	}
	delete hash_array;
	hash_array=new_hash;
	s=new_size;
    }

template<typename TKey, typename TContent, typename H> 
    inline void
    MAPSHashTable<TKey,TContent,H>::Clear(int n/*=16*/)
    {
	delete hash_array;
	int m=n-1,p=1;
	s=0;
	while (m) {
	    p<<=1;
	    ++s;
	    m>>=1;
	}
	hash_array=new MAPSArray< MAPSList< MAPSPair<TKey,TContent> > >(p); 
	nb=0;
    }

template<typename TKey, typename TContent, typename H> 
    inline 
    MAPSHashTable<TKey,TContent,H>::MAPSHashTable(int n/*=16*/, bool adjust/*=true*/)
    {
	int m=n-1,p=1;
	s=0;
	while (m) {
	    p<<=1;
	    ++s;
	    m>>=1;
	}
	hash_array=new MAPSArray< MAPSList< MAPSPair<TKey,TContent> > >(p); 
	auto_adjust=adjust;
	nb=0;
    }

template<typename TKey, typename TContent, typename H> 
	inline 
	MAPSHashTable<TKey,TContent,H>::MAPSHashTable(const MAPSHashTable& src)
	: auto_adjust(src.auto_adjust), nb(src.nb), s(src.s), _hash(src._hash)
	{
		this->hash_array = new MAPSArray< MAPSList< MAPSPair<TKey,TContent> > >(*src.hash_array);
	}

template<typename TKey, typename TContent, typename H> 
	inline MAPSHashTable<TKey,TContent,H>&
	MAPSHashTable<TKey,TContent,H>::operator=(const MAPSHashTable& src)
	{
		_hash=src._hash;
		auto_adjust=src.auto_adjust;
		nb=src.nb;
		s=src.s;
		this->hash_array = new MAPSArray< MAPSList< MAPSPair<TKey,TContent> > >(*src.hash_array);
		return *this;
	}

template<typename TKey>
    struct MAPSHashFunction
    {
	virtual unsigned int
	operator()(const TKey& key) const =0;
	virtual
	~MAPSHashFunction() {}
    };

struct MAPSStringHashFunction : public MAPSHashFunction<MAPSString>
{
private:
#ifdef MAPS_64
    static void
    mix64(MAPSUInt64 &a,
	  MAPSUInt64 &b,
	  MAPSUInt64 &c);

    static MAPSUInt64
    hash(MAPSUInt8* k,
	 MAPSUInt64 length,
	 MAPSUInt64 level);
#else
    static void
    mix(MAPSUInt32 &a,
	MAPSUInt32 &b,
	MAPSUInt32 &c);

    static MAPSUInt32
    hash(MAPSUInt8 *k,
	 MAPSUInt32 length,
	 MAPSUInt32 initval);
#endif

public:
    unsigned int 
    operator()(const MAPSString& s) const
    { return (unsigned int)hash((MAPSUInt8*)(const char*)s,s.Length(),0xdead); }
};

//! The RTMaps string hash table class
/*! As its name says, the key is a string.
 * Most of the code of hashing function for strings is inspired of Bob Jenkins' work (bob_jenkins@burtleburtle.net).
 *  See http://burtleburtle.net/bob/hash/evahash.html
 * \ingroup bds
 */
template <typename TContent> 
    class MAPSStringHashTable : public MAPSHashTable<MAPSString, TContent, MAPSStringHashFunction>
    {
    public:
	//! Constructor
	explicit
	MAPSStringHashTable(int n=16)
	: MAPSHashTable<MAPSString, TContent, MAPSStringHashFunction>(n)
	{ }

	virtual ~MAPSStringHashTable() {}
    };

struct MAPSIntegerHashFunction : public MAPSHashFunction<int>
{
    unsigned int
    operator()(const int& i) const
    { return (unsigned int)i; }
};

//! The RTMaps integer hash table template class
/*! As its name says, the key is an integer.
 * \ingroup bds
 */
template <typename TContent> 
    class MAPSIntegerHashTable : public MAPSHashTable<int, TContent, MAPSIntegerHashFunction>
    {
    public:
	//! Constructor
	explicit
	MAPSIntegerHashTable(int n=16)
	: MAPSHashTable<int, TContent, MAPSIntegerHashFunction>(n)
	{ }

	virtual ~MAPSIntegerHashTable() {}
    };

struct MAPSPtrHashFunction : public MAPSHashFunction<void*>
{
    unsigned int
    operator()(void* const& ptr) const
    {
#ifdef MAPS_64
	return (unsigned int)((MAPSUInt64)ptr>>8);
#else
	return (unsigned int)((MAPSUInt32)ptr>>4);
#endif
    }
};

//! The RTMaps pointer hash table template class
/*! As its name says, the key is a pointer
 * \ingroup bds
 */
template <typename TContent> 
    class MAPSPtrHashTable : public MAPSHashTable<void*, TContent, MAPSPtrHashFunction>
    {
    public:
	//! Constructor
	explicit
	MAPSPtrHashTable(int n=16)
	: MAPSHashTable<void*, TContent, MAPSPtrHashFunction>(n)
	{ }

	virtual ~MAPSPtrHashTable() {}
    };

//! Standard operator.
/*! \relates MAPSCouple */
template<typename T>
    MAPSStreamedString&
    operator<<(MAPSStreamedString& out,
	       MAPSCouple<T>& c)
    {
	out<<"("<<c.first()<<","<<c.second()<<")";
	return out;
    }

//! Standard operator.
/*! \relates MAPSList */
template <typename T>
    MAPSStreamedString&
    operator<<(MAPSStreamedString& out,
	       MAPSList<T>& L)
    {
	out<<"(";
	typename MAPSList<T>::MAPSIterator it(L);
	while(it.HasNext()) {
	    out << *it << ",";
	    ++it;
	}
	if(!it.End())
	    out << *it;
	out<<")";
	return out;
    }

//! Standard operator
/*! \relates MAPSArray */
template<typename T>
    MAPSStreamedString&
    operator<<(MAPSStreamedString& out,
	       const MAPSArray<T>& v)
    {
	out<<"[";
	if (v.Size()) {
	    int i;
	    for (i=0;i<v.Size()-1;++i)
		out<<v[i]<<" ";
	    out<<v[i];
	}
	out<<"]";
	return out;
    }

//! Standard operator
/*! \relates MAPSMatrix2 */
template<typename T>
    MAPSStreamedString&
    operator<<(MAPSStreamedString& out,
	       const MAPSMatrix2<T>& v)
    {
	int i,j;
	out<<"[";
	if (v.NbColumns()&&v.NbRows()) {
	    for (i=0;i<v.NbRows()-1;++i) {
		int j;
		for (j=0;j<v.NbColumns()-1;++j) {
		    out<<v[i][j]<<", ";
		}
		out<<v[i][j]<<";\n";
	    }
	    for (j=0;j<v.NbColumns()-1;++j) {
		out<<v[i][j]<<", ";
	    }
	    out<<v[i][j];
	}
	out<<"]";
	return out;
    }

//! Standard operator
/*! \relates MAPSStackedString */
template<int BUFFER_SIZE>
    MAPSStreamedString&
    operator<<(MAPSStreamedString& out,
	       const MAPSStackedString<BUFFER_SIZE>& str)
    {
	for(int i=0;i<str.Length();++i)
	    if(str[i]) out << str[i];
	    else break;
	return out;
    }

// STL output compatibility
#ifdef USE_STL_OUTPUT
#ifdef MAPS_STD_CPP
#include <iostream>
#else
#include <ostream.h>
#endif
/*! \addtogroup bds
 * @{
 */
//! Standard operator.
/*! \relates MAPSCouple */
template<typename Ch, typename Tr, typename T>
    std::basic_ostream<Ch,Tr>&
    operator<<(std::basic_ostream<Ch,Tr>& out,
	       MAPSCouple<T>& c)
    {
	out<<"("<<c.first()<<","<<c.second()<<")";
	return out;
    }

//! Standard operator.
/*! \relates MAPSList */
template <typename Ch, typename Tr, typename T>
    std::basic_ostream<Ch,Tr>&
    operator<<(std::basic_ostream<Ch,Tr>& out,
	       MAPSList<T>& L)
    {
	out<<"(";
	MAPSList<T>::MAPSIterator it(L);
	while(it.HasNext()) {
	    out << *it << ",";
	    ++it;
	}
	if(!it.End())
	    out << *it;
	out<<")";
	return out;
    }

//! Standard operator
/*! \relates MAPSArray */
template<typename Ch, typename Tr, typename T>
    std::basic_ostream<Ch,Tr>&
    operator<<(std::basic_ostream<Ch,Tr>& out,
	       const MAPSArray<T>& v)
    {
	out<<"[";
	if (v.Size()) {
	    int i;
	    for (i=0;i<v.Size()-1;++i)
		out<<v[i]<<" ";
	    out<<v[i];
	}
	out<<"]";
	return out;
    }

//! Standard operator
/*! \relates MAPSMatrix2 */
template<typename Ch, typename Tr, typename T>
    std::basic_ostream<Ch,Tr>&
    operator<<(std::basic_ostream<Ch,Tr>& out,
	       const MAPSMatrix2<T>& v)
    {
	int i,j;
	stream<<"[";
	if (v.NbColumns()&&v.bRows()) {
	    for (i=0;i<v.NbRows()-1;++i) {
		int j;
		for (j=0;j<v.NbColumns()-1;++j) {
		    stream<<v[i][j]<<", ";
		}
		stream<<v[i][j]<<";\n";
	    }
	    for (j=0;j<v.NbColumns()-1;++j) {
		stream<<v[i][j]<<", ";
	    }
	    stream<<v[i][j];
	}
	stream<<"]";
	return stream;
    }

//! Standard operator
/*! \relates MAPSStackedString */
template<typename Ch, typename Tr, int BUFFER_SIZE>
    std::basic_ostream<Ch,Tr>&
    operator<<(std::basic_ostream<Ch,Tr>& out,
	       const MAPSStackedString<BUFFER_SIZE>& str)
    {
	for(int i=0;i<str.Length();++i)
	    if(str[i]) out << str[i];
	    else break;
	return out;
    }

/*@}*/
#endif //  USE_STL_OUTPUT

#endif
 

typedef MAPSListIterator MAPSFastIOHandle;

class MAPSThread
{
public:
    virtual bool
    operator==(const MAPSThreadId& id)=0;
    virtual int
    GetContextSwitches() {return 0;}
    virtual int
    GetCPULoad() {return 0;}
    virtual void
    SetProcessor(int processor) {}
    virtual void
    SetPriority(int priority) {}
    virtual
    ~MAPSThread() {}
};

// OS specific includes
#if defined(_WIN32)
//  // RTMaps for Windows NT/2000/XP
// 

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef _WIN32_WINNT
#define MAPS_NO_TRYLOCK
#endif
#ifndef STRICT
#define STRICT
#endif
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0500
#endif

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <mmsystem.h>

//! The RTMaps mutex class.
/*! A mutex blocks concurrent threads if they both want
 *	to access shared data at the same time.
 */
class MAPSMutex
{
private:
    // Use of critical section is faster
    CRITICAL_SECTION m_cs;
public:
    //! Standard constructor
    MAPSMutex();
    //! Standard destructor
    virtual
    ~MAPSMutex();
    //! Locks the mutex
    /*! Any other thread will block until this thread calls MAPSMutex::Release
     */
    void
    Lock() { EnterCriticalSection(&m_cs); }
    //! Releases the mutex
    /*! Any other thread blocked on this monitor will be able to lock it.
     */
    void
    Release() { LeaveCriticalSection(&m_cs); }
    //! Tries to lock the mutex
#ifndef MAPS_NO_TRYLOCK
    bool
    TryLock() { return (TryEnterCriticalSection(&m_cs))?true:false; }
#endif
    //! Resets the mutex
    /*! Reset is very important to avoid deadlocking :
     *	when a mutex is possibly still owned by a dead thread, then
     *  a call to reset in MAPSComponent::Death() will solve the problem.
     *	Not calling Reset() in that case will probably deadlock
     *	the component during its next start, on the first call to Lock().
     */
    void
    Reset() { DeleteCriticalSection(&m_cs); InitializeCriticalSection(&m_cs); }
};

//! The RTMaps event class.
/*! An event is simply a 2-state variable, with wait and delayed set capabilities.
 *
 *	This is one of the most important classes in RTMaps. It is extensively used by the
 *	engine to rule all threads switching.
 */
class MAPSEvent
{
    friend class MAPSWin32;
    HANDLE m_handle;
    MAPSString m_name;
    bool m_owner;
    bool m_isInTimerList;
public:
    //! Sets the event.
    void
    Set() {SetEvent(m_handle);}
    //! Resets the event.
    void
    Reset() {ResetEvent(m_handle);}
    //! Waits for the event to be set by another thread.
    /*! The thread that called Wait will block until the event is set.
     *	\warning This is a very dangerous function since if it is badly used, it may
     *	block infinitely your RTMaps system. If you develop some components, please
     *	use MAPSComponent::Wait4Event. It is a safe 
     *	function that will never dead lock your RTMaps system.
     */
    int
    Wait(MAPSDelay timeout=-1/*MAPS::Infinite*/);	
    //! Waits for one of a set of events to be set by another thread.
    /*! The thread that called Wait will block until one of the events are set.
     *	\warning This is a very dangerous function since if it is badly used, it may
     *	block infinitely your RTMaps system. If you develop some components, please
     *	use MAPSComponent::Wait4Events. It is a safe 
     *	function that will never dead lock your RTMaps system.
     */
    static int
    Wait(int nCount,
	 MAPSEvent* events[],
	 MAPSDelay timeout=-1/*MAPS::Infinite*/);
    //! Waits for one event to be set by another thread OR one message to arrive.
    /*!	This function exists only for message based operating systems, like Microsoft Windows.
     *	It allows to process some messages while processing some MAPSEvents. This is very
     *	useful to make some single-threaded Windows graphical components.
     */
    static int
    MsgWait(int nCount,
	    MAPSEvent* events[],
	    MAPSDelay timeout=-1/*MAPS::Infinite*/);
    //! Tells if there is a message in the queue for this thread.
    /*! This function has a meaning only for message based operating systems, like Microsoft Windows.
     */
    static bool
    MsgInQueue() { if (GetQueueStatus(QS_ALLINPUT)) return true; return false;} 
    //! Returns the current state of this event
    bool
    State() {if (WaitForSingleObject(m_handle,0)==WAIT_TIMEOUT) return false; return true;}
    //! Sets a trigger on this event.
    /*! This function is very important. It automatically sets the event after the expiry of a given delay.
     *	This means that is any thread is waiting on this event, the latter thread will be unblocked after
     *	a finite amount of time. This enables the scheduling of threads in RTMaps.
     *  \warning This functions uses multimedia timers. Under windows NT, 2000, XP, the number of multimedia
     *  timers is limited to 16. If the function fails, the function returns \c false.
     */
    bool
    SetTrigger(MAPSDelay delay);
    //! Resets a trigger;
    /*! This will cancel any previously set trigger.
     */
    void
    ResetTrigger();// { if (m_timerId) timeKillEvent(m_timerId); }
    //! Returns the Windows event handle corresponding to this MAPSEvent (Windows only function)
    /*! This must be used only for low level RTMaps programming, since this function is OS-specific.
     */
    HANDLE
    GetHandle() {return m_handle;} // Windows specific for windows only modules
    //! The MAPSEvent constructor.
    /*!	\param initialState Initial state for the event. Default is \c false.
     *	\param n Optionnal name to give to the event.
     */
    MAPSEvent(bool initialState=false,
	      const char* n=NULL);
    //! Windows specific constructor
    /*!	Embeds a Windows event in a MAPSEvent. Useful if you want to combine Windows provided event
     *	with RTMaps provided events and use the RTMaps MAPSEvent functions to control them.
     *
     *	This a very low-level function, OS-specific, that must be used with care.
     */
    MAPSEvent(HANDLE h);
    //! The MAPSEvent destructor
    virtual ~MAPSEvent();
};

class MAPSFileHandle
{
    friend class MAPSFileIO;
    HANDLE m_handle;
    __int64 m_filePos;
    int m_options;
    DWORD m_bytesPerSector;
public:
    virtual ~MAPSFileHandle() {}
protected:
    MAPSFlowVolume* m_flowVolume;
    MAPSFileHandle() : m_filePos(0) {}
};

struct MAPSWin32TimerEvent
{
    MAPSEvent* m_pEvent;
    MAPSTimestamp m_when;
};

//! The class that is used to manage all RTMaps write file operations
class MAPSFileWriteHandle: public MAPSFileHandle
{
    friend class MAPSFileIO;
    int m_current;
    int m_theOldest;
    static const int s_nbMaxOfSimultaneousWriteOperations;	
    int m_nbOfPendingOperations;
    int* m_statistics;
    bool* m_pending;
    void** m_elts2Unlock;
    OVERLAPPED* m_overlapped;

    MAPSFileWriteHandle():m_nbOfPendingOperations(0),m_theOldest(0),m_current(0) {}
    //Virtual destructor to workaround VC6 bug.
    virtual ~MAPSFileWriteHandle() {}
};

//! The class that is used to manage all RTMaps read file operations
class MAPSFileReadHandle: public MAPSFileHandle
{
    friend class MAPSFileIO;
};

class MAPSWin32;
extern MAPSWin32* _MAPSWin32;

// Win32-related useful functions
//! The RTMaps Win32 support class
class MAPSWin32
{
    friend class MAPSOS;
    friend class MAPSSystemClock;
    friend class MAPSEvent;
    
    MAPSMutex m_mutex;
    int m_versionNumber;
    LARGE_INTEGER m_frequency;
    LARGE_INTEGER m_reference;
    int m_coeff;
    DWORD m_mainThreadId;
    
    bool m_isWindowsNT;
    bool m_usePerformanceCounter;

    MMRESULT m_timerId;
    volatile MAPS3States m_timerThreadState;
    MAPSMutex m_timerMutex;
    MAPSEvent m_abortTimerEvent;
    MAPSList<MAPSWin32TimerEvent> m_timerEvents;
    static void* TimerThread(void* param);
    static void CALLBACK TimerCallback(UINT wTimerID, UINT msg, DWORD user, DWORD dw1, DWORD dw2);
    
    void
    InsertTimerEvent(MAPSEvent* event,
		     MAPSTimestamp when);
    void
    RemoveTimerEvent(MAPSEvent* event);
    void
    RescheduleTimer();

    void Run();
    void Shutdown();


    MAPSWin32();
    //Virtual Destructor to workaround VC6 bug.
    virtual ~MAPSWin32() {}
    
public:
    //! \name Useful functions
    //@{
    //! Returns \c true if the OS is Windows 2000, XP or Server 2003, \c false otherwise.
    static bool
    OSIsWindowsNT() {return _MAPSWin32->m_isWindowsNT;}
    //! Returns 5 for Windows 2000, XP or Server 2003, 4 for Windows NT4.
    static int
    OSVersion() {return _MAPSWin32->m_versionNumber;}
    //! Translate MAPSAbsoluteTime structure to Windows SYSTEMTIME structure.
    static void
    AbsoluteTimeToSystemTime(MAPSAbsoluteTime& s,
			     SYSTEMTIME& t);
    //! Translate Windows SYSTEMTIME structure to MAPSAbsoluteTime structure.
    static void
    SystemTimeToAbsoluteTime(SYSTEMTIME& t,
			     MAPSAbsoluteTime& s);
    //! Returns the main thread Id (the one that processes WM_QUIT messages generally)
    static DWORD
    MainThreadId() {return _MAPSWin32->m_mainThreadId;}
    //@}
    
    static void
    SetKernel(MAPSKernelInterface* kernel);
};

// 
 
#elif defined(_LINUX) || defined(_QNX)
//  // Posix common header
#include <sys/time.h>

#include <dirent.h>
#include <pthread.h>

//! The RTMaps mutex class.
/*! A mutex blocks concurrent threads if they both want
 *	to access shared data at the same time.
 */
class MAPSMutex
{
    pthread_mutex_t m_mutex;
public:
    //! Standard constructor. No options.
    MAPSMutex(); // Initializes a fast mutex
    //! Standard destructor.
    virtual
    ~MAPSMutex();
    //! Locks the mutex.
    /*! Any other thread will block until this thread calls MAPSMutex::Release
     */
    void
    Lock();
    //! Tries to lock the mutex
    bool
    TryLock();
    //! Releases the mutex.
    /*! Any other thread blocked on this monitor will be able to lock it.
     */
    void
    Release();
    //! Resets the mutex
    /*! Reset is very important to avoid deadlocking :
     *	when a mutex is possibly still owned by a dead thread, then
     *  a call to reset in MAPSComponent::Death() will solve the problem.
     *	Not calling Reset() in that case will probably deadlock
     *	the component during its next start, on the first call to Lock().
     */
    void
    Reset();
};

class MAPSPosixWaitingThread;

//! The RTMaps event class.
/*! An event is simply a 2-state variable, with wait and delayed set capabilities.
 *
 *	This is one of the most important classes in RTMaps. It is extensively used by the
 *	engine to rule all threads switching.
 */
class MAPSEvent
{
    pthread_mutex_t m_mutex;
    pthread_cond_t m_condvar;
    volatile bool m_state;
    MAPSArray<MAPSPosixWaitingThread*> m_waitingThreads;
public:
    //! Sets the event.
    void
    Set();
    //! Resets the event.
    void
    Reset();
    //! Waits for the event to be set by another thread.
    /*! The thread that called Wait will block until the event is set.
     *	\warning This is a very dangerous function since if it is badly used, it may
     *	block infinitely your RTMaps system. If you develop some components, please
     *	use MAPSComponent::Wait4Event. It is a safe 
     *	function that will never dead lock your RTMaps system.
     */
    int
    Wait(MAPSDelay timeout=-1/*MAPS::Infinite*/);	
    //! Waits for one of a set of events to be set by another thread.
    /*! The thread that called Wait will block until one of the events are set.
     *	\warning This is a very dangerous function since if it is badly used, it may
     *	block infinitely your RTMaps system. If you develop some components, please
     *	use MAPSComponent::Wait4Events. It is a safe 
     *	function that will never dead lock your RTMaps system.
     */
    static int
    Wait(int nCount,
	 MAPSEvent* events[],
	 MAPSDelay timeout=-1/*MAPS::Infinite*/);
    //! Waits for one event to be set by another thread OR one message to arrive.
    /*!	This function exists only for message based operating systems, like Microsoft Windows.
     */
    static int
    MsgWait(int nCount,
	    MAPSEvent *events[],
	    MAPSDelay timeout=-1/*MAPS::Infinite*/);
    //! Tells if there is a message in the queue for this thread.
    /*! This function has a meaning only for message based operating systems, like Microsoft Windows.
     */
    static bool
    MsgInQueue();
    //! Returns the current state of this event
    bool
    State() {return m_state;}
    //! Sets a trigger on this event.
    /*! This function is very important. It automatically sets the event after the expiry of a given delay.
     *	This means that if any thread is waiting on this event, the latter thread will be unblocked after
     *	a finite amount of time. This enables the scheduling of threads in RTMaps.
     */
    bool
    SetTrigger(MAPSDelay delay);
    //! Resets a trigger
    /*! This will cancel any previously set trigger.
     */
    void
    ResetTrigger();
    //! The MAPSEvent constructor.
    /*!	\param initialState Initial state for the event. Default is \c false.
     *	\param n Optional name to give to the event.
     */
    MAPSEvent(bool initialState=false,
	      const char* n=NULL);
    //! The MAPSEvent destructor
    ~MAPSEvent();
};

// Posix file I/O implementation
//! The base class for all RTMaps file I/O operations
class MAPSFileHandle
{
    friend class MAPSFileIO;
protected:
    FILE* m_handle;
    MAPSFlowVolume* m_flowVolume;
};

//! The class that is used to manage all RTMaps write file operations
class MAPSFileWriteHandle: public MAPSFileHandle
{
    friend class MAPSFileIO;
};

//! The class that is used to manage all RTMaps read file operations
class MAPSFileReadHandle: public MAPSFileHandle
{
    friend class MAPSFileIO;
};

struct MAPSPosixTimerEvent
{
    MAPSEvent* m_event;
    struct timeval m_when;
};

// 
 
#else
#error "OS not supported"
#endif

#ifndef TEXT
#define TEXT
#endif

// Other includes
//  // IPL structures
#if (!defined(__IPL_H__)) && (!defined(_CXCORE_TYPES_H_))
#ifndef __MAPSIPL_H
#define __MAPSIPL_H

#ifndef MIN
#define MIN(a,b) ((a)<(b)?(a):(b))
#endif

#ifndef MAX
#define MAX(a,b) ((a)>(b)?(a):(b))
#endif

//! The IPL Region Of Interest structure
typedef struct _IplROI {
	//! The channel of interest number
	/*! This parameter indicates which channel in the original image will
	 *  be affected by processing taking place in the region of interest;
	 *  \a coi equal to 0 indicates that all channel will be affected
	 */
	MAPSInt32			coi;
	//! The horizontal offset from the origin of the rectangular ROI.
	MAPSInt32			xOffset;
	//! The vertical offset from the origin of the rectangular ROI.
	MAPSInt32			yOffset;
	//! The width of the rectangular ROI
	MAPSInt32			width;
	//! The height of the rectangular ROI
	MAPSInt32			height;
} IplROI;

//! The famous IplImage structure, today's standard structure for image processing.
/*!	The IplImage structure is the preferred structure for image processing with RTMaps.
 * 
 * The IplImage structure is rather complex but very complete.
 *  Easy handling of this structure by RTMaps is provided by MAPS::IplImageModel and MAPSOutput::AllocOutputBufferIplImage.
 */
typedef struct _IplImage {
    //! Size of iplImage struct
    MAPSInt32			nSize;
    //! Image header version
    MAPSInt32			ID;
    //! Number of channels in the image (generally 1,3 or 4)
    MAPSInt32			nChannels;
    //! Alpha channel number (0 if there is no alpha channel in the image)
    MAPSInt32			alphaChannel;
    //! Bit depth of pixels.
    /*! Can be one of:
     *  - \c IPL_DEPTH_1U
     *  - \c IPL_DEPTH_8U
     *  - \c IPL_DEPTH_8S
     *  - \c IPL_DEPTH_16U
     *  - \c IPL_DEPTH_16S
     *  - \c IPL_DEPTH_32S
     *  - \c IPL_DEPTH_32F.
     */
    MAPSInt32			depth;
    //! A four-character string describing the color model. 
    /*! Use \c MAPS_COLORMODEL_xxx to fill this field
     */
    char		colorModel[4];
    //! A four-character string describing the sequence of color channels.
    /*! Use \c MAPS_CHANNELSEQ_xxx to fill this field.
     */
    char		channelSeq[4];
    //! Can be \c IPL_DATA_ORDER_PIXEL or \c IPL_DATA_ORDER_PLANE
    MAPSInt32			dataOrder;
    //! The origin of the image: can be \c IPL_ORIGIN_TL (top left corner) or \c IPL_ORIGIN_BL (bottom left corner)
    MAPSInt32			origin;
    //! Alignment of image data: can be \c IPL_ALIGN_DWORD (4-byte align) or \c IPL_ALIGN_QWORD (8-byte align)
    MAPSInt32			align;
    //! Width of the image in pixels
    MAPSInt32			width;
    //! Height of the image in pixels
    MAPSInt32			height;
    //! Pointer to a ROI (Region of interest) structure.
    /*! This argument can be \c NULL, which implies that a region of interest
     *  comprises all channels and the entire image area.
     */
	IplROI 	*roi;
    //! Pointer to the header of another image that specifies the mask ROI.
    /*! This argument can be \c NULL, which indicates that no mask ROI is used.
     *  A pixel is processed if the corresponding mask pixel is 1, and is not
     *	processed if the mask pixel is 0. The \a maskROI field of the mask image's
     *  header is ignored.
     */
    struct _IplImage	*maskROI;
    //! The image ID
    /*! Field reserved for the use of the application to identify the image.
     */
    void		*imageId;
    //! The pointer to the IplTileInfo structure containing information used for image tiling.
    struct  _IplTileInfo *tileInfo;
    //! Useful size in bytes
    MAPSInt32			imageSize;
    //! Pointer to aligned image
    char		*imageData;
    //! The size of aligned line in bytes
    MAPSInt32			widthStep;
    //! The top, bottom, left and right border mode
    MAPSInt32			BorderMode[4];
    //! Constants for the top, bottom, left and right border
    MAPSInt32			BorderConst[4];
    //! Pointer to full, nonaligned image
    char		*imageDataOrigin;
} IplImage;

/*==========================================================================
       Section: IPLibrary Definitions
 ==========================================================================*/

#define IPL_DEPTH_SIGN 0x80000000               
#define IPL_DEPTH_MASK 0x7FFFFFFF               

#define IPL_DEPTH_1U     1
#define IPL_DEPTH_8U     8
#define IPL_DEPTH_16U   16
#define IPL_DEPTH_32F   32

#define IPL_DEPTH_8S  (IPL_DEPTH_SIGN| 8)
#define IPL_DEPTH_16S (IPL_DEPTH_SIGN|16)
#define IPL_DEPTH_32S (IPL_DEPTH_SIGN|32)

#define IPL_DATA_ORDER_PIXEL  0
#define IPL_DATA_ORDER_PLANE  1

#define IPL_ORIGIN_TL 0
#define IPL_ORIGIN_BL 1

#define IPL_ALIGN_4BYTES   4
#define IPL_ALIGN_8BYTES   8
#define IPL_ALIGN_16BYTES 16
#define IPL_ALIGN_32BYTES 32

#define IPL_ALIGN_DWORD   IPL_ALIGN_4BYTES
#define IPL_ALIGN_QWORD   IPL_ALIGN_8BYTES

#define IPL_GET_TILE_TO_READ   1
#define IPL_GET_TILE_TO_WRITE  2
#define IPL_RELEASE_TILE       4

#define IPL_LUT_LOOKUP 0
#define IPL_LUT_INTER  1

/*==========================================================================
      Section: Image Creation Functions
 ==========================================================================*/

#define IPL_BORDER_CONSTANT   0
#define IPL_BORDER_REPLICATE  1
#define IPL_BORDER_REFLECT    2
#define IPL_BORDER_WRAP       3

/*---  Indexes to access IplImage.BorderMode[],IplImage.BorderConst[]  ----*/
#define IPL_SIDE_TOP_INDEX    0
#define IPL_SIDE_BOTTOM_INDEX 1
#define IPL_SIDE_LEFT_INDEX   2
#define IPL_SIDE_RIGHT_INDEX  3

/*----------  values of argument of iplSetBorderMode(,,border,)  ----------*/
#define IPL_SIDE_TOP        (1<<IPL_SIDE_TOP_INDEX)
#define IPL_SIDE_BOTTOM     (1<<IPL_SIDE_BOTTOM_INDEX)
#define IPL_SIDE_LEFT       (1<<IPL_SIDE_LEFT_INDEX)
#define IPL_SIDE_RIGHT      (1<<IPL_SIDE_RIGHT_INDEX)
#define IPL_SIDE_ALL \
     (IPL_SIDE_RIGHT | IPL_SIDE_TOP | IPL_SIDE_LEFT | IPL_SIDE_BOTTOM)


#endif /*__MAPSIPL_H*/
#endif /*__IPL_H__ || _CXCORE_TYPES_H_*/
 

// RTMaps structures

//! Absolute time structure
/*! This is the RTMaps standard structure for absolute time handling
 */
struct MAPSAbsoluteTime
{
    //! 19XX or 20XX
    MAPSUInt32 year;
    //! Ranging from 1 to 12
    MAPSUInt32 month;
    //! Ranging from 1 to 31
    MAPSUInt32 day;
    //! Ranging from 0 to 23
    MAPSUInt32 hour;
    //! Ranging from 0 to 59
    MAPSUInt32 minutes;
    //! Ranging from 0 to 59
    MAPSUInt32 seconds;
    //! Ranging from 0 to 999
    MAPSUInt32 milliseconds;
    //! Ranging from 0 to 999
    MAPSUInt32 microseconds;
};

//! Standard comparison operator on absolute time.
/*! \relates MAPSAbsoluteTime
 */
bool
operator<(const MAPSAbsoluteTime& t0,
	  const MAPSAbsoluteTime& t1);
//! Standard comparison operator on absolute time.
/*! \relates MAPSAbsoluteTime
 */
bool
operator==(const MAPSAbsoluteTime& t0,
	   const MAPSAbsoluteTime& t1);
//! Standard comparison operator on absolute time.
/*! \relates MAPSAbsoluteTime
 */
bool
operator>(const MAPSAbsoluteTime& t0,
	  const MAPSAbsoluteTime& t1);

// RTMaps output types :

//! CAN Frames structure
/*! This is the RTMaps standard CAN frame type.
 *  It contains all the necessary members to describe completely a frame on a CAN bus
 */
struct MAPSCANFrame
{
    //! The constant to "or" with arbitrationId if EXTENDED identifier is to be used
    static const MAPSUInt32 ExtendedId;
    //! A 0 constant for STANDARD identifiers
    static const MAPSUInt32 StandardId;
    //! The mask to use in order to get the id from arbitrationId member
    static const MAPSUInt32 AddressMask;
    //! The arbitration field for the frame (11 or 29 bits)
    MAPSUInt32 arbitrationId;
    //! Is this a remote frame ?
    bool isRemote;
    //! The number of bytes in the frame, ranging from 1 to 8
    MAPSInt8 dataLength;
    //! The n bytes of data (1<n<8)
    MAPSUInt8 data[8];
};

//! All real objects in RTMaps (vehicles, etc.) inherit from this structure
struct MAPSRealObjectVariant
{
};

//! The MAPSVehicle structure : a kind of MAPSRealObject
/*! \sa MAPSRealObject
 */
struct MAPSVehicle : public MAPSRealObjectVariant
{
    //! The vehicle is a car (\a kind)
    static const int Car;
    //! The vehicle is a bus (\a kind)
    static const int Bus;
    //! The vehicle is a truck (\a kind)
    static const int Truck;
    //! The vehicle is a bike (\a kind)
    static const int Bike;
    //! The vehicle is a motorcycle (\a kind)
    static const int Motorcycle;

    //! The vehicle's heading (in degrees)
    MAPSFloat theta;
    //! The vehicle's speed
    MAPSFloat speed;
    //! The kind of vehicle
    int kind;
    //! The width of the vehicle (in meters)
    MAPSFloat width;
    //! The height of the vehicle (in meters)
    MAPSFloat height;
    //! The length of the vehicle (in meters)
    MAPSFloat length;
    //! Type of car
    int model;
    //! Is the vehicle currently breaking?
    bool braking;

    //! Confidence in the information about this vehicle
    MAPSFloat confidence;
    //! \name The vehicle position accuracy
    //@{
    //! Accuracy of coordinate along x
    MAPSFloat dx;
    //! Accuracy of coordinate along y
    MAPSFloat dy;
    //! Accuracy of coordinate along z
    MAPSFloat dz;
    //@}
};

//! The MAPSTree structure : a kind of MAPSRealObject
/*! \sa MAPSRealObject
 */
struct MAPSTree : public MAPSRealObjectVariant
{
    //! One possible shape
    static const int Cone;
    //! Another possible shape
    static const int Round;
    //! A third possible shape
    static const int Cylinder;

    //! The height of the tree
    MAPSFloat height;
    //! The radius of the tree
    MAPSFloat radius;
    //! The shape of the tree
    int shape;
};

//! The MAPSSign structure : a kind of MAPSRealObject
/*! \sa MAPSRealObject
 */
struct MAPSSign : public MAPSRealObjectVariant
{
    //! Stop sign
    static const int Stop;
    //! Yield sign
    static const int Yield;
    //! One Way sign
    static const int OneWay;
    //! Turn left sign
    static const int TurnLeft;
    //! Turn right sign
    static const int TurnRight;
    //! Speed limit sign
    static const int SpeedLimit;
    //! End of speed limit sign
    static const int EndOfSpeedLimit;
    //! A traffic light
    static const int TrafficLight;
    //! Text only sign
    static const int TextOnly;

    //! The traffic light is red
    static const int Red;
    //! The traffic light is orange
    static const int Orange;
    //! The traffic light is green
    static const int Green;
    //! The orange is blinking
    static const int OrangeBlinking;

    //! The type of the sign
    int type;
    union {
	//! Speed limit information
	int speedLimit;
	//! State of a traffic light
	int trafficLightState;
	//! Text information
	char text[256];
    };
};

//! The standard structure to transmit info about real objects.
/*! The real content of the MAPSRealObject depends on the <CODE>kind</CODE> parameter.
 * 
 *	By convention the coordinates of the object represent the position of the lower point in the middle of the object.
 *	The vehicle's coordinates represent the point on the floor at the middle of the back of the vehicle.
 *	A tree's coordinate is the coordinate of its trunk, at its base on the floor.
 *
 *	This structured type should evolve with the upcoming new RTMaps applications.
 */
struct MAPSRealObject
{
    //! One kind of real object
    static const int Vehicle;
    //! Another kind of real object
    static const int Sign;
    //! Another kind of real object
    static const int Tree;
    
    //! Specifies the kind of real object for this object
    int kind;
    //! Identifier (for instance, obstacle number)
    int id;
    //! The x coordinate of the object
    MAPSFloat x;
    //! The y coordinate of the object
    MAPSFloat y;
    //! The z coordinate of the object
    MAPSFloat z;
    //! The main color of the real object (RGB 24bits, Intel oriented : use MAPS_RGB macro)
    int color;
    //! \name User defined information
    //@{
    //! Miscellaneous 1
    int misc1;
    //! Miscellaneous 2
    int misc2;
    //! Miscellaneous 3
    int misc3;
    //! TO USE WITH CARE! C++ EXPERTS ONLY
    void* userdata;
    //@}
    union {
	//! The real object is a vehicle
	MAPSVehicle vehicle;
	//! The real object is traffic sign
	MAPSSign sign;
	//! The real object is a tree
	MAPSTree tree;
    };
};

//! All drawing objects in RTMaps (circles, etc.) inherit from MAPSDrawingObjectVariant
struct MAPSDrawingObjectVariant
{
};

//! The MAPSLine structure
struct MAPSLine : public MAPSDrawingObjectVariant
{
    //! First point x
    int x1;
    //! First point y
    int y1;
    //! Second point x
    int x2;
    //! Second point y
    int y2;
};

//! The MAPSRectangle structure
struct MAPSRectangle : public MAPSDrawingObjectVariant
{
    //! First point x
    int x1;
    //! First point y
    int y1;
    //! Second point x
    int x2;
    //! Second point y
    int y2;
    int
    Surface() {return (x2-x1+1)*(y2-y1+1);}
};

//! The MAPSCircle structure
struct MAPSCircle : public MAPSDrawingObjectVariant
{
    //! The x-center of the circle
    int x;
    //! The y-center of the circle
    int y;
    //! The radius of the circle
    int radius;
};

//! The MAPSEllipse structure
struct MAPSEllipse : public MAPSDrawingObjectVariant
{
    //! The x-center of the circle
    int x;
    //! The y-center of the circle
    int y;
    //! The horizontal size of the ellipse
    int sx;
    //! The vertical size of the ellipse
    int sy;
};

//! The MAPSText structure
struct MAPSText : public MAPSDrawingObjectVariant
{
    //! The leftmost coordinate
    int x;
    //! The topmost coordinate
    int y;
    //! The width of one character (in pixels)
    int cwidth;
    //! The height of one character (in pixels)
    int cheight;
    //! In degrees, around the upper left corner, counter clockwise.
    int orientation;
    //! Background color (Use the MAPS_RGB macro to set the color).
    int bkcolor;
    //! The text to write
    char text[32];
};

//! The MAPSSpot structure
struct MAPSSpot: public MAPSDrawingObjectVariant
{
    //! This spot is simply a point
    static const int Point;
    //! This spot is a cross
    static const int Cross;
    //! This spot is a small circle
    static const int Circle;
    //! This spot is a point with a circle around
    static const int CircledPoint;
    //! This spot is a cross with a circle around
    static const int CircledCross;
    
    //! The x coordinate of the spot
    int x;
    //! The y coordinate of the spot
    int y;
    //! The kind of spot (MAPSSpot::Cross, etc.)
    int kind;
};

//! The MAPSDrawingObject : a standard for simple overlay shapes
/*! This structure can also be used as a standard to return sensor information.
*/
struct MAPSDrawingObject
{
    //! One kind of drawing object
    static const int Line;
    //! Another kind of drawing object
    static const int Rectangle;
    //! Another kind of drawing object
    static const int Circle;
    //! Another kind of drawing object
    static const int Text;
    //! Another kind of drawing object
    static const int Spot;
    //! Another kind of drawing object
    static const int Ellipse;
    
    //! Specifies the kind of drawing object
    int kind;
    //! Use the MAPS_RGB macro to set the color of this drawing object
    int color;
    //! A width for the drawing
    int width;
    //! An optional id for the element
    int id;
    //! \name User defined information
    //@{
    //! Miscellaneous information 1
    int misc1;
    //! Miscellaneous information 2
    int misc2;
    //! Miscellaneous information 3
    int misc3;
    //! TO USE WITH CARE! C++ EXPERTS ONLY
    void* userdata;
    //@}
    union {
	//! The drawing object is a line
	MAPSLine line;
	//! The drawing object is a rectangle
	MAPSRectangle rectangle;
	//! The drawing object is a circle
	MAPSCircle circle;
	//! The drawing object is some text
	MAPSText text;
	//! The drawing object is a spot
	MAPSSpot spot;
	//! The drawing object is an ellipse
	MAPSEllipse ellipse;
    };
};

//! The RTMaps structure for supporting 3D triangles output
struct MAPSTriangles3D
{
    //! The 3D point structure
    struct Point
    {
	//! x coordinate
	float x;
	//! y coordinate
	float y;
	//! z coordinate
	float z;
	//! Optional. May be useful if you need to make space transformations. Just to keep an index to the reference location.
	int landmark;
	//! Texture x coordinate, for texture mapping
	int xt;
	//! Texture y coordinate, for texture mapping
	int yt;
    };
    //! The triangle structure itself
    struct Triangle
    {
	//! The 1st point constituting the triangle
	int p1;
	//! The 2nd point constituting the triangle
	int p2;
	//! The 3rd point constituting the triangle
	int p3;
	//! The color of the triangle if no texture is provided (use MAPS_RGB or MAPS_RGBA to set the color)
	int color;
    };
    //! The object structure
    struct Object
    {
	//! An ID for the object
	int id;
	//! Texture ID information
	int texture;
	//! The index of the first triangle among all the triangles
	int firstTriangle;
	//! The number of triangles for this object
	int nbOfTriangles;
    };
    //! Number of objects in the scene
    int nbObjects;
    //! All the objects in the scene
    Object* objects;
    //! Number of points in the scene
    int nbPoints;
    //! All the points in the scene
    Point* points;
    //! Number of triangles in the scene
    int nbTriangles;
    //! All the triangles in the scene
    Triangle* triangles;
};

//! Complex number structure
/*! This is the RTMaps standard structure for complex numbers handling
*/
struct MAPSComplex
{
    //! Real part
    MAPSFloat r;
    //! Imaginary part
    MAPSFloat i;
};

//! MATLAB-Like matrix (Complex and columnwise, like in fortran)
/*! This is the RTMaps standard matrix type designed to be used as an output type.
 *	It is a very simple structure designed to contain an array of complex numbers.
 * 
 *	Usually, this structure is used only for inter RTMaps components communication,
 *	and thus is usually allocated through a call to MAPSOutput::AllocOutputBuffer.
 *	As a matter of fact, this structure also has some constructors, so 
 *	it can be used as a general purpose fortran-like matrix as well.
 *	Be aware that a much more powerful class exists for matrices computation in
 *	RTMaps : the MAPSMatrix2 class. We recommend that you use the latter for
 *	matrices computation and convert it to MAPSMatrix to output results from
 *	your components.
 */
struct MAPSMatrix
{
    //! Number of rows in the matrix
    MAPSInt32 m;
    //! Number of columns in the matrix
    MAPSInt32 n;
    //! Real part. Array of double
    MAPSFloat64* pr;
    //! Imaginary part. Array of double
    MAPSFloat64* pi;
    //! Determines if the memory was allocated by the component itself (owner=true), or by RTMaps (owner=false).
    bool owner;

    //! Gives access to the real part of one element of the matrix
    /*! This function allows to read or set the real part of one element of the matrix
     *	\param row the row of the element to get access to
     *  \param column the column of the element to get access to
     *  \return a reference to the real part of the (row, column) element of the matrix
     */
    MAPSFloat64&
    Real(int row,
	 int column)
    { return pr[row+column*m]; }
    //! Gives access to the imaginary part of one element of the matrix
    /*! This function allows to read or set the imaginary part of one element of the matrix
     *  \param row the row of the element to get access to
     *  \param column the column of the element to get access to
     *  \return a reference to the imaginary part of the (row, column) element of the matrix
     */
    MAPSFloat64&
    Im(int row,
       int column)
    { return pi[row+column*m]; }
#ifdef USE_COMPATIBILITY_V2
    //! \name Deprecated members
    //@{
    double& Complex(int row, int column) {return pi[row+column*m];} //!< \deprecated Use MAPSMatrix#Im(int,int) instead
    //@}
#endif
    //! Sets all the elements of a matrix to zero
    void
    Zero() { int x,y; double *p1, *p2; for (p1=pr, p2=pi, y=0; y<m; y++) { for (x=0; x<n; ++x, ++p1, ++p2) { *p1=0; *p2=0; } } } 
    //! Frees the real and imaginary part arrays. Called by the destructor
    void Free();
    //! Allocates the real and imaginary part arrays.
    void Alloc(int mm, int nn);
    //! Copy operator, from a MAPSMatrix2 (easier to use)
    MAPSMatrix&
    operator=(MAPSMatrix2<double>& mat);
    //! Copy operator, from another MAPSMatrix
    MAPSMatrix&
    operator=(MAPSMatrix& mat);
    //! Cast to MAPSMatrix2. Very useful to process MAPSMatrix in input of components.
    /*! Note that this cast only converts the real part of the MAPSMatrix.
     */
    operator MAPSMatrix2<double>() const;
    //! Creates a non allocated MAPSMatrix. Size is set by default to 0.
    MAPSMatrix():m(0),n(0),pr(NULL),pi(NULL),owner(true) {}
    //! Constructor that takes a MAPSMatrix2 in input
    /*! \sa MAPSMatrix2
     */
    MAPSMatrix(MAPSMatrix2<double>& mat):pr(NULL),pi(NULL),owner(true) {*this=mat;}
    //! Copy constructor
    MAPSMatrix(MAPSMatrix& mat):pr(NULL),pi(NULL),owner(true) {*this=mat;}
    //! Creates an allocated MAPSMatrix. Arrays are not initialized. 
    MAPSMatrix(int mm,
	       int nn);
    //! Destructor
    virtual ~MAPSMatrix();
};

inline MAPSMatrix&
MAPSMatrix::operator=(MAPSMatrix2<double>& mat)
{
    if (owner) {
	Free();
	Alloc(mat.M(),mat.N()); 
    } else {
	m=mat.M();
	n=mat.N();
    }
    int x,y;
    double *p1, *p2;
    for (p1=pr, p2=pi, x=0; x<n; x++) {
	for (y=0; y<m; y++, p1++, p2++) { 
	    *p1=mat(y,x);
	    *p2=0;
	}
    } 
    return (*this);
}

inline MAPSMatrix&
MAPSMatrix::operator=(MAPSMatrix& mat)
{
    if (owner) {
	Free(); 
	Alloc(mat.m,mat.n); 
    } else {
	m=mat.m; 
	n=mat.n;
    }
    int x,y;
    double *p1, *p2;
    for (p1=pr, p2=pi, x=0; x<n; ++x) {
	for (y=0; y<m; ++y, ++p1, ++p2) { 
	    *p1=mat.Real(y,x);
	    *p2=mat.Im(y,x);
	}
    } 
    return (*this);
}

inline
MAPSMatrix::operator MAPSMatrix2<double>() const
{
    MAPSMatrix2<double> mat(m,n);
    int x,y;
    double* p1;
    for(p1=pr, x=0; x<n; ++x) {
	for(y=0; y<m; ++y, ++p1) 
	    mat(m,n)=*p1;
    }
    return mat;
}

//! The RTMaps Image type
/*! The IplImage structure must be preferably used when dealing with images,
 *  since these structures can be directly used with the IPL (Image Processing Library)
 *  or the CV lib (Open Source Computer Vision Library).
 *
 *  However, the IplImage structure was not designed to contain some compressed images
 *  or some images coded neither pixel or planar oriented (for instance YUYV images).
 *  To deal with these kind of images, the MAPSImage structure should be used.
 *
 *  The MAPSImage structure is much simpler than the IplImage structure : it retains only the main parameters,
 *  i.e. the width or the height of the image, and the imageSize parameters.
 *  Some new members appeared :
 *
 *		- bufferSize, which contains the size in bytes of the buffer actually allocated
 *		- imageId, that can point to an identifier of the content, for instance a string.
 *		- imageId2, which can supplement imageId
 *		- imageCoding, which can contain four characters specifying the coding used for the image, for instance "JPEG" or "YUYV"
 *		- imageType, which can supplement imageCoding. Generally, it is "COLR" or "MONO"
 *
 *	\sa IplImage
 */
struct MAPSImage
{
    //! The width of the image in pixels
    MAPSInt32 width;
    //! The height of the image in pixels
    MAPSInt32 height;
    //! The number of bytes the stored image actually consumes
    MAPSInt32 imageSize;
    //! The number of bytes allocated in the buffer
    MAPSInt32 bufferSize;
    //! Points to an identifier for the image
    void* imageId;
    //! Supplements imageId
    void* imageId2;
    //! Points to the actual bytes composing the image
    char* imageData;
    //! 4 characters describing the coding used for the image
    char imageCoding[4];
    //! 4 characters that supplement imageCoding (generally "COLR" or "MONO")
    char imageType[4];
};

//! The RTMaps type information structure
/*! A MAPSTypeInfo structure must be provided for each output, since all data
 *	flowing within an RTMaps must have a type associated to it.
 */
struct MAPSTypeInfo
{
    //! This is the 64-bit value describing the type (integer, MAPSFloat...) of the output data.
    MAPSTypeInfoValue value;
    //! An optional name associated to the output data type.
    /*! This is optional, except when value is set to MAPS::Structure, which
     *	means that the type of the output is a user-specific structure. In that case, a name must be
     *	associated to the structure in order to distinguish it among all the user-specific structures.
     */
    MAPSString* name;			
    //! A string describing the unit of the output data.
    /*! For instance, "km/h", or "m/s".
     */
    MAPSString* unit;			

    //! Dynamically sets the optional name of the output data.
    /*! Note that the optional name is usually set statically in the output definitions.
     *	\sa MAPS_OUTPUT
     */
    void SetName(const char *n);
    //! Dynamically sets the unit of the output data.
    /*! Note that the unit is usually set statically in the output definitions.
     *	\sa MAPS_OUTPUT
     */
    void
    SetUnit(const char* u);
    //! Copy operator
    MAPSTypeInfo&
    operator=(const MAPSTypeInfo& type);
    //! Destructor
    ~MAPSTypeInfo();
};

//! The RTMaps type filter structure
/*! A type filter structure must be provided for each input, since all data
 *	flowing within a RTMaps system do have a type associated to it and are filtered before
 *	getting in an input.
 *
 *	The RTMaps type filtering behaviour is the following :
 *	- First of all, if a name or a unit filter is set, only the data whose unit or name
 *		matches the filter unit or name parameter pass through the filter.
 *	- In a second phase, a AND mask is applied to the type (parameter mask). The
 *		result of this AND operation must be equal to the x parameter. Otherwise,
 *		the type is rejected. This enables to test some bits. For instance, if we
 *		want to let only vectors pass through the filter, we set mask to MAPS::VectorFlag
 *		and set x to MAPS::VectorFlag. This will require the bit MAPS::VectorFlag to
 *		be set. On the contrary, if we don't want to let vectors pass through the
 *		filter, we set mask to MAPS::VectorFlag and set x to 0.
 *	- In a third and last phase, another AND mask is applied (parameter maskX). This
 *		mask is less restrictive, since if the result is different from 0, i.e. if any
 *		of the bits are set, the type will pass through the filter. For instance, if
 *		we want to let numbers pass through the filter, we can set maskX to MAPS::Integer | MAPS::Float.
 *
 *	This is actually the code of the MAPS::TypeFilter function :
 *
 *	\code
bool MAPS::TypeFilter(const MAPSTypeInfo& type, MAPSTypeFilterBase& filter)
{
    if (filter.nameFilter) {
	if (filter.nameRegexp==NULL) filter.nameRegexp=new MAPSRegExp(filter.nameFilter);
	if ((type.name==NULL)||(filter.nameRegexp->Match(*(type.name))==false)) {
	    return false;
	}
    }
    if (filter.unitFilter) {
	if (filter.unitRegexp==NULL) filter.unitRegexp=new MAPSRegExp(filter.unitFilter);
	if ((type.unit==NULL)||(filter.unitRegexp->Match(*(type.unit))==false)) {
	    return false;
	}
    }
    return (((type.value&filter.mask)==(filter.x&filter.mask))&&(type.value&filter.maskX));
}
	\endcode
 *
 *	\sa MAPS::TypeFilter
 */
struct MAPSTypeFilterBase
{
    //! First AND filter mask applied by MAPS::TypeFilter to the data type to pass through the filter.
    MAPSTypeInfoValue mask;
    //! The data type to pass through the filter is compared to x after applying the first AND mask.
    MAPSTypeInfoValue x;
    //! The second AND filter mask
    MAPSTypeInfoValue maskX;
    //! The optional name filter. 
    /*! Only the types matching this regular expression can pass through the filter.
     *	If set to \c NULL, no optional name filtering is done (any named data can pass through the filter).
     */
    const char* nameFilter;
    //! The optional unit filter. 
    /*! Only the types matching this regular expression can pass through the filter.
     *	If set to \c NULL, no unit filtering is done (any unit can pass through the filter).
     */
    const char* unitFilter;
    MAPSRegExp* nameRegexp; // Should be set to NULL by structure initialization
    MAPSRegExp* unitRegexp; // Should be set to NULL by structure initialization
};

//! The RTMaps type filter class
class MAPSTypeFilter : private MAPSTypeFilterBase
{
public:
    //! Constructs a type filter based on a model. Assigns a new name or unit filter (regular expression) if needed.
    MAPSTypeFilter(const MAPSTypeFilterBase& model, const char *filterName=NULL, const char *filterUnit=NULL) {
	mask=model.mask; x=model.x; maskX=model.maskX; nameFilter=model.nameFilter; unitFilter=model.unitFilter;
	if (filterName) nameFilter=filterName;
	if (filterUnit) unitFilter=filterUnit;
	nameRegexp=NULL; unitRegexp=NULL;
    }
    virtual ~MAPSTypeFilter();
};

class MAPSFile
{
private:
    MAPSString fileName;
    MAPSString path;
    // Removes duplicate '/', interprets ../
    void
    CompactPath(void) {/*TODO*/}

public:
    void
    SetLongName(const char* newName);
    bool
    Exists(void) const;
    bool
    IsAbsolute(void) const;
    //bool IsReadable(void) const {/*TODO*/return Exists();}
    //bool IsWritable(void) const {/*TODO*/return Exists();}
    bool
    IsDirectory(void) const;
    MAPSString
    GetFilename(void) const {return fileName;}
    MAPSString
    GetPath(void) const {return path;}
    MAPSString
    GetFullPath(void) const {return (path+fileName);}
    //void ChangeDirectory(const MAPSString newDir) {/*TODO*/}
    //MAPSString GetRelativePath(const MAPSString refDir) const {/*TODO*/}
	//MAPSString GetExtension() {/*TODO*/}
    explicit
    MAPSFile(const char* newName)
    { SetLongName(newName); }
    MAPSFile(const MAPSFile& mf) : fileName(mf.fileName),path(mf.path) {}
    virtual ~MAPSFile() {}
};

struct MAPSInputDefinition
{
    const char* name;
    int reserved;
    MAPSTypeFilterBase typeFilter;
    int readerType;
    int subsampling;
};

/*! \defgroup inputdef Input definition macros
 * \ingroup component
 * @{
 */
//! \internal
#define MAPS_INPUTS_DECLARATION static MAPSInputDefinition inputsDefinition[]

//! Starts the definition of the inputs of a module.
/*! \param className The module class name
 * \hideinitializer
 */
#define MAPS_BEGIN_INPUTS_DEFINITION(className) MAPSInputDefinition className::inputsDefinition[]={

//! Basic definition of an input.
/*! 
 * \param namex	a simple string used to further identify the input (e.g. "input")
 * \param filter the kind of expected data, a MAPSTypeFilterBase (e.g. MAPS::FilterInteger)
 * \param typex the reader type of the input (e.g. MAPS::FifoReader, MAPS::SamplingReader)
 * \hideinitializer
 */
#define MAPS_INPUT(namex,filter,typex)	\
{										\
	namex,								\
	0,									\
	{									\
		filter.mask,					\
		filter.x,						\
		filter.maskX,					\
		filter.nameFilter,				\
		filter.unitFilter,				\
		NULL,							\
		NULL							\
	},									\
	typex,								\
	1									\
},

//! Ends the definition of the inputs of a module.
/*! \hideinitializer
 */
#define MAPS_END_INPUTS_DEFINITION {NULL}};

//! \internal
#define MAPS_INPUT2(namex,mask,x,maskX,name,unit,typex) {namex,0,{mask,x,maskX,name,unit,NULL,NULL},typex,1},
/*!@}*/

struct MAPSOutputDefinition
{
    const char* name;
    int reserved;    
    MAPSTypeInfo type;
    int size;
    int fifoSize;
    int subsampling;
    int shareable;

    MAPSOutputDefinition&
    operator=(const MAPSOutputDefinition &outputDef);
};
/*! \defgroup outputdef Output definition macros
 * \ingroup component
 * @{
 */
//! \internal
#define MAPS_OUTPUTS_DECLARATION static MAPSOutputDefinition outputsDefinition[]

//! Starts the definition of the outputs of a module.
/*! \param className The module class name
 * \hideinitializer
 */
#define MAPS_BEGIN_OUTPUTS_DEFINITION(className) MAPSOutputDefinition className::outputsDefinition[]={

//! Basic definition of an output.
/*! 
 * \param name a simple string used to further identify the output (e.g. "output")
 * \param value the type of the output data, a MAPSTypeInfoValue (e.g. MAPS::Integer)
 * \param namex An optional name associated to the output data type. Can be \c NULL.
 * \param unit A string describing the unit of the output data (e.g. "m/s"). Can be \c NULL.
 * \param size The size of the output. Use 1 for a single piece of data, 0 if you do not know the data size yet, but do not
 * forget to allocate the output later. MAPSOutput::AllocOutputBuffer documentation gives more details about output buffer allocation.
 * \hideinitializer
 */
#define MAPS_OUTPUT(name,value,namex,unit,size) \
    MAPS_OUTPUT_FIFOSIZE(name,value,namex,unit,size,16)

//! Definition of an output with control of the FIFO size.
/*! \copydoc MAPS_OUTPUT
 * \param fifosize size of the FIFO (default size of the output FIFO is 16)
 * \hideinitializer
 */
#define MAPS_OUTPUT_FIFOSIZE(name,value,namex,unit,size,fifosize) {name,0,{value,new MAPSString((const char*)namex),new MAPSString((const char*)unit)},size,fifosize,1,1},

//! Definition of an output using a user structure.
/*! If you need to transmit your own data structure between RTMaps components, you can define your own structure. It is the goal of this macro.
 * \param name a simple string used to further identify the output (e.g. "output")
 * \param structureName the class name of your own structure
 * \hideinitializer
 */
#define MAPS_OUTPUT_USER_STRUCTURE(name,structureName) {name,0,{MAPS::Structure,new MAPSString((const char*)#structureName),new MAPSString((const char*)NULL)},sizeof(structureName),16,1,1},

//! Ends the definition of the outputs of a module.
/*! \hideinitializer
 */
#define MAPS_END_OUTPUTS_DEFINITION {NULL}};
/*!@}*/

//! Enumeration structure.
/*!This structure represents an array of strings with a selection index. 
 * The exchange string of a MAPSEnumStruct is like \c "numberOfItems|selectedItem|item0|item1|...|itemN".
 */
struct MAPSEnumStruct
{
    //! The selection index. -1 means no item has been selected yet.
    MAPSInt32 selectedEnum;
    //! A pointer to the collection of strings.
    MAPSArray<MAPSString>* enumValues;

    //! Default constructor: creates an empty enumeration
    MAPSEnumStruct();
    MAPSEnumStruct(const char* eV,
		   MAPSInt32 selected);
    //! Copy constructor
    MAPSEnumStruct(const MAPSEnumStruct& enumStruct);
    virtual ~MAPSEnumStruct();
    void
    Copy(const MAPSEnumStruct& eS);
    //! Counterparts of MAPSEnumStruct::ToString() unless \a selectIndexOnly is \c true; in this case, only the index is changed.
    void
    FromString(const MAPSString& str, 
	       const bool selectIndexOnly = false);
    //! Assignement operator
    MAPSEnumStruct&
    operator=(const MAPSEnumStruct& enumStruct);

    //! Returns \c true if the string argument is a well-formatted enumeration string
    static bool
    IsEnumString(const MAPSString& str);

    //! Retrieves the index of the selected property.
    MAPSInt32
    GetSelected() const {return selectedEnum;}
    //! Sets \a selectedEnum to the index of the string \a value in the collection. 
    /*! Returns \c false if \a value is an invalid choice.*/
    bool
    Select(const MAPSString& value);
    //! Converts the enumeration and its associated index to a single string
    MAPSString
    ToString() const;
};

struct MAPSPropertyValue
{
    int type;
    union {
	bool boolValue;
	MAPSInt64 intValue;
	MAPSFloat floatValue;
	MAPSString* stringValue;
	MAPSEnumStruct* enumStruct;
    };

    void Copy(const MAPSPropertyValue& pV);
    explicit MAPSPropertyValue(bool bV);
    explicit MAPSPropertyValue(MAPSInt64 iV);
    explicit MAPSPropertyValue(MAPSInt32 iV);
    explicit MAPSPropertyValue(MAPSFloat fV);
    explicit MAPSPropertyValue(const char* sV);
    MAPSPropertyValue(const char* eV,
		      MAPSInt32 selected);
    explicit MAPSPropertyValue(MAPSEnumStruct enumStruct);
    MAPSPropertyValue() {}
    virtual ~MAPSPropertyValue();
};

struct MAPSPropertyDefinition
{
    const char* name;
    MAPSPropertyValue* value;
    const char* unit;
    bool needs2BeInitialized;
    bool canBeModifiedAfterStart;
    bool readOnly;
    bool locked;
    ~MAPSPropertyDefinition();
};

/*! \defgroup propdef Property definition macros
 * \ingroup component
 * \ingroup rrm
 * @{
 */
//! \internal
#define MAPS_PROPERTIES_DECLARATION static MAPSPropertyDefinition propertiesDefinition[]

//! Starts the definition of the properties of a module.
/*! \param className The module class name
 * \hideinitializer
 */
#define MAPS_BEGIN_PROPERTIES_DEFINITION(className) MAPSPropertyDefinition className::propertiesDefinition[]={

//! Basic definition of a property.
/*! 
 * \param name a simple string used to further identify the output (e.g. "property")
 * \param value the initial value of the property. Its type is automatically deduced from this value. Use <CODE>(char*)NULL</CODE> to initialize an empty string.
 * \param needs2BeInitialized when set to \a true, the property must be initialized before run
 * \param canBeChangedAfterInstantiation when set to \a false, the property cannot be changed while RTMaps is running
 * \hideinitializer
 */
#define MAPS_PROPERTY(name,value,needs2BeInitialized,canBeChangedAfterInstantiation) \
    MAPS_PROPERTY_UNIT(name,value,NULL,needs2BeInitialized,canBeChangedAfterInstantiation)

//! Basic definition of a property.
/*! \copydoc MAPS_PROPERTY
 * \param unit a simple string used for display only
 * \hideinitializer
 */
#define MAPS_PROPERTY_UNIT(name,value,unit,needs2BeInitialized,canBeChangedAfterInstantiation) {name,new MAPSPropertyValue(value),unit,needs2BeInitialized,canBeChangedAfterInstantiation,false,true},

//! Basic definition of a property.
/*! 
 * \param name a simple string used to further identify the output (e.g. "property")
 * \param enumstr a string of enumeration values, e.g. "value0|value1|value2"
 * \param selected the initial selected index (starting from 0). -1 means not yet selected.
 * \param needs2BeInitialized when set to \a true, the property must be initialized before run
 * \param canBeChangedAfterInstantiation when set to \a false, the property cannot be changed while RTMaps is running
 * \hideinitializer
 */
#define MAPS_PROPERTY_ENUM(name,enumstr,selected,needs2BeInitialized,canBeChangedAfterInstantiation) \
    MAPS_PROPERTY_ENUM_UNIT(name,enumstr,selected,NULL,needs2BeInitialized,canBeChangedAfterInstantiation)

//! Basic definition of a property.
/*! \copydoc MAPS_PROPERTY_ENUM
 * \param unit a simple string used for display only
 * \hideinitializer
 */
#define MAPS_PROPERTY_ENUM_UNIT(name,enumstr,selected,unit,needs2BeInitialized,canBeChangedAfterInstantiation) {name, new MAPSPropertyValue(enumstr,selected),unit,needs2BeInitialized,canBeChangedAfterInstantiation,false,true},

//! Definition of a read-only property.
/*! 
 * \param name a simple string used to further identify the output (e.g. "property")
 * \param value the initial value of the property. Its type is automatically deduced from this value. Use <CODE>(char*)NULL</CODE> to initialize an empty string.
 * \hideinitializer
 */
#define MAPS_PROPERTY_READ_ONLY(name,value) \
    MAPS_PROPERTY_READ_ONLY_UNIT(name,value,NULL)

//! Definition of a read-only property with a specified unit.
/*! \copydoc MAPS_PROPERTY_READ_ONLY
 * \param unit a simple string used for display only
 * \hideinitializer
 */
#define MAPS_PROPERTY_READ_ONLY_UNIT(name,value,unit) {name,new MAPSPropertyValue(value),unit,false,false,true,true},

//! Ends the definition of the properties of a module.
/*! \hideinitializer
 */
#define MAPS_END_PROPERTIES_DEFINITION {NULL}};
/*!@}*/

// 

struct MAPSActionDefinition
{
    const char* name;
    void (*action)(MAPSModule* module,int actionNb);
    bool allowedWhenComponentDead;
    char reserved[3];
};
/*! \defgroup actiondef Action definition macros
 * \ingroup component
 * \ingroup rrm
 * @{
 */
//! \internal
#define MAPS_ACTIONS_DECLARATION static MAPSActionDefinition actionsDefinition[]

//! Starts the definition of the actions of a module.
/*! \param className The module class name
 * \hideinitializer
 */
#define MAPS_BEGIN_ACTIONS_DEFINITION(className) MAPSActionDefinition className::actionsDefinition[]={

//! Basic definition of an action.
/*! This action definition is for use only while RTMaps is running.
 * \param name a simple string used to further identify the output (e.g. "action")
 * \param proc the callback function to launch when the action is called (e.g. <code>MyModule::anAction</code>)
 * \hideinitializer
 */
#define MAPS_ACTION(name,proc) \
    MAPS_ACTION2(name,proc,false)

//! Second definition of an action.
/* Like #MAPS_ACTION with \a allowedWhenDead parameter added. If \a allowedWhenDead is set to \c true, 
 * the action can be called at any time.
 * \hideinitializer
 */
#define MAPS_ACTION2(name,proc,allowedWhenDead) {name,proc,allowedWhenDead},

//! Ends the definition of the actions of a module.
/*! \hideinitializer
 */
#define MAPS_END_ACTIONS_DEFINITION {NULL}};
/*!@}*/

extern "C" MAPS_DLL_EXPORT MAPSFloat MAPSGetPackageVersion();
extern "C" MAPS_DLL_EXPORT const char* MAPSGetPackageName();

#ifdef __GNUC__
//! Use this macro to determine the version of your package file.
/*! Use this macro in one (and only one) of your package .cpp files
 * in order to set a version for your RTMaps .pck file. If this macro
 * is not present, the version of the package is 1.0.
 */
#define MAPS_PACKAGE_DEFINITION(name,version) \
    MAPS_DLL_EXPORT MAPSFloat \
    MAPSGetPackageVersion(void) {return version;} \
    MAPS_DLL_EXPORT const char* \
    MAPSGetPackageName(void) {return name;} \
    extern "C" MAPS_DLL_EXPORT MAPSInt32 \
    MAPSGetGCCVersion(void) {return (__GNUC__*10000+__GNUC_MINOR__*100+__GNUC_PATCHLEVEL__);}

#else
//! Use this macro to determine the version of your package file.
/*! Use this macro in one (and only one) of your package .cpp files
 * in order to set a version for your RTMaps .pck file. If this macro
 * is not present, the version of the package is 1.0.
 */
#define MAPS_PACKAGE_DEFINITION(name,version) \
    MAPS_DLL_EXPORT MAPSFloat \
    MAPSGetPackageVersion() {return version;} \
    MAPS_DLL_EXPORT const char* \
    MAPSGetPackageName() {return name;}

#endif


// RTMaps main objects :

//! The RTMaps Module Output class
/*! You must use this class in particular to allocate the data buffers in RTMaps.
 *	They are called "output buffers", because they are associated to the output.
 *	In the RTMaps component model, the outputs are responsible for the data buffering,
 *	and the components are responsible for their outputs.
 * \ingroup component rrm
 */
class MAPSOutput
{
public:
    /*!	\name Buffer allocation functions
     *  These are very important functions.
     *	\note There is no deallocation functions, since RTMaps automatically
     *	deallocates the output buffer when the output dies.
     */
    //@{
    //! Allocates an output buffer with an element size of \a size
    /*! This function is automatically called when an output is created, but it
     *	provides only basic output buffer capabilities, since it has only one
     *	parameter.
     *
     *	What this function really does depends on the type of the output:
     *	- For "vectorizable" types, like integers, floats or CAN frames, this defines
     *	the size of the vector (\a size must be 1 for a scalar).
     *	- For text types (MAPS::TextAscii or MAPS::TextUnicode), 
     *	this function allocates some strings for the buffer of \a size characters. 
     *	The default value MAPS::DefaultTextSize is generally used.
     *	- For the MAPSMatrix type, an array of \a size elements
     *	is allocated for each buffer element, which means that the maximum <I>m*n</I> size
     *	of the matrix must be less than \a size. <I>m</I> and <I>n</I> are
     *	set to 0 by default. A call to MAPSOutput::AllocOutputBufferMatrix can be
     *	more appropriate.
     *	- For a MAPSImage type, an image buffer of \a size is allocated
     *	for each buffer element. All the parameters of the structure are set to 0 or
     *	unknown. A call to MAPSOutput::AllocOutputBufferImage or MAPSOutput::AllocOutputBufferMAPSImage
     *	might be more appropriate.
     *	- For an IplImage type, no allocation is done whatever \a size is.
     *	You MUST call MAPSOutput::AllocOutputBufferIplImage to allocate the buffer.
     *
     *	If \a size is 0, no output buffer allocation is done, and another
     *	call to an output buffer allocation function will have to be done before the buffer can be
     *	actually used.
     */
    virtual void
    AllocOutputBuffer(int size,
		      bool doAllocate=true)=0;
    //! Allocates an IplImage output buffer
    /*!	\param model A model to replicate for each buffer element. This model can
     *	be provided by a call to MAPS::IplImageModel.
     *	\param headerOnly If set to \c true, this function will only allocate the header,
     *	and no memory will be allocated for the content of the image. This is useful
     *	to directly connect the IplImage structure to a frame buffer allocated by
     *	a hardware frame grabber. For experts only. headerOnly should usually be set to \c false.
     */
    virtual void
    AllocOutputBufferIplImage(IplImage& model,
			      bool headerOnly=false)=0;
    //! Allocates a MAPSImage output buffer
    /*!	\param size The size in bytes to allocate for the image buffer. Note that it can be
     *	less than <I>w*h</I> since a MAPSImage can contain compressed images.
     *	\param w The width of the image
     *	\param h The height of the image
     *	\param coding Integer describing the code used by the image buffer. For instance
     *	<CODE>MAPS_IMAGECODING_JPEG</CODE> or <CODE>MAPS_IMAGECODING_YUYV</CODE>.
     *	\param imageType Additional information describing the type of the image. Generally, it
     *	is set to MAPS_IMAGETYPE_COLOR or MAPS_IMAGETYPE_MONO.
     *	\param headerOnly If set to \c true, this function will only allocate the header,
     *	and no memory will be allocated for the content of the image. This is useful
     *	to directly connect the IplImage structure to a frame buffer allocated by
     *	a hardware frame grabber. For experts only. \a headerOnly should usually be set to \c false.
     */
    virtual void
    AllocOutputBufferImage(int size,
			   int w=0,
			   int h=0,
			   MAPSUInt32 coding=MAPS_IMAGECODING_UNKNOWN,
			   MAPSUInt32 imageType=MAPS_IMAGETYPE_COLOR,
			   bool headerOnly=false)=0;
    //! Allocates a MAPSImage output buffer
    /*! \copydoc AllocOutputBufferImage
     */
    virtual void
    AllocOutputBufferMAPSImage(int size,
			       int w=0,
			       int h=0,
			       MAPSUInt32 coding=MAPS_IMAGECODING_UNKNOWN,
			       MAPSUInt32 imageType=MAPS_IMAGETYPE_COLOR,
			       bool headerOnly=false)=0;
    //! MAPSMatrix output buffer allocation
    /*!	\param m The number of rows of the allocated matrix
     *	\param n The number of columns of the allocated matrix
     */
    virtual void
    AllocOutputBufferMatrix(int m,
			    int n)=0;
    //@}
    
    //! Returns the name of the output
    /*! In the form <CODE>componentName.outputName</CODE>
     */
    virtual MAPSString&
    Name()=0;
    //! Returns the number of FIFO overflows on this output (performance monitoring)
    virtual int
    NbFIFOOverflows()=0;
    //! Returns a pointer to the MAPSIOMonitor object associated to this output (the buffer management system)
    virtual MAPSIOMonitor&
    Monitor()=0;
    //! Makes this output buffer shareable among different RTMaps processes (expert only)
    virtual void
    MakeShareable()=0;
    //! Makes this output buffer unshareable among different RTMaps processes (expert only)
    virtual void
    MakeUnshareable()=0;
    
    //! Returns the size in bytes of one element of the buffer according to the allocation. 
    /*!	This is very useful to copy a buffer element.
     * Beware that MAPS::Memcpy() cannot generally be used to copy buffers, in particular for IplImage and MAPSImage
     */
    virtual int
    IOEltSizeBytes()=0;

    //! Returns the type of the output
    virtual MAPSTypeInfo&
    Type()=0;
    //! Sets the unit of the output (dynamically modifies the type)
    /*! \sa MAPSTypeInfo */
    virtual void
    SetTypeUnit(const char* unit)=0;
    //! Sets the unit of the output (dynamically modifies the type)
    /*! \sa MAPSTypeInfo */
    virtual void
    SetUnit(const char* unit)=0;
    //! Gets the unit of the output ("m/s" for instance)
    virtual const char*
    TypeUnit()=0;
    //! Gets the unit of the output ("m/s" for instance)
    virtual const char*
    Unit()=0;
    //! Sets the name parameter of the type of the output (dynamically modifies the type)
    /*! \sa MAPSTypeInfo */
    virtual void
    SetTypeName(const char* name)=0;
    //! Gets the name parameter of the type of the output
    /*! This is completely different from the name of the output itself. Don't get confused.
     * \sa MAPSTypeInfo
     */
    virtual const char*
    TypeName()=0;	
    
    //! Destructor
    virtual
    ~MAPSOutput() { }
};

//! The RTMaps Module Input class
/*!
 * \ingroup component rrm
 */
class MAPSInput
{
public:
    //! Returns \c true if the input is connected to another component output.
    /*! It is useless to try to get some data from an input that is not connected.
    *	\sa MAPSComponent::StartReading
    */
    virtual bool
    IsConnected()=0;
    //! Returns a pointer to the output (of type MAPSOutput) connected to this input.
    /*! Returns \c NULL if no output is connected
    */
    virtual MAPSOutput*
    ConnectedOutput()=0;
    //! Gets the type of the output to which this input is connected.
    /*!	Note that generally this function is used after a
    *	call to MAPSComponent::StartReading, that is after getting some
    *	data from the connected output, so that it is not generally necessary to
    *	check the connection.
    */
    virtual MAPSTypeInfo&
    Type()=0;
    //! Returns the name of the input
    /*! In the form <CODE>componentName.inputName</CODE>
    */
    virtual MAPSString&
    Name()=0;
    //! Returns the short name of the input
    /*! This is the name after the dot
    */
    virtual const char*
    ShortName()=0;
    //! Checks if any data is available in the FIFO connected to this input
    virtual bool
    DataAvailableInFIFO()=0;
    
    virtual ~MAPSInput() {}
};

typedef void (*MAPSPropertyGetCallback)(MAPSProperty& prop, void*);
typedef void (*MAPSPropertySetCallback)(MAPSProperty& prop);

//! The RTMaps Module Property class
/*!
 * \ingroup component rrm
 */
class MAPSProperty
{
public:
    //! Returns a reference to the integer content of the property
    virtual MAPSInt64&
    IntegerValue()=0;
    //! Returns a reference to the string content of the property
    virtual MAPSString&
    StringValue()=0;
    //! Returns a reference to the boolean content of the property
    virtual bool&
    BoolValue()=0;
    //! Returns a reference to the float content of the property
    virtual MAPSFloat&
    FloatValue()=0;
    //! Returns a reference to the enum structure of the property
    virtual MAPSEnumStruct&
    EnumValues()=0;

    //! Returns the name of the property
    /*! In the form <CODE>componentName.propertyName</CODE>
     */
    virtual MAPSString&
    Name()=0;
    //! Returns the short name of the property
    /*! Returns the part of the name after the first dot (module name)
     */
    virtual MAPSString&
    ShortName()=0;
    //! Returns the unit of the property
    virtual const char*
    Unit()=0;
    //! Returns the owner of this property
    virtual MAPSModule&
    Owner()=0;
    //! Returns the type of this property
    virtual int
    Type()=0;
    //! Tells if the property value has changed.
    /*! Returns \c true if the property value has changed since the last 
	 * call to MAPSProperty::AcknowledgePropertyChanged
     */
    virtual bool
    PropertyChanged()=0;
    //! Acknowledges the property value change
    /*! Must be called regularly so that we can know the property has changed.
     * \sa MAPSProperty::PropertyChanged
     */
    virtual void
    AcknowledgePropertyChanged()=0;
    //! Gets the MAPSEvent related to the change of property value event
    /*! This can be very useful to make some wait on property value change.
     *	You can do that with the MAPSModule::Wait4Event or MAPSModule::Wait4Events function.
     */
    virtual MAPSEvent*
    GetPropertyChangedEvent()=0;
    //! Gets the MAPSEvent related to the change of property value event
    /*! This can be very useful to make some wait on property value change.
     *	You can do that with the MAPSModule::Wait4Event or MAPSModule::Wait4Events function.
     */
    virtual MAPSEvent&
    PropertyChangedEvent()=0;
    /*! \name The Set functions (to set the property value)
     *
     *	These functions call the MAPSModule::Set functions associated to the
     *	owner of the property. Behaviour might differ depending on the type of the property.
     */
    //@{
    //! Sets the boolean property value to \a value for boolean properties
    virtual void
    Set(bool value)=0;
    //! Sets the integer property value to \a value for integer properties, or the selected item for enum properties
    virtual void
    Set(MAPSInt64 value)=0;
    //! \copydoc MAPSProperty::Set(MAPSInt64)
    virtual void
    Set(MAPSInt32 value)=0;
    //! Sets the float property value to \a value for floating point properties
    virtual void
    Set(MAPSFloat value)=0;
    //! Sets string property value to \a value for string properties, or fills the enum values for enum properties
    virtual void
    Set(const char* value)=0;
    //! \copydoc MAPSProperty::Set(const char*)
    virtual void
    Set(const MAPSString& value)=0;
    //! Sets the enum property value to \a enumStruct (enum strings and the selected item) for enum properties
    virtual void
    Set(MAPSEnumStruct enumStruct)=0;
    //! Sets enum values to \a enumValues and the selected enum value to \a selected for enum properties
    virtual void
    Set(const MAPSString& enumValues,
	const int selected)=0;

    //@}

    /*! \name The Get functions (to get the property value)
     *
     *	These functions call the MAPSModule::Get functions associated to the
     *	owner of the property. Behaviour might differ depending on the type of the property.
     */
    //@{
    //! Gets the boolean property value into \a value for boolean properties
    virtual void
    Get(bool& value)=0;
    //! Gets the integer property value into \a value for integer properties, or the selected item for enum properties
    virtual void
    Get(MAPSInt64& value)=0;
    //! \copydoc MAPSProperty::Get(MAPSInt64&)
    virtual void
    Get(MAPSInt32& value)=0;
    //! Gets the float property value into \a value for float properties
    virtual void
    Get(MAPSFloat& value)=0;
    //! Gets the string property value int \a value for string properties, or the selected item for enum properties
    virtual void
    Get(MAPSString& value)=0;
    //! Gets the enum property value into \a enumStruct for enum properies
    virtual void
    Get(MAPSEnumStruct& enumStruct)=0;
    //! Gets the enum property value for enum properties
    /*! The enum value is split into the array of strings, put into \a enumValues,
     * and the selected value, put into \a selectedValue
     */
    virtual void
    Get(MAPSArray<MAPSString>& enumValues,
	MAPSInt32& selectedValue)=0;
    //@}

    /*! \name Properties callback management functions
     *
     *	For RTMaps experts only!
     */
    //@{
    //! Sets a callback which will be called on a call to Set
    virtual void
    SetSetCallback(MAPSPropertySetCallback cb)=0;
    //! Sets a callback which will be called on a call to Get
    virtual void
    SetGetCallback(MAPSPropertyGetCallback cb)=0;
    //@}

    virtual void
    Lock(bool bLock=true)=0;

    virtual
    ~MAPSProperty() { }
};

//! The RTMaps Module Action class
/*!
 * \ingroup component
 */
class MAPSAction
{
public:
    //! Executes the action
    virtual void
    DoIt()=0;
    virtual MAPSString&
    Name()=0;
    virtual MAPSString&
    ShortName()=0;
    virtual
    ~MAPSAction() { }
};

class MAPSReplayAsyncCommData
{
// 
private:
    int recordNumber;
    MAPSString hint;
    MAPSTimestamp timeOfIssue;
    MAPSTimestamp timestamp;
    MAPSOutput* output;
    bool immediate; 
    MAPSInt64 frequency;
    int quality;
    int misc1;
    int misc2;
    int misc3;
};

// OS dependent functions
class MAPSOS
{
// 
    static void
    Init(void);
    static void
    Run(void);
    static void
    Shutdown(void);
    static void
    Exit(void);
public:
    //! Returns the OS general mutex, for use by OS specific components (generally during initialization)
    static MAPSMutex&
    Mutex();
};

//! \internal The MAPSHMIKeysHandler class
/*! \internal Inherit this class if you want to influence the keys states on different HMIs : run, shutdown, fast or slow forward, rewind, etc...
 *  If a key is not enabled nor disabled by any MAPSHMIHandler, the engine disables the key.
 *  If a key is disabled in any MAPSHMIKeysHandler and enabled elsewhere, priority is given to the disabled state.
 */
class MAPSHMIKeysHandler
{
private :
    int m_EnabledKeysState;
    int m_DisabledKeysState;

    void Register();
    void Unregister();
    void UpdateKeysState();

public :
    int GetEnabledKeysState() {return m_EnabledKeysState;}
    int GetDisabledKeysState() {return m_DisabledKeysState;}

    void SetEnabledKeys(int keys) {m_EnabledKeysState= keys; UpdateKeysState();}
    void SetDisabledKeys(int keys) {m_DisabledKeysState= keys; UpdateKeysState();}
    void EnableKeys(int keys) { m_EnabledKeysState |= keys; UpdateKeysState();}
    void DontEnableKeys(int keys) {m_EnabledKeysState &= !keys; UpdateKeysState();}
    void DisableKeys(int keys) {m_DisabledKeysState |= keys; UpdateKeysState();}
    void DontDisableKeys(int keys) {m_DisabledKeysState &= !keys; UpdateKeysState();}

    MAPSHMIKeysHandler();
    virtual ~MAPSHMIKeysHandler();
};

//! The main RTMaps namespace.
/*! The MAPS namespace contains a lot of useful functions to deal with the engine features.
 *  All its members are declared as static.
 */
namespace MAPS {
    /*! \name RTMaps Typing related constants
     */
    //@{
    const MAPSTypeInfoValue NoMask	    = 0x0000000000000000;		//!< No mask
    const MAPSTypeInfoValue NoType	    = 0x0000000000000000;		//!< No type
    const MAPSTypeInfoValue AnyType	    = 0x00000000FFFFFFFF;		//!< Any type
    const MAPSTypeInfoValue TypeMask	    = 0x00000000FFFFFFFF;		//!< Type mask
    const MAPSTypeInfoValue Structure	    = 0x0000000000000001;		//!< User defined structure
    const MAPSTypeInfoValue Integer	    = 0x0000000000000002;		//!< Integer (32 bits, signed)
    const MAPSTypeInfoValue Float	    = 0x0000000000000004;		//!< MAPSFloat (double : 64 bits, double precision)
    const MAPSTypeInfoValue TextAscii	    = 0x0000000000000008;       //!< Ascii characters (1 character = 8 bits).
	/*!< Remark: When dealing with an output with type TextAscii, the arbitrary convention is: 
	 * the text bytes are ended with the '\\0' character in the MAPSIOElt buffer, but this '\\0'
	 * character is not counted in the VectorSize() field of the MAPSIOElt. So, ioElt->VectorSize()
	 * should return the same result than std::strlen(ioElt->TextAscii()).
	 */
    const MAPSTypeInfoValue TextUnicode	    = 0x0000000000000010;		//!< Unicode characters (1 character = 16 bits)
    const MAPSTypeInfoValue IplImage	    = 0x0000000000000020;		//!< IplImage (Image Processing Library image description structure)
    const MAPSTypeInfoValue MAPSImage	    = 0x0000000000000040;		//!< MAPSImage (RTMaps specific image structure)
    const MAPSTypeInfoValue CANFrame	    = 0x0000000000000080;		//!< CAN frame
    const MAPSTypeInfoValue Matrix	    = 0x0000000000000100;		//!< MATLAB-Like matrix
    const MAPSTypeInfoValue RealObject	    = 0x0000000000000200;		//!< Real object (car, tree, etc.)
    const MAPSTypeInfoValue DrawingObject   = 0x0000000000000400;		//!< Drawing object (line, circle, etc.)
    const MAPSTypeInfoValue Triangles3D	    = 0x0000000000000800;		//!< 3D triangles for 3D scene rendering
    const MAPSTypeInfoValue Stream8	    = 0x0000000000001000;		//!< 8-bit stream (sound, numerized data)
    const MAPSTypeInfoValue Stream16	    = 0x0000000000002000;		//!< 16-bit stream (sound, numerized data)
    const MAPSTypeInfoValue Stream32	    = 0x0000000000004000;		//!< 32-bit stream (numerized data)
    const MAPSTypeInfoValue Integer64	    = 0x0000000000008000;		//!< Integer (64 bits, signed)
    
    const MAPSTypeInfoValue AnyText	    = 0x0000000000000018;		//!< Some textual information, ascii or unicode
    const MAPSTypeInfoValue AnyInteger	    = 0x0000000000008002;		//!< 32 or 64 bits integers.
    
    const MAPSTypeInfoValue VectorFlag	    = (((MAPSInt64)1)<<63);		//!< Indicates that the piece of data is a vector (an array) of a basic type
    const MAPSTypeInfoValue FrequencyFlag   = (((MAPSInt64)1)<<62);		//!< Indicates that a frequency is provided with the data
    const MAPSTypeInfoValue QualityFlag	    = (((MAPSInt64)1)<<61);		//!< Indicates that a quality is transmitted along with the data (for instance a compression or a noise ratio).
    const MAPSTypeInfoValue MiscFlag	    = (((MAPSInt64)1)<<60);		//!< Indicates that 3 miscellaneous integers are transmitted along with the data. See MAPSIOElt::Misc1(), Misc2() and Misc3()
    //@}
    
    /*! \name RTMaps Inputs behaviours
    */
    //@{
    const int FifoReader	    =0;	//!< FIFO Reader behaviour
    const int NeverSkippingReader   =1;	//!< Never Skipping Reader behaviour
    const int LastOrNextReader	    =2;	//!< Last or Next Reader behaviour
    const int Wait4NextReader	    =3;	//!< Wait For Next Reader behaviour
    const int SamplingReader	    =4;	//!< Sampling Reader behaviour
    //@}
    
    /*! \name RTMaps Properties types
    */
    //@{
    const int BoolProperty	=0;	//!< Boolean property (\c false or \c true)
    const int IntegerProperty	=1;	//!< Integer property (64-bit signed integer)
    const int FloatProperty	=2;	//!< Floating point property (\c double: 64-bit floating point number)
    const int StringProperty	=3;	//!< String property (ASCII string)
    //! Enum property (ASCII string)
    /*! \sa MAPSEnumStruct */
    const int EnumProperty	=4;
    //@}
    
    /*! \name RTMaps Type Filters
     */
    //@{
    const MAPSTypeFilterBase FilterStructure	= {MAPS::NoMask,MAPS::NoType,MAPS::Structure};			    //!< Filters any user-defined structure type
    const MAPSTypeFilterBase FilterInteger	= {MAPS::NoMask,MAPS::NoType,MAPS::Integer};				    //!< Filters integer type (same as FilterIntegers)
    const MAPSTypeFilterBase FilterInteger64	= {MAPS::NoMask,MAPS::NoType,MAPS::Integer64};			    //!< Filters 64 bits integer type (same as FilterIntegers64)
    const MAPSTypeFilterBase FilterFloat	= {MAPS::NoMask,MAPS::NoType,MAPS::Float};				    //!< Filters MAPSFloat type (same as FilterFloats)
    const MAPSTypeFilterBase FilterNumber	= {MAPS::NoMask,MAPS::NoType,MAPS::Float|MAPS::Integer|MAPS::Integer64};    //!< Filters integer or MAPSFloat type (same as FilterNumbers)
    const MAPSTypeFilterBase FilterIntegers	= {MAPS::NoMask,MAPS::NoType,MAPS::Integer};				    //!< Filters integer scalars or vectors
    const MAPSTypeFilterBase FilterIntegers64	= {MAPS::NoMask,MAPS::NoType,MAPS::Integer64};			    //!< Filters 64 bits integer scalars or vectors
    const MAPSTypeFilterBase FilterFloats	= {MAPS::NoMask,MAPS::NoType,MAPS::Float};				    //!< Filters MAPSFloat scalars or vectors
    const MAPSTypeFilterBase FilterNumbers	= {MAPS::NoMask,MAPS::NoType,MAPS::Float|MAPS::Integer|MAPS::Integer64};    //!< Filters integer or MAPSFloat scalars or vectors
    const MAPSTypeFilterBase FilterOneInteger	= {MAPS::TypeMask|MAPS::VectorFlag,MAPS::Integer,MAPS::AnyType};	    //!< Filters integer type (excludes vectors of integers)
    const MAPSTypeFilterBase FilterOneInteger64	= {MAPS::TypeMask|MAPS::VectorFlag,MAPS::Integer64,MAPS::AnyType};	    //!< Filters 64 bits integer type (excludes vectors of integers64)
    const MAPSTypeFilterBase FilterOneFloat	= {MAPS::TypeMask|MAPS::VectorFlag,MAPS::Float,MAPS::AnyType};		    //!< Filters MAPSFloat type (excludes vectors of MAPSFloat)
    const MAPSTypeFilterBase FilterOneNumber	= {MAPS::VectorFlag,MAPS::NoType,MAPS::Float|MAPS::Integer|MAPS::Integer64};//!< Filters integer or MAPSFloat type (excludes vectors)
    const MAPSTypeFilterBase FilterTextAscii	= {MAPS::NoMask,MAPS::NoType,MAPS::TextAscii};				    //!< Filters ASCII text string
    const MAPSTypeFilterBase FilterTextUnicode	= {MAPS::NoMask,MAPS::NoType,MAPS::TextUnicode};			    //!< Filters Unicode (16 bits) text string
    const MAPSTypeFilterBase FilterImage	= {MAPS::NoMask,MAPS::NoType,MAPS::IplImage|MAPS::MAPSImage};		    //!< Filters IplImages or MAPSImages
    const MAPSTypeFilterBase FilterIplImage	= {MAPS::NoMask,MAPS::NoType,MAPS::IplImage};				    //!< Filters IplImages
    const MAPSTypeFilterBase FilterMAPSImage	= {MAPS::NoMask,MAPS::NoType,MAPS::MAPSImage};				    //!< Filters MAPSImages
    const MAPSTypeFilterBase FilterCANFrame	= {MAPS::NoMask,MAPS::NoType,MAPS::CANFrame};				    //!< Filters CANFrames
    const MAPSTypeFilterBase FilterMatrix	= {MAPS::NoMask,MAPS::NoType,MAPS::Matrix};				    //!< Filters MATLAB-Like matrices
    const MAPSTypeFilterBase FilterRealObjects	= {MAPS::NoMask,MAPS::NoType,MAPS::RealObject};				    //!< Filters RTMaps Real Objects
    const MAPSTypeFilterBase FilterDrawingObjects= {MAPS::NoMask,MAPS::NoType,MAPS::DrawingObject};			    //!< Filters RTMaps Drawing Objects
    const MAPSTypeFilterBase FilterTriangles3D	= {MAPS::NoMask,MAPS::NoType,MAPS::Triangles3D};			    //!< Filters 3D triangles
    const MAPSTypeFilterBase FilterStream8	= {MAPS::NoMask,MAPS::NoType,MAPS::Stream8};				    //!< Filters 8-bit data streams
    const MAPSTypeFilterBase FilterStream16	= {MAPS::NoMask,MAPS::NoType,MAPS::Stream16};			    //!< Filters 16-bit data streams
    const MAPSTypeFilterBase FilterStream32	= {MAPS::NoMask,MAPS::NoType,MAPS::Stream32};			    //!< Filters 32-bit data streams
	const MAPSTypeFilterBase FilterAudioSignal = {MAPS::NoMask,MAPS::NoType,MAPS::Float|MAPS::Stream8};		//!< Filters audio signals (either MAPSFloat or Stream8)
	const MAPSTypeFilterBase FilterAny		= {MAPS::NoMask,MAPS::NoType,MAPS::AnyType};				    //!< Filters any kind of data
    //@}
    
    /* \name RTMaps options flags
    */
    //@{
    //const int NoFolderOptionFlag	= 0x00000008;	// The database for this session is not put in a new folder
    //const int CurrentDirectoryOptionFlag= 0x00000010;	// If set, RTMaps doesn't change the current directory
    //const int DebugOptionFlag		= 0x00000001;	// The debug mode
    //const int NoLoggingOptionFlag	= 0x00000040;	// If set, no log file, no database will be created by RTMaps
    //const int HaltOptionFlag		= 0x00000080;	// If set, RTMaps will be halted on start (timespeed=0)
    //@}
    
    /*! \name RTMaps kinds of information outputs
    */
    //@{
    const int Info	= 0;	//!< Simple information
    const int Warning	= 1;	//!< Warning
    const int Error	= 2;	//!< Error	
    const int ParserEcho= 3;	//!< Echo from the parser
    //@}
    
    /*! \name RTMaps replay modes
    */
    //@{
    const int ReplayModeNormal	    = 0;    //!< Normal replay
    const int ReplayModeImmediate   = 1;    //!< Immediate replay mode (replay ahead of real time)
    const int ReplayModeTimestamp   = 2;    //!< Replay using timestamp instead of time of issue
    //@}
    
    /*! \name RTMaps VCR Keys codes
    */
    //@{
    const int VCRKeyStop    = 0x00000002;
    const int VCRKeyPlay    = 0x00000001;
    const int VCRKeyRecord  = 0x00000010;
    const int VCRKeyPause   = 0x00000020;
    const int VCRKeyRewind  = 0x00000004;
    const int VCRKeyForward = 0x00000008;
    const int VCRKeyNext    = 0x00000040;
    const int VCRKeyOrganize= 0x00000080;
    const int VCRSlider	    = 0x00000100;
    const int VCRAllKeys    = 0x0000FFFF;
    //@}
    /*! \name RTMaps Engine Keys codes
    */
    //@{
    const int KernelKeyRun	= 0x00000200;
    const int KernelKeyShutdown	= 0x00000400;
    const int DefaultKeysState	= 0x00000600;
    //@}

    /*! \name Others RTMaps Constants */
    //@{
    extern const MAPSString OperatingSystem;//!< Contains the operating system string information ("Win32","QNX" or "Linux" for instance)
    extern const int OSBuild;		    //!< Contains the OS support build number information
    extern const MAPSString Distribution;   //!< Contains the distribution information string ("3.0" for instance)
    extern const MAPSString KernelVersion;  //!< Contains the engine version string information ("1.0" for instance)
	extern const MAPSString RTMapsMinorVersion; //!< Contains the RTMaps minor version (changes in the minor versions preserve backward compatibility with packages). "5" for example -> complete version string will look like "3.0.5".
    extern const int KernelBuild;	    //!< Contains the engine build number information
    extern const MAPSString Copyright;	    //!< Contains the RTMaps copyright string information
    extern const MAPSString License;	    //!< Contains the RTMaps license grant string information (depends on the customer)
    extern const MAPSString ProductName;
    //! Tells if we are running on a big-endian platform (\c true) or a little-endian platform (\c false)
#ifdef MAPS_BIG_ENDIAN
    const bool BigEndian=true;
#else
    const bool BigEndian=false;
#endif
    
    const int DefaultTextSize	= 1024;		//!< Default allocation size (in characters) for ascii text type outputs
    const int Infinite		=-1;		//!< Infinite number
    
    const int ModuleDied	=0xdead;	//!< State indicating a dead RTMaps module
    const int GotAMessage	=-2;		//!< State indicating that RTMaps got a Windows message
    const int TimeOut		=-3;		//!< State indicating a time out in RTMaps
    const int FatalKernelError	=-4;		//!< State indicating a fatal engine error
    const int ErrorException	=-5;		//!< State indicating an error in an RTMaps module
    
    const int Running		= 0x01;		//!< State telling that RTMaps is running (that time is flowing)
    const int Paused		= 0x02;		//!< State telling that RTMaps is in pause mode
    const int ShuttingDown	= 0x04;		//!< RTMaps is currently shutting down
    const int Resetting		= 0x08;		//!< RTMaps is currently resetting
    const int WaitingForSynchBeforeRun= 0x10;	//!< RTMaps is currently waiting to be synchronized before running.
   
    const int DeadState		= 0;		//!< State indicating a dead thread or module
    const int DyingState	= 1;		//!< State indicating a dying thread or module
    const int LivingState	= 2;		//!< State indicating a living thread or module
    
    const int Threaded		= 1;		//!< The component is threaded
    const int Sequential	= 2;		//!< The component is sequential
    
    //@}
    
    /*! \name RTMaps Main functions
     */
    //@{
    //! Required after any use of RTMaps...
    void
    Exit();
    //! Starts the execution of the current session
    bool
    Run();
    //! Shutdowns the RTMaps session currently running
    bool
    Shutdown();
    //! Resets the RTMaps system
    bool
    Reset();
    //! Parses a string containing MAPSScript instructions
    bool
    Parse(const char* s);
    //! Parses a file containing MAPSScript instructions
    bool
    ParseFile(const char* s);
    
    //! Loads a core function and activates it
    void
    LoadCoreFunction(const char* cf);
	//! Returns a pointer to the core function named \c cf if it was previously loaded (returns NULL otherwise)
	MAPSCoreFunctionInterface*
	CoreFunction(const char* cf);
    //! Loads a package (set of components compiled as a shared object).
    void
    RegisterPackage(const char* fileName);
    //! Unloads a package (set of components compiled as a shared object).
    void
    UnregisterPackage(const char* fileName);
    
    //! Is there a RTMaps system currently running?
    bool
    IsRunning();
    //! Is the RTMaps clock in paused state ?
    bool
    IsPaused();
    //! Returns true if a file is open for replay
    bool
    IsReplaying();
    //! Returns the level of structure control in the current system (0=no control, fastest, 2=max control, slowest)
    int
    CheckLevel();
    //@}
    /*! \name RTMaps functions for distribution and synchronization
     */
    //@{
    //! Is RTMaps acting as a Master on a network of distributed RTMaps.
    bool
    IsDistributedAsMaster();
    //! Is RTMaps acting as a Slave on a network of distributed RTMaps.
    bool
    IsDistributedAsSlave();

    //! Requests a pointer on the synchronizer object
    /*! When the module no more needs the synchronizer ability, it has to call MAPS::ReleaseSynchronizer
     *  \param owner Pointer to the MAPSModule requesting the synchronizer ability
     *  \param ppSynchronizer Pointer to the place where the synchronizer object pointer will be stored 
     *         if the function succeeds. If the function fails, it is set to \c NULL.
     *  \return \c true if the function succeeds, \c false if the function fails 
     *          (i.e. the synchronizer object is already held by someone else).
    */
    bool
    GetSynchronizer(void* owner,
		    MAPSSynchronizer** ppSynchronizer);
    //! Releases a pointer on the synchronizer object
    bool
    ReleaseSynchronizer(void* module,
			MAPSSynchronizer** ppSynchronizer);
    //@}
    MAPSTimestamp
    TimeReferenceX();
    /*! \name RTMaps Virtual Time management functions
     */
    //@{
    //! Gets the time reference (the absolute time the timestamps always refer to)
    MAPSAbsoluteTime&
    TimeReference();
    //! Gets the current time in the RTMaps system (virtual time)
    MAPSTimestamp
    CurrentTime(bool lock=true,
		bool release=true);
    //! Sets the current time (jumps in time!)
    void
    SetCurrentTime(MAPSTimestamp t);
    //! Sets the current time speed (1000 = real time)
    void
    SetTimeSpeed(int speed);
    //! Gets the current time speed (1000 = real time)
    int
    TimeSpeed();
    //! Gets the current time state (MAPS::Running or MAPS::Paused)
    int
    TimeState();
    //@}
    
    void
    Play();
    void
    Stop();
    void
    Pause();
    void
    Record();
    void
    Next();
    void
    Previous();
    
    /*! \name RTMaps Index management
     */
    //@{
    //! Adds an index now (during recording)
    void
    SetIndex();
    //! Goes to index \a num during replay
    void
    Go2Index(int num);
    //! Goes to previous index (during replay)
    void
    Go2PreviousIndex();
    //! Goes to next index (during replay)
    void
    Go2NextIndex();
    //@}
    
    /*! \name RTMaps Recording management
     */
    //@{
    //! Starts recording information (starts all)
    void
    StartRecording(void);
    //! Stops recording information (stops all)
    void
    StopRecording(void);
    //@}
    
    /*! \name RTMaps Type Filtering management
     */
    //@{
    /*! Passes a type through a filter type
     *	\param outputType type to pass through the filter, generally the type of an output
     *	\param filter filter to use
     *	\return returns true when <I>filter</I> filters out <I>outputType</I>
     *	For RTMaps type filtering behaviour, see the documentation of MAPSTypeFilterBase.
     *
     *	\sa MAPSTypeInfo MAPSTypeFilterBase
     */
    bool
    TypeFilter(const MAPSTypeInfo& outputType,
	       MAPSTypeFilterBase& filter);
    //@}
    bool
    TypeFilter(const MAPSTypeInfo& outputType,
	       const MAPSTypeFilterBase& filter); // For const type filters
    /*! \name RTMaps General purpose functions
     */
    //@{
    //! Creates a model of an operational IplImage structure with the provided parameters.
    /*! The generated model can be thus used with MAPSOutput::AllocOutputBufferIplImage
     *	\sa MAPSOutput::AllocOutputBufferIplImage
     */
    ::IplImage
    IplImageModel(int width,
		  int height,
		  unsigned int channelSeq=MAPS_CHANNELSEQ_BGR,
		  unsigned int dataOrder=IPL_DATA_ORDER_PIXEL,
		  unsigned int depth=IPL_DEPTH_8U);
    ::IplImage
    IplImageModel(int width,
		  int height,
		  const char* channelSeq,
		  unsigned int dataOrder=IPL_DATA_ORDER_PIXEL,
		  unsigned int depth=IPL_DEPTH_8U);
    //! Checks that an image is the same type and size as the model given in second parameter
    bool
    IplImageCheck(::IplImage& image,
		  ::IplImage& model);
    //@}
    
    /*! \name RTMaps Maintenance functions
     */
    //@{
    /*! The returned string contains information on the RTMaps version and the list of all
     *	the registered modules with their respective versions.
     */
    MAPSString
    About(void);
    //@}
    
    /*! \name RTMaps Reporting functions
     *
     *  For all these functions, the importance must be set between 0 (not important) to 2 (of the utmost importance)
     */
    //@{
    /*! Only for use by programs out of the RTMaps system. For RTMaps components, please use the MAPSComponent::Report function.
     *	\sa MAPSComponent::Report
     */
    //! Reports a piece of information
    void
    ReportInfo(const char* text,
	       int importance=0);
    //! Reports a warning
    void
    ReportWarning(const char* text,
		  int importance=0);
    //! Reports an error
    void
    ReportError(const char* text,
		int importance=0);
    //! Reports something (with user feedback + logging)
    /*! \param type The report type. Can be MAPS::Info, MAPS::Warning or MAPS::Error.
     *	\param text The message to report to the user
     *  \param importance Unused
     */
    void
    Report(const char* text,
	   int type,
	   int importance=0);
    //! Displays a modal MessageBox. To be used for debugging only since this function blocks until the user 
    /*! \param message The message to be displayed in the message box
	\param type The type can be MAPS::Info, MAPS::Warning, or MAPS::Error
	\param importance Unused
    */
    void
    MessageBox(const char* message,
	       int type,
	       int importance=0); // OS dependent
    
    //@}
    
    //! \name Miscellaneous functions
    //@{
    MAPSTimestamp
    TimestampFromString(const char* s, char** endptr=NULL);
    MAPSString
    TimeString(MAPSTimestamp t);
    MAPSString
    Timestamp2String(MAPSTimestamp t);
    //@}

    /*! \name RTMaps Flow monitoring functions
     */
    //@{
    //! Gets the detailed statistics about the data flow to the hard disks (write file operations).
    MAPSString GetWriteStatistics();
    //! Gets the detailed statistics about the read file operations.
    MAPSString GetReadStatistics();
    //! Gets the overall remaining recording time, if the flows stays constant.
    int GetRemainingTime();
    //! Gets the total write flow (file write operations)
    MAPSInt64 GetWriteFlow();
    //! Gets the total read flow (file read operations)
    MAPSInt64 GetReadFlow();
    //! Notifies RTMaps that such a memory write flow has occured. Do not use in conjonction with MAPS::Memcpy(...)
    void RecordMemoryWriteFlow(MAPSInt64 size);
    //! Notifies RTMaps that such a memory read flow has occured. Do not use in conjonction with MAPS::Memcpy(...)
    void RecordMemoryReadFlow(MAPSInt64 size);
    //! Gets the total memory read flow
    MAPSInt64 GetMemoryReadFlow();
    //! Gets the total memory write flow
    MAPSInt64 GetMemoryWriteFlow();
    //! Gets the total memory flow (read+write)
    MAPSInt64 GetMemoryFlow();
    //! Gets the free disk space on the disk containing <CODE>path</CODE>.
    MAPSInt64 GetDiskFreeSpace(const char* path);
    //@}
        
    /*! \name RTMaps OS wrapping functions
     *	These functions give access to all the needed features of an OS except file I/O.
     *	These functions should be called instead of their OS-specific counterparts to
     *	ensure a cross-platform programming.
     */
    //@{
    //! Gets an immediate timestamp (in microseconds).
    /*!	Note that you must generally use MAPS::CurrentTime() in your own components, since
     *	the GetSystemAccurateTiming function returns a timestamp in real time, not in the RTMaps
     *	virtual time. Furthermore, the timestamp returned by this function has no reference.
     *	It is the RTMaps engine that uses this OS wrapping function to get some accurate
     *	timings and translates them to a referenced and scaled virtual time.
     */
    MAPSTimestamp
    GetSystemAccurateTiming(void);
    //! Starts a thread
    bool
    CreateThread(void *(*startAdress)(void*),
		 void* argList);
    //! Sets the priority of the current thread (between 0 and 255)
    void
    SetCurrentThreadPriority(int priority);
    //! Gets an id for the current thread
    MAPSThreadId
    GetCurrentThreadId();
    //! Releases the current thread.
    /*! Gives up its remaining time slice.
     *	This may optimize the behaviour of a component and it is used in the RTMaps engine
     *	to optimize threads switching.
     */
    void
    ReleaseCurrentThread(void);				
    //! Sleeps for a certain amount of time
    /*! The current thread is released for a delay of <CODE>delay</CODE> microseconds. Note that it is not
     *	considered to be an accurate function. For instance, in the Windows implementation of
     *	RTMaps, the delay precision is about 40ms.
     *
     *	Note that you must generally use MAPSComponent::Rest() or MAPSComponent::Wait() in your own components, 
     *	since the MAPS::Sleep function works in real time, not in the RTMaps
     *	virtual time. Furthermore, MAPSComponent::Rest() and MAPSComponent::Wait() are
     *	much more accurate.
     */
    void
    Sleep(MAPSDelay delay);
    //! Waits for a while. Useful little function.
    void
    Wait4AWhile(void);
    //! Gets the absolute current time
    void
    GetAbsoluteTime(MAPSAbsoluteTime* time);
    //! Gets the current directory path
    MAPSString
    GetCurrentDirectory();
    //! Sets the current directory path
    bool
    SetCurrentDirectory(const MAPSString path);
    //! Creates a directory
    bool
    CreateDirectory(const char* dirName);
    //! Gets a path to the temporary directory
    MAPSString
    GetTempDirectory();
    //! Gets a path refering to RTMaps installation.
    /*! \param pathName Specifies the path to retrieve. Can be "mapspath", "jrepath", "classpath" for java classes, "docpath", "xmlpath".
     */
    MAPSString
    GetInstallPath(const char* pathName);	
								
    //! Transforms an absolute time to an integer, for absolute time comparison
    /*! This is not as simple as it seems to be, since there are lots of parameters to that,
     *	length of months, year 2000 compatibility, etc. We rely on the OS to compute this.
     */
    MAPSInt64
    AbsoluteTime2Integer(MAPSAbsoluteTime& time);
    //! Transforms an integer to an absolute time
    void
    Integer2AbsoluteTime(MAPSInt64 integer,
			 MAPSAbsoluteTime& time);
    //! Memory allocation. Assumes it is allocated in a way that it can be accessed by a RTMaps system running in another process
    void*
    AllocSharedMemory(int size);
    //! Memory deallocation. Assumes it was allocated in a way that it can be accessed by a RTMaps system running in another process	
    void
    DeallocSharedMemory(void* ptr);
    //@}
    
    /*! \name RTMaps C++ interface functions
     *	These functions are designed to control the RTMaps engine directly through C++ calls.
     *	These are rather low-level functions that are not of any interest for a component developer.
     */
    //@{
    //! Gets a boolean property
    bool
    GetBoolProperty(const char* s,
		    bool* ok=NULL);
    //! Gets an integer property or enum property selected index
    MAPSInt64
    GetIntegerProperty(const char* s,
		       bool* ok=NULL);
    //! Gets a string property or enum property selected string
    MAPSString
    GetStringProperty(const char* s,
		      bool* ok=NULL);
    //! Gets a float property
    MAPSFloat
    GetFloatProperty(const char* s,
		     bool* ok=NULL);
    //! Gets an enum property.
    MAPSEnumStruct
    GetEnumsProperty(const char* s,
		     bool* ok=NULL);
    //! Returns a pointer to component <CODE>s</CODE> (returns NULL if it does not exist)
    MAPSComponent*
    Component(const char* s);
    //! Returns a pointer to the property named <CODE>s</CODE> (returns NULL if it does not exist)
    MAPSProperty*
    Property(const char* s);
    
    //! Component instantiation
    MAPSComponent*
    CreateComponent(const char* modelName,
		    const char* componentName,
		    bool start=true);
    //! Starts the component if frozen
    void
    StartComponent(const char* componentName);
    //! Dynamically destroys a component
    void
    KillComponent(const char* componentName);
    //! Dynamically destroys a component
    void
    KillComponent(MAPSComponent& component);
    //! Renames a component
    void
    RenameComponent(const char* componentName,
		    const char* newName);
    //! Dynamically attaches an input to an output
    void
    Attach(MAPSOutput& output,
	   MAPSInput& input);
    //! Dynamically attaches an input to an output
    void
    Attach(const char* outputName,
	   const char* inputName);
    //! Dynamically attaches an input to an output. Extended version.
    /*! This new version of attach allows the <CODE>name</CODE> parameter to
    *	be the name of an output OR the <CODE>name</CODE> parameter of the type of
    *	an output. This allows dynamic connection using a plain name, not a technical
    *	output name.
    *
    *	This function is designed to be used in components that automatically attach
    *	to some outputs.
    */
    bool
    Attach2(const char* name,
	    const char* inputName);
    //! Dynamically detaches an output from an input
    void
    Detach(MAPSOutput& output,
	   MAPSInput& input);
    //! Records an output
    void
    Record(const char* outputName,
	   const char* recorder=NULL,
	   const char* method=NULL,
	   bool neverskipping=false,
	   bool useTimestamp=false);
    //! Opens a database to replay records from
    void
    Open(const char* pattern,
	 const char* nspace=NULL,
	 MAPSInt64 offset=0,
	 MAPSTimestamp beginning=0,
	 MAPSTimestamp end=0);
    //! Replays some data
    void
    Replay(const char* outputname);
    //! Copies some data
    void
    Copy(const char* outputname,
	 const char* recorderName);
    //! Aborts the copy of an output.
    void
    StopCopy(const char* outputname);
    //! Sets the time automatic adaptation algorithms
    void
    SetTimeAdapt(int ta);
    //! Returns the first timestamp of all the currently open databases
    MAPSTimestamp
    GetFirstTimestamp();
    //! Returns the last timestamp of all the currently open databases
    MAPSTimestamp
    GetLastTimestamp();
    //! Gets a pointer to the shutdown event
    MAPSEvent*
    GetShutdownEvent();
    //@}
    
    // RTMaps Semi-public Data
    // These are declared public just because they are called from parts of engine
    // written in C, not C++ (parts concerning lex & yacc)
    // These functions mustn't be called by a user and thus are not documented
    void
    Echo(const MAPSString& s);
    void
    SyntaxError(const char* s);
};

class MAPSCondition
{
protected:
    MAPSMutex* mutex;
    MAPSEvent* abortEvent;
    struct MAPSEventInPool
    {
	MAPSEvent event;
	bool available;
    };
    MAPSList<MAPSEventInPool> pool;
    MAPSDelay timeout;
public:
    MAPSCondition(MAPSMutex* mut,
		  MAPSEvent* abort,
		  MAPSDelay to=MAPS::Infinite);
    void
    Wait();
    void*
    WaitX(MAPSEvent** event);
    void
    AchieveWaitX(void* handle);
    void
    Signal();
    void
    Abort();
};

//! The RTMaps advanced mutex class.
/*! This mutex has a reference counter and knows which thread owns it.
 *	Therefore, a thread can lock it several times without deadlocking it!
 */
class MAPSMutex2 : public MAPSMutex
{
private:
	MAPSMutex hlMutex; // Higher level monitor
	volatile int owner;
	volatile int nbRef;
	volatile bool requested;
public:
	//! Standard constructor. No options.
	MAPSMutex2() {nbRef = 0; requested = false;};
	//! Locks the mutex
	void
	Lock();
	//! Tries to lock the mutex. If it is already locked by somebody else, returns \c false immediatly.
	bool
	TryLock();
	//! Releases all references on the mutex (if any)
	/*! Very useful when an exception occured and we are not sure about the number
	 *	of references really held by the thread.
	 */
	void
	ReleaseAll();
	//! Releases the mutex
	void
	Release();
};

inline void
MAPSMutex2::Lock()
{
    hlMutex.Lock();
    int id=MAPS::GetCurrentThreadId();
    if ((nbRef)&&(id==owner)) {
	++nbRef;
    } else {
	requested=true;
	hlMutex.Release();
	MAPSMutex::Lock();
	hlMutex.Lock();
	requested=false;
	owner=id;
	nbRef=1;
    }
    hlMutex.Release();
}

inline void
MAPSMutex2::ReleaseAll()
{
    hlMutex.Lock();
    int id=MAPS::GetCurrentThreadId();
    if ((nbRef)&&(id==owner)) {
	MAPSMutex::Release();
	nbRef=0;
    }
    hlMutex.Release();
}

inline void
MAPSMutex2::Release()
{
    hlMutex.Lock();
    --nbRef;
    if (nbRef==0) {
	MAPSMutex::Release();
    }
    hlMutex.Release();
}

inline bool
MAPSMutex2::TryLock()
{
    bool result;
    hlMutex.Lock();
    if (requested) {
	result=false;
    } else {
	int id=MAPS::GetCurrentThreadId();
	if (nbRef) {
	    if (id==owner) {
		++nbRef;
		result=true;
	    } else {
		result=false;
	    }
	} else {
	    MAPSMutex::Lock();
	    owner=id;
	    nbRef=1;
	    result=true;
	}
    }
    hlMutex.Release();
    return result;
}

#ifdef USE_COMPATIBILITY_V2
// A monitor is a mutex.
#define MAPSMonitor MAPSMutex
#define MAPSMonitor2 MAPSMutex2
#endif

//! The RTMaps File I/O support class
/*! Provides the functions that any RTMaps module MUST use so that it can
 * properly run in the RTMaps environment. Please forget about fopen and fclose stuff.
 */
class MAPSFileIO
{
protected:
    /*! \name Fast file write functions
     *	These functions take full advantage of the underlying OS and they
     *	collect automatically some performance data that are very important
     *	for the fine tuning of a RTMaps system.
     *	Please never use the functions provided by the OS for file I/O.
     */
    //@{
    //! Opens a file for writing
    /*!	\param name The file name to create or open
     *	\param options Can be a combination of the following flags :
     *		- Asynchronous : Sets an asynchronous mode of operation.
     *			This means that all write operations will return immediately, before
     *			the write operation has really finished. With this mode of operation, you
     *			must specify an element to unlock (parameter elt2Unlock) to any write operation,
     *			so that RTMaps can know when the data can be disposed of.
     *		- NoBuffering : Sets a no buffering mode of operation.
     *			This mode is very fast on Windows NT/2000 for huge transfers to disk.
     *			It does not use any cache (so saves many memory transfers) but puts
     *			the data directly on the disk. Note that the size of the data must
     *			be proportional to the size of sector on the disk.
     *		- Append : Tells not to overwrite the file if it already exists, but to
     *			append any new data to the end of the file.
     *		- RandomAccess : Optimization flag. Specifies that the file will be accessed from any point.
     *		- SequentialAccess : Optimization flag. Specifies that the file will always be accessed sequentially.
     *	\return The file handle that can be used with fileWriteXXX functions. Returns \c NULL is the file creation or opening failed.
     *
     *	Note that this function also opens the file for reading too.
     */
    MAPSFileWriteHandle*
    FileOpen4Writing(const char* name,
		     int options=0);
    //! Writes data to a file
    /*! \param fileHandle Specifies the file to write to
     *	\param buffer Specifies where the data must be taken from
     *	\param size The size of data to write
     *	\param elt2Unlock The function automatically calls MAPSFileIO::Unlock with this parameter
     *         when the file write has finished. Should be \c NULL with synchronous write operations.
     *	\return \c true if the operation has succeeded, \c false otherwise.
     */	
    bool
    FileWrite(MAPSFileWriteHandle* fileHandle,
	      const void* buffer,
	      int size,
	      void* elt2Unlock=NULL);
    //! Writes text data to a file
    /*!	\param fileHandle Specifies the file to write to
     *	\param text The string to write to the file
     *	\param mayContainCarriageReturns Indicates that the string may contains '\\n' characters. If so, RTMaps
     *	       will convert all '\\n' characters to the MAPSFileIO::Endl string which depends on the underlying operating
     *	       system, so that the resulting text file can be read with a text editor. If you do know that your file
     *	       does not contain '\\n' characters, set this parameter to false and RTMaps will spare some time as it will not
     *	       try to find out the '\\n' in your string.
     *	\return \c true if the operation has succeeded, \c false otherwise.
     */
    bool
    FileWriteText(MAPSFileWriteHandle* fileHandle,
		  const MAPSString& text,
		  bool mayContainCarriageReturns=true);
    //! Closes the file
    /*!	\param fileHandle Specifies the file to close
     */
    void
    FileClose(MAPSFileWriteHandle* fileHandle);
    //@}

    /*! \name File read functions
     *	(That should have read-ahead capability)
     *
     *	All these functions work synchronously. No asynchronous mode is supported for reading operations.
     */
    //@{
    //! Opens a file for reading
    /*!	\param name The name of the file to open
     *	\param options Can be a combination of the following flags :
     *		- RandomAccess : Optimization flag. Specifies that the file will be accessed from any point.
     *		- SequentialAccess : Optimization flag. Specifies that the file will always be accessed sequentially.
     *	\return The file handle that can be used with fileReadXXX functions. Returns \c NULL is the file does not exist.
     */	
    MAPSFileReadHandle*
    FileOpen4Reading(const char* name,
		     int options=0);
    //! Reads some data from the file
    /*! \param fileHandle Specifies the file to read from
     *	\param buffer Specifies the buffer to transfer the read bytes to
     *	\param size Max size of data to read
     *	\param nbRead Pointer to a variable that will get the resulting number of read bytes.
     *		If set to \c NULL, the function will not return this information. If <var>*nbRead</var> is 0 and the
     *		function returns \c true, then the end of file was reached before the call to the function.
     *	\return Returns \c false if an error was detected. Note that if the end of file is reached,
     *		this function returns \c true, but the \a nbRead result may be less than \a size.
     */
    bool
    FileRead(MAPSFileHandle* fileHandle,
	     void* buffer,
	     int size,
	     int* nbRead=NULL);
    //! Closes the file
    /*!	\param fileHandle Specifies the file to close
     */
    void
    FileClose(MAPSFileReadHandle* fileHandle);
    //@}

    /*! \name General purpose functions
     */
    //@{
    //! Sets the file pointer.
    bool
    FileSetPos(MAPSFileHandle* fileHandle,
	       MAPSInt64 pos);
    //! Moves to another position in the file, relative to the current position.
    bool
    FileMovePos(MAPSFileHandle* fileHandle,
		int move);
    //! Returns the current position of the file pointer in the file.
    MAPSInt64
    FileGetPos(MAPSFileHandle* fileHandle);
    //! Returns the next line in the text file
    /*!	Removes the line feed/carriage return character from the read line of text.
     */
    bool
    FileNextLine(MAPSFileHandle* fileHandle,
		 MAPSString& record,
		 MAPSInt64* oldPos=NULL);
    //! Returns the previous line in the text file
    /*!	Removes the line feed/carriage return character from the read line of text.
     */
    bool
    FilePreviousLine(MAPSFileHandle* fileHandle,
		     MAPSString& record,
		     MAPSInt64* oldPos=NULL);	
    //! Finds a string in a file
    bool
    FileFind(MAPSFileHandle* fileHandle,
	     const MAPSString& lookFor,
	     char delimiter=0,
	     MAPSInt64* pos=NULL);
    //! Finds a string in a file 
    bool
    FileFind(MAPSFileHandle* fileHandle,
	     const MAPSString& lookFor,
	     MAPSString& result,
	     char delimiter=0,
	     MAPSInt64* pos=NULL);
    //! Rewinds the file.
    bool
    FileRewind(MAPSFileHandle* fileHandle);
    //! Goes to the end of the file.
    bool
    FileGo2End(MAPSFileHandle* fileHandle);
    //! Returns the size of the file.
    MAPSInt64
    FileSize(MAPSFileHandle* fileHandle);
    //@}

    /*! \name Error management functions
     */
    //@{
    //! Called when a write error occurs. Gets an explanation to the OS and reports it.
    void
    FileWriteError();
    //! Called when a read error occurs. Gets an explanation to the OS and reports it.
    void
    FileReadError();
    //@}

    //! Unlocking function, for asynchronous I/O.
    /*! Default implementation : do nothing
     */
    virtual void
    Unlock(void* elt) {};

public:
    virtual
    ~MAPSFileIO() {}
    /*! \name File I/O flags
     *  Constants to use in MAPSFileIO::OpenFile4Writing and MAPSFileIO::OpenFile4Reading
     */
    //@{
    static const int Asynchronous;		//!< Sets an asynchronous mode of operation. 
    static const int NoBuffering;		//!< Sets a no buffering mode of operation. The data are sent directly to disk. No caching.
    static const int Append;			//!< Opens the file in append mode. It means that it won't destroy the previous content if the fils existed.
    static const int RandomAccess;		//!< Specifies that the file will be accessed from any point. Optimizes the cache management for random access.
    static const int SequentialAccess;		//!< Specifies that the file will always be accessed sequentially. Optimizes the cache management. (Random access is still possible but slower)
    //@}

    //! End of line sequence for text files.
    /*! The presence of the carriage return ('\\r') character depends on the OS.
     *	This sequence of characters defines the behaviour for text files in RTMaps.
     *
     *  For linux and QNX, this is set to '\\n'.
     *
     *	For the Windows operating systems, this is set to '\\r\\n'
     */
    static const char* Endl;
};

//! The RTMaps I/O Buffer Element
/*! This class is of the highest interest for all RTMaps developers.
 *	It's the basic element of communication between RTMaps modules.
 */
class MAPSIOElt {
public:
    //! Returns a pointer to the data contained in the buffer element
    /*!	Beware : Do not set this pointer yourself. Please let RTMaps set it itself during
     *	buffer allocation. The setting is allowed only for "expert" cases
     */
    virtual void*&
    Data()=0;
    //! Returns the time of issue of the I/O element.
    /*! The time of issue is the timestamp corresponding to the instant when the element was issued by the writing component.
     *	This might not be the same number as the timestamp of the data itself
     *	(which can be older and may have flowed through several components, or which may have been time stamped by a hardware device).
     *
     *	Not that you should not set this variable, since the time of issue time stamping is done automatically by
     *	the RTMaps system itself. In fact, setting this parameter can be useful in very "expert" cases.
     *
     */
    virtual MAPSTimestamp&
    TimeOfIssue()=0;
    //! Gets/Sets the Timestamp of the data contained in the buffer element
    /*! The timestamp is the real data time stamping. It is generally set by the acquisition component
     *	that grabbed the data.
     *
     *	If you do generate some data in your own components, you can time stamp them
     *	at your will, for instance by using "IOElt->Timestamp()=MAPS::CurrentTime()". If you
     *	do not timestamp yourself your data, RTMaps will do it for you and the timestamp
     *	will match the time of issue timestamp (i.e. the "IOElt->Timestamp()=MAPS::CurrentTime()"
     *	is executed within the call to StopWriting).
     *
     *	If you process some data in you own components, do not forget to propagate the
     *	timestamp of the incoming data, generally by writing "IOEltWriting->Timestamp()=IOEltReading->Timestamp()".
     *	This is very important, because if you never do that, you will not be able to trace back the
     *	origin of your data in the data flow.
     */
    virtual MAPSTimestamp&
    Timestamp()=0;
    //! Gets/Sets the variable size of a vector (must be less than the allocated size (the max size) returned by MAPSIOElt::Size()
    /*!	For variable length vectors, this function allows to set or retrieve the actual size of the vector,
     *	as opposed to the max size of the vector (the size allocated in memory for the buffer).
     *
     *	The vector size is automatically set to Size() by RTMaps if you do not set it.
     *	This means that the vector is considered by default to be full.
     */	
    virtual MAPSInt32&
    VectorSize()=0;
    //! Returns the allocated size for the buffer element
    /*! The meaning of this info depends on the data type in the buffer :
     *
     *	- For vectors (of integers, MAPSFloat, MAPSCANFrame, MAPSRealObject), sets the max size of the vector
     *	- For text (TextAscii, TextUnicode), sets the max length of the string in the buffer
     *	- For MAPSMatrix buffers, sets the max size of the matrix, i.e. the max m*n elements in the matrix.
     *	- For IplImage buffers, retrieves the max size of the imageData zone of the IplImage (call to MAPSOutput::AllocOutputBufferIplImage).
     *	- For MAPSImage buffers, retrieves the max size of the imageData zone of the MAPSImage (call to MAPSOutput::AllocOutputBufferMAPSImage).
     *
     *	\sa MAPSOutput::AllocOutputBuffer MAPSOutput::AllocOutputBufferIplImage MAPSOutput::AllocOutputBufferMAPSImage
     */	
    virtual MAPSInt32&
    Size()=0;
    //! Returns the allocated size for the buffer element
    /*! \copydoc MAPSIOElt::Size()
     */
    virtual MAPSInt32&
    BufferSize()=0;
    //! Gets/Sets the frequency of the data contained in the buffer element
    /*! The frequency can be used by replay facilities to perfectly link data packets
     *	one to the others, when the time stamping are not accurate enough.
     *
     *	The value of frequency must be set such that 1000 means 1Hz. This implies
     *	that no signal slower than 0.001Hz can be considered, but signals with
     *	very high frequencies (GHz...) can.
     *
     *	Note that the frequency is considered as a valid value only when the flag MAPS::FrequencyFlag is set
     *	in the type specification.
     *
     *	\sa MAPSIOElt::Type
     */
    virtual MAPSInt64&
    Frequency()=0;
    //! Gets/Sets the quality of the data contained in the buffer element
    /*! The quality contains an additional information about the data, for instance
     *	a signal/noise ratio or compression ratio.
     *
     *	Note that the quality is considered as a valid value only when the flag MAPS::QualityFlag is set
     *	in the type specification.
     *
     *	\sa MAPSIOElt::Type
     */
    virtual MAPSInt32&
    Quality()=0;
    //! Returns the type of the data contained in the buffer element
    virtual MAPSTypeInfo&
    Type()=0;
    virtual int&
    BufferItemSize()=0;

    //! Gets/Sets the value of the data contained in the buffer element (must be of integer type)
    virtual MAPSInt32&
    Integer(int index=0)=0;

    //! Gets/Sets the value of the data contained in the buffer element (must be of MAPSInt64 type)
    virtual MAPSInt64&
    Integer64(int index=0)=0;

    //! Gets/Sets the value of the data contained in the buffer element (must be of MAPSFloat type)
    virtual MAPSFloat&
    Float(int index=0)=0;
    //! Gets/Sets the text content of the data contained in the buffer element (must be of TextAscii type)
    virtual char*
    Text()=0;
    //! Gets/Sets the text content of the data contained in the buffer element (must be of TextAscii type)
    virtual char*
    TextAscii()=0;
    //! Gets/Sets the text content of the data contained in the buffer element (must be of TextUnicode type)
    virtual wchar_t*
    TextUnicode()=0;
    //! Gets a reference to the data contained in the buffer element (must be of IplImage type)
    virtual ::IplImage&
    IplImage()=0;
    //! Gets a reference to the data contained in the buffer element (must be of MAPSImage type)
    virtual ::MAPSImage&
    Image()=0;
    //! Gets a reference to the data contained in the buffer element (must be of MAPSImage type)
    virtual ::MAPSImage&
    MAPSImage()=0;
    //! Gets a reference to the data contained in the buffer element (must be of MAPSCANFrame type)
    virtual MAPSCANFrame&
    CANFrame(int index=0)=0;
    //! Gets a reference to the data contained in the buffer element (must be of MAPSMatrix type)
    virtual MAPSMatrix&
    Matrix()=0;
    //! Gets a reference to the data contained in the buffer element (must be of MAPSRealObject type)
    virtual MAPSRealObject&
    RealObject(int index=0)=0;
    //! Gets a reference to the data contained in the buffer element (must be of MAPSDrawingObject type)
    virtual MAPSDrawingObject&
    DrawingObject(int index=0)=0;
    //! Gets a pointer to the stream packet contained in the buffer element (must be of MAPSTriangles3D type)
    virtual MAPSTriangles3D&
    Triangles3D()=0;
    //! Gets a pointer to the stream packet contained in the buffer element (must be of MAPSStream8 type)
    virtual MAPSUInt8*
    Stream8()=0;
    //! Gets a pointer to the stream packet contained in the buffer element (must be of MAPSStream16 type)
    virtual MAPSUInt16*
    Stream16()=0;
    //! Gets a pointer to the stream packet contained in the buffer element (must be of MAPSStream32 type)
    virtual MAPSUInt32*
    Stream32()=0;

    //! Gets/Sets additional information associated to the buffer element
    virtual MAPSInt32&
    Misc()=0;
    //! Gets/Sets additional information associated to the buffer element
    virtual MAPSInt32&
    Misc1()=0;
    //! Gets/Sets additional information associated to the buffer element
    virtual MAPSInt32&
    Misc2()=0;
    //! Gets/Sets additional information associated to the buffer element
    virtual MAPSInt32&
    Misc3()=0;

    // Automatic casts
    //! Casts the data in the buffer element to <tt>char*</tt> (only for TestAscii type data)
    virtual
    operator char*()=0;
    //! Casts the data in the buffer element to <tt>wchar_t*</tt> (only for TestUnicode type data)
    virtual
    operator wchar_t*()=0;
    //! Casts the data in the buffer element to <tt>int&</tt> (only for integer type data)
    /*! Use <tt>int& MAPSIOElt::Integer(int index=0)</tt> for integer vectors
     */
    virtual
    operator MAPSInt32&()=0;
    //! Casts the data in the buffer element to <tt>MAPSInt64&</tt> (only for MAPSInt64 type data)
    /*! Use <tt>MAPSInt64& MAPSIOElt::Integer64(int index=0)</tt> for integer64 vectors
     */
    virtual
    operator MAPSInt64&()=0;
    //! Casts the data in the buffer element to <tt>MAPSFloat&</tt> (only for MAPSFloat type data)
    /*! Use <tt>MAPSFloat& MAPSIOElt::Float(int index=0)</tt> for MAPSFloat vectors 
     */
    virtual
    operator MAPSFloat&()=0;
    //! Casts the data in the buffer element to IplImage* (only for IplImage type data)
    virtual
    operator ::IplImage*()=0;
    //! Casts the data in the buffer element to MAPSImage (only for MAPSImage type data)
    virtual
    operator ::MAPSImage&()=0;
    //! Casts the data in the buffer element to MAPSCANFram& (only for MAPSCANFrame type data)
    virtual
    operator MAPSCANFrame&()=0;
    //! Casts the data in the buffer element to MAPSMatrix& (only for MAPSMatrix type data)
    virtual
    operator MAPSMatrix&()=0;
    //! Casts the data in the buffer element to MAPSRealObject& (only for MAPSRealObject type data)
    virtual
    operator MAPSRealObject&()=0;
    //! Casts the data in the buffer element to MAPSRealObject& (only for MAPSDrawingObject type data)
    virtual
    operator MAPSDrawingObject&()=0;
    //! Casts the data in the buffer element to MAPSRealObject& (only for MAPSTriangles3D type data)
    virtual
    operator MAPSTriangles3D&()=0;

    //! Retrieves the size of the allocated buffer in bytes.
    /*! \param isMemContiguous This boolean is set to \c true if the data contained in the MAPSIOElt is kept 
     * in a contiguous memory zone. This is not the case for exemple for a MAPSIOElt containing an IplImage:
     * the imageData can be stored anywhere in memory.
     *	\return the size of the allocated buffer in bytes, or -1 if the function fails.
     */
    virtual int
    IOEltBufferSizeInBytes(bool* isMemContiguous)=0;
    //! Retrieves the size that is actually used in the allocated buffer in bytes.
    /*! \param isMemContiguous This boolean is set to \c true if the data contained in the MAPSIOElt is kept
     * in a contiguous memory zone. This is not the case for exemple for a MAPSIOElt containing an IplImage:
     * the imageData can be stored anywhere in memory.
     *	\return the size used in the allocated buffer in bytes, or -1 if the function fails.
     */
    virtual int
    IOEltUsedSizeInBytes(bool* isMemContiguous)=0;
    //! Copies the MAPSIOElt object to a continuous zone in memory. 
    /*! \warning Since MAPSIOElt structure can change with the version of RTMaps, do not use this function 
     * for recording purpose: your records may not be compatible with future versions.
     *	\param dest Pointer to the destination memory zone. This zone has to be already allocated.
     *	\param maxSize Determines the size of the memory zone pointed by dest. The function fails if the zone
     * is too small to contain the entire MAPSIOElt.
     *	\return the size of the memory that has been used to serialize the MAPSIOElt, or -1 if the function
     * fails (\a maxSize is too small for example).
     */
    virtual int
    Serialize(void* dest,
	      unsigned int maxSize)=0;
    //! Deserializes a memory zone from a serialized MAPSIOElt to the current MAPSIOElt.
    /*! \param serializedIoElt Pointer to the memory zone containing the serialized MAPSIOElt. 
     *         \warning The validity of this pointer cannot be checked entirely. Use it at your own risk.
     *	\param doAllocateIfNecessary If \c true and if the current MAPSIOElt (the destination one) is not allocated, 
     *         the allocation is performed automatically. Deallocation then has to be done manually by the programmer.
     *         Usually let this parameter to \c false.
     */
    virtual int
    Deserialize(void* serializedIoElt,
		bool doAllocateIfNecessary=false)=0;
    //! Retrieves the memory size needed to deserialize a MAPSIOElt
    /*! sizeof(MAPSIOElt) is not taken into account there. So add it if you need.
     * \param serializedIoElt Pointer to the serialized MAPSIOElt
     *	\return Returns -1 if the function fails.
     */
    static int
    SerializedIOEltSizeInBytes(void* serializedIoElt);
    //! Checks if serializedIOElt can be deserialized in the current MAPSIOElt.
    /*! \param serializedIoElt Pointer to the serialized MAPSIOElt
     */
    static bool
    IsDeserializable(void* serializedIoElt);

    virtual
    ~MAPSIOElt() { }
};

class MAPSIOMonitor
{
public:
    virtual MAPSIOElt&
    operator[](MAPSFastIOHandle handle)=0;
    virtual MAPSFastIOHandle
    InitBegin()=0;
    virtual void
    InitNext(MAPSFastIOHandle& h)=0;
    virtual
    ~MAPSIOMonitor(){}
};

//Listeners interfaces.
//! The MAPSRunShutdownListener interface.
/*! Your class should inherit from this interface if you want it to be notified
 *  of RTMaps Run/Shutdown events through functions CallbackRun() and CallbackShutdown();
 *  \warning If a MAPSComponent inherits this class, the Callback functions are called
 *  from a different thread than the main thread of the component. Pay attention to 
 *  threads synchronization.
 *  The callback function can be called as soon as the child class is constructed (in case of
 *  a MAPSComponent, the callbacks can be called even when the component is not running).
 * \ingroup component
 */
class MAPSRunShutdownListener
{
private :
    void Register();
    void Unregister();
public :
    MAPSRunShutdownListener();
    virtual ~MAPSRunShutdownListener();

    //! Implement this function to execute some code when diagram starts running.
    /*! This function will be called by the RTMaps engine when the diagram starts to run,
     * just before the diagram components start to run (i.e. just before their Birth() method
     * are called).
     */
    virtual void CallbackRun()=0;
    //! Implement this function to execute some code when diagram stops running.
    /*! This function will be called by the RTMaps engine when the diagram stops running,
     * after the diagram components stop running (i.e. after their Death() method
     * are called).
     */
    virtual void CallbackShutdown()=0;
};

//! The MAPSTimeStateListener interface.
/*! Your class should inherit from this interface if you want it to be notified
 *  of RTMaps clocks states changes such as changes of timespeed, jumps in time, 
 *  or pausing events. 
 *  \warning If a MAPSComponent inherits this class, the Callback functions are called
 *  from a different thread than the main thread of the component. Pay attention to 
 *  threads synchronization.
 *  The callback function can be called as soon as the child class is constructed (in case of
 *  a MAPSComponent, the callbacks can be called even when the component is not running).
 * \ingroup component
 */
class MAPSTimeStateListener
{
private :
    void Register();
    void Unregister();
public :
    MAPSTimeStateListener();
    virtual ~MAPSTimeStateListener();

    //! Implement this function to execute some code each time the RTMaps current time jumps. (due to an action of the user on the VCR slider for example)
    /*! \param destTimestamp The timestamp that has been reached during the jump in time. This function is called just after the destination
     * timestamp has been taken into account by the RTMaps engine.
     */
    virtual void CallbackTimeJumped(const MAPSTimestamp destTimestamp)=0;
    //! Implement this function to execute some code each time the RTMaps changes during execution of a diagram.
    /*! \param newTimeSpeed The new timespeed that has been reached. This function is called just after the new timespeed
     * has been taken into account by the RTMaps engine.
     */
    virtual void CallbackTimeSpeedChanged(const int newTimeSpeed)=0;
    //! Implement this function to execute some code each time the RTMaps execution is paused, or the pause is released.
    /*! \param paused It is set to \c true if the execution is pause, and to \c false if the pause is released. 
     */
    virtual void CallbackTimePaused(const bool paused)=0;
};

//! The MAPSRecordingStateListener interface.
/*! Your class should inherit from this interface if you want it to be notified
 *  when RTMaps starts and stops recording.
 *  \warning If a MAPSComponent inherits this class, the Callback function is called
 *  from a different thread than the main thread of the component. Pay attention to 
 *  threads synchronization.
 *  The callback function can be called as soon as the child class is constructed (in case of
 *  a MAPSComponent, the callback can be called even when the component is not running).
 * \ingroup component
 */
class MAPSRecordingStateListener
{
private :
    void Register();
    void Unregister();
public :
    MAPSRecordingStateListener();
    virtual ~MAPSRecordingStateListener();

    //! Implement this function if you want to execute some code when the recording mode is activated or deactivated.
    /*! \param recording It is set to \c true when the recording is activated, and to \c false when recording is stopped.
     */
    virtual void CallbackRecordingStateChanged(const bool recording) = 0;
};

typedef void (MAPSModule::*MAPSThreadFunction)(void);
typedef void (MAPSComponent::*MAPSComponentThreadFunction)(void);

// // 
//! MAPSBaseClock class. 
/*! Every object inheriting from this class has to implement a basic clock
 * and this clock will be available for the RTMaps engine.
 */
class MAPSBaseClock
{
	unsigned char m_ClockID; //Do not use. This ID is automatically set.
	MAPSString m_ClockName;

	//! For internal use. Do not overload.
	virtual void Register();
	//! For internal use. Do not overload.
	virtual void Unregister();

public :
	//! For internal use.
	unsigned char   GetClockID(); 
	//! For internal use.
	const char*	    GetClockName(); 
	//! For internal use. Do not overload.
	/*! \return Always return \c false.
	 */
	virtual bool    IsSynchronizable() {return false;} 
	//! For internal use. Do not overload.
	/*! \return Always return \c false.
	 */
	virtual bool    IsReplayable() {return false;} // For internal use. Always return false.

	//! Overloading this function is necessary.
	/*! This function is called when the object is chosen as the current clock in the RTMaps diagram.
	 * You have to implement this function, but implemenation can often remain empty...
	 */
	virtual void InitClock()=0;
	//! Overloading this function is necessary.
	/*! This function is called when your custom clock has to start running.
	 */
	virtual void RunClock() =0;
	//! Overloading this function is necessary.
	/*! This function is called when your custom clock has to stop running.
	 */
	virtual void ShutdownClock() =0;
	//!
	/*! This function is called each time someone in RTMaps (the VCR, the Player, or any other component)
	 * queries the current (through the MAPS::CurrentTime() method).
	 * \return The current time of your custom clock, expressed in microseconds.
	 * \warning This function is called very often. It has to be the fastest possible.
	 * \warning This function can be called concurrently from several threads. Make sure it is thread safe.
	 */
	virtual MAPSTimestamp CurrentTime()=0;

	//! This function returns the maximum "real" timespeed currently declared by this clock.
	/*! \return The maximum timespeed your custom clock can reach on the absolute time base (i.e. your watch, not the RTMaps VCR). 
	 * \sa SetAbsoluteTimeSpeed(int absoluteTimeSpeed);
	 */
	int GetAbsoluteTimeSpeed();
	//! Use this function to declare the maximum "real" timespeed your custom clock can reach.
	/*! \param absoluteTimeSpeed The maximum "real" timespeed your custom clock can reach.
	 * "real" timespeed means "in comparison with real time", the time of your watch.
	 * Declaring a max. timespeed which is much higher than the effective timespeed of your clock
	 * will result in little more CPU consumption. On the other hand, declaring a max. timespeed which
	 * is lower thant the effective timespeed of your clock will result in timers inacurracy within RTMaps
	 * (the Wait and Rest functions will be affected).
	 * A value of 1000 means real-time, a value of 500 means half the real-time for example.
	 */
	void SetAbsoluteTimeSpeed(int absoluteTimeSpeed);

	//! Constructor
	/*! \param name The name of your custom clock. To be simple, just pass the name of your 
	 * RTMaps component.
	 */
	MAPSBaseClock(const char* name);
	virtual ~MAPSBaseClock();
};
// 

// // The RTMaps Synchroniser tool
#ifndef _MAPS_SYNCHRONIZER_H_
#define _MAPS_SYNCHRONIZER_H_

//! The RTMaps Synchroniser tool
/*! Provides the function that any RTMaps module can use to synchronize
 * the RTMaps clock on an external signal/clock (GPS, another RTMaps...).
 * Since there can be only one synchronizer at a time, the pointer to the
 * synchronizer object can be requested through function MAPS::GetSynchronizer.
 * \ingroup component
 */
class MAPSSynchronizer
{
// 
public :
    //! Sends a synchronization command to the synchronizable clock.
    /*! \param t Specifies the timestamp to synchronize the synchronizable clock on.
    */
    void SignalSynchronizationCommand(MAPSTimestamp t);

private :
    static int nbInstances;
    MAPSSynchronizer();
    virtual ~MAPSSynchronizer();
};

#endif //_MAPS_SYNCHRONIZER_H_

// // 

class MAPSModuleDefinition;
class MAPSModuleGroupDefinition;

/*! \internal
*  \addtogroup moduleManagement
* @{
*/

/* ********************************* CLASS MAPSModuleBase **************************************/
/*! \internal Base class of every module in RTMaps
*/
class MAPSModuleBase
{
// 
	MAPSModuleDefinition* m_moduleDefinition;
	char* m_name;
	static const char m_id[];

protected :
	MAPSModuleBase(const char* newName);
	virtual 
	~MAPSModuleBase();
	virtual void
	Rename(	const char* oldName,
			const char* newName);
public :
	const char*
	Name()
	{ return m_name; }
};

// 

/* ********************************* CLASS MAPSModuleGroupDefinition *******************************/
typedef void (*GroupUnregistrationFunction)();
typedef void (*GroupRegistrationFunction)();
//! \internal Definition of a group
class MAPSModuleGroupDefinition
{
// 
private :
	// This group name
	char* groupName; 
	GroupRegistrationFunction registrationFunct;
	GroupUnregistrationFunction unregistrationFunct;

	MAPSList<MAPSModuleDefinition*> modules;
	MAPSList<MAPSModuleGroupDefinition*> childGroups;

public:
	/*! Constructor */
	MAPSModuleGroupDefinition(	const char* childGroupName,
								GroupRegistrationFunction registrationFunct=NULL,
								GroupUnregistrationFunction unregistrationFunct=NULL,
								const char* parentGroupName=NULL);

	/*! Destructor */
	virtual ~MAPSModuleGroupDefinition();

	/*! Add the module "module" to the list of modules */
	void
	AddModule(MAPSModuleDefinition* module);

	/*! Remove the module "module" from the list of modules */
	void
	RemoveModule(MAPSModuleDefinition* module);

	/*! Remove the module named "moduleName" from the list of modules */
	void
	RemoveModule(const char* moduleName);

	/*! Add the group childGroup to the list of child groups */
	void
	AddChildGroup(MAPSModuleGroupDefinition* childGroup);

	/*! Remove the group childGroup from the list of child groups */
	void
	RemoveChildGroup(MAPSModuleGroupDefinition* childGroup);

	/*! Remove the group named "childGroupName" from the list of child groups */
	void
	RemoveChildGroup(const char* childGroupName);

	/*! Looks for a module named "searchedName" in the list of modules */
	MAPSModuleDefinition*
	FindModule(	const char* searchedModuleName,
				bool recurse=false);

	bool
	operator==(const char* secondGroupName);

	const char*
	GroupName()
	{ return groupName; }
};

/* ********************************* CLASS MAPSModuleDefinition *******************************/
typedef MAPSModuleBase * (*InstantiationFunction)(const char* name,void* defStruct);
typedef void (*RegistrationFunction)(void* defStruct);
typedef void (*UnregistrationFunction)(void* defStruct);
//! \internal Definition of a module
class MAPSModuleDefinition
{
// 
private :
	char* moduleName;
	const char* groupName;
	InstantiationFunction instFunct;
	RegistrationFunction registrationFunct;
	UnregistrationFunction unregistrationFunct;
	void* defStruct;

	bool
	operator==(const char* secondModuleName);

public :
	/*! Constructor */
	MAPSModuleDefinition(	const char* moduleName,
							const char* groupName,
							InstantiationFunction instFunct=NULL,
							void* defStruct=NULL,
							RegistrationFunction registrationFunct=NULL,
							UnregistrationFunction unregistrationFunct=NULL);

	/*! Destructor */
	virtual ~MAPSModuleDefinition();

	/*! Instantiate the module */
	MAPSModuleBase*
	Instantiate(const char* newObjectName);

	/*! Give the name of the module */
	const char*
	ModuleName()
	{ return moduleName; }

	/*! Give the name of the module group*/
	const char*
	GroupName()
	{ return groupName; }

	/*! Give the definition structure of the module */
	void*
	DefStruct()
	{ return defStruct; }
};

/* **************************** DEFINES ***********************************************/
/*! \internal DEFINE for modules definitions */
#define MAPS_NEW_MODULE_DEFINITION(moduleName,groupName,instFunct,defStruct,registrationFunct,unregistrationFunct)	\
class __NewModuleDeclarationClass##moduleName {																		\
public :																											\
	static MAPSModuleDefinition *moduleDefinition;																	\
};																													\
MAPSModuleDefinition *__NewModuleDeclarationClass##moduleName::moduleDefinition=new MAPSModuleDefinition(#moduleName,#groupName,instFunct,defStruct,registrationFunct,unregistrationFunct);

#define MAPS_NEW_MODULE_DEFINITION_X(moduleName,groupName,instFunct,defStruct,registrationFunct,unregistrationFunct) \
MAPSModuleDefinition *__NewModuleDeclarationClass=new MAPSModuleDefinition(moduleName,groupName,instFunct,defStruct,registrationFunct,unregistrationFunct);
#define MAPS_NEW_MODULE_DEFINITION_N(moduleName,groupName,instFunct,defStruct,registrationFunct,unregistrationFunct,N) \
static MAPSModuleDefinition *__newModuleDeclaration##N=new MAPSModuleDefinition(moduleName,groupName,instFunct,defStruct,registrationFunct,unregistrationFunct);
#define MAPS_NEW_MODULE_DEFINITION_XN(moduleName,groupName,instFunct,defStruct,registrationFunct,unregistrationFunct,N) \
MAPSModuleDefinition *__NewModuleDeclarationClass##N=new MAPSModuleDefinition(moduleName,groupName,instFunct,defStruct,registrationFunct,unregistrationFunct);

/*! \internal DEFINE for groups definitions */
#define MAPS_NEW_CHILD_GROUP_DEFINITION(parentGroupName,childGroupName,registrationFunct,unregistrationFunct)	\
struct __NewGroupDeclarationClass##childGroupName {																\
	static MAPSModuleGroupDefinition *groupDefinition;															\
};																												\
MAPSModuleGroupDefinition *__NewGroupDeclarationClass##childGroupName::groupDefinition=new MAPSModuleGroupDefinition(#childGroupName,registrationFunct,unregistrationFunct,#parentGroupName);

/*! \internal DEFINE for groups definitions */
#define MAPS_NEW_GROUP_DEFINITION(groupName,registrationFunct,unregistrationFunct)	\
struct __NewGroupDeclarationClass##groupName {										\
	static MAPSModuleGroupDefinition *groupDefinition;								\
};																					\
MAPSModuleGroupDefinition *__NewGroupDeclarationClass##groupName::groupDefinition=new MAPSModuleGroupDefinition(#groupName,registrationFunct,unregistrationFunct);

// 
/* @} */
// 

// // RTMaps module
// 

//! The base class for all RTMaps modules.
/*! Defines a scriptable module with inputs, outputs, properties and actions.
 * \ingroup component rrm
 */
class MAPSModule : public MAPSModuleBase
{
// 
private:
    virtual MAPSString
    MakeReport(const char* text);
    virtual MAPSString
    Kind();
    //! Code to execute when a sequential call is made
    /*! When a piece of data is transmitted to this module, and if this module
     *	is considered as non threaded (default behaviour), the virtual function
     *  SequentialCore is executed. It can be overloaded to produce the
     *  desired behaviour.
     */
    virtual void
    SequentialCore(MAPSInput* input)
    { }

    const static MAPSPropertyDefinition IOSubsamplingPropertyDefinition;
    const static MAPSPropertyDefinition FifoSizePropertyDefinition;
    const static MAPSPropertyDefinition PeriodicPropertyDefinition; // Pierre 15/05/2002
    const static MAPSPropertyDefinition ReaderTypePropertyDefinition;
    const static MAPSPropertyDefinition ProcessorPropertyDefinition;
    // Performance monitoring related structures
    const static MAPSPropertyDefinition CounterPropertyDefinition;
    const static MAPSPropertyDefinition NbPeriodsPropertyDefinition;
    const static MAPSPropertyDefinition AveragePeriodPropertyDefinition;
    const static MAPSPropertyDefinition MaxPeriodPropertyDefinition;
    const static MAPSPropertyDefinition MinPeriodPropertyDefinition;
    const static MAPSPropertyDefinition ContextSwitchesPropertyDefinition;
    const static MAPSPropertyDefinition CPULoadPropertyDefinition;
    MAPSProperty* processorProperty;
    MAPSProperty* counterProperty;
    MAPSProperty* nbPeriodsProperty;
    MAPSProperty* averagePeriodProperty;
    MAPSProperty* maxPeriodProperty;
    MAPSProperty* minPeriodProperty;
    MAPSProperty* contextSwitchesProperty;
    MAPSProperty* CPULoadProperty;
    MAPSArray<MAPSDelay> periodMeasures;
    int periodNbMeasures;
    int periodPos;
    MAPSTimestamp periodTimestamp;
    MAPSDelay periodTotal;
    MAPSInput* sequentialInput; // Input on which data arrival caused the execution of the Core function

    static void
    GetPerfMonProperties(MAPSProperty& p,
			 void* x);
    static void
    SetProcessorProperty(MAPSProperty& p);
    static void
    SetInputSubsamplingProperty(MAPSProperty& p);
    static void
    SetInputReaderTypeProperty(MAPSProperty& p);
    static void
    SetOutputSubsamplingProperty(MAPSProperty& p);
    static void 
    SetOutputFifoSizeProperty(MAPSProperty& p);
    static void
    SetOutputPeriodicProperty(MAPSProperty& p); // Pierre 15/05/2002

    // Dynamic internal support (functions and data not supposed to be used by modules programmers)
    MAPSListIterator dynInputsIterator;
    MAPSListIterator dynOutputsIterator;
    MAPSListIterator dynPropertiesIterator;
    MAPSListIterator dynActionsIterator;
    MAPSInput&
    NewInputX(const MAPSString& name,
	      const char* modelName,
	      bool& newOne);
    MAPSOutput&
    NewOutputX(const MAPSString& name,
	       const char* modelName,
	       bool& newOne);
    MAPSProperty&
    NewPropertyX(const MAPSString& name,
		 const char* modelName,
		 bool& newOne);
    MAPSAction&
    NewActionX(const MAPSString& name,
	       const char* modelName,
	       bool& newOne);
    MAPSProperty&
    NewSpecialProperty(const MAPSPropertyDefinition& def,
		       const char* name=NULL);

protected:
    bool volatile dying;
    bool volatile dead;
    bool volatile started;
    bool threaded;
    MAPSEvent isDyingEvent;
    MAPSEvent isDeadEvent;
    MAPSMutex2 monitor; // Used to protect internal linked list from concurrent access
    MAPSMutex2 propertyMonitor; // Used to protect properties from concurrent access
    int volatile runningThreads;

    MAPSList<MAPSString> symbolTableRecords;
    MAPSList<MAPSThread*> threads;
    int firstUserPropertyIndex;
    MAPSListIterator firstUserPropertyPredecessorIterator;

    MAPSList<MAPSInput*> inputs;
    MAPSList<MAPSOutput*> outputs;
    MAPSList<MAPSProperty*> properties;
    MAPSList<MAPSAction*> actions;

    MAPSInput&
    NewInput(const MAPSInputDefinition& def,
	     const char* name=NULL);
    MAPSOutput&
    NewOutput(const MAPSOutputDefinition& def,
	      const char* name=NULL);
    MAPSProperty&
    NewProperty(const MAPSPropertyDefinition& def,
		const char* name=NULL);
    MAPSAction&
    NewAction(const MAPSActionDefinition& def,
	      const char* name=NULL);

    //! Returns the number of inputs of the module
    int
    NbInputs(void) {return inputs.Size();}
    //! Returns the number of outputs of the module
    int
    NbOutputs(void) {return outputs.Size();}
    //! Returns the number of properties of the module
	/*! The number of properties may be higher that expected due
	 * to the fact that this function takes into account the 
	 * module inputs and outputs properties (such as \c subsampling, \c fifosize
	 * etc. and also some fixed number of "hidden" properties that are reserved
	 * for internal use. In order to retrieve the number of "user" properties,
	 * you might use the <tt>MAPSModule::firstUserPropertyIndex</tt> member like:
	 * <tt>NbProperties() - firstUserPropertyIndex</tt>
	*/
    int
    NbProperties(void) {return properties.Size();}
    //! Returns the number of actions of the module
    int
    NbActions(void) {return actions.Size();}

    //! Gets a reference to the input \a inputnb
    MAPSInput& Input(const int inputnb);
    //! Gets a reference to the input called \a name
    MAPSInput& Input(const char* name);
    //! Gets a reference to the output \a outputnb
    MAPSOutput& Output(const int outputnb);
    //! Gets a reference to the output called \a name
    MAPSOutput& Output(const char* name);
    //! Gets a reference to the property \a propertynb
    MAPSProperty& Property(const int propertynb);
    //! Gets a reference to the property called \a name
    MAPSProperty& Property(const char* name);
    //! Gets a reference to the action \a actionnb
    MAPSAction& Action(const int actionnb);
    //! Gets a reference to the action called \a name
    MAPSAction& Action(const char* name);
    
    virtual void
    Rename(const char* oldName,
	   const char* newName);

    void
    RemoveInput(MAPSInput* input);
    void
    RemoveOutput(MAPSOutput* output);
    void
    RemoveProperty(MAPSProperty* property);
    void
    RemoveAction(MAPSAction* action);

    //! Tells if the property value has changed.
    /*! \copydoc MAPSProperty::PropertyChanged
     */
    bool
    PropertyChanged(MAPSProperty& p)
    { return p.PropertyChanged(); }
    //! Tells if the property value has changed.
    /*! \copydoc MAPSProperty::PropertyChanged
     */
    bool
    PropertyChanged(int propertynb)
    { return Property(propertynb).PropertyChanged(); }
    //! Tells if the property value has changed.
    /*! \copydoc MAPSProperty::PropertyChanged
     */
    bool
    PropertyChanged(const char* name)
    { return Property(name).PropertyChanged(); }
    //! Acknowledges the property value change
    /*! \copydoc MAPSProperty::AcknowledgePropertyChanged
     */
    void
    AcknowledgePropertyChanged(MAPSProperty& p)
    { p.AcknowledgePropertyChanged(); }
    //! Acknowledges the property value change
    /*! \copydoc MAPSProperty::AcknowledgePropertyChanged
     */
    void
    AcknowledgePropertyChanged(int propertynb)
    { Property(propertynb).AcknowledgePropertyChanged(); }
    //! Acknowledges the property value change
    /*! \copydoc MAPSProperty::AcknowledgePropertyChanged
     */
    void
    AcknowledgePropertyChanged(const char* name)
    { Property(name).AcknowledgePropertyChanged(); }

    //! Returns the number of threads currently associated to the component.
    int
    RunningThreads() {return runningThreads;}
    //! Returns the FastI/O reader handle of input \a inputnb
    int&
    ReaderHandle(int inputnb);

    //! Returns \c true if input \a input is connected to another component output. cf. MAPSInput::IsConnected
    bool
    IsConnected(MAPSInput& input) {return input.IsConnected();}
    //! Returns \c true if some data are available in FIFO connected to this input
    bool
    DataAvailableInFIFO(MAPSInput& input);
    //! Returns \c true if the FIFO connected is empty
    bool
    IsFIFOEmpty(MAPSOutput& output);
    //! Returns \c true if the FIFO connected is full
    /*! This information can be very useful. When you know the FIFO is full, you know that you
     *	don't process the data fast enough. You can then act and process the data faster
     *	(or skip some data in a smart way)
     */
    bool
    IsFIFOFull(MAPSOutput& output);		
	
    //! \name MAPSEvent management
    //@{
    //! Waits for an event to occur
    /*!	This function blocks until the event occurs.
     *
     *	\param event Event to wait for 
     *	\param timeout The wait aborts after \a timeout microseconds.
     *	\return \c true if the event occurred (was set), \c false if a timeout occured.
     *
     *	Note that using this function is safe. RTMaps knows how to unblock
     *	this function if the component dies, i.e. if the event never occurs, this
     *	function will not block your RTMaps system forever.
     */
    bool
    Wait4Event(MAPSEvent* event,
	       MAPSDelay timeout=MAPS::Infinite);
    //! Waits for an event to occur
    /*!	This function blocks until the event occurs.
     *
     *	\param event Event to wait for 
     *	\param timeout The wait aborts after \a timeout microseconds.
     *	\return \c true if the event occurred (was set), \c false if a timeout occured.
     *
     *	Note that using this function is safe. RTMaps knows how to unblock
     *	this function if the component dies, i.e. if the event never occurs, this
     *	function will not block your RTMaps system forever.
     */
    bool
    Wait4Event(MAPSEvent& event,
	       MAPSDelay timeout=MAPS::Infinite);
    //! Waits for some events to occur
    /*!	This function blocks until one of the events occurs.
     *
     *	\param nCount Number of events to wait on. It is the size of the event parameter array.
     *	\param events Array of pointers to MAPSEvent instances.
     *	\param timeout The wait aborts after \a timeout microseconds.
     *	\param addIsDyingEvent
     *	\return MAPS::TimeOut if a timeout occured, the index of the event that occured (was set)
     *	and led to the function return.
     *
     *	Note that using this function is safe. RTMaps knows how to unblock
     *	this function if the component dies, i.e. if no event ever occurs, this
     *	function will not block your RTMaps system forever.
     */
    int
    Wait4Events(int nCount,
		MAPSEvent* events[],
		MAPSDelay timeout=MAPS::Infinite,
		bool addIsDyingEvent=true);
    //! Waits for an event to occur OR a message to come.
    /*!	This function blocks until the event occurs OR a message comes to the thread.
     *	Note that this function has a meaning only for message based OS like Windows.
     *
     *	\param event Event to wait for 
     *	\param timeout The wait aborts after \a timeout microseconds.
     *	\return 0 if the event occured (was set),MAPS::TimeOut if a timeout occured, MAPS::GotAMessage if a message arrived.
     *
     *	Note that using this function is safe. RTMaps knows how to unblock
     *	this function if the component dies, i.e. if the event never occurs, this
     *	function will not block your RTMaps system forever.
     */
    int
    MsgWait4Event(MAPSEvent* event,
		  MAPSDelay timeout=MAPS::Infinite); // Only for Windows or any other message based OS
    //! Waits for some events to occur OR a message to come
    /*!	This function blocks until one of the events occurs OR a message comes to the thread.
     *	Note that this function has a meaning only for a message based OS like Windows.
     *
     *	\param nCount Number of events to wait on. It's the size of the events parameter array.
     *	\param events Array of pointers to MAPSEvent classes.
     *	\param timeout The wait aborts after \a timeout microseconds.
     *	\param addIsDyingEvent
     *	\return MAPS::TimeOut if a timeout occured, the index of the event that occured (was set)
     *	and led to the function return, MAPS::GotAMessage if a message arrived.
     *
     *	Note that using this function is safe. RTMaps knows how to unblock
     *	this function if the component dies, i.e. if no event ever occurs, this
     *	function will not block your RTMaps system forever.
     *
     */
    int
    MsgWait4Events(int nCount,
		   MAPSEvent* events[],
		   MAPSDelay timeout=MAPS::Infinite,
		   bool addIsDyingEvent=true); // Only for Windows or any other message based OS
    //@}

    //! Waits for a handshake on output
    /*! This function is useful for handshaking communication between components.
     *	It will block until one of the components connected on \a output have read
     *	the data recently output by MAPSModule::StopWriting
     */
    void
    Wait4Handshake(MAPSOutput& output);

    //! \name Virtual time management functions
    /*! The parameters of these functions are expressed in virtual microseconds. 
     *	This means that these functions make full usage of the RTMaps virtual
     *	time management feature. You MUST use these functions when you design
     *	some time-related components.
     */
    //@{
    //! Blocks the current thread for a delay of \a delay microseconds.
    /*! Any absolute reference is lost with this function. It works perfectly when the
     *	the virtual time speed is negative.
     *
     *	The optional parameter \a cancel allows to asynchronously stop the wait.
     *
     *	Returns \c true when the delay has elapsed. Returns \c false
     *	if the wait was canceled by setting the \a cancel event.
     *
     *	Note that using this function is safe. RTMaps knows how to unblock
     *	this function if the component dies, i.e. this
     *	function will not block your RTMaps system until the delay expires, which
     *	can take some hours...
     */
    bool
    Rest(MAPSDelay delay,
	 MAPSEvent* cancel=NULL);
    //! Blocks the current thread until the time reaches \a timestamp
    /*! Beware : the time might never reach \a timestamp, especially
     *	if the virtual time speed is negative (the time goes backward).
     *
     *	The optional parameter \a cancel allows to asynchronously stop the wait.
     *
     *	Returns \c true when the timestamp \a timestamp is reached.
     *	Returns \c false is the wait was canceled by setting the \a cancel event.
     *
     *	Note that using this function is safe. RTMaps knows how to unblock
     *	this function if the component dies, i.e. this
     *	function will not block your RTMaps system until \a timestamp is reached, which
     *	might never happen.
     */
    bool
    Wait(MAPSTimestamp timestamp,
	 MAPSEvent* cancel=NULL);
    //@}
	
    //! \name Death management functions
    //@{
    //! Returns \c true if the module is currently dying (but is not dead).
    /*! Some other component or the RTMaps kernel asked for its death. */
    bool
    IsDying(void) {return dying;}
    //! Returns \c true if the module is dead.
    bool
    IsDead(void) {return dead;}
    //@}

    //! \name Threads management functions
    //@{
    //! Creates a thread associated to the module
    void
    CreateThread(MAPSThreadFunction);
    //! Function to be called by the module itself during termination. Ask all the associated threads to die.
    /*!	The result is not immediate : some threads could refuse to die.
     *	Should be followed by a call to Wait4Death that waits for the death itself
     */
    void
    Die(); 
    //! Adds a "life" to the module. The "life" number is a counter for the threads associated to the module.
    /*! Do not use this function directly. Let the RTMaps kernel manage the life and death of
     *	module threads. Only for very experienced RTMaps developers.
     */
    void
    OneMoreLife(void);
    //! Removes a "life" from the module. The "life" number is a counter for the threads associated to the module.
    /*! Do not use this function directly. Let the RTMaps kernel manage the life and death of
     *	module threads. Only for very experienced RTMaps developers.
     */
    void
    LoseOneLife(void);
    //! To be called when the module starts. It correctly resets the dying and death events and variables.
    void
    Start();
    //! Waits for the death of all associated threads
    bool
    Wait4Death();
    //@}

    //! \name Error management functions
    //@{
    //! Emits an error.
    /*! This will kill definitely the module. Use MAPSModule::ReportError if you
     *	want to go on executing some code.
     *
     *	\param string Error string to log and display
     *	\param importance Importance of the error :
     *		- 0 : The error is minor. It will not have any impact on the overall system behaviour. Some actions
     *		have been taken that will fix the problem.
     *		- 1 : The error is important. Some actions have been taken that might fix the problem. The system
     *		can go on running, but it might not function properly.
     *		- 2 : The error is critical. The system will not function properly thereafter.
     */
    virtual void
    Error(const char* string,
	  int importance=1);
    //@}

    //! \name Reporting management functions
    //@{
    //! Reports an error.
    /*!	\param string Error string to log and display
     *	\param importance Importance of the error :
     *		- 0 : The error is minor. It will not have any impact on the overall system behaviour. Some actions
     *		have been taken that will fix the problem.
     *		- 1 : The error is important. Some actions have been taken that might fix the problem. The system
     *		can go on running, but it might not function properly.
     *		- 2 : The error is critical. The system will not function properly thereafter.
     */
    virtual void
    ReportError(const char* string,
		int importance=0);
    //! Reports a warning
    /*!	\param string Warning string to log and display
     *	\param importance Importance of the warning :
     *		- 0 : The warning is minor. It has no impact on the behaviour of the module.
     *		- 1 : The warning is important. It signals a malfunction in the module that could result in an error.
     *		- 2 : The warning is critical. It signals a major malfunction in the module. It will certainly result in an error.
     */
    virtual void
    ReportWarning(const char* string,
		  int importance=0);
    //! Reports a piece of information
    /*!	\param string Info string to log and display
     *	\param importance Importance of the information :
     *		- 0 : The information is minor.
     *		- 1 : The information is important.
     *		- 2 : The information is critical.
     */
    virtual void
    ReportInfo(const char* string,
	       int importance=0);
    //! General report function
    /*!	\param string String to log and display
     *	\param type Type of report. Can be MAPS::Info, MAPS::Warning or MAPS::Error.
     *	\param importance Importance of the report (0: minor, 1: important, 2: critical)
     */
    virtual void
    Report(const char* string,
	   int type=MAPS::Info,
	   int importance=0);
    //@}

    /*! \name DirectSet functions
     *  These functions do not check the \c canBeModifierAfterStart and \c readOnly
     *	parameters of properties, so the RTMaps modules can set their own properties with these functions
     *	in all situations. Usual MAPSModule::Set functions check the parameters of properties, and
     *	so cannot be used if for instance \c readOnly is set. 
     *
     *	MAPSModule::DirectSet functions are protected, so they can only be used in RTMaps modules.
     *	On the contrary, MAPSModule::Set functions are public and thus can be called by anyone.
     * @{
     */
    //! Sets boolean \a value for boolean property \a p
    void
    DirectSet(MAPSProperty& p,
	      bool value);
    //! Sets integer \a value for integer property \a p
    void
    DirectSet(MAPSProperty& p,
	      MAPSInt64 value);
    //! \copydoc MAPSModule::DirectSet(MAPSProperty&,MAPSInt64)
    void
    DirectSet(MAPSProperty& p,
	      MAPSInt32 value)
    { DirectSet(p,(MAPSInt64)value); }
    //! Sets float \a value for float property \a p
    void
    DirectSet(MAPSProperty& p,
	      double value);
    //! Sets string \a value for string property \a p
    void
    DirectSet(MAPSProperty& p,
	      const MAPSString& value);
    //! \copydoc MAPSModule::DirectSet(MAPSProperty&,const MAPSString&)
    void
    DirectSet(MAPSProperty& p,
	      const char* value)
    { DirectSet(p,MAPSString(value)); }
    //! Sets enum values to \a enumValues and the selected enum to \a selected for enum property \a p
    void
    DirectSet(MAPSProperty& p,
	      const char* enumValues,
	      MAPSInt32 selected)
    { DirectSet(p,MAPSString(enumValues),selected); }
    //! \copydoc MAPSModule::DirectSet(MAPSProperty&,const char*,MAPSInt32)
    void
    DirectSet(MAPSProperty& p,
	      const MAPSString& enumValues,
	      MAPSInt32 selected);
    //! Sets enum value to \a enumStruct for enum property \a p
    void
    DirectSet(MAPSProperty& p,
	      const MAPSEnumStruct& enumStruct);
    //! Sets boolean \a value for boolean property at index \a propertynb
    void
    DirectSetProperty(int propertynb,
		      bool value)
    { DirectSet(Property(propertynb),value); }
    //! Sets integer \a value for integer property at index \a propertynb
    void
    DirectSetProperty(int propertynb,
		      MAPSInt64 value)
    { DirectSet(Property(propertynb),value); }
    //! Sets integer \a value for integer property at index \a propertynb
    void
    DirectSetProperty(int propertynb,
		      MAPSInt32 value)
    { DirectSet(Property(propertynb),(MAPSInt64)value); }
    //! Sets float \a value for float property at index \a propertynb
    void
    DirectSetProperty(int propertynb,
		      double value)
    { DirectSet(Property(propertynb),value); }
    //! Sets string \a value for string property at index \a propertynb
    void
    DirectSetProperty(int propertynb,
		      const char* value)
    { DirectSet(Property(propertynb),value); }
    //! Sets string \a value for string property at index \a propertynb
    void
    DirectSetProperty(int propertynb,
		      const MAPSString& value)
    { DirectSet(Property(propertynb),value); }
    //! Sets enum values to \a enumValues and the selected enum to \a selected for enum property at index \a propertynb
    void
    DirectSetProperty(int propertynb,
		      const char* enumValues,
		      MAPSInt32 selected)
    { DirectSet(Property(propertynb),enumValues,selected); }
    //! Sets enum value to \a enumStruct for enum property at index \a propertynb
    void
    DirectSetProperty(int propertynb,
		      const MAPSEnumStruct& enumStruct)
    { DirectSet(Property(propertynb),enumStruct); }
    //! Sets boolean \a value for boolean property \a p
    void
    DirectSetProperty(const char* p,
		      bool value)
    { DirectSet(Property(p),value); }
    //! Sets integer \a value for integer property \a p
    void
    DirectSetProperty(const char* p,
		      MAPSInt64 value)
    { DirectSet(Property(p),value); }
    //! Sets integer \a value for integer property \a p
    void
    DirectSetProperty(const char* p,
		      MAPSInt32 value)
    { DirectSet(Property(p),(MAPSInt64)value); }
    //! Sets float \a value for float property \a p
    void
    DirectSetProperty(const char* p,
		      double value)
    { DirectSet(Property(p),value); }
    //! Sets string \a value for string property \a p
    void
    DirectSetProperty(const char* p,
		      const char* value)
    { DirectSet(Property(p),value); }
    //! Sets string \a value for string property \a p
    void
    DirectSetProperty(const char* p,
		      const MAPSString& value)
    { DirectSet(Property(p),value); }
    //! Sets enum values to \a enumValues and the selected enum to \a selected for enum property \a p
    void
    DirectSetProperty(const char* p,
		      const char* enumValues,
		      MAPSInt32 selected)
    { DirectSet(Property(p), enumValues, selected); }
    //! Sets enum value to \a enumStruct for enum property \a p
    void
    DirectSetProperty(const char* p,
		      const MAPSEnumStruct& enumStruct)
    { DirectSet(Property(p),enumStruct); }
    
    //! Performance monitoring function
    /*! This function is called automatically by the RTMaps kernel
     *  along with the Core function of the component.
     */
    virtual void
    PerformanceMonitoring();

    //! For a sequential module (or component), returns a reference to the input that is the source of the current execution
    MAPSInput&
    SequentialInput() {return *sequentialInput;}

    //! Constructor
    MAPSModule(const char* n,
	       bool perfMon=false,
	       bool stdProperties=true);
    //! Virtual destructor. Overloaded.
    virtual
    ~MAPSModule();
    //@}

public:
    //! \name Get/Set property functions
    /*! The first 4 Get/Set functions can be overloaded. The other functions always call these 4 basic functions.
     */
    //@{
    //! Gets boolean \a value from boolean property \a p
    virtual void
    Get(MAPSProperty& p,
	bool& value);
    //! Gets integer \a value from integer property \a p
    virtual void
    Get(MAPSProperty& p,
	MAPSInt64& value);
    //! Gets float \a value from float property \a p
    virtual void
    Get(MAPSProperty& p,
	MAPSFloat& value);
    //! Gets string \a value from string property \a p
    virtual void
    Get(MAPSProperty& p,
	MAPSString& value);
    //! Gets enum struct \a enumStruct from enum property \a p
    virtual void
    Get(MAPSProperty& p,
	MAPSEnumStruct& enumStruct);
    //! Gets enum struct values into \a enumValues and selected item into \a selectedValue from enum property \a p
    void
    Get(MAPSProperty& p,
	MAPSArray<MAPSString>& enumValues,
	MAPSInt32& selectedValue)
    { MAPSEnumStruct enumStruct; Get(p,enumStruct); enumValues = *enumStruct.enumValues; selectedValue = enumStruct.selectedEnum; }
    //! Gets integer \a value from integer property \a p
    void
    Get(MAPSProperty& p,
	MAPSInt32& value)
    { MAPSInt64 val; Get(p,val); value=(MAPSInt32)val; }
    //! Gets boolean \a value from boolean property \a propertynb
    void
    GetProperty(const int propertynb,
		bool& value)
    { Get(Property(propertynb),value); }
    //! Get integer \a value from integer property \a propertynb
    void
    GetProperty(const int propertynb,
		MAPSInt32& value)
    { MAPSInt64 val;Get(Property(propertynb),val);value=(MAPSInt32)val; }
    //! Gets integer \a value from integer property \a propertynb
    void
    GetProperty(const int propertynb,
		MAPSInt64& value)
    { Get(Property(propertynb),value); }
    //! Gets float \a value from float property \a propertynb
    void
    GetProperty(const int propertynb,
		MAPSFloat& value)
    { Get(Property(propertynb),value); }
    //! Gets string \a value from string property \a propertynb
    void
    GetProperty(const int propertynb,
		MAPSString& value)
    { Get(Property(propertynb),value); }
    //! Gets enum struct values into \a enumValues and selected item into \a selectedValue from enum property \a propertynb
    void
    GetProperty(const int propertynb,
		MAPSArray<MAPSString>& enumValues,
		MAPSInt32& selectedValue)
    { Get(Property(propertynb),enumValues,selectedValue); }
    //! Gets enum struct value into \a enumStruct from enum property \a p
    void
    GetProperty(const int propertynb,
		MAPSEnumStruct& enumStruct)
    { Get(Property(propertynb),enumStruct); }
    //! Gets boolean \a value from boolean property \a p
    void
    GetProperty(const char* p,
		bool& value)
    { Get(Property(p),value); }
    //! Gets integer \a value from integer property \a p
    void
    GetProperty(const char* p,
		MAPSInt32& value)
    { MAPSInt64 val;Get(Property(p),val);value=(MAPSInt32)val; }
    //! Gets integer \a value from integer property \a p
    void
    GetProperty(const char* p,
		MAPSInt64& value)
    { Get(Property(p),value); }
    //! Gets float \a value from float property \a p
    void
    GetProperty(const char* p,
		MAPSFloat& value)
    { Get(Property(p),value); }
    //! Gets string \a value from string property \a p
    void
    GetProperty(const char* p,
		MAPSString& value)
    { Get(Property(p),value); }
    //! Gets enum struct values into \a enumValues and selected item into \a selectedValue from enum property \a p
    void
    GetProperty(const char* p,
		MAPSArray<MAPSString>& enumValues,
		MAPSInt32& selectedValue)
    { Get(Property(p),enumValues,selectedValue); }
    //! Gets enum struct value into \a enumStruct from enum property \a p
    void
    GetProperty(const char* p,
		MAPSEnumStruct& enumStruct)
    { Get(Property(p),enumStruct); }
    //! Gets boolean value from boolean property \a propertynb
    bool
    GetBoolProperty(const int propertynb)
    { bool b; GetProperty(propertynb, b); return b; }
    //! Gets integer value from integer property \a propertynb
    MAPSInt64
    GetIntegerProperty(const int propertynb)
    { MAPSInt64 i=0xdead; GetProperty(propertynb, i); return i; }
    //! Gets float value from float property \a propertynb
    MAPSFloat
    GetFloatProperty(const int propertynb)
    { MAPSFloat i=6.55957; GetProperty(propertynb, i); return i; }
    //! Gets string value from string property \a propertynb
    MAPSString
    GetStringProperty(const int propertynb)
    { MAPSString s; GetProperty(propertynb, s); return s; }
    //! Gets enum value from enum property \a propertynb
    MAPSEnumStruct
    GetEnumProperty(const int propertynb)
    { MAPSEnumStruct enums; GetProperty(propertynb, enums); return enums; }
    //! Gets boolean value from boolean property \a p
    bool
    GetBoolProperty(const char* p)
    { bool b; GetProperty(p, b); return b; }
    //! Gets integer value from integer property \a p
    MAPSInt64
    GetIntegerProperty(const char* p)
    { MAPSInt64 i=0xdead; GetProperty(p, i); return i; }
    //! Gets float value from float property \a p
    MAPSFloat
    GetFloatProperty(const char* p)
    { MAPSFloat i=6.55957; GetProperty(p, i); return i; }
    //! Gets string value from string property \a p
    MAPSString
    GetStringProperty(const char* p)
    { MAPSString s; GetProperty(p, s); return s; }
    //! Gets enum value from enum property \a p
    MAPSEnumStruct
    GetEnumProperty(const char* p)
    { MAPSEnumStruct enums; GetProperty(p,enums); return enums; }
    //! Gets boolean value from boolean property \a p
    bool
    GetBoolProperty(MAPSProperty& p)
    { bool b; Get(p, b); return b; }
    //! Gets integer value from integer property \a p
    MAPSInt64
    GetIntegerProperty(MAPSProperty& p)
    { MAPSInt64 i=0xdead; Get(p, i); return i; }
    //! Gets float value from float property \a p
    MAPSFloat
    GetFloatProperty(MAPSProperty& p)
    { MAPSFloat i=6.55957; Get(p, i); return i; }
    //! Gets string value from string property \a p
    MAPSString
    GetStringProperty(MAPSProperty& p)
    { MAPSString s; Get(p, s); return s; }
    //! Gets enum value from enum property \a p
    MAPSEnumStruct
    GetEnumProperty(MAPSProperty& p)
    { MAPSEnumStruct enums; Get(p,enums); return enums; }

    //! Sets boolean \a value from boolean property \a p
    virtual void
    Set(MAPSProperty& p,
	bool value);
    //! Sets integer \a value from integer property \a p
    virtual void
    Set(MAPSProperty& p,
	MAPSInt64 value);
    //! Sets float \a value from float property \a p
    virtual void
    Set(MAPSProperty& p,
	MAPSFloat value);
    //! Sets string \a value from string property \a p
    virtual void
    Set(MAPSProperty& p,
	const MAPSString& value);
    //! Sets enum struct \a enumStruct from enum property \a p
    virtual void
    Set(MAPSProperty& p,
	const MAPSEnumStruct& enumStruct);
    //! \copydoc MAPSModule::Set(MAPSProperty&,MAPSInt64)
    void
    Set(MAPSProperty& p,
	MAPSInt32 value)
    { Set(p,(MAPSInt64)value); }
    //! \copydoc MAPSModule::Set(MAPSProperty&,const MAPSString&)
    void
    Set(MAPSProperty& p,
	const char* value)
    { Set(p,MAPSString(value)); }
    //! \copydoc MAPSModule::Set(MAPSProperty&,const MAPSString&, MAPSInt32)
    void
    Set(MAPSProperty& p,
	const char* enumValues,
	MAPSInt32 selected)
    { Set(p,MAPSString(enumValues),selected); }
    //! Sets enum values to \a enumValues and the selected enum to \a selected for enum property \a p
    void
    Set(MAPSProperty& p,
	const MAPSString& enumValues,
	MAPSInt32 selected);
    //! Sets boolean \a value from boolean property \a propertynb
    void
    SetProperty(int propertynb,
		bool value)
    { Set(Property(propertynb),value); }
    //! Sets integer \a value from integer property \a propertynb
    void
    SetProperty(int propertynb,
		MAPSInt32 value)
    { Set(Property(propertynb),(MAPSInt64)value); }
    //! Sets integer \a value from integer property \a propertynb
    void
    SetProperty(int propertynb,
		MAPSInt64 value)
    { Set(Property(propertynb),value); }
    //! Sets float \a value from float property \a propertynb
    void
    SetProperty(int propertynb,
		MAPSFloat value)
    { Set(Property(propertynb),value); }
    //! Sets float \a value from float property \a propertynb
    void
    SetProperty(int propertynb,
		const MAPSString& value)
    { Set(Property(propertynb),value); }
    //! Sets string \a value from string property \a propertynb
    void
    SetProperty(int propertynb,
		const char* value)
    { Set(Property(propertynb),MAPSString(value)); }
    //! Sets enum values to \a enumValues and the selected enum to \a selected for enum property \a propertynb
    void
    SetProperty(int propertynb,
		const char* enumValues,
		MAPSInt32 selected)
    { Set(Property(propertynb),MAPSString(enumValues),selected); }
    //! Sets enum value to \a enumStruct for enum property \a propertynb
    void
    SetProperty(int propertynb,
		const MAPSEnumStruct& enumStruct)
    { Set(Property(propertynb),enumStruct); }
    //! Sets boolean \a value from boolean property \a p
    void
    SetProperty(const char* p,
		bool value)
    { Set(Property(p),value); }
    //! Sets integer \a value from integer property \a p
    void
    SetProperty(const char* p,
		MAPSInt32 value)
    { Set(Property(p),(MAPSInt64)value); }
    //! Sets integer \a value from integer property \a p
    void
    SetProperty(const char* p,
		MAPSInt64 value)
    { Set(Property(p),value); }
    //! Sets float \a value from float property \a p
    void
    SetProperty(const char* p,
		MAPSFloat value)
    { Set(Property(p),value); }
    //! Sets string \a value from string property \a p
    void
    SetProperty(const char* p,
		const MAPSString& value)
    { Set(Property(p),value); }
    //! Sets string \a value from string property \a p
    void
    SetProperty(const char* p,
		const char* value)
    { Set(Property(p),MAPSString(value)); }
    //! Sets enum values to \a enumValues and the selected enum to \a selected for enum property \a p
    void
    SetProperty(const char* p,
		const char* enumValues,
		MAPSInt32 selected)
    { Set(Property(p),MAPSString(enumValues),selected); }
    //! Sets enum value to \a enumStruct for enum property \a p
    void
    SetProperty(const char* p,
		const MAPSEnumStruct& enumStruct)
    { Set(Property(p),enumStruct); }
    //@}

    /*! \name Start/Stop Reading/Writing functions
     */
    //@{
    //! The StartWriting function, one of the most important functions in the RTMaps component architecture.
    MAPSIOElt*
    StartWriting(MAPSOutput& output);
    //! The StopWriting function, one of the most important functions in the RTMaps component architecture.
    /*! \param IOElt A pointer to the MAPSIOElt obtained by a previous call to StartWriting.
     *	\param discard If set to \c true, the write will not be done, that is the data will be lost.
     */
    void
    StopWriting(MAPSIOElt* IOElt,
		bool discard=false);
    //! The StartReading function, one of the most important functions in the RTMaps component architecture.
    /*!	Sequential components: returns \c NULL when no input is connected or when no data is directly available, or when a detachment was asked for.
     *	
     *	\note Dynamic detachment is transparent to this version of StartReading in threading mode,
     *	but not in sequential mode. Watch out for \c NULL return values when developping sequential mode
     *	compatible components.
     */
    MAPSIOElt*
    StartReading(MAPSInput& input);
    //! The multiple StartReading function, one of the most important functions in the RTMaps component architecture.
    /*! The multiple StartReading returns \c NULL when a detachment occured.
     *  In that case, \a inputThatAnswered indicates which input was detached.
     *
     *	This version of StartReading enables to simultaneously wait for data on inputs and wait for
     *	events, exactly like MAPSModule::Wait4Event does. If some events are specified, and 
     *  one of these events is triggered, the function returns \a NULL. In order to discrimate between
     *  a detachment and an occured event, the user MUST test the \a inputThatAnswered
     *	variable. If \a inputThatAnswered is more than \a nCount, then obviously
     *	an event occured, and its number is given by <CODE>inputThatAnswered-ncount</CODE>, starting
     *	from 0.
     *
     *	\note There is priority of inputs over events.
     *
     *	\note The order of priority among inputs is set by the inputs table \a inputs
     */
    MAPSIOElt*
    StartReading(int nCount,
		 MAPSInput* inputs[],
		 int* inputThatAnswered,
		 int nCountEvents=0,
		 MAPSEvent* events[]=NULL);
    //! The MsgStartReading function.
    /*! Returns \c NULL when a detachment or a message occured.
     *	
     *	\warning Detachment is not completly transparent to this version of StartReading
     *	since when there is a detachment, nothing happens, but the next answer can be \c NULL (that is, be a message)
     *	in that case, the last MAPSIOElt is no longer valid (there was an implicit StopReading
     *	due to the detachment)
     *
     *	\note This function has a meaning only with message based OSes like Windows.
     */
    MAPSIOElt*
    MsgStartReading(MAPSInput& input);
    //! The multiple MsgStartReading function.
    /*! The multiple StartReading returns \c NULL when a detachment or a message occured.
     *  In that case, \a inputThatAnswered indicates which input was detached, except if \a inputThatAnswered is 
     *  equal to <CODE>MAPS::GotAMessage</CODE>, then the \c NULL answer corresponds to a Windows Message.
     *  Sequential components: Always returns \c NULL (should not be used)
     *
     *	\note There is priority of inputs over events.
     *
     *	\note The order of priority among inputs is set by the inputs table \a inputs 
     *
     *	\note This function has a meaning only with message based OSes like Windows.
     *
     */
    MAPSIOElt*
    MsgStartReading(int nCount,
		    MAPSInput* inputs[],
		    int* inputThatAnswered,
		    int nCountEvents=0,
		    MAPSEvent* events[]=NULL);
    //! The synchronized StartReading function.
    /*!
     *	The SynchroStartReading function can retrieve synchronized data on several
     *	different inputs. The function returns when input data with the same timestamp
     *  are found on the inputs (or approximately the same in case the parameter \a synchroTolerance
     *  is specified and more than 0). The corresponding MAPSIOElts are then returned via the
     *	provided \a IOElts array.
     *
     *	An \a abortEvent can be provided in order to force the function to return when the event is set.
     *	In this case, the returned timestamp is -1 and the IOElts in the array are set to \c NULL.
     *	This \a abortEvent can be used for example in order to update the \a synchroTolerance if the latest value
     *	has been too low and the function blocks since it cannot synchronize signals.
     */
    MAPSTimestamp
    SynchroStartReading(int nb,
			MAPSInput** inputs,
			MAPSIOElt** IOElts,
			MAPSInt64 synchroTolerance=0,
			MAPSEvent* abortEvent=NULL);
    //! The N last MAPSIOElts StartReading function.
    /*! Returns \c true when all the MAPSIOElts are correctly fetched. In threaded
     *	mode, the function should always return \c true.
     *
     *	Returns \c false in sequential mode until the right number
     *	of elements are fetched, or when a dynamic detachment occurs.
     */
    bool
    NLastStartReading(MAPSInput& input,
		      int nb,
		      MAPSIOElt** IOElts);
    //! The StopReading function, one of the most important functions in the RTMaps component architecture.
    void
    StopReading(MAPSInput& input);
    //@}

    //! \name Action execution functions
    //@{
    //! Executes action \a a
    void
    DoAction(MAPSAction& a)
    { a.DoIt(); }
    // Executes action \a a
    void
    ExecuteAction(MAPSAction& a)
    { DoAction(a); }
    //! Executes action number \a nb
    void
    DoAction(int nb)
    { Action(nb).DoIt(); }
    //! Executes action called \a name
    void
    DoAction(const char* name)
    { Action(name).DoIt(); }
    // Executes action number \a nb
    void
    ExecuteAction(int nb)
    { DoAction(nb); }
    // Executes action called \a name
    void
    ExecuteAction(const char* name)
    { DoAction(name); }
    //@}
};

// 
// // 
class MAPSCoreFunctionInterface
{
public:
    virtual void
    LoadConfiguration()=0;
    virtual void
    SaveConfiguration()=0;
    virtual void*
    GetInterface()=0;
    virtual
    ~MAPSCoreFunctionInterface() {}
};

// Core functions definitions
class MAPSCoreFunction : public MAPSModule, public MAPSCoreFunctionInterface
{
// 
    virtual MAPSString
    Kind();

protected:
    MAPSCoreFunction(const char *name);
    virtual
    ~MAPSCoreFunction() {}

public:
    virtual void
    LoadConfiguration() {}
    virtual void
    SaveConfiguration() {}
    virtual void*
    GetInterface() { return NULL; }
};

struct MAPSCFDefinition;
typedef MAPSCoreFunction* (*MAPSCFInstantiationFunction)(const char* name,MAPSCFDefinition* md);

struct MAPSCFDefinition {
	const char* name;
	const char* version;
	MAPSCFInstantiationFunction instFunct;
	MAPSPropertyDefinition* properties;
	MAPSActionDefinition* actions;
	int refCounter;
};

/* Standard header code */
#define MAPS_CF_STANDARD_HEADER_CODE()												\
	MAPS_PROPERTIES_DECLARATION;													\
	MAPS_ACTIONS_DECLARATION;														\
public:																				\
	static MAPSCoreFunction* InstFunct(const char *name, MAPSCFDefinition* md);	\
	static MAPSCFDefinition s_cfDefinition;

/* Core function definition */
#define MAPS_CF_DEFINITION(cf,groupClass,name,version)		\
MAPSCFDefinition cf::s_cfDefinition = {						\
	#name,													\
	version,												\
	cf::InstFunct,											\
	cf::propertiesDefinition,								\
	cf::actionsDefinition,									\
	0														\
};															\
 MAPS_NEW_MODULE_DEFINITION(name,groupClass,NULL,&cf::s_cfDefinition,NULL,NULL);

// 

// // RTMaps component
// 
/*! \defgroup component Component design*/

typedef MAPSComponent* (*MAPSComponentInstantiationFunction)(const char* name,MAPSComponentDefinition& md);

struct MAPSComponentDefinition
{
	const char* name;
	const char* version;
	MAPSComponentInstantiationFunction instFunct;
	MAPSRegisteringFunction regFunct;
	MAPSUnregisteringFunction unregFunct;
	MAPSUInt32 maxNumberOfInstances; // 0 = infinite number of instances
	MAPSUInt32 nbInputs;
	MAPSInputDefinition* inputs;
	MAPSUInt32 nbOutputs;
	MAPSOutputDefinition* outputs;
	MAPSUInt32 nbProperties;
	MAPSPropertyDefinition* properties;
	MAPSUInt32 nbActions;
	MAPSActionDefinition* actions;
	MAPSUInt16 priority;// Between 0 and 255
	MAPSUInt32 kind;	// Kind of component : threaded, sequential, both (MAPS::Threading + MAPS::Sequential)
	bool threaded;	// Behaviour on start: threaded (true), sequential (false) (only if kind is both)
	const char* documentation;
	MAPSInt32 reserved;	// For future use
};

//! The base class for all RTMaps components
/*!
 * Some useful macros are available for easier development of components. They are detailed in \ref component.
 * \ingroup component
 */
class MAPSComponent : public MAPSModule {
// 
    
private:
    MAPSComponentDefinition *componentDefinition;
    MAPSMutex sequentialMonitor;
    MAPSMutex2 dynamicMonitor;
    MAPSEvent startedEvent;
    bool inDynamic;
    bool volatile sequentialDead;
    bool volatile startAutomatically;
    
    const static MAPSPropertyDefinition PriorityPropertyDefinition;
    const static MAPSPropertyDefinition ThreadedPropertyDefinition;
    
    void Die4Good(void);
    void CleanUp(void); // Called by Die4Good. Ensures a good ending status for the component
    void SequentialDeath(void); // Should only be called by the RTMaps engine
    MAPSString Kind(); // Overload of MAPSModule::Kind. Returns "component"
    
    static void SetPriorityPropertyCallback(MAPSProperty& p);
    
    // Sequential call support. Overload of MAPSModule::SequentialCore
    void SequentialCore(MAPSInput *input);
    
protected:
    MAPSOutput& NewOutput(const MAPSOutputDefinition& def, const char *name=NULL);
    MAPSInput& NewInput(const MAPSInputDefinition& inputDef, const char* name=NULL);
    //! Dynamically creates an input
    /*! \param model model number (its order in the MAPS_INPUTS_DEFINITION description)
    *	\param name the name for the new input. If \c NULL, the model name is used.
    *	\return A reference to the newly created MAPSInput object.
    */
    MAPSInput& NewInput(int model, const char *name=NULL);
    //! Dynamically creates an input
    /*! \param model model name (its name in the MAPS_INPUTS_DEFINITION description)
    *	\param name the name for the new input. If \c NULL, the model name is used.
    *	\return A reference to the newly created MAPSInput object.
    */
    MAPSInput& NewInput(const char* model, const char *name=NULL);
    //! Dynamically creates an output
    /*! \param model model number (its order in the MAPS_OUTPUTS_DEFINITION description)
    *	\param name the name for the new output. If \c NULL, the model name is used.
    *	\return A reference to the newly created MAPSOutput object.
    */
    MAPSOutput& NewOutput(int model, const char *name=NULL);
    //! Dynamically creates an output
    /*! \param model model name (its name in the MAPS_OUTPUTS_DEFINITION description)
    *	\param name the name for the new output. If \c NULL, the model name is used.
    *	\return A reference to the newly created MAPSOutput object.
    */
    MAPSOutput& NewOutput(const char* model, const char *name=NULL);
    //! Dynamically creates a property
    /*! \param model model number (its order in the MAPS_PROPERTIES_DEFINITION description)
    *	\param name the name for the new property. If \c NULL, the model name is used.
    *	\return A reference to the newly created MAPSProperty object.
    */
    MAPSProperty& NewProperty(int model, const char *name=NULL);
    //! Dynamically creates a property
    /*! \param model model name (its name in the MAPS_PROPERTIES_DEFINITION description)
    *	\param name the name for the new property. If \c NULL, the model name is used.
    *	\return A reference to the newly created MAPSProperty object.
    */
    MAPSProperty& NewProperty(const char* model, const char *name=NULL);
    //! Dynamically creates an action
    /*! \param model model number (its order in the MAPS_ACTIONS_DEFINITION description)
    *	\param name the name for the new action. If \c NULL, the model name is used.
    *	\return A reference to the newly created MAPSAction object.
    */
    MAPSAction& NewAction(int model, const char *name=NULL);
    //! Dynamically creates an action
    /*! \param model model name (its name in the MAPS_ACTIONS_DEFINITION description)
    *	\param name the name for the new action. If \c NULL, the model name is used.
    *	\return A reference to the newly created MAPSAction object.
    */
    MAPSAction& NewAction(const char* model, const char *name=NULL);
    //! Creates a new thread and associates it to the component. 
    /*! Takes the function to execute in parameter. This function takes itself no
    *	parameter, but is a function of an object, so the \c this pointer to
    *	this object is given as a hidden parameter.
    *
    *	MAPSThreadFunction is defined as :
    *	\verbatim
    typedef void (MAPSModule::*MAPSThreadFunction)(void);
    \endverbatim
    *   To ensure portability, if the thread function is a member of MAPSMyComponentClass,
    *   it must be called like this:
    *	\verbatim
    CreateThread((MAPSThreadFunction)&MAPSMyComponentClass::memberFunction);
    \endverbatim
    */
    void CreateThread(MAPSThreadFunction);
    void CreateThread(MAPSComponentThreadFunction threadFunction); // Obsolete!
    
    /*! \name User provided functions
    */
    //@{
    //! User provided function for dynamic input/output/property generation
    /*! If your component dynamically creates inputs,
    *	outputs or properties through some calls to MAPSComponent::NewInput, ...,
    *	these calls must be made in a Dynamic() overloaded function, so that
    *	the RTMaps engine can update its view of the component.
    *	By default, confirm any previous inputs, outputs, properties and actions creation.
    */
    virtual void Dynamic(void);
    //! The component main function. Its brain.
    /*! Must be overloaded. Obviously.
    *	
    *	In threaded mode, the code in Core() is executed in the main thread of the component, the
    *	one created automatically. In sequential mode, the code in Core() is
    *	executed each time data arrives on one of the component inputs.
    */
    virtual void Core(void)=0;
    //! The Birth function.
    /*! Called only once, just before the first call to Core, when the component
    *	starts. Note that the birth function is executed in the very same
    *	thread as the Core function.
    */
    virtual void Birth(void) {};
    //! The Banzai function.
    /*! It is called as soon as someone asks the component to die. Must be defined
    *	only in very rare cases (generally when we need to kill a blocked on hardware
    *	related thread). For experts only.
    */
    virtual void Banzai(void) {};
    //! The Death function
    /*! Called when the component dies, after the call the Banzai() and the death
    *	of all threads. The Death function is executed in the very same thread
    *	as the Core function.
    */
    virtual void Death(void) {};
    //@}
    
    //! Frees the output buffers
    /*! Note that this can be overloaded for very specific buffer allocations,
    *	like allocation of frames in DMA memory (by frame grabbers).
    */
    virtual void FreeBuffers();
    
    //! Must be called by the component itself
    void CommitSuicide(void);
    
    //! Dynamic behaviour support
    /*! Can be called so that inputs or outputs can be dynamically changed
    *	according to a user-supplied Dynamic() function
    */
    void CallDynamic();
    
    //! Confirms all previously created inputs, outputs, actions and properties
    /*! Can be called in Dynamic(), if no change is detected.
    */
    void DynamicConfirm();
    // The MAPSComponent standard constructor
    MAPSComponent(const char *name, MAPSComponentDefinition &cd);
    // Destructor overload
    virtual ~MAPSComponent();
    
public:
    // Overloaded functions : call dynamic when a property changes
    virtual void Set(MAPSProperty& p, bool value);				
    virtual void Set(MAPSProperty& p, MAPSInt64 value);			
    virtual void Set(MAPSProperty& p, MAPSFloat value);			
    virtual void Set(MAPSProperty& p, const MAPSString& value);
    virtual void Set(MAPSProperty& p, const MAPSEnumStruct& enumStruct);
    //! Starts the component
    /*! A component is started automatically if RTMaps is running,
    *	except when start is set to \c false in the call to MAPS::CreateComponent.
    *	If so, a call to Start is necessary to effectively start the component.
    */
    void Start();
    //! Sets automatic start mode
    /*! Usually, a component starts automatically on general run, or when the system
    *	is already running on creation. Sometimes, this is not the case, One wants to start
    *	the component later.
    *	\sa MAPS::CreateComponent
    */
    void SetAutomaticStart(bool s) { startAutomatically=s; }
    const char* ModelName() {return componentDefinition->name;}; //!< Returns the model of the component
};

/*! \addtogroup component
 * @{
 */
/*! \name Component definition macros
 * @{
 */

//! One of the standard macros required for the definition of an RTMaps component.
/*! When building an RTMaps component, one of these macros must be used in the implementation file.
 * \param component the class component name (e.g. MAPSMyComponent)
 * \param name a string used to identify the component (e.g. "myComponent")
 * \param version the version of the component; can be any string, but we recommand the use of 
 *        a float number when possible (e.g. "2.3")
 * \param priority the default component priority; must be an integer between 0 (lowest) and 255
 * \param kind MAPS::Sequential for a sequential only component, MAPS::Threaded for a threaded only component, 
 *        or a combination with a '|' if both behaviours are allowed
 * \param defaultBehaviour MAPS::Sequential or MAPS::Threaded; must be coherent with the \a kind parameter
 * \param nbOfInputs an integer indicating the (minimum) number of inputs
 * \param nbOfOutputs an integer indicating the (minimum) number of outputs
 * \param nbOfProperties an integer indicating the (minimum) number of properties
 * \param nbOfActions an integer indicating the (minimum) number of actions
 */
#define MAPS_COMPONENT_DEFINITION(component,name,version,priority,kind,defaultBehaviour,nbOfInputs,nbOfOutputs,nbOfProperties,nbOfActions) \
    MAPS_COMPONENT_DEFINITION_DOC(component,name,version,priority,kind,defaultBehaviour,nbOfInputs,nbOfOutputs,nbOfProperties,nbOfActions,NULL)

//! One of the standard macros required for the definition of an RTMaps component.
/*! Allows to add a short documentation to the component definition.
 * \copydoc MAPS_COMPONENT_DEFINITION
 * \param doc a string describing the component
 */
#define MAPS_COMPONENT_DEFINITION_DOC(component,name,version,priority,kind,defaultBehaviour,nbOfInputs,nbOfOutputs,nbOfProperties,nbOfActions,doc) \
	MAPSComponentDefinition component::s_componentDefinition={name,version,component::Instantiate,NULL,NULL,0, \
	nbOfInputs,component::inputsDefinition, \
	nbOfOutputs,component::outputsDefinition, \
	nbOfProperties,component::propertiesDefinition, \
	nbOfActions,component::actionsDefinition, \
	priority,kind,((defaultBehaviour==MAPS::Threaded)?true:false),\
	doc\
};\
MAPS_NEW_MODULE_DEFINITION(component,MAPSComponent,NULL,&component::s_componentDefinition,NULL,NULL);

//! One of the standard macros required for the definition of an RTMaps component.
/*! Use this macro when the component can be instantiated only once.
 * \copydoc MAPS_COMPONENT_DEFINITION
 * \warning When using this macro, you must use #MAPS_COMPONENT_REGISTERING_HEADER_CODE too.
 */
#define MAPS_COMPONENT_DEFINITION_UNIQUE(component,name,version,priority,kind,defaultBehaviour,nbOfInputs,nbOfOutputs,nbOfProperties,nbOfActions) \
	MAPSComponentDefinition component::s_componentDefinition={name,version,component::Instantiate,\
	component::Register,component::Unregister,1, \
	nbOfInputs,component::inputsDefinition, \
	nbOfOutputs,component::outputsDefinition, \
	nbOfProperties,component::propertiesDefinition, \
	nbOfActions,component::actionsDefinition, \
	priority,kind,((defaultBehaviour==MAPS::Threaded)?true:false) \
};\
MAPS_NEW_MODULE_DEFINITION(component,MAPSComponent,NULL,&component::s_componentDefinition,NULL,NULL);

//! One of the standard macros required for the definition of an RTMaps component.
/*! Use this macro when you need to perform special operations on component (un)registration.
 * \copydoc MAPS_COMPONENT_DEFINITION
 * \warning When using this macro, you must use #MAPS_COMPONENT_REGISTERING_HEADER_CODE too.
 */
#define MAPS_COMPONENT_DEFINITION_REGISTRATION(component,name,version,priority,kind,defaultBehaviour,nbOfInputs,nbOfOutputs,nbOfProperties,nbOfActions) \
	MAPSComponentDefinition component::s_componentDefinition={name,version,component::Instantiate, \
	component::Register,component::Unregister,0, \
	nbOfInputs,component::inputsDefinition, \
	nbOfOutputs,component::outputsDefinition, \
	nbOfProperties,component::propertiesDefinition, \
	nbOfActions,component::actionsDefinition, \
	priority,kind,((defaultBehaviour==MAPS::Threaded)?true:false) \
};\
MAPS_NEW_MODULE_DEFINITION(component,MAPSComponent,NULL,&component::s_componentDefinition,NULL,NULL);
/*@}*/

/*! \name Component class declaration macros
 * @{
 */

/*! One of the standard macros required for the definition of an RTMaps component class.
 * This macro has to be placed in the class definition (usually in the header file).
 * \param component The class name of the component
 */
#define MAPS_COMPONENT_STANDARD_HEADER_CODE(component)					\
MAPS_COMPONENT_HEADER_CODE_WITHOUT_CONSTRUCTOR(component)				\
	component(const char *componentName, MAPSComponentDefinition& md)	\
	: MAPSComponent(componentName, md) {}

/*! \copydoc MAPS_COMPONENT_STANDARD_HEADER_CODE
 * Include this macro inside your component class definition if your component implements an RTMaps clock.
 * Your component must derive from MAPSBaseClock in this case.
 */
#define MAPS_CLOCK_COMPONENT_HEADER_CODE(component)						\
MAPS_COMPONENT_HEADER_CODE_WITHOUT_CONSTRUCTOR(component)				\
	component(const char *componentName, MAPSComponentDefinition& md)	\
	: MAPSComponent(componentName, md), MAPSBaseClock(componentName) {}

/*! Include this macro inside your component class definition if you need to create/suppress inputs, outputs, properties or actions dynamically.
 * Then you need to implement MAPSComponent::Dynamic.
 * \param component The class name of the component
 * \hideinitializer
 */
#define MAPS_COMPONENT_DYNAMIC_HEADER_CODE(component) \
	void Dynamic();

/*! Include this macro inside your component class definition if you need to implement your own constructor.
 * This macro replaces #MAPS_COMPONENT_STANDARD_HEADER_CODE.
 * You need then to implement <code>MAPSMyComponent::MAPSMyComponent(const char* componentName, MAPSComponentDefinition& md):MAPSComponent(componentName,md)</code>
 * \param component The class name of the component
 */
#define MAPS_COMPONENT_HEADER_CODE_WITHOUT_CONSTRUCTOR(component)		\
	static MAPSComponent *Instantiate(	const char *componentName,		\
										MAPSComponentDefinition& md)	\
	{return new component(componentName, md);}							\
	MAPS_INPUTS_DECLARATION;											\
	MAPS_OUTPUTS_DECLARATION;											\
	MAPS_PROPERTIES_DECLARATION;										\
	MAPS_ACTIONS_DECLARATION;											\
public:																	\
	static MAPSComponentDefinition s_componentDefinition;				\
protected:																\
	void Birth();														\
	void Core(void);													\
	void Death();

/*! Include this macro inside your component class definition if you need to perform special operations whe the component is (un)registered.
 * You need then to implement <code>void MAPSMyComponent::Register(void)</code>
 * and <code>void MAPSMyComponent::Unregister(void)</code>, which are static functions.
 * \param component The class name of the component
 */
#define MAPS_COMPONENT_REGISTERING_HEADER_CODE(component)	\
	static void Register();									\
	static void Unregister();

//! Include this macro inside your component class definition if its parent is a descendant of MAPSComponent.
/*! This macro replaces #MAPS_COMPONENT_STANDARD_HEADER_CODE.
 * \param component The class name of the component
 * \param parent The class name of the parent (e.g. MAPSStream8IOComponent)
 * \hideinitializer
 */
#define MAPS_CHILD_COMPONENT_HEADER_CODE(component,parent)				\
MAPS_CHILD_COMPONENT_HEADER_CODE_WITHOUT_CONSTRUCTOR(component,parent)	\
	component(const char *componentName, MAPSComponentDefinition& md)	\
	: parent(componentName, md) {}

//! Include this macro inside your component class definition if its parent is a descendant of MAPSComponent and you need to implement your own constructor.
/*! This macro replaces #MAPS_COMPONENT_STANDARD_HEADER_CODE.
 * \param component The class name of the component
 * \param parent The class name of the parent (e.g. MAPSStream8IOComponent)
 * \hideinitializer
 */
#define MAPS_CHILD_COMPONENT_HEADER_CODE_WITHOUT_CONSTRUCTOR(component,parent)	\
	static MAPSComponent *Instantiate(	const char *componentName,				\
										MAPSComponentDefinition& md)			\
	{return new component(componentName, md);}									\
	MAPS_INPUTS_DECLARATION;													\
	MAPS_OUTPUTS_DECLARATION;													\
	MAPS_PROPERTIES_DECLARATION;												\
	MAPS_ACTIONS_DECLARATION;													\
public:																			\
	static MAPSComponentDefinition s_componentDefinition;						\
private:																		\
	void Birth();																\
	void Core(void);															\
	void Death(); 


//! Include this macro inside a parent component class definition.
/*! 
 * \param component The class name of the component
 * \param parent The class name of the parent (usually MAPSComponent).
 * \hideinitializer
 */
#define MAPS_PARENT_COMPONENT_HEADER_CODE(component,parent)				\
MAPS_PARENT_COMPONENT_HEADER_CODE_WITHOUT_CONSTRUCTOR(component,parent) \
	component(const char *componentName, MAPSComponentDefinition& md)	\
	: parent(componentName, md) {}

//! Include this macro inside a parent component class definition when you need to implement your own constructor.
/*! 
 * \param component The class name of the component
 * \param parent The class name of the parent (usually MAPSComponent).
 * \hideinitializer
 */
#define MAPS_PARENT_COMPONENT_HEADER_CODE_WITHOUT_CONSTRUCTOR(component,parent) \
private :																		\
	MAPS_INPUTS_DECLARATION;													\
	MAPS_OUTPUTS_DECLARATION;													\
	MAPS_PROPERTIES_DECLARATION;												\
	MAPS_ACTIONS_DECLARATION;													\
protected:																		\
	void Dynamic();																\
	void Birth();																\
	void Core(void);															\
	void Death();
/* @} */
/* @} */
// 
// // RTMaps RRM
// // 
const int MAPSSettingFastIOMaxNbReaders=32;

template<typename T>
	class MAPSFastIOElt
	{
		friend class MAPSFastIO<T>;	
		static const int Available;
		static const int Writing;
		static const int Reading;
		static const int Uninitialized;	

		int state;
		int readers;
		int consumptionByFIFOReaders;
		T data;
	};

class MAPSCondition2;

typedef void (*MAPSFastIOWarningFunction)(void* s);

template<typename T>
	class MAPSFastIO : public MAPSMutex
	{
	private:
		MAPSFastIOHandle
		StartReadingXX(int reader);
		MAPSCondition2* readWaitQueue;

	protected:
		MAPSEvent hHandshakeEvent;
		MAPSEvent hNeverSkippingReaderReadItEvent;
		MAPSEvent hAbortWritingEvent;
		MAPSEvent* hWriterIsDyingEvent;

		MAPSList< MAPSFastIOElt<T> > xfifo;
		int nbReaders;
		int inputSubsampling;
		int subsamplingCounter;
		int readersMask;
		int FIFOReadersMask;
		int neverSkippingReadersMask;
		int lastOrNextReadersMask;
		int timeout;
		unsigned char readersInfo[MAPSSettingFastIOMaxNbReaders];

		// Warnings handling
		MAPSFastIOWarningFunction warningFunct;
		void* paramWarningFunct;

	public:
		MAPSFastIO(MAPSEvent* hWIDEvent,
				   int fs=16,
				   int is=1,
				   int tout=MAPS::Infinite,
				   MAPSFastIOWarningFunction wf=NULL,
				   void* param=NULL);
		virtual
		~MAPSFastIO();

		T&
		operator[](MAPSFastIOHandle handle) {return xfifo[handle].data;}
		T&
		Data(MAPSFastIOHandle handle) {return xfifo[handle].data;}

		MAPSFastIOHandle
		InitBegin() {return xfifo.First();}
		void
		InitNext(MAPSFastIOHandle& h) {xfifo.Next(h);}

		int
		RegisterReader(int type=MAPS::SamplingReader, int outputSubsampling=1);
		void
		UnregisterReader(int reader);
		void
		SetReaderType(int readerNb,
					  int readerType);
		void
		SetSubsamplingRate(int readerNb,
						   int subsamplingRate);
		void
		SetWriterSubsamplingRate(int subsamplingRate);
		void
		ResetFifoSize(int fifoSize);
		MAPSFastIOHandle
		StartWriting(MAPSMutex *monitor2Release=NULL);
		void
		StopWriting(MAPSFastIOHandle h,
					bool discard=false);
		MAPSFastIOHandle
		StartReading(int reader, bool ignoresubsampling=false);
		void
		StopReading(int reader);
		void
		StopReading(int reader,
					MAPSFastIOHandle h);

		bool
		DataAvailableInFIFO(int reader);
		bool
		IsFIFOFull(void);
		bool
		IsFIFOEmpty(void);

		void
		Purge(void);
		void
		Reset(void);

		MAPSEvent*
		GetHandshakeHandle() {return &hHandshakeEvent;};

		// StartReading without the wait (abortable StartReading)
		bool
		StartReadingX(int reader,
					  MAPSEvent** handle1,
					  MAPSEvent** handle2,
					  MAPSFastIOHandle* result);
		void
		AbortStartReadingX(int reader);
		MAPSFastIOHandle
		AchieveStartReadingX(int reader,
							 int answer);

		MAPSFastIOElt<T>
		Debug(int reader,
			  MAPSFastIOHandle it);
	};

// 

//! The MAPSTimer class for managing timers
class MAPSTimer
{
    MAPSInt64 totalTime;
    MAPSInt64 lastStart;
    bool state;
public:
    //! Starts the timer
    void Start();
    //! Stops the timer
    void Stop();
    //! Resets the timer
    void Reset();
    //! Returns the current time measure in microseconds
    MAPSTimestamp Time();
    MAPSTimestamp CurrentTime() {return Time();} // Returns the current time measure in microseconds
    MAPSTimestamp GetTime() {return Time();} // Returns the current time measure in microseconds

    //! Default constructor
    MAPSTimer();
};

//Pierre 15/05/2002
struct MAPSTimestampIndexed
{
    int		    index;
    MAPSTimestamp   ts;
    static int
    CompareMAPSTimestampIndexed(const void* d1,
				const void* d2);
};

//! Defines for type filters
/*! \hideinitializer */
#define MAPSFilterUserStructure(structureName) {MAPS::TypeMask,MAPS::Structure,MAPS::AnyType,#structureName,NULL}

#endif /* _cplusplus */

#if (_MSC_VER >= 1300)
    #pragma warning(pop)
#endif

#endif

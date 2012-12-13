#ifndef MAPS_I2CSERIAL_INTERFACE_H
#define MAPS_I2CSERIAL_INTERFACE_H

#include "maps.hpp"

class MAPSI2CPortInterface
{
public:
	virtual bool isConnected() = 0;
	virtual void lock() = 0;
	virtual void release() = 0;

	virtual MAPSUInt8* setCommand(MAPSUInt8 address, MAPSUInt8 data1, MAPSUInt8 data2) = 0;
	virtual MAPSUInt8* readMultipleBytes_NoAdress(MAPSUInt8 address, int nb2read, MAPSUInt8** tab) = 0;
	virtual MAPSUInt8 readSingleByte_NoInternalRegisters(MAPSUInt8 address) = 0;
	virtual MAPSUInt8* readByte_InternalRegisters(MAPSUInt8 address, MAPSUInt8 regAdr, MAPSUInt8 nb2read ) = 0;
	virtual bool writeSingleByte_NoInternalRegisters(MAPSUInt8 address, MAPSUInt8 data) = 0;
	virtual bool writeSingleByte_InternalRegisters(MAPSUInt8 address, MAPSUInt8 regAdr, MAPSUInt8 nbData, MAPSUInt8 data ) = 0;
};

class MAPSI2CManagerInterface
{
public:
	virtual MAPSI2CPortInterface* ConnectI2CPort(MAPSString com, MAPSString compName) = 0;
	virtual void DisconnectI2C(MAPSString com, MAPSString compName) = 0;
};

#endif /* MAPS_I2CSERIAL_INTERFACE_H */

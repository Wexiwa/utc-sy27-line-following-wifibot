////////////////////////////////
// RTMaps SDK Component
////////////////////////////////

////////////////////////////////
// Purpose of this module :
////////////////////////////////


#include "maps.hpp"
#include "maps_wifibot_i2c.h"

#define MAX_TEXT_SIZE 512

#define VERSION_CMD	0x01
#define MAX_BUFF 15

// Add here more inputs to your components
MAPS_BEGIN_INPUTS_DEFINITION(MAPSwifibot_i2c)
	MAPS_INPUT("iMotorsLR", MAPS::FilterNumber, MAPS::FifoReader)
	MAPS_INPUT("iStreamIHM", MAPS::FilterStream8, MAPS::FifoReader)
MAPS_END_INPUTS_DEFINITION

// Add here more outputs to your components
MAPS_BEGIN_OUTPUTS_DEFINITION(MAPSwifibot_i2c)
	MAPS_OUTPUT("oBattery",MAPS::Integer,NULL,NULL,1)
	MAPS_OUTPUT("oOdemeterLR",MAPS::Integer,NULL,NULL,2)
	MAPS_OUTPUT("oStreamIHM",MAPS::Stream8,NULL,NULL,MAX_BUFF)
MAPS_END_OUTPUTS_DEFINITION

// Add here more properties to your components
MAPS_BEGIN_PROPERTIES_DEFINITION(MAPSwifibot_i2c)
	MAPS_PROPERTY("pCOM", "COM3", false, false)
	//MAPS_PROPERTY_ENUM("pMode", "TCP mode|UDP mode", 0, false, false)
	MAPS_PROPERTY_READ_ONLY("pBatteryLevel", -1)
	MAPS_PROPERTY("pPeriod", 500000, false, true)
	MAPS_PROPERTY("pUseWifibotIHM", false, false, true)
MAPS_END_PROPERTIES_DEFINITION

// Add here more actions to your components
MAPS_BEGIN_ACTIONS_DEFINITION(MAPSwifibot_i2c)
	MAPS_ACTION("aGetMotors",MAPSwifibot_i2c::GetMotors)
	MAPS_ACTION2("aStop",MAPSwifibot_i2c::Stop, true)
MAPS_END_ACTIONS_DEFINITION

// Define your component name, version, behaviour, etc. Define also its inputs, outputs, properties and actions number.
MAPS_COMPONENT_DEFINITION(MAPSwifibot_i2c,"wifibot_i2c","1.4",128,MAPS::Sequential|MAPS::Threaded,
			  MAPS::Threaded,0,2,4,2)


void MAPSwifibot_i2c::Dynamic()
{
	if (GetBoolProperty("pUseWifibotIHM") == true)
	{
		NewInput(1, "iStreamIHM");
		NewOutput(2, "oStreamIHM");
	}else
	{
		NewInput(0, "iMotorsLR"); 
	}
}

		  
void MAPSwifibot_i2c::GetMotors(MAPSModule *module,int actionNb)
{
	if (MAPS::IsRunning())
		MAPSwifibot_i2c &comp=*(MAPSwifibot_i2c *)module;
}

void MAPSwifibot_i2c::Stop(MAPSModule *module,int actionNb)
{
	if (!MAPS::IsRunning())
		return;
	MAPSwifibot_i2c &comp=*(MAPSwifibot_i2c *)module;

	comp.pI2CCom->writeSingleByte_InternalRegisters(0xA2, 0x00, 0x01, 0x00);
	comp.pI2CCom->writeSingleByte_InternalRegisters(0xA4, 0x00, 0x01, 0x00);
}

void MAPSwifibot_i2c::Birth()
{
	// Load core function
	try {
		MAPS::LoadCoreFunction("I2CSerial");
	} catch(...) {Error("Failed to load I2CSerial core module.");}
	MAPSCoreFunctionInterface* cfi = MAPS::CoreFunction("I2CSerial");
	if(cfi == NULL)
		Error("I2CSerial was not registered.");
	pI2CCom = ((MAPSI2CManagerInterface*)cfi->GetInterface())->ConnectI2CPort(GetStringProperty("pCOM"), this->Name());
	if (pI2CCom == NULL) {
		MAPSString sx("Could not connect to com port ");
		sx += GetStringProperty("pCOM");
		Error(sx);
	}
	_firstTime = true;
	
	_ihmConnected = false;

	MAPS::Memcpy(buffer_address,"0.0.0.0", 8);
}


struct SensorData MAPSwifibot_i2c::GetI2cMotor(MAPSUInt8 adr)
{
	SensorData data; data.SpeedFront = -1; data.SpeedRear = -1; data.IR = -1;data.IR2 = -1; data.odometry = -1;
	MAPSUInt8* tmp = pI2CCom->readByte_InternalRegisters(adr, 0x00, 8);

	if (tmp == NULL)
		return data;
	data.SpeedFront = tmp[0];
	data.SpeedRear = tmp[1];
	data.IR = tmp[2];
	data.IR2 = tmp[3];
	data.odometry=((((long)tmp[7] << 24))+(((long)tmp[6] << 16))+(((long)tmp[5] << 8))+((long)tmp[4]));

	return data;
}

void MAPSwifibot_i2c::Core()
{
    if (_firstTime)
    {
		_firstTime=false;
		MAPSUInt8 * res = pI2CCom->setCommand(VERSION_CMD,0x00,0x00);
	
		if (res!=NULL)
		{
			int firmware = (int)res[0];
			MAPSStreamedString s;
			s << "Firmware version : " << firmware;
			ReportInfo(s);
		}

		if (GetBoolProperty("pUseWifibotIHM") == true)
			CreateThread((MAPSThreadFunction)&MAPSwifibot_i2c::ReceivedIHMStream);
		else
			CreateThread((MAPSThreadFunction)&MAPSwifibot_i2c::SetMotorsInputs);
	}

	if (GetBoolProperty("pUseWifibotIHM") == true)
	{
		Wait4Event(isDyingEvent);
	}else{
		SensorData dataL = GetI2cMotor(0xA2);
		SensorData dataR = GetI2cMotor(0xA4);

		DirectSetProperty("pBatteryLevel", dataL.SpeedRear+20);

		MAPSIOElt* out = StartWriting(Output("oBattery"));
		out->Integer(0) = dataL.SpeedRear+20;
		StopWriting(out);

		MAPSIOElt* outO = StartWriting(Output("oOdemeterLR"));
		outO->Integer(0) = dataL.odometry;
		outO->Integer(1) = dataR.odometry;
		StopWriting(outO);

		Rest(GetIntegerProperty("pPeriod"));
	}
}

void MAPSwifibot_i2c::SetMotorsInputs()
{
	while (!IsDying())
	{
		MAPSIOElt* ioElt = StartReading(Input("iMotorsLR"));
		if (ioElt==NULL)
			return;

		unsigned int speedL;
		unsigned int speedR;

		if (MAPS::TypeFilter(ioElt->Type(), MAPS::FilterInteger) )
		{
			speedL = normalize(ioElt->Integer(0));
			speedR = normalize(ioElt->Integer(1));
		}else{
			speedL = normalize((int)ioElt->Float(0));
			speedR = normalize((int)ioElt->Float(1));
		}
		pI2CCom->writeSingleByte_InternalRegisters(0xA2, 0x00, 0x01, speedL);
		pI2CCom->writeSingleByte_InternalRegisters(0xA4, 0x00, 0x01, speedR);
	}
	//Si on sort du while c'est que le composant est en train de mourir
	pI2CCom->release();
}

void MAPSwifibot_i2c::ReceivedIHMStream()
{
	while (!IsDying())
	{
		MAPSIOElt* ioElt = StartReading(Input("iStreamIHM"));
		if (ioElt==NULL)
			return;

		_ihmConnected = true;
		
		int size = ioElt->VectorSize();
		char* buff = new char[size];
		
		MAPS::Memcpy(buff, ioElt->Stream8(), 2 );
		MAPSUInt8 speedL = (unsigned char)buff[0];
		MAPSUInt8 speedR = (unsigned char)buff[1];

		pI2CCom->writeSingleByte_InternalRegisters(0xA2, 0x00, 0x01, speedL);
		pI2CCom->writeSingleByte_InternalRegisters(0xA4, 0x00, 0x01, speedR);

		SensorData dataL = GetI2cMotor(0xA2);
		SensorData dataR = GetI2cMotor(0xA4);

		unsigned char buffso_send[15];

		buffso_send[0]=(char)(dataL.SpeedRear+20);//GetADC(hUSB,0x48);
		buffso_send[1]=(char)dataL.SpeedFront;
		buffso_send[2]=(char)dataL.SpeedFront;
		buffso_send[3]=(char)dataR.SpeedFront;
		buffso_send[4]=(char)dataR.SpeedFront;
		buffso_send[5]=(char)dataL.IR;
		buffso_send[6]=(char)dataR.IR;
		buffso_send[7]=(char)dataL.odometry;
		buffso_send[8]=(char)(dataL.odometry >> 8);
		buffso_send[9]=(char)(dataL.odometry >> 16);
		buffso_send[10]=(char)(dataL.odometry >> 24);
		buffso_send[11]=(char)dataR.odometry;
		buffso_send[12]=(char)(dataR.odometry >> 8);
		buffso_send[13]=(char)(dataR.odometry >> 16);
		buffso_send[14]=(char)(dataR.odometry >> 24);
		_mutexIHM.Release();

		MAPSIOElt* out = StartWriting(Output("oStreamIHM"));
		MAPS::Memcpy(out->Data(), buffso_send, MAX_BUFF);
		StopWriting(out);

		DirectSetProperty("pBatteryLevel", dataL.SpeedRear+20);

		out = StartWriting(Output("oBattery"));
		out->Integer(0) = dataL.SpeedRear+20;
		StopWriting(out);

		MAPSIOElt* outO = StartWriting(Output("oOdemeterLR"));
		outO->Integer(0) = dataL.odometry;
		outO->Integer(1) = dataR.odometry;
		StopWriting(outO);

	}
}

/*
TROOPER
void MAPSwifibot_i2c::ReceivedIHMStream()
{
	while (!IsDying())
	{
		MAPSIOElt* ioElt = StartReading(Input("iStreamIHM"));
		if (ioElt==NULL)
			return;

		int size = ioElt->VectorSize();
		char* buff = new char[size];
		
		MAPS::Memcpy(buff, ioElt->Stream8(), size );
		if (!( strstr(buff, "init") == NULL))
		{
			_ihmConnected = true;
			MAPSIOElt* out = StartWriting(Output("oStreamIHM"));
			MAPS::Memcpy(out->Data(), "ok", 2);
			StopWriting(out);

			ReportWarning("Asking for init, reply ok");
		}else if (!(strstr(buff,"data")==NULL))
		{
			//We look if the reference address has changed
			if(strstr(buff,buffer_address)==NULL)
			{
				//We get the new reference address
				MAPS::Memcpy(buffer_address, buff+5*sizeof(char), 15*sizeof(char));
				//printf("address: %s\n",buffer_address);
			}//enf of if address

			//We get our IP address
			if (gethostname(ac, sizeof(ac)) != SOCKET_ERROR)
			{
				phe = gethostbyname(ac);
				if (phe!=0) memcpy(&addr, phe->h_addr_list[0], sizeof(struct in_addr));
				sprintf_s(buffer_localip,15,"%d.%d.%d.%d",addr.S_un.S_un_b.s_b1,addr.S_un.S_un_b.s_b2,addr.S_un.S_un_b.s_b3,addr.S_un.S_un_b.s_b4);
			}

			MAPS::Memcpy((buffso_sendOUT+5*sizeof(char),buffer_localip,15*sizeof(char));
		}
	}
}*/

unsigned int MAPSwifibot_i2c::normalize(int value)
{
	unsigned int res;

	if (value >0)
	{
		value += 64;
		if ( value>= 128)
			res = 127;
		else
			res = value;
	}
	else {
		if (abs(value) >=64)
			res = 63;
		else res = abs(value);
	}

	return res;
}

// The Death() method
void MAPSwifibot_i2c::Death()
{
	Stop(this,0);
	MAPSCoreFunctionInterface* cfi = MAPS::CoreFunction("I2CSerial");
	if(cfi != NULL)
		((MAPSI2CManagerInterface*)cfi->GetInterface())->DisconnectI2C(GetStringProperty("pCOM"), this->Name());
}

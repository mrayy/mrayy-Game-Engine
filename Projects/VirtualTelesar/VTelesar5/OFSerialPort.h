

/********************************************************************
	created:	2013/02/18
	created:	18:2:2013   14:13
	filename: 	C:\Development\mrayEngine\Projects\VirtualTelesar\VTelesar5\OFSerialPort.h
	file path:	C:\Development\mrayEngine\Projects\VirtualTelesar\VTelesar5
	file base:	OFSerialPort
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __OFSerialPort__
#define __OFSerialPort__


#include "ISerialPort.h"

class ofSerial;

namespace mray
{
namespace OS
{

class OFSerialPort:public ISerialPort
{
protected:
	ofSerial* m_sp;
	bool m_open;
	IStream* m_stream;
public:
	OFSerialPort();
	virtual~OFSerialPort();


	virtual bool OpenByName(const core::string& port,int baudRate);
	virtual bool OpenByID(int ID,int baudRate);
	virtual void Close();

	virtual int Write(const void* data,size_t size);
	virtual int Read(void* data,size_t size);

	virtual int AvailableData();
	virtual bool IsOpen();
	virtual void Flush(bool flushIn,bool flushOut);

	virtual IStream* GetStream();

};


class OFSerialPortService:public ISerialPortService
{
public:
	virtual std::vector<SerialPortInfo> EnumAvaliablePorts();
};


}
}


#endif

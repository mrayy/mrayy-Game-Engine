

#include "stdafx.h"
#include "ISerialPort.h"





namespace mray
{
namespace OS
{


SerialPortStream::SerialPortStream(ISerialPort* sp)
{
	m_serialPort=sp;
}
SerialPortStream::~SerialPortStream()
{
}

bool SerialPortStream::canRead()
{
	return true;
}
bool SerialPortStream::canWrite()
{
	return true;
}
bool SerialPortStream::canSeek()
{
	return false;
}
int SerialPortStream::length()
{
	return m_serialPort->AvailableData();
}
int SerialPortStream::getPos()
{
	return -1;
}

bool SerialPortStream::isOpen()
{
	return m_serialPort->IsOpen();
}
int SerialPortStream::read(void*data,size_t size)
{
	return m_serialPort->Read(data,size);
}

int SerialPortStream::write(const void*data,size_t size)
{
	return m_serialPort->Write(data,size);
}

int SerialPortStream::seek(int bytes,EStreamSeek mode)
{
	return 0;
}
bool SerialPortStream::eof()
{
	return isOpen();
}
void SerialPortStream::close()
{
	m_serialPort->Close();
}


}
}

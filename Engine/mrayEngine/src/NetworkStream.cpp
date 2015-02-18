

#include "StdAfx.h"
#include "NetworkStream.h"
#include "ISocket.h"


namespace mray
{
namespace network
{

NetworkStream::NetworkStream(ISocket*socket)
:m_socket(socket),IStream(mT("SocketStream"),OS::BIN_APPEND)
{
}
NetworkStream::~NetworkStream()
{
}


bool NetworkStream::canRead()
{
	if(m_socket->isConnected())
		return true;
	return false;
}
bool NetworkStream::canWrite()
{
	if(m_socket->isConnected())
		return true;
	return false;
}
bool NetworkStream::canSeek()
{
	return false;
}
int NetworkStream::length()
{
	return 0;
}
int NetworkStream::getPos()
{
	return 0;
}


bool NetworkStream::isOpen()
{
	if(m_socket->isConnected())
		return true;
	return false;
}
int NetworkStream::read(void*data,size_t size)
{
	if(!m_socket->isConnected())
		return 0;
	return m_socket->readBytes(data,size,0);
}
int NetworkStream::write(const void*data,size_t size)
{
	if(!m_socket->isConnected())
		return 0;
	//return m_socket->writeBytes(data,size,0,true,ESM_RELIABLE);
	return false;
}
int NetworkStream::seek(int bytes,OS::EStreamSeek mode)
{
	return 0;
}
bool NetworkStream::eof()
{
	return false;
}
void NetworkStream::close()
{
}

int NetworkStream::writeByte(uchar v)
{
	return 0;
}
uchar NetworkStream::readByte()
{
	return 0;
}


}
}

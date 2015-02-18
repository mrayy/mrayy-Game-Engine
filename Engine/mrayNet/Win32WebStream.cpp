
#include "stdafx.h"
#include "Win32WebStream.h"
#include "Win32WebRequest.h"


namespace mray
{
namespace network
{


Win32WebStream::Win32WebStream(Win32WebRequest*owner)
:m_owner(owner)
{
	m_length=0;
}
Win32WebStream::~Win32WebStream()
{
}

void Win32WebStream::Refresh()
{
	m_length = m_owner->Length();
}
uint Win32WebStream::calcSizeInternal()
{
	return sizeof(Win32WebStream);
}

bool Win32WebStream::canRead()
{
	return true;
}
bool Win32WebStream::canWrite()
{
	return true;
}
bool Win32WebStream::canSeek()
{
	return false;
}
int Win32WebStream::getPos()
{
	return 0;
}


bool Win32WebStream::isOpen()
{
	return m_owner->IsConnected();
}
int Win32WebStream::read(void*data,size_t size)
{
	return m_owner->Read(data,size);
}
int Win32WebStream::write(const void*data,size_t size)
{
	return m_owner->Write(data,size);
}
int Win32WebStream::seek(int bytes,OS::EStreamSeek mode)
{
	return 0;
}
bool Win32WebStream::eof()
{
	return m_owner->EoF();
}
void Win32WebStream::close()
{
	m_owner->Disconnect();
}

}
}


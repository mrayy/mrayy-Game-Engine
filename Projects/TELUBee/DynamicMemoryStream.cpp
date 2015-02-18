


#include "stdafx.h"
#include "DynamicMemoryStream.h"


namespace mray
{
namespace OS
{

DynamicMemoryStream::DynamicMemoryStream()
{
	m_pos=0;
}
DynamicMemoryStream::~DynamicMemoryStream()
{

}
int DynamicMemoryStream::read(void*data,size_t size)
{
	if(m_pos>=m_data.size())
		return 0;
	if(m_pos + size>m_data.size())
		size=m_data.size()-m_pos;
	mraySystem::memCopy(data,&m_data[0]+m_pos,size);
	m_pos+=size;
	return size;

}
int DynamicMemoryStream::write(const void*data,size_t size)
{
	if(m_pos + size>length())
	{
		m_data.resize(m_pos+size);
	}
	mraySystem::memCopy(&m_data[0]+m_pos,data,size);
	m_pos+=size;
	return size;
}
int DynamicMemoryStream::seek(int bytes,EStreamSeek mode)
{

	switch(mode){
	case ESeek_Set:
		m_pos=bytes;
		break;
	case ESeek_Cur:
		m_pos+=bytes;
		break;
	case ESeek_End:
		m_pos=length()-bytes;
		break;
	}
	if(m_pos>length())
		m_pos=length();
	return m_pos;
}
bool DynamicMemoryStream::eof()
{
	return m_pos==length();
}
void DynamicMemoryStream::close()
{

}

}
}


#include "stdafx.h"

#include "SoftwareStreamBuffer.h"
#include "mraySystem.h"


namespace mray{
namespace video{

	const core::string SoftwareStreamBuffer::s_type=mT("Software");

SoftwareStreamBuffer::SoftwareStreamBuffer(EStreamDataType dataType,uint length,
	uint id,EUsageType usage):IHardwareStreamBuffer(dataType,length,usage,id,false)
{
	m_data.resize(m_sizeInBytes);
	if(m_sizeInBytes)
		mraySystem::memSet(&m_data[0],0,m_sizeInBytes*sizeof(uchar));
}
SoftwareStreamBuffer::~SoftwareStreamBuffer(){
	m_data.clear();
}

bool SoftwareStreamBuffer::inner_resize(uint newSize){
	if(!IHardwareStreamBuffer::inner_resize(newSize))
		return false;
	m_data.resize(newSize);
	return true;
}

void*SoftwareStreamBuffer::inner_lock(uint offset,uint length,ELockOptions options){
	if(m_data.size())
		return &m_data[0]+offset;
	else return 0;
}
void SoftwareStreamBuffer::inner_unlock(){
	
}

void*SoftwareStreamBuffer::lock(uint offset,uint length,IHardwareBuffer::ELockOptions options){
	m_isLocked=true;
	return inner_lock(offset,length,options);
}
void SoftwareStreamBuffer::unlock(){
	m_isLocked=false;
}


uint SoftwareStreamBuffer::readData(uint offset,uint length,void*dest){
	if(length==0)
		length=m_data.size()-offset;
	else
		if(offset+length>m_data.size())
			length=m_data.size()-offset;
	if(m_data.size())
		mraySystem::memCopy(dest,&m_data[0]+offset,length);
	return length;
}
uint SoftwareStreamBuffer::writeData(uint offset,uint length,const void*src,bool disacardWholeBuffer){
	if(length==0)
		length=(uint)((long)m_data.size()-(long)offset);
	else
		if(offset+length>m_data.size())
			length=(uint)((long)m_data.size()-(long)offset);
	if(m_data.size())
		mraySystem::memCopy(&m_data[0]+offset,src,length);
	return length;

}
void* SoftwareStreamBuffer::getData(uint offset){
	if(m_data.size()==0)
		return 0;
	return (void*)(&m_data[0]+offset);
}

}
}

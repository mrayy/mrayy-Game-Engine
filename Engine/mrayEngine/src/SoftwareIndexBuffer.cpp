#include "stdafx.h"

#include "SoftwareIndexBuffer.h"

namespace mray{
namespace video{


const core::string SoftwareIndexBuffer::s_type=mT("Software");

SoftwareIndexBuffer::SoftwareIndexBuffer(EIndexType type,uint numIndicies,uint id,EUsageType usage):
	IHardwareIndexBuffer(type,numIndicies,usage,id,false)
{
	m_data.resize(m_sizeInBytes);
}

SoftwareIndexBuffer::~SoftwareIndexBuffer(){
}


bool SoftwareIndexBuffer::inner_resize(uint newSize){
	if(!IHardwareIndexBuffer::inner_resize(newSize))
		return false;
	m_data.resize(newSize);
	return true;
}

void*SoftwareIndexBuffer::inner_lock(uint offset,uint length,ELockOptions options){
	if(m_data.size())
		return &m_data[0]+offset;
	return 0;
}

void SoftwareIndexBuffer::inner_unlock(){
}


void*SoftwareIndexBuffer::lock(uint offset,uint length,ELockOptions options){
	m_isLocked=true;
	return inner_lock(offset,length,options);
}

void SoftwareIndexBuffer::unlock(){
	m_isLocked=false;
}



uint SoftwareIndexBuffer::readData(uint offset,uint length,void*dest){
	if(length==0)
		length=m_data.size()-offset;
	else
	if(offset+length>m_data.size())
		length=m_data.size()-offset;
	if(m_data.size())
		mraySystem::memCopy(dest,&m_data[0]+offset,length);
	return length;
}

uint SoftwareIndexBuffer::writeData(uint offset,uint length,const void*src,bool disacardWholeBuffer){
	if(length==0)
		length=m_data.size()-offset;
	else
		if(offset+length>m_data.size())
			length=m_data.size()-offset;
	if(m_data.size())
		mraySystem::memCopy(&m_data[0]+offset,src,length);
	return length;
}
void* SoftwareIndexBuffer::getData(uint offset){
	if(m_data.size())
		return (void*)(&m_data[0]+offset);
	return 0;
}


}
}
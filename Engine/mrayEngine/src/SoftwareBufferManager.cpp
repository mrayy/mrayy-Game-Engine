#include "stdafx.h"

#include "SoftwareBufferManager.h"
#include "SoftwareStreamBuffer.h"
#include "SoftwareIndexBuffer.h"


namespace mray{
namespace video{


SoftwareBufferManager::SoftwareBufferManager(){
	m_lastStreamID=0;
	m_lastIndexID=0;
}

SoftwareBufferManager::~SoftwareBufferManager(){
}

uint SoftwareBufferManager::getStreamID(){
	return m_lastStreamID++;
}
uint SoftwareBufferManager::getIndexID(){
	return m_lastIndexID++;

}

IHardwareStreamBuffer* SoftwareBufferManager::createHardwareStreamBuffer(EStreamDataType dataType,uint length,
																			   IHardwareBuffer::EUsageType usage,bool useVirtualBuffer)
{
	return createSoftwareStreamBuffer(dataType,length,usage);
}


IHardwareIndexBuffer* SoftwareBufferManager::createHardwareIndexBuffer(IHardwareIndexBuffer::EIndexType type,uint numIndicies,IHardwareBuffer::EUsageType usage,bool useVirtualBuffer){
	return createSoftwareIndexBuffer(type,numIndicies,usage);
}
SoftwareStreamBuffer* SoftwareBufferManager::createSoftwareStreamBuffer(EStreamDataType dataType,uint length,
															   IHardwareBuffer::EUsageType usage)
{
	return new SoftwareStreamBuffer(dataType,length,getStreamID(),usage);
}

SoftwareIndexBuffer* SoftwareBufferManager::createSoftwareIndexBuffer(IHardwareIndexBuffer::EIndexType type,uint numIndicies,IHardwareBuffer::EUsageType usage)
{
	return new SoftwareIndexBuffer(type,numIndicies,getIndexID(),usage);

}

}
}


#include "stdafx.h"
#include "D3DHardwareBufferManager.h"

#include "D3DStreamBuffer.h"
#include "D3DIndexBuffer.h"
#include "SoftwareStreamBuffer.h"
#include "SoftwareIndexBuffer.h"

namespace mray
{
namespace video
{

D3DHardwareBufferManager::D3DHardwareBufferManager(LPDIRECT3DDEVICE9 lpDev)
{
	m_lpDev=lpDev;
	m_lastStreamID=0;
	m_lastIndexID=0;
}

D3DHardwareBufferManager::~D3DHardwareBufferManager()
{
}

uint D3DHardwareBufferManager::getStreamID(){
	return m_lastStreamID++;
}
uint D3DHardwareBufferManager::getIndexID(){
	return m_lastIndexID++;

}

IHardwareStreamBuffer*  D3DHardwareBufferManager::createHardwareStreamBuffer(EStreamDataType dataType,uint length,
	IHardwareBuffer::EUsageType usage,bool useVirtualBuffer)
{
	return new D3DStreamBuffer(dataType,length,usage,true,m_lpDev,getStreamID(),useVirtualBuffer);
}


IHardwareIndexBuffer*  D3DHardwareBufferManager::createHardwareIndexBuffer(IHardwareIndexBuffer::EIndexType type,uint numIndicies,IHardwareBuffer::EUsageType usage,bool useVirtualBuffer){
	return new D3DIndexBuffer(type,numIndicies,usage,true,m_lpDev,getIndexID(),useVirtualBuffer);
}

SoftwareStreamBuffer* D3DHardwareBufferManager::createSoftwareStreamBuffer(EStreamDataType dataType,uint length,
	IHardwareBuffer::EUsageType usage)
{
	return new SoftwareStreamBuffer(dataType,length,getStreamID(),usage);
}

SoftwareIndexBuffer*  D3DHardwareBufferManager::createSoftwareIndexBuffer(IHardwareIndexBuffer::EIndexType type,uint numIndicies,IHardwareBuffer::EUsageType usage)
{
	return new SoftwareIndexBuffer(type,numIndicies,getIndexID(),usage);

}

}
}



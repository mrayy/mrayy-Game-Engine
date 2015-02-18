
#include "stdafx.h"
#include "D3DIndexBuffer.h"

#include "D3DEnums.h"
#include "VideoLoggerSystem.h"
#include "D3D9Device.h"

namespace mray
{
namespace video
{

D3DIndexBuffer::D3DIndexBuffer(EIndexType type,uint numIndicies,EUsageType usage,bool useSystemMem,
							   LPDIRECT3DDEVICE9 lpDev,uint id,bool useVirtualBuffer):
IHardwareIndexBuffer(type,numIndicies,usage,id,useVirtualBuffer),m_lpDev(lpDev)
{
	m_pool=useSystemMem ? D3DPOOL_SYSTEMMEM : 
		((usage & EUT_Discardable ) ? D3DPOOL_DEFAULT : D3DPOOL_MANAGED);
	HRESULT hr=lpDev->CreateIndexBuffer(m_sizeInBytes,
										 D3DEnums::Map(usage),D3DEnums::Map(type),m_pool,&m_d3dBuff,0);
	if(FAILED(hr))
	{
		gVideoLoggerSystem.log(mT("D3DIndexBuffer() - Faield to create index buffer"),ELL_WARNING);
	}
}
D3DIndexBuffer::~D3DIndexBuffer()
{
	SAFE_RELEASE(m_d3dBuff);
}



uint D3DIndexBuffer::readData(uint offset,uint length,void*dest)
{
	void* pSrc = this->lock(offset, length, ELO_ReadOnly);
	memcpy(dest, pSrc, length);
	this->unlock();
	return length;
}
uint D3DIndexBuffer::writeData(uint offset,uint length,const void*src,bool disacardWholeBuffer)
{
	void* pDst = this->lock(offset, length, disacardWholeBuffer ? ELO_Discard: ELO_Normal);
	memcpy(pDst, src, length);
	this->unlock();
	return length;
}

const core::string& D3DIndexBuffer::getDeviceType()
{
	return D3D9Device::m_deviceType;
}

void*D3DIndexBuffer::inner_lock(uint offset,uint length,ELockOptions options)
{
	void* pBuf;
	HRESULT hr=m_d3dBuff->Lock(offset,length,&pBuf,
		D3DEnums::Map(options,m_usage));

	if(FAILED(hr))
	{
		gVideoLoggerSystem.log(mT("D3DIndexBuffer::inner_lock() - Faield to lock index buffer"),ELL_WARNING);
	}
	return pBuf;
}
void D3DIndexBuffer::inner_unlock()
{
	m_d3dBuff->Unlock();
}

bool D3DIndexBuffer::inner_resize(uint newSize)
{
	if(newSize==m_sizeInBytes)
		return true;
	SAFE_RELEASE(m_d3dBuff);
	HRESULT hr=m_lpDev->CreateIndexBuffer(m_sizeInBytes,
		D3DEnums::Map(m_usage),D3DEnums::Map(m_indexType),m_pool,&m_d3dBuff,0);
	return SUCCEEDED(hr);
}

}
}


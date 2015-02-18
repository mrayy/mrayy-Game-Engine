



/********************************************************************
	created:	2012/03/04
	created:	4:3:2012   16:22
	filename: 	d:\Development\mrayEngine\Engine\mrayD3D9Device\D3DHardwareBufferManager.h
	file path:	d:\Development\mrayEngine\Engine\mrayD3D9Device
	file base:	D3DHardwareBufferManager
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __D3DHardwareBufferManager__
#define __D3DHardwareBufferManager__


#include <IHardwareBufferManager.h>
#include "compileConfig.h"

namespace mray
{
namespace video
{

class D3DHardwareBufferManager:public IHardwareBufferManager
{
private:
protected:
	uint m_lastStreamID;
	uint m_lastIndexID;

	LPDIRECT3DDEVICE9 m_lpDev;

	uint getStreamID();
	uint getIndexID();
public:
	D3DHardwareBufferManager(LPDIRECT3DDEVICE9 lpDev);
	virtual~D3DHardwareBufferManager();

	void SetD3DDevice(LPDIRECT3DDEVICE9 lpDev){m_lpDev=lpDev;}
	LPDIRECT3DDEVICE9 GetD3DDevice(){return m_lpDev;}

	virtual IHardwareStreamBuffer*  createHardwareStreamBuffer(EStreamDataType dataType,uint length,
		IHardwareBuffer::EUsageType usage,bool useVirtualBuffer);

	virtual IHardwareIndexBuffer*  createHardwareIndexBuffer(IHardwareIndexBuffer::EIndexType type,uint numIndicies,IHardwareBuffer::EUsageType usage,bool useVirtualBuffer);

	virtual SoftwareStreamBuffer*  createSoftwareStreamBuffer(EStreamDataType dataType,uint length,
		IHardwareBuffer::EUsageType usage);

	virtual SoftwareIndexBuffer*  createSoftwareIndexBuffer(IHardwareIndexBuffer::EIndexType type,uint numIndicies,IHardwareBuffer::EUsageType usage);
};


}
}

#endif
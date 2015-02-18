


/********************************************************************
	created:	2012/03/04
	created:	4:3:2012   15:41
	filename: 	d:\Development\mrayEngine\Engine\mrayD3D9Device\D3DIndexBuffer.h
	file path:	d:\Development\mrayEngine\Engine\mrayD3D9Device
	file base:	D3DIndexBuffer
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __D3DIndexBuffer__
#define __D3DIndexBuffer__

#include <IHardwareIndexBuffer.h>
#include "compileConfig.h"

namespace mray
{
namespace video
{

class D3DIndexBuffer:public IHardwareIndexBuffer
{
private:
protected:
	LPDIRECT3DINDEXBUFFER9 m_d3dBuff;
	D3DPOOL m_pool;
	LPDIRECT3DDEVICE9 m_lpDev;

	virtual void*inner_lock(uint offset,uint length,ELockOptions options);
	virtual void inner_unlock();

	virtual bool inner_resize(uint newSize);
public:
	D3DIndexBuffer(EIndexType type,uint numIndicies,EUsageType usage,bool useSystemMem,
		LPDIRECT3DDEVICE9 lpDev,uint id,bool useVirtualBuffer);
	virtual~D3DIndexBuffer();

	LPDIRECT3DINDEXBUFFER9 getD3DBuffer()const{return m_d3dBuff;}

	uint readData(uint offset,uint length,void*dest);
	uint writeData(uint offset,uint length,const void*src,bool disacardWholeBuffer);

	virtual bool isInGPU(){return true;}
	virtual const core::string& getDeviceType();
};

}
}

#endif


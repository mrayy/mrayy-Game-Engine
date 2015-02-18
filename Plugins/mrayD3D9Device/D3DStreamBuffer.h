

/********************************************************************
	created:	2012/03/04
	created:	4:3:2012   14:52
	filename: 	d:\Development\mrayEngine\Engine\mrayD3D9Device\D3DStreamBuffer.h
	file path:	d:\Development\mrayEngine\Engine\mrayD3D9Device
	file base:	D3DStreamBuffer
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __D3DStreamBuffer__
#define __D3DStreamBuffer__


#include <IHardwareStreamBuffer.h>

namespace mray
{
namespace video
{

class D3DStreamBuffer:public IHardwareStreamBuffer
{
private:
protected:
	LPDIRECT3DVERTEXBUFFER9 m_d3dBuff;
	D3DPOOL m_pool;
	LPDIRECT3DDEVICE9 m_lpDev;

	virtual void*inner_lock(uint offset,uint length,ELockOptions options);
	virtual void inner_unlock();

	virtual bool inner_resize(uint newSize);
public:
	D3DStreamBuffer(EStreamDataType dataType,uint length,EUsageType usage,bool useSystemMem,
		LPDIRECT3DDEVICE9 lpDev,uint id,bool useVirtualBuffer);
	virtual~D3DStreamBuffer();

	LPDIRECT3DVERTEXBUFFER9 getD3DBuffer()const{return m_d3dBuff;}

	uint readData(uint offset,uint length,void*dest);
	uint writeData(uint offset,uint length,const void*src,bool disacardWholeBuffer);

	virtual bool isInGPU(){return true;}
	virtual const core::string& getDeviceType();
};

}
}

#endif


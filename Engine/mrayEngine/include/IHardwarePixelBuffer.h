

/********************************************************************
	created:	2009/08/12
	created:	12:8:2009   8:19
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\IHardwarePixelBuffer.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine
	file base:	IHardwarePixelBuffer
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___IHardwarePixelBuffer___
#define ___IHardwarePixelBuffer___

#include "IHardwareBuffer.h"
#include "LockedPixelBox.h"

namespace mray{
namespace video{

class MRAY_DLL IHardwarePixelBuffer:public IHardwareBuffer
{
private:
protected:
	uint m_width,m_height,m_depth;
	uint m_rowPitch,m_slicePitch;

	//uint m_sizeInBytes;

	ETextureType m_texType;
	EPixelFormat m_format;
	LockedPixelBox m_lockedBox;

	virtual LockedPixelBox inner_lock(const math::box3d&lockBox,ELockOptions options)=0;
	virtual void inner_unlock()=0;

	virtual void*inner_lock(uint offset,uint length,ELockOptions options);

public:
	IHardwarePixelBuffer(uint width,uint height,uint depth,ETextureType texType,EPixelFormat format,
		EUsageType usage,uint id,bool useVirtualBuffer);

	virtual~IHardwarePixelBuffer();

	EPixelFormat getFormat(){return m_format;}

	virtual const LockedPixelBox&lock(const math::box3d&lockBox,ELockOptions options);
	virtual void*lock(uint offset,uint length,ELockOptions options);

	virtual uint readData(uint offset,uint length,void*dest);
	virtual uint writeData(uint offset,uint length,const void*src,bool disacardWholeBuffer);

	const LockedPixelBox& getLockedBox(){return m_lockedBox;}

	uint getWidth(){return m_width;}
	uint getHeight(){return m_height;}
	uint getDepth(){return m_depth;}

	ETextureType getTextureType(){return m_texType;}

	virtual void blitFromMemory(const LockedPixelBox&b)=0;

	virtual bool isRenderBuffer()=0;

};

MakeSharedPtrType(IHardwarePixelBuffer)

}
}


#endif //___IHardwarePixelBuffer___

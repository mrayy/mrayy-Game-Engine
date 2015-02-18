/********************************************************************
	created:	2009/02/08
	created:	8:2:2009   22:57
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayGLDevice\GLStreamBuffer.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayGLDevice
	file base:	GLStreamBuffer
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___GLStreamBuffer___
#define ___GLStreamBuffer___


#include <IHardwareStreamBuffer.h>
#include "compileConfig.h"

namespace mray{
namespace video{

class MRAY_GL_DLL GLStreamBuffer:public IHardwareStreamBuffer
{
protected:
	uint m_bufferID;

	virtual void*inner_lock(uint offset,uint length,ELockOptions options);
	virtual void inner_unlock();

	virtual void updateFromVirtual();

	virtual bool inner_resize(uint newSize);
public:
	GLStreamBuffer(EStreamDataType dataType,uint length,EUsageType usage,uint id,bool useVirtualBuffer);
	virtual~GLStreamBuffer();

	uint getBufferID()const{return m_bufferID;}

	uint readData(uint offset,uint length,void*dest);
	uint writeData(uint offset,uint length,const void*src,bool disacardWholeBuffer);

	virtual bool isInGPU(){return true;}
	virtual const core::string& getDeviceType();
};

}
}


#endif //___GLStreamBuffer___


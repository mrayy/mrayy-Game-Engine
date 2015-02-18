/********************************************************************
	created:	2009/02/08
	created:	8:2:2009   22:57
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayGLDevice\GLIndexBuffer.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayGLDevice
	file base:	GLIndexBuffer
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___GLIndexBuffer___
#define ___GLIndexBuffer___


#include <IHardwareIndexBuffer.h>
#include "compileConfig.h"

namespace mray{
namespace video{

class MRAY_GL_DLL GLIndexBuffer:public IHardwareIndexBuffer
{
protected:
	uint m_bufferID;

	virtual void*inner_lock(uint offset,uint length,ELockOptions options);
	virtual void inner_unlock();

	virtual bool inner_resize(uint newSize);
public:
	GLIndexBuffer(EIndexType type,uint numIndicies,EUsageType usage,uint id,bool useVirtualBuffer);
	virtual~GLIndexBuffer();

	uint getBufferID()const{return m_bufferID;}
	uint readData(uint offset,uint length,void*dest);
	uint writeData(uint offset,uint length,const void*src,bool disacardWholeBuffer);

	virtual bool isInGPU(){return true;}
	virtual const core::string& getDeviceType();
};

}
}


#endif //___GLIndexBuffer___


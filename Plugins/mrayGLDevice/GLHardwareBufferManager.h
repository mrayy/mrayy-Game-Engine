
/********************************************************************
	created:	2009/02/08
	created:	8:2:2009   23:50
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayGLDevice\GLHardwareBufferManager.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayGLDevice
	file base:	GLHardwareBufferManager
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___GLHardwareBufferManager___
#define ___GLHardwareBufferManager___

#include <IHardwareBufferManager.h>
#include "compileConfig.h"

namespace mray{
namespace video{

class MRAY_GL_DLL GLHardwareBufferManager:public IHardwareBufferManager
{
protected:
	uint m_lastStreamID;
	uint m_lastIndexID;

	uint getStreamID();
	uint getIndexID();
public:
	GLHardwareBufferManager();
	virtual~GLHardwareBufferManager();

	virtual IHardwareStreamBuffer*  createHardwareStreamBuffer(EStreamDataType dataType,uint length,
		IHardwareBuffer::EUsageType usage,bool useVirtualBuffer);

	virtual IHardwareIndexBuffer*  createHardwareIndexBuffer(IHardwareIndexBuffer::EIndexType type,uint numIndicies,IHardwareBuffer::EUsageType usage,bool useVirtualBuffer);

	virtual SoftwareStreamBuffer*  createSoftwareStreamBuffer(EStreamDataType dataType,uint length,
		IHardwareBuffer::EUsageType usage);

	virtual SoftwareIndexBuffer*  createSoftwareIndexBuffer(IHardwareIndexBuffer::EIndexType type,uint numIndicies,IHardwareBuffer::EUsageType usage);
	static uint mapGLUsage(IHardwareBuffer::EUsageType usage);
};

}

}


#endif //___GLHardwareBufferManager___

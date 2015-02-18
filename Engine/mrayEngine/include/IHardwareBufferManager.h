

/********************************************************************
	created:	2009/02/08
	created:	8:2:2009   18:52
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\IHardwareBufferManager.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	IHardwareBufferManager
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___IHardwareBufferManager___
#define ___IHardwareBufferManager___

#include <ISingleton.h>
#include "IHardwareBuffer.h"
#include "IHardwareIndexBuffer.h"

namespace mray{
namespace video{

class IHardwareStreamBuffer;
class IHardwareIndexBuffer;
class SoftwareStreamBuffer;
class SoftwareIndexBuffer;

enum EStreamDataType;

class MRAY_DLL IHardwareBufferManager:public ISingleton<IHardwareBufferManager>
{
public:


	IHardwareBufferManager(){};
	virtual~IHardwareBufferManager(){};

	virtual IHardwareStreamBuffer* createHardwareStreamBuffer(EStreamDataType dataType,uint length,
		IHardwareBuffer::EUsageType usage,bool useVirtualBuffer)=0;

	virtual IHardwareIndexBuffer* createHardwareIndexBuffer(IHardwareIndexBuffer::EIndexType type,uint numIndicies,IHardwareBuffer::EUsageType usage,bool useVirtualBuffer)=0;

	virtual SoftwareStreamBuffer* createSoftwareStreamBuffer(EStreamDataType dataType,uint length,
		IHardwareBuffer::EUsageType usage)=0;

	virtual SoftwareIndexBuffer* createSoftwareIndexBuffer(IHardwareIndexBuffer::EIndexType type,uint numIndicies,IHardwareBuffer::EUsageType usage)=0;
};

}
}


#endif //___IHardwareBufferManager___

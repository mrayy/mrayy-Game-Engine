
/********************************************************************
	created:	2009/02/08
	created:	8:2:2009   22:46
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\SoftwareBufferManager.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	SoftwareBufferManager
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___SoftwareBufferManager___
#define ___SoftwareBufferManager___


#include "IHardwareBufferManager.h"

namespace mray{
namespace video{

class MRAY_DLL SoftwareBufferManager:public IHardwareBufferManager
{
protected:
	uint m_lastStreamID;
	uint m_lastIndexID;

	uint getStreamID();
	uint getIndexID();
public:


	SoftwareBufferManager();
	virtual~SoftwareBufferManager();

	virtual IHardwareStreamBuffer* createHardwareStreamBuffer(EStreamDataType dataType,uint length,
		IHardwareBuffer::EUsageType usage,bool useVirtualBuffer);

	virtual IHardwareIndexBuffer* createHardwareIndexBuffer(IHardwareIndexBuffer::EIndexType type,uint numIndicies,IHardwareBuffer::EUsageType usage,bool useVirtualBuffer);


	virtual SoftwareStreamBuffer* createSoftwareStreamBuffer(EStreamDataType dataType,uint length,
		IHardwareBuffer::EUsageType usage);

	virtual SoftwareIndexBuffer* createSoftwareIndexBuffer(IHardwareIndexBuffer::EIndexType type,uint numIndicies,IHardwareBuffer::EUsageType usage);
};

}
}



#endif //___SoftwareBufferManager___


/********************************************************************
	created:	2009/02/08
	created:	8:2:2009   22:03
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\SoftwareStreamBuffer.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	SoftwareStreamBuffer
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___SoftwareStreamBuffer___
#define ___SoftwareStreamBuffer___

#include "IHardwareStreamBuffer.h"

namespace mray{
namespace video{

class MRAY_DLL SoftwareStreamBuffer:public IHardwareStreamBuffer
{
protected:
	static const core::string s_type;
	std::vector<uchar> m_data;

	virtual void*inner_lock(uint offset,uint length,ELockOptions options);
	virtual void inner_unlock();
	virtual bool inner_resize(uint newSize);
public:
	SoftwareStreamBuffer(EStreamDataType dataType,uint length,uint id,EUsageType usage);
	virtual~SoftwareStreamBuffer();

	void*lock(uint offset,uint length,ELockOptions options);
	void unlock();


	uint readData(uint offset,uint length,void*dest);
	uint writeData(uint offset,uint length,const void*src,bool disacardWholeBuffer);

	void* getData(uint offset);

	virtual const core::string& getDeviceType(){return s_type;}
	virtual bool isInGPU(){return false;}
};

MakeSharedPtrType(SoftwareStreamBuffer);

}
}

#endif //___SoftwareStreamBuffer___

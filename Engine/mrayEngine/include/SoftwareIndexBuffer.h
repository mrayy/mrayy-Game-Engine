
/********************************************************************
	created:	2009/02/08
	created:	8:2:2009   22:34
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\SoftwareIndexBuffer.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	SoftwareIndexBuffer
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___SoftwareIndexBuffer___
#define ___SoftwareIndexBuffer___

#include "IHardwareIndexBuffer.h"

namespace mray{
namespace video{

class MRAY_DLL SoftwareIndexBuffer:public IHardwareIndexBuffer
{
protected:
	static const core::string s_type;

	std::vector<uchar> m_data;

	virtual void*inner_lock(uint offset,uint length,ELockOptions options);
	virtual void inner_unlock();
	virtual bool inner_resize(uint newSize);
public:
	SoftwareIndexBuffer(EIndexType type,uint numIndicies,uint id,EUsageType usage);
	virtual~SoftwareIndexBuffer();

	void*lock(uint offset,uint length,ELockOptions options);
	void unlock();


	uint readData(uint offset,uint length,void*dest);
	uint writeData(uint offset,uint length,const void*src,bool disacardWholeBuffer);

	virtual bool isInGPU(){return false;}

	void* getData(uint offset);


	virtual const core::string& getDeviceType(){return s_type;}
};

MakeSharedPtrType(SoftwareIndexBuffer);

}
}


#endif //___SoftwareIndexBuffer___


/********************************************************************
	created:	2009/02/08
	created:	8:2:2009   22:06
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\IHardwareIndexBuffer.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	IHardwareIndexBuffer
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___IHardwareIndexBuffer___
#define ___IHardwareIndexBuffer___


#include "IHardwareBuffer.h"

namespace mray{
namespace video{



class MRAY_DLL IHardwareIndexBuffer:public IHardwareBuffer
{
public:
	
	enum EIndexType{
		EIT_16Bit,
		EIT_32Bit
	};

protected:
	
	EIndexType m_indexType;
	uint m_indexSize;
	uint m_numIndicies;


	virtual bool inner_resize(uint newSize);
public:
	IHardwareIndexBuffer(EIndexType type,uint count,EUsageType usage,uint id,bool useVirtualBuffer);
	virtual~IHardwareIndexBuffer();

	EIndexType getIndexType(){return m_indexType;}
	uint getIndexSize(){return m_indexSize;}
	uint getIndexCount(){return m_numIndicies;}
};
MakeSharedPtrType(IHardwareIndexBuffer);

class MRAY_DLL IndexData
{
public:
	IndexData();
	virtual ~IndexData();

	IHardwareIndexBufferPtr indexBuffer;
	uint firstIndex;
	uint indexCount;

	IndexData*duplicate(bool copyIB=true);
};
MakeSharedPtrType(IndexData);


}
}


#endif //___IHardwareIndexBuffer___


/********************************************************************
	created:	2009/02/08
	created:	8:2:2009   22:06
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\IHardwareStreamBuffer.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	IHardwareStreamBuffer
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___IHardwareStreamBuffer___
#define ___IHardwareStreamBuffer___


#include "IHardwareBuffer.h"
#include "videoCommon.h"

namespace mray{
namespace video{



class MRAY_DLL IHardwareStreamBuffer:public IHardwareBuffer
{
public:


protected:
	EStreamDataType m_elemType;
	uint m_length;

	virtual bool inner_resize(uint newSize);

public:
	IHardwareStreamBuffer(EStreamDataType dataType,uint length,
					EUsageType usage,uint id,bool useVirtualBuffer);
	virtual~IHardwareStreamBuffer();

	virtual void setElementType(EStreamDataType dataType);
	EStreamDataType getElementData();

	uint getElementSize();	//size in byte
	uint getComponentsCount();
	uint length();

	void SetLength(uint len);

	static uint getDataComponentsCount(EStreamDataType type);
	static uint getDataSize(EStreamDataType type);
};

MakeSharedPtrType(IHardwareStreamBuffer);

struct MeshStreamKey
{
	MeshStreamKey(){
	}
	MeshStreamKey(int i,EMeshStreamType t){
		index=i;
		type=t;
	}
	int index;
	EMeshStreamType type;

	bool operator < (const MeshStreamKey& o)const
	{
		if(type==o.type)return index<o.index;
		return type<o.type;
	}
	bool operator == (const MeshStreamKey& o)const
	{
		return (type==o.type)&&(index==o.index);
	}
};
typedef std::map<MeshStreamKey,GCPtr<IHardwareStreamBuffer>> StreamsMap;
typedef StreamsMap::iterator StreamsMapIT;

class MRAY_DLL StreamsData
{
public:
	StreamsData();
	virtual~StreamsData();

	StreamsMap streams;

	uint start;
	uint count;

	StreamsData*duplicate(bool copyStreams=true);
};

MakeSharedPtrType(StreamsData);

}
}


#endif //___IHardwareStreamBuffer___

#include "stdafx.h"

#include "IHardwareIndexBuffer.h"
#include "SoftwareIndexBuffer.h"

#include "IHardwareBufferManager.h"


namespace mray{
namespace video{

IHardwareIndexBuffer::IHardwareIndexBuffer(EIndexType type,uint numIndicies,EUsageType usage,uint id,bool useVirtualBuffer):
	IHardwareBuffer(usage,id,useVirtualBuffer)
{
	m_indexType=type;
	m_numIndicies=numIndicies;
	m_indexSize = type==EIT_16Bit ? sizeof(ushort) :sizeof(uint);

	m_sizeInBytes=numIndicies*m_indexSize;

	if(useVirtualBuffer){
		m_virtualBuffer=new SoftwareIndexBuffer(type,numIndicies,id,EUT_Dynamic);
	}
}
IHardwareIndexBuffer::~IHardwareIndexBuffer()
{
}
bool IHardwareIndexBuffer::inner_resize(uint newSize){
	if(IHardwareBuffer::inner_resize(newSize)==true){
		m_numIndicies=newSize/m_indexSize;
		return true;
	}
	return false;
}


//////////////////////////////////////////////////////////////////////////

IndexData::IndexData()
{
	indexBuffer=0;
	firstIndex=0;
	indexCount=0;
}
IndexData::~IndexData()
{
	indexBuffer=0;
}
IndexData*IndexData::duplicate(bool copyIB){
	IndexData*id=new IndexData();
	id->firstIndex=firstIndex;
	id->indexCount=indexCount;
	if(indexBuffer){
		if(copyIB){
			id->indexBuffer=IHardwareBufferManager::getInstance().createHardwareIndexBuffer(indexBuffer->getIndexType(),
				indexBuffer->getIndexCount(),indexBuffer->getUsageType(),indexBuffer->hasVirtualBuffer());
			id->indexBuffer->copyData(indexBuffer,0,0,indexBuffer->getSizeInBytes());
		}else{
			id->indexBuffer=indexBuffer;
		}
	}
	return id;
}


}
}


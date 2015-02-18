#include "stdafx.h"


#include "IHardwareStreamBuffer.h"
#include "IHardwareBufferManager.h"
#include "SoftwareStreamBuffer.h"


namespace mray{
namespace video{

IHardwareStreamBuffer::IHardwareStreamBuffer(EStreamDataType dataType,uint length,
											 EUsageType usage,uint id,bool useVirtualBuffer):
	IHardwareBuffer(usage,id,useVirtualBuffer)
{
	m_elemType=dataType;
	m_length=length;

	m_sizeInBytes=length*getElementSize();

	if(useVirtualBuffer){
		m_virtualBuffer=new SoftwareStreamBuffer(dataType,length,id,EUT_Dynamic);
	}
}
IHardwareStreamBuffer::~IHardwareStreamBuffer(){
}


void IHardwareStreamBuffer::setElementType(EStreamDataType dataType){
	m_elemType=dataType;
}
bool IHardwareStreamBuffer::inner_resize(uint length){
	if(IHardwareBuffer::inner_resize(length)==true){
		m_length=length/getElementSize();
		return true;
	}
	return false;
}

uint IHardwareStreamBuffer::getElementSize(){
	return getDataSize(m_elemType);
}
uint IHardwareStreamBuffer::getComponentsCount(){
	return getDataComponentsCount(m_elemType);
}
EStreamDataType IHardwareStreamBuffer::getElementData(){
	return m_elemType;
}
uint IHardwareStreamBuffer::length()
{
	return m_length;
}
void IHardwareStreamBuffer::SetLength(uint len)
{
	m_length=len;
	uint sizeInBytes=len*getElementSize();
	resize(sizeInBytes);
}

uint IHardwareStreamBuffer::getDataSize(EStreamDataType type){
	switch (type)
	{
	case ESDT_Point1f:
		return sizeof(float);
	case ESDT_Point2f:
		return sizeof(float)*2;
	case ESDT_Point3f:
		return sizeof(float)*3;
	case ESDT_Point4f:
		return sizeof(float)*4;
	case ESDT_Point1s:
		return sizeof(short);
	case ESDT_Point2s:
		return sizeof(short)*2;
	case ESDT_Point3s:
		return sizeof(short)*3;
	case ESDT_Point4s:
		return sizeof(short)*4;
	case ESDT_Color4b:
		return sizeof(uchar)*4;
	}
	return 0;
}

uint IHardwareStreamBuffer::getDataComponentsCount(EStreamDataType type){
	switch (type)
	{
	case ESDT_Point1f:
	case ESDT_Point1s:
		return 1;
	case ESDT_Point2f:
	case ESDT_Point2s:
		return 2;
	case ESDT_Point3f:
	case ESDT_Point3s:
		return 3;
	case ESDT_Point4f:
	case ESDT_Point4s:
		return 4;
	case ESDT_Color4b:
		return 4;
	}
	return 0;
}


//////////////////////////////////////////////////////////////////////////

StreamsData::StreamsData(){
	start=0;
	count=0;
}
StreamsData::~StreamsData(){
	streams.clear();
}
StreamsData*StreamsData::duplicate(bool copyStreams){
	StreamsData*sd=new StreamsData();
	sd->start=start;
	sd->count=count;
	StreamsMapIT it=streams.begin();
	for(;it!=streams.end();++it){
		GCPtr<IHardwareStreamBuffer> stream=it->second;
		if(copyStreams){
			GCPtr<IHardwareStreamBuffer>s=IHardwareBufferManager::getInstance().createHardwareStreamBuffer(
				stream->getElementData(),stream->length(),stream->getUsageType(),stream->hasVirtualBuffer());

			s->copyData(stream,0,0,stream->getSizeInBytes());
			sd->streams.insert(StreamsMap::value_type(it->first,s));
		}else{
			sd->streams.insert(StreamsMap::value_type(it->first,stream));
		}
	}
	return sd;
}


}
}

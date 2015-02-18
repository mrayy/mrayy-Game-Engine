#include "stdafx.h"


#include "GLHardwareBufferManager.h"
#include "GLStreamBuffer.h"
#include "GLIndexBuffer.h"
#include "SoftwareStreamBuffer.h"
#include "SoftwareIndexBuffer.h"
#include <windows.h>
#include <gl/glew.h>
#include <gl/gl.h>


namespace mray{
namespace video{


GLHardwareBufferManager::GLHardwareBufferManager(){
	m_lastStreamID=0;
	m_lastIndexID=0;
}

GLHardwareBufferManager::~GLHardwareBufferManager(){
}

uint GLHardwareBufferManager::getStreamID(){
	return m_lastStreamID++;
}
uint GLHardwareBufferManager::getIndexID(){
	return m_lastIndexID++;

}

IHardwareStreamBuffer*  GLHardwareBufferManager::createHardwareStreamBuffer(EStreamDataType dataType,uint length,
	IHardwareBuffer::EUsageType usage,bool useVirtualBuffer)
{
	return new GLStreamBuffer(dataType,length,usage,getStreamID(),useVirtualBuffer);
}


IHardwareIndexBuffer*  GLHardwareBufferManager::createHardwareIndexBuffer(IHardwareIndexBuffer::EIndexType type,uint numIndicies,IHardwareBuffer::EUsageType usage,bool useVirtualBuffer){
	return new GLIndexBuffer(type,numIndicies,usage,getIndexID(),useVirtualBuffer);
}

uint GLHardwareBufferManager::mapGLUsage(IHardwareBuffer::EUsageType usage){
	switch(usage){
		case IHardwareBuffer::EUT_Static:
		case IHardwareBuffer::EUT_StaticWriteOnly:
			return GL_STATIC_DRAW_ARB;
		case IHardwareBuffer::EUT_DynamicWriteOnlyDiscardable:
			return GL_STREAM_DRAW_ARB;
		default:
			return GL_DYNAMIC_DRAW_ARB;
	}
}
SoftwareStreamBuffer* GLHardwareBufferManager::createSoftwareStreamBuffer(EStreamDataType dataType,uint length,
																			  IHardwareBuffer::EUsageType usage)
{
	return new SoftwareStreamBuffer(dataType,length,getStreamID(),usage);
}

SoftwareIndexBuffer*  GLHardwareBufferManager::createSoftwareIndexBuffer(IHardwareIndexBuffer::EIndexType type,uint numIndicies,IHardwareBuffer::EUsageType usage)
{
	return new SoftwareIndexBuffer(type,numIndicies,getIndexID(),usage);

}

}
}



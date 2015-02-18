#include "stdafx.h"

#include "GLStreamBuffer.h"
#include <gl\ExtGL.h>
#include <macros.h>
#include "GLHardwareBufferManager.h"
#include "GLDev.h"

namespace mray{
	namespace video{
GLStreamBuffer::GLStreamBuffer(EStreamDataType dataType,uint length,
	EUsageType usage,uint id,bool useVirtualBuffer):
	IHardwareStreamBuffer(dataType,length,usage,id,useVirtualBuffer)
{
	glGenBuffersARB(1,&m_bufferID);
	FATAL_ERROR(!m_bufferID,mT("couldn't generate GL Hardware Stream Buffer!!"));
	glBindBufferARB(GL_ARRAY_BUFFER_ARB,m_bufferID);
	glBufferData(GL_ARRAY_BUFFER_ARB,m_sizeInBytes,0,GLHardwareBufferManager::mapGLUsage(usage));
	glBindBufferARB(GL_ARRAY_BUFFER_ARB,0);
}
GLStreamBuffer::~GLStreamBuffer(){
	glDeleteBuffersARB(1,&m_bufferID);
}

void*GLStreamBuffer::inner_lock(uint offset,uint length,IHardwareBuffer::ELockOptions options){
	glBindBufferARB(GL_ARRAY_BUFFER_ARB,m_bufferID);

	if(options==ELO_Discard){
		glBufferDataARB(GL_ARRAY_BUFFER_ARB,m_sizeInBytes,0,GLHardwareBufferManager::mapGLUsage(m_usage));
	}
	GLenum access=0;
	if(m_usage & EUT_WriteOnly)
		access=GL_WRITE_ONLY_ARB;
	else if(options == ELO_ReadOnly)
		access=GL_READ_ONLY_ARB;
	else access=GL_READ_WRITE_ARB;

	void*buff=glMapBufferARB(GL_ARRAY_BUFFER_ARB,access);
	FATAL_ERROR(!buff,mT("GLStreamBuffer::inner_lock() : out of memory!!"));

	glBindBufferARB(GL_ARRAY_BUFFER_ARB,0);
	return static_cast<void*>(static_cast<char*>(buff)+offset);
}
void GLStreamBuffer::inner_unlock(){
	glBindBufferARB(GL_ARRAY_BUFFER_ARB,m_bufferID);
	FATAL_ERROR(!glUnmapBufferARB(GL_ARRAY_BUFFER_ARB),mT("GLStreamBuffer::inner_unlock() : Buffer data corrupted!!"));
	glBindBufferARB(GL_ARRAY_BUFFER_ARB,0);
}


bool GLStreamBuffer::inner_resize(uint newSize){
	if(newSize==m_sizeInBytes)
		return true;
	glBindBufferARB(GL_ARRAY_BUFFER_ARB,m_bufferID);
	glBufferData(GL_ARRAY_BUFFER_ARB,newSize,0,GLHardwareBufferManager::mapGLUsage(m_usage));
	glBindBufferARB(GL_ARRAY_BUFFER_ARB,0);
	return true;
}
uint GLStreamBuffer::readData(uint offset,uint length,void*dest){
	if(length==0)
		length=m_sizeInBytes-offset;
	else
		if(offset+length>m_sizeInBytes)
			length=m_sizeInBytes-offset;

	glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_bufferID);

	glGetBufferSubDataARB(GL_ARRAY_BUFFER_ARB, offset, length, dest);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB,0);
	return length;
}
uint GLStreamBuffer::writeData(uint offset,uint length,const void*src,bool discardWholeBuffer)
{
	if(length==0)
		length=m_sizeInBytes-offset;
	else
		if(offset+length>m_sizeInBytes)
			length=m_sizeInBytes-offset;
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_bufferID);
	if(m_useVirtualBuffer){
		void* dest = m_virtualBuffer->lock(offset, length, ELO_Normal);
		mraySystem::memCopy(dest, src, length);
		m_virtualBuffer->unlock();
	}

	if (offset == 0 && length == m_sizeInBytes)
	{
		glBufferDataARB(GL_ARRAY_BUFFER_ARB, m_sizeInBytes, src, GLHardwareBufferManager::mapGLUsage(m_usage));
	}
	else
	{
 		if(discardWholeBuffer)
 		{
 			glBufferDataARB(GL_ARRAY_BUFFER_ARB, m_sizeInBytes, NULL,GLHardwareBufferManager::mapGLUsage(m_usage));
 		}

		glBufferSubDataARB(GL_ARRAY_BUFFER_ARB, offset, length, src); 
	}
	glBindBufferARB(GL_ARRAY_BUFFER_ARB,0);
	return length;

}


void GLStreamBuffer::updateFromVirtual(){
	if (m_useVirtualBuffer && m_VirtualBufferChanged)
	{
		const void *src = m_virtualBuffer->lock(m_startLock,m_sizeLocked, ELO_ReadOnly);

		glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, m_bufferID);

		// Update whole buffer if possible, otherwise normal
		if (m_startLock == 0 && m_sizeLocked == m_sizeInBytes)
		{
			glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB, m_sizeInBytes, src,GLHardwareBufferManager::mapGLUsage(m_usage));
		}
		else
		{
			glBufferSubDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB, m_startLock, m_sizeLocked, src);
		}

		m_virtualBuffer->unlock();
		m_VirtualBufferChanged = false;
		glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB,0);
	}
}
const core::string& GLStreamBuffer::getDeviceType()
{
	return GLDev::m_deviceType;
}

}
}



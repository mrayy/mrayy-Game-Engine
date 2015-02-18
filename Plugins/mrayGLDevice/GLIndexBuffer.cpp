#include "stdafx.h"

#include "GLIndexBuffer.h"
#include <gl\ExtGL.h>
#include <macros.h>
#include "GLHardwareBufferManager.h"
#include "GLDev.h"

namespace mray{
	namespace video{

GLIndexBuffer::GLIndexBuffer(EIndexType type,uint numIndicies,EUsageType usage,uint id,bool useVirtualBuffer):
		IHardwareIndexBuffer(type,numIndicies,usage,id,useVirtualBuffer)
{
	glGenBuffersARB(1,&m_bufferID);
	FATAL_ERROR(!m_bufferID,mT("couldn't generate GL Hardware Index Buffer!!"));
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB,m_bufferID);
	glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB,m_sizeInBytes,0,GLHardwareBufferManager::mapGLUsage(usage));
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB,0);
}
GLIndexBuffer::~GLIndexBuffer(){
	glDeleteBuffersARB(1,&m_bufferID);
}
bool GLIndexBuffer::inner_resize(uint newSize){
	if(newSize==m_sizeInBytes)
		return true;
	m_numIndicies=newSize/getIndexSize();
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB,m_bufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER_ARB,newSize,0,GLHardwareBufferManager::mapGLUsage(m_usage));
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB,0);
	return true;
}

void*GLIndexBuffer::inner_lock(uint offset,uint length,IHardwareBuffer::ELockOptions options){
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB,m_bufferID);

	if(options==ELO_Discard){
		glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB,m_sizeInBytes,0,GLHardwareBufferManager::mapGLUsage(m_usage));
	}
	GLenum access=0;
	if(m_usage & EUT_WriteOnly)
		access=GL_WRITE_ONLY_ARB;
	else if(options == ELO_ReadOnly)
		access=GL_READ_ONLY_ARB;
	else access=GL_READ_WRITE_ARB;

	void*buff=glMapBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB,access);
	FATAL_ERROR(!buff,mT("GLIndexBuffer::inner_lock() : out of memory!!"));
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB,0);
	
	return static_cast<void*>(static_cast<char*>(buff)+offset);
}
void GLIndexBuffer::inner_unlock(){
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB,m_bufferID);
	FATAL_ERROR(!glUnmapBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB),mT("GLIndexBuffer::inner_unlock() : Buffer data corrupted!!"));
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB,0);
}

const core::string& GLIndexBuffer::getDeviceType()
{
	return GLDev::m_deviceType;
}

uint GLIndexBuffer::readData(uint offset,uint length,void*dest){
	if(length==0)
		length=m_sizeInBytes-offset;
	else
		if(offset+length>m_sizeInBytes)
			length=m_sizeInBytes-offset;

	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, m_bufferID);

	glGetBufferSubDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB, offset, length, dest);
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB,0);
	return length;
}
uint GLIndexBuffer::writeData(uint offset,uint length,const void*src,bool discardWholeBuffer){
	if(length==0)
		length=m_sizeInBytes-offset;
	else
		if(offset+length>m_sizeInBytes)
			length=m_sizeInBytes-offset;
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, m_bufferID);
	if(m_useVirtualBuffer){
		void* dest = m_virtualBuffer->lock(offset, length, ELO_Discard);
		mraySystem::memCopy(dest, src, length);
		m_virtualBuffer->unlock();
	}

	if (offset == 0 && length == m_sizeInBytes)
	{
		glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB, m_sizeInBytes, src, GLHardwareBufferManager::mapGLUsage(m_usage));
	}
	else
	{
 		if(discardWholeBuffer)
 		{
 			glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB, m_sizeInBytes, NULL,GLHardwareBufferManager::mapGLUsage(m_usage));
 		}

		glBufferSubDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB, offset, length, src); 
	}
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB,0);
	return length;

}

}
}



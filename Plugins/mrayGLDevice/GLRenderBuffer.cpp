#include "stdafx.h"

#include "GLRenderBuffer.h"
#include "GLTextureUtil.h"

#define USE_RENDER_BUFFER

namespace mray{
namespace video{


GLRenderBuffer::GLRenderBuffer(EPixelFormat format,uint width,uint height,uint numSamples)
:GLHardwarePixelBuffer(width,height,1,ETT_2DTex,format,EUT_WriteOnly,0),m_numSamples(numSamples)
{
	uint glFormat=GLTextureUtil::getClosestFormat(format);
	m_internalFormat=GLTextureUtil::getGLInternalFormat(format);

	GLenum type=GLTextureUtil::getGLDataType(format);
#ifndef USE_RENDER_BUFFER

	glGenTextures(1,&m_renderBufferID);
	glBindTexture(GL_TEXTURE_2D, m_renderBufferID);

	glTexImage2D(GL_TEXTURE_2D,0,m_internalFormat,width, height,0,glFormat,type,0);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

#else



	glGenRenderbuffersEXT(1,&m_renderBufferID);
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, m_renderBufferID);

	if (numSamples > 0)
	{
		glRenderbufferStorageMultisampleEXT(GL_RENDERBUFFER_EXT, 
			numSamples, glFormat, width, height);
	}
	else
	{
		glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, glFormat,
			width, height);
	}
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);

#endif
}
GLRenderBuffer::~GLRenderBuffer(){
#ifndef USE_RENDER_BUFFER
	glDeleteTextures(1, &m_renderBufferID);
#else
	glDeleteRenderbuffersEXT(1, &m_renderBufferID);
#endif
}
void GLRenderBuffer::Resize(uint w,uint h,uint d)
{
	GLHardwarePixelBuffer::Resize(w,h,1);

	uint glFormat=GLTextureUtil::getClosestFormat(m_format);
	GLenum type=GLTextureUtil::getGLDataType(m_format);
#ifndef USE_RENDER_BUFFER
	glBindTexture(GL_TEXTURE_2D, m_renderBufferID);

	glTexImage2D(GL_TEXTURE_2D,0,m_internalFormat,w, h,0,glFormat,type,0);

	glBindTexture(GL_TEXTURE_2D, 0);
#else
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, m_renderBufferID);

	if (m_numSamples > 0)
	{
		glRenderbufferStorageMultisampleEXT(GL_RENDERBUFFER_EXT, 
			m_numSamples, glFormat, w, h);
	}
	else
	{
		glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, glFormat,
			w, h);
	}
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);
#endif
}
void GLRenderBuffer::bindToFrameBuffer(GLenum attachment,uint zoffset){
	FATAL_ERROR(zoffset >= m_depth,mT("GLRenderBuffer::bindToFrameBuffer(): zoffset >= m_depth "));
#ifndef USE_RENDER_BUFFER
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, attachment,
		GL_TEXTURE_2D, m_renderBufferID,0);
#else
	glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, attachment,
		GL_RENDERBUFFER_EXT, m_renderBufferID);
#endif
}


}
}

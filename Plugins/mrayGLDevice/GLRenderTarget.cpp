
#include "GLRenderTarget.h"
#include <gl/ExtGL.h>
#include "GLDev.h"
#include <LogManager.h>

namespace mray{
namespace video{

bool GLRenderTarget::checkFrameBufferStatus(){

#ifdef GL_EXT_framebuffer_object
	GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
	switch (status)
	{
		//Our FBO is perfect, return true
	case GL_FRAMEBUFFER_COMPLETE_EXT:
		break;

	case GL_INVALID_FRAMEBUFFER_OPERATION_EXT:
		gLogManager.log(mT("invalid framebuffer operation"),ELL_WARNING);
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT:
		gLogManager.log(mT("FBO has invalid read buffer"),ELL_WARNING);
		break;

	case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT:
		gLogManager.log(mT("FBO has invalid draw buffer"),ELL_WARNING);
		break;

	case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT:
		gLogManager.log(mT("FBO has one or several incomplete image attachments"), ELL_WARNING);
		break;

	case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
		gLogManager.log(mT("FBO has one or several image attachments with different internal formats"),ELL_WARNING);
		break;

	case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
		gLogManager.log(mT("FBO has one or several image attachments with different dimensions"), ELL_WARNING);
		break;

		// not part of fbo_object anymore, but won't harm as it is just a return value
#ifdef GL_FRAMEBUFFER_INCOMPLETE_DUPLICATE_ATTACHMENT_EXT
	case GL_FRAMEBUFFER_INCOMPLETE_DUPLICATE_ATTACHMENT_EXT:
		gLogManager.log(mT("FBO has a duplicate image attachment"), ELL_WARNING);
		break;
#endif

	case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT:
		gLogManager.log(mT("FBO missing an attachment"), ELL_WARNING);
		break;

	case GL_FRAMEBUFFER_UNSUPPORTED_EXT:
		gLogManager.log(mT("FBO format unsupported"), ELL_WARNING);
		break;

	default:
		break;
	}
	//gLogManager.log(mT("Frame Buffer Output"), ELL_WARNING);
#endif
	return false;
}


 
GLRenderTarget::GLRenderTarget(const core::string&name,GLDev *device,math::vector2di size,int numSamples,
							   EPixelFormat format,bool depthBuffer){
	m_device=device;
	m_size=size;
	m_name=name;
	m_FrameBuff{}
	m_multiSampleFrameBuff{}

	GCPtr<RenderTextureBuffer> color;

	m_ColorTex.resize(8);

	core::string tmpStr=m_name+L"_ColorTex0";
	color=new RenderTextureBuffer(m_device,format,m_size.x,m_size.y,0);
	color->setResourceName(tmpStr.c_str());
	m_ColorTex[0]=color;

	tmpStr=m_name+L"_ColorTex1";
	color=new RenderTextureBuffer(m_device,format,m_size.x,m_size.y,1);
	color->setResourceName(tmpStr.c_str());
	m_ColorTex[1]=color;

	m_numSamples=numSamples;
	if(!device)
		return;


/*
	if (GLEW_EXT_framebuffer_blit && GLEW_EXT_framebuffer_multisample)
	{
		// check samples supported
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_FrameBuff);
		int maxSamples;
		glGetIntegerv(GL_MAX_SAMPLES_EXT, &maxSamples);
		numSamples = math::Min(numSamples, maxSamples);
		if (numSamples)
		{
			glGenFramebuffersEXT(1,&m_multiSampleFrameBuff);
		}
		
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	}
	else*/
	{
		numSamples = 0;
	}

	recreate(size,format,true);
//	recreate(size,format,depthBuffer);
}
GLRenderTarget::~GLRenderTarget(){
	unbind();
	if(m_multiSampleFrameBuff)
		glDeleteFramebuffersEXT( 1, &m_multiSampleFrameBuff );
}

void GLRenderTarget::recreate(math::vector2di newSize,EPixelFormat format,bool depthBuffer){
 
	m_size=newSize;

	core::string tmpStr;
	bind();
	for (int i{}i<m_ColorTex.size();++i)
	{
		if(m_ColorTex[i]){
			//glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT+i,GL_TEXTURE_2D, m_ColorTex[i]->getTextureID(),0);
			m_ColorTex[i]->recreate(format,newSize.x,newSize.y,m_numSamples);
			m_ColorTex[i]->bind(GL_COLOR_ATTACHMENT0_EXT+i);
		}
		else{
			//glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT,GL_RENDERBUFFER_EXT, 0);
			glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT+i,GL_RENDERBUFFER_EXT, 0,0);
		}
	}
	
	if(depthBuffer){
		if(!m_DepthTex){
			m_DepthTex=new RenderTextureBuffer(m_device,EPixel_DEPTH,m_size.x,m_size.y,m_numSamples);
			tmpStr=m_name+L"_DepthTex";
			m_DepthTex->setResourceName(tmpStr.c_str());
		}else
			m_DepthTex->recreate(EPixel_DEPTH,m_size.x,m_size.y,m_numSamples);
		m_DepthTex->bind(GL_DEPTH_ATTACHMENT_EXT);
	}
	else
		glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT,GL_RENDERBUFFER_EXT, 0);
		//glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT,GL_TEXTURE_2D, 0,0);
	//glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_STENCIL_ATTACHMENT_EXT,GL_RENDERBUFFER_EXT, 0);
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_STENCIL_ATTACHMENT_EXT,GL_RENDERBUFFER_EXT, 0,0);

	checkFrameBufferStatus();

	unbind();
}

math::vector2di GLRenderTarget::getSize(){
	return m_size;
}

video::mrayDev*GLRenderTarget::getDevice(){
	return m_device;
}
void GLRenderTarget::setDevice(video::mrayDev* dev){
	if(dev->getDeviceType()!=EDT_OPENGL){
		return;
	}
	m_device=dynamic_cast<GLDev*>(dev);
}

void GLRenderTarget::clear(const video::SColor&c,bool clearBackbuffer,bool clearDepthBuffer){

	GLbitfield mask = 0;
	if (clearBackbuffer)
	{
		//glClearColor(c.R , c.G ,c.B , c.A);

		m_device->setClearColor(c);

		mask |= GL_COLOR_BUFFER_BIT;
	}
	if (clearDepthBuffer)
	{
		glDepthMask(GL_TRUE);
		mask |= GL_DEPTH_BUFFER_BIT;
	}

	glClear(mask);
}

void GLRenderTarget::bind(){
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_FrameBuff);
}
void GLRenderTarget::unbind(){
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}

GCPtr<ITexture> GLRenderTarget::getColorTexture(int i){
	if(i<m_ColorTex.size())
		return m_ColorTex[i];
	return 0;
}
GCPtr<ITexture> GLRenderTarget::getDepthTexture(){
	return m_DepthTex;
}	


}
}
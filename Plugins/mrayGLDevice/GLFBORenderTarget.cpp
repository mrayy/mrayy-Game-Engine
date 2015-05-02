#include "stdafx.h"


#include <gl/ExtGL.h>
#include <LogManager.h>

#include "GLFBORenderTarget.h"
#include "GLDev.h"
#include "GLRenderBuffer.h"
#include "GLTextureUtil.h"
#include "GLTextureBuffer.h"
#include "Engine.h"

namespace mray{
namespace video{

	static const int G_maxColorAttachements=16;

	static GLenum G_ColorTargetAttachments[G_maxColorAttachements]={
		GL_COLOR_ATTACHMENT0_EXT,GL_COLOR_ATTACHMENT1_EXT,GL_COLOR_ATTACHMENT2_EXT,GL_COLOR_ATTACHMENT3_EXT,
		GL_COLOR_ATTACHMENT4_EXT,GL_COLOR_ATTACHMENT5_EXT,GL_COLOR_ATTACHMENT6_EXT,GL_COLOR_ATTACHMENT7_EXT,
		GL_COLOR_ATTACHMENT8_EXT,GL_COLOR_ATTACHMENT9_EXT,GL_COLOR_ATTACHMENT10_EXT,GL_COLOR_ATTACHMENT11_EXT,
		GL_COLOR_ATTACHMENT12_EXT,GL_COLOR_ATTACHMENT13_EXT,GL_COLOR_ATTACHMENT14_EXT,GL_COLOR_ATTACHMENT15_EXT,

	};


	class GLFBORenderTargetListener:public ITextureListener
	{
	public:
		GLFBORenderTarget*m_owner;
	public:
		GLFBORenderTargetListener(GLFBORenderTarget*o):m_owner(o)
		{
		}

		void OnTextureChanged(ITexture* tex)
		{
			if(m_owner)
				m_owner->OnTextureChanged(tex);
		}
	};

bool GLFBORenderTarget::checkFrameBufferStatus(){

#ifdef GL_EXT_framebuffer_object
	GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
	switch (status)
	{
		//Our FBO is perfect, return true
	case GL_FRAMEBUFFER_COMPLETE_EXT:
		return true;
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



GLFBORenderTarget::GLFBORenderTarget(const core::string&name)
{
	m_name=name;
	m_FrameBuffer=0;
	m_depthBuffer=0;
	m_stencilBuffer=0;
	m_sharedStencilBuffer=0;
	m_sharedDepthBuffer=0;
	m_usedTarget=0;
	m_listener=new GLFBORenderTargetListener(this);
}
GLFBORenderTarget::~GLFBORenderTarget(){
	unbind();

	m_depthBuffer=0;
	m_stencilBuffer=0;

	if(m_FrameBuffer)
		glDeleteFramebuffersEXT( 1, &m_FrameBuffer );

	m_FrameBuffer=0;
	m_sharedStencilBuffer=0;
	m_sharedDepthBuffer=0;

	((GLFBORenderTargetListener*)m_listener.pointer())->m_owner=0;
	m_listener=0;

}
bool GLFBORenderTarget::initialize(const ITexturePtr&colorBuffer,
		   const IHardwarePixelBufferPtr& depthBuffer,const IHardwarePixelBufferPtr& stencilBuffer,bool depthOnly)
{
	m_targets.resize(G_maxColorAttachements);

	// Create the frame buffer
	glGenFramebuffersEXT(1,&m_FrameBuffer);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_FrameBuffer);

	if(!colorBuffer)
	{
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
		gLogManager.log(mT("GLFBORenderTarget::GLFBORenderTarget() - Attach to one texture at least!"),ELL_WARNING);
		return false;
	}
	m_colorBuffer=colorBuffer;

	ETextureType texType=colorBuffer->getType();
	if(texType!= ETT_2DTex && texType!= ETT_CubeTex)
	{
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
		gLogManager.log(mT("Only 2D and Cube textures are supported"),ELL_WARNING);
		return false;
	}

	m_usedTarget=1;

	math::vector3di sz=colorBuffer->getSize();

	m_size.x=sz.x;
	m_size.y=sz.y;
	GCPtr<GLHardwarePixelBuffer> tmpBuffer;


	m_sharedDepthBuffer=0;
	m_sharedStencilBuffer=0;
	if(depthBuffer)
	{
		m_sharedDepthBuffer=true;
		m_depthBuffer=depthBuffer;
	}else{
		m_depthBuffer=new GLRenderBuffer(EPixel_Depth,m_size.x,m_size.y,0);
	}


	tmpBuffer=m_depthBuffer;
	tmpBuffer->bindToFrameBuffer(GL_DEPTH_ATTACHMENT_EXT,0);

//	glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_STENCIL_INDEX, GL_RENDERBUFFER_EXT, 0);

/*
	if(stencilBuffer)
	{
		m_sharedStencilBuffer=true;
		m_stencilBuffer=stencilBuffer;
	}else{
		m_stencilBuffer=new GLRenderBuffer(EPixel_Stecil,m_size.x,m_size.y,0);
	}
	tmpBuffer=m_stencilBuffer;
	tmpBuffer->bindToFrameBuffer(GL_STENCIL_ATTACHMENT_EXT,0);
	*/


	GCPtr<GLTexture> glTex=colorBuffer;

	// 			0);
	if(depthOnly)
	{
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE); 
	}else{
		//  			glReadBuffer(GL_COLOR_ATTACHMENT0_EXT);
		//  			glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);
	}
	/*
	if(texType== ETT_2DTex){
// 		GLenum target=glTex->getTarget();
// 		uint id=glTex->getTextureID();
// 		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT,
// 			//depthOnly ? GL_DEPTH_ATTACHMENT_EXT : 
// 			GL_COLOR_ATTACHMENT0_EXT,
// 			target, 
// 			id,
// 			0);
		if(depthOnly)
		{
			glDrawBuffer(GL_NONE);
			glReadBuffer(GL_NONE); 
		}else{
//  			glReadBuffer(GL_COLOR_ATTACHMENT0_EXT);
//  			glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);
		}
	}
	else // Cube Map
	{
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT,
			GL_COLOR_ATTACHMENT0_EXT,
			GL_TEXTURE_CUBE_MAP_POSITIVE_X, 
			glTex->getTextureID(),
			0);
	}*/

	attachRenderTarget(colorBuffer,0);

	// ... then inform OpenGL that we wish to render to these two targets
	//glDrawBuffers(m_usedTarget,G_ColorTargetAttachments);

	gLogManager.log(core::string(mT("FBO Render Target Created, dim:"))+core::StringConverter::toString(m_size),ELL_INFO,EVL_Heavy);

	bool res=checkFrameBufferStatus();
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

	return res;
}

void GLFBORenderTarget::OnTextureChanged(ITexture* tex)
{
	//find the texture
	for(int i=0;i<m_targets.size();++i)
	{
		if(m_targets[i].pointer()==tex)
		{
			refreshAttachment(i);
			break;
		}
	}
	if(m_colorBuffer.pointer()==tex)
	{
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_FrameBuffer);
		GLenum target=m_colorBuffer->getTarget();
		uint id=m_colorBuffer->getTextureID();
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT,
			//depthOnly ? GL_DEPTH_ATTACHMENT_EXT : 
			GL_COLOR_ATTACHMENT0_EXT,
			target, 
			id,
			0);

		math::vector3di sz=m_colorBuffer->getSize();

		m_size.x=sz.x;
		m_size.y=sz.y;


		GCPtr<GLHardwarePixelBuffer> tmpBuffer;
		tmpBuffer=m_depthBuffer;
		tmpBuffer->Resize(m_size.x,m_size.y,1);

		tmpBuffer->bindToFrameBuffer(GL_DEPTH_ATTACHMENT_EXT,0);

		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	}
}
void GLFBORenderTarget::refreshAttachment(uint index)
{
	if(index>=m_targets.size())
		return;
	if(m_targets[index].pointer()==0)
		return;

	bind();

	if(m_usedTarget<=index)
		m_usedTarget=index+1;

	GLTexture* glTex=dynamic_cast<GLTexture*>(m_targets[index].pointer());

	int target = glTex->getTarget();
	if(m_FrameBuffer)
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT + index,
		target, glTex->getTextureID(), 0);
	else
		gLogManager.log(mT("GLFBORenderTarget::attachRenderTarget() - Invalid FrameBuffer Object ID"),ELL_WARNING);
	//if(target!=0)
	m_size.set(glTex->getSize().x,glTex->getSize().y);

	if (!m_sharedDepthBuffer && !m_depthBuffer.isNull())
	{
		((GLRenderBuffer*)m_depthBuffer.pointer())->Resize(m_size.x, m_size.y);
		((GLRenderBuffer*)m_depthBuffer.pointer())->bindToFrameBuffer(GL_DEPTH_ATTACHMENT_EXT, 0);
	}

	glDrawBuffers(m_usedTarget,G_ColorTargetAttachments);
	bool res=checkFrameBufferStatus();
	unbind();
}
void GLFBORenderTarget::attachRenderTarget(const ITexturePtr&tex,uint index){

	if(!tex){
		deattachRenderTarget(tex,index);
		return;
	}
	index     = math::clamp<uint>(index, 0, 15);
	if(m_targets[index]==tex)
		return;
	tex->setMipmapsFilter(false);
	tex->setRenderTarget(true);
	m_targets[index]=tex;
	refreshAttachment(index);


	tex->AddListener(m_listener);
}
void GLFBORenderTarget::deattachRenderTarget(const ITexturePtr&tex,uint index)
{

	if(!tex)
		return;
	bind();
	index     = math::clamp<uint>(index, 0, 15);
	m_targets[index]=0;
	for(int i=m_targets.size()-1;i>=0;--i){
		if(m_targets[i].pointer()!=0){
			m_usedTarget=i+1;
			break;
		}
	}

	glDrawBuffers(m_usedTarget,G_ColorTargetAttachments);
	if(!tex){
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT + index,
			0, 0, 0);
		unbind();
		return;
	}
	GCPtr<GLTexture> glTex=tex;

	int target = glTex->getTarget();
	if(m_FrameBuffer)
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT + index,
		target, 0, 0);
	else
		gLogManager.log(mT("GLFBORenderTarget::attachRenderTarget() - Invalid FrameBuffer Object ID"),ELL_WARNING);

	unbind();


	tex->RemoveListener(m_listener);
}

void GLFBORenderTarget::clear(const video::SColor&c,bool clearBackbuffer,bool clearDepthBuffer){

	GLbitfield mask = 0;
	if (clearBackbuffer)
	{
// 		glClearColor(c.R , c.G ,
// 			c.B, c.A);
		Engine::getInstance().getDevice()->setClearColor(c);

		mask |= GL_COLOR_BUFFER_BIT;
	}
	if (clearDepthBuffer)
	{
		//glDepthMask(GL_TRUE);
		mask |= GL_DEPTH_BUFFER_BIT;
	}

	glClear(mask);
}

math::vector2di GLFBORenderTarget::GetSize(){
	return m_size;
}

void GLFBORenderTarget::bind(){
	_OnBind();
	if(m_FrameBuffer)
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_FrameBuffer);
	else{
		gLogManager.log(mT("GLFBORenderTarget::bind() - Invalid FrameBuffer Object ID"),ELL_WARNING);
	}

}
void GLFBORenderTarget::unbind(){
	_OnUnBind();
	checkFrameBufferStatus();
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}

const ITexturePtr& GLFBORenderTarget::GetColorTexture(int i){
	if(i>=m_targets.size())
		return ITexturePtr::Null;
	return m_targets[i];
}
const IHardwarePixelBufferPtr& GLFBORenderTarget::GetDepthBuffer(){
	return m_depthBuffer;
}	

const IHardwarePixelBufferPtr& GLFBORenderTarget::GetStencilBuffer(){
	return m_stencilBuffer;
}

void GLFBORenderTarget::Resize(int x,int y)
{
	int c=m_targets.size();
	math::vector3di sz(x,y,1);
	for(int i=0;i<c;++i)
	{
		video::ITexture* tex=GetColorTexture(i);
		if(tex && tex->getSize()!=sz)
			tex->createTexture(sz,tex->getImageFormat());
	}
}
}
}
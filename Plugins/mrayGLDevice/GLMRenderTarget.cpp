#include "stdafx.h"

#include "GLMRenderTarget.h"
#include "GLDev.h"
#include "ILogManager.h"


namespace mray{
namespace video{


GLMRenderTarget::GLMRenderTarget(video::GLDev* dev,bool floatingPoint,int auxBuffers,int numComponents,int bitPerChannel){
	m_device=dev;
	m_pbuffer=new GLPBuffer(dev,floatingPoint,true,auxBuffers,GLPBuffer::EPBT_2D,numComponents,bitPerChannel);
	m_textures.resize(auxBuffers);
	
}
GLMRenderTarget::~GLMRenderTarget(){
	m_textures.clear();
	m_pbuffer=0;
}


void GLMRenderTarget::initialize(const math::vector2di&s,bool shareContexts,bool shareObjects){
	m_pbuffer->initialize(s.x,s.y,shareContexts,shareObjects);
	for(int i=0;i<m_textures.size();i++){
		m_textures[i]=new GLPBufferTexture(m_device,m_pbuffer,i);
	}
}

video::mrayDev*GLMRenderTarget::getDevice(){
	return m_device;
}
void GLMRenderTarget::setDevice(video::mrayDev* dev){
	if(dev->getDeviceType()!=GLDev::m_deviceType){
		return;
	}
	m_device=dynamic_cast<GLDev*>(dev);
}
math::vector2di GLMRenderTarget::GetSize(){
	return m_pbuffer->getSize();
}

void GLMRenderTarget::clear(const video::SColor&c,bool clearBackbuffer,bool clearDepthBuffer){
	m_pbuffer->clear(c,clearBackbuffer,clearDepthBuffer);
	m_pbuffer->setDrawBuffers();
}


void GLMRenderTarget::attachRenderTarget(const ITexturePtr&tex,uint index){
	if(index>=m_textures.size()){
		gLogManager.log(mT("GLMRenderTarget::attachRenderTarget() - index is out of range!"),ELL_ERROR);
		return;
	}
	GCPtr<GLTexture> glTex=tex;
	if(!glTex){
		m_textures[index]=0;
	}else{
		if(!glTex->isPBuffer()){
			gLogManager.log(mT("GLMRenderTarget::attachRenderTarget() - cann't attach non PBuffer Texture!"),ELL_ERROR);
			return;
		}
		m_textures[index]=glTex;
		((GLPBufferTexture*)m_textures[index].pointer())->setPBuffer(m_pbuffer,index);
	}
}
void GLMRenderTarget::deattachRenderTarget(const ITexturePtr&tex,uint index){
	if(index>=m_textures.size()){
		gLogManager.log(mT("GLMRenderTarget::deattachRenderTarget() - index is out of range!"),ELL_ERROR);
		return;
	}
	m_textures[index]=0;
}

void GLMRenderTarget::bind(){
	_OnBind();
	GLPBuffer*b= m_device->getCurrentPBuffer();
	m_pbuffer->activate(b);
	m_device->setCurrentPBuffer(m_pbuffer);
}
void GLMRenderTarget::unbind(){
	_OnUnBind();
	m_pbuffer->deactivate();
	GLPBuffer*b= m_device->getCurrentPBuffer();
	if(m_pbuffer.pointer()==b)
		m_device->setCurrentPBuffer(0);
}

const ITexturePtr& GLMRenderTarget::GetColorTexture(int idx){
	if(idx>=m_textures.size()){
		gLogManager.log(mT("GLMRenderTarget::getColorTexture() - index is out of range!"),ELL_ERROR);
		return ITexturePtr::Null;
	}
	return m_textures[idx];
}

const IHardwarePixelBufferPtr& GLMRenderTarget::GetDepthBuffer(){
	return IHardwarePixelBufferPtr::Null;
}
const IHardwarePixelBufferPtr& GLMRenderTarget::GetStencilBuffer(){
	return IHardwarePixelBufferPtr::Null;
}

void GLMRenderTarget::Resize(int x,int y)
{
	int c=m_textures.size();
	math::vector3di sz(x,y,1);
	for(int i=0;i<c;++i)
	{
		video::ITexture* tex = GetColorTexture(i);
		if(tex && tex->getSize()!=sz)
			tex->createTexture(sz,tex->getImageFormat());
	}
}
void GLMRenderTarget::GetParameter(const core::string& name,void* param)
{
	if(name==mT("GLCONTEXT"))
		param=m_pbuffer->GetContext();
}

}
}


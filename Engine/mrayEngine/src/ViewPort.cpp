#include "stdafx.h"

#include "ViewPort.h"
#include "ISceneManager.h"
#include "DefaultColors.h"

#include "Engine.h"
#include "RenderWindow.h"
#include "CameraNode.h"
//#include "IRenderTarget.h"

namespace mray{
namespace scene{

ViewPort::ViewPort():m_clearColor(0,0,0,0),m_name(""),m_camera(0),m_vp(0,0,1,1),m_renderWindow(0),
	m_zorder(0),m_isActive(true),m_isAutoClearRT(true),m_rt(0),m_autoUpdateAbsRect(true)
{
	if(!m_rt)
		m_absRenderingVP=m_absVP=m_vp;
	m_onlyRenderTarget=false;
	m_autoUpdateRenderTarget=false;
	m_rtSizeDirty=false;

	m_enablePostProcessing=0;
	m_renderTargetTexUnit=new video::TextureUnit();

	updateViewPort();
}
ViewPort::ViewPort(const core::string &name,CameraNode*camera,video::IRenderTarget* rt,
				   video::RenderWindow* wnd,const math::rectf&vp,int zorder):
	m_clearColor(0,0,0,0),m_name(name),m_camera(camera),m_vp(vp),m_renderWindow(wnd),
		m_zorder(zorder),m_isActive(true),m_isAutoClearRT(true),m_rt(rt),m_autoUpdateAbsRect(true)
{
	if(!m_rt)
		m_absRenderingVP=m_absVP=vp;
	m_onlyRenderTarget=false;
	m_autoUpdateRenderTarget=false;
	m_rtSizeDirty=false;

	m_enablePostProcessing=0;

	m_renderTargetTexUnit=new video::TextureUnit();

	updateViewPort();
}
ViewPort::~ViewPort(){
	m_listeners.clear();
}

bool ViewPort::isAutoClearRT(){
	return m_isAutoClearRT;
}

void ViewPort::setAutoClearRT(bool s){
	m_isAutoClearRT=s;
}

void ViewPort::SetAutoUpdateRTSize(bool autoUpdate)
{
	m_autoUpdateRenderTarget=autoUpdate;
	if(m_autoUpdateRenderTarget && m_rt&& m_rt->GetColorTexture())
	{
		math::vector3d sz=m_rt->GetColorTexture()->getSize();
		if(sz.x!=m_absVP.getSize().x && sz.y!=m_absVP.getSize().y)
			m_rtSizeDirty=true;
	}
}


bool ViewPort::isOnlyToRenderTarget()
{
	return m_onlyRenderTarget;
}
void ViewPort::setOnlyToRenderTarget(bool s)
{
	m_onlyRenderTarget=s;
}
void ViewPort::setActive(bool a){
	m_isActive=a;
}
bool ViewPort::isActive(){
	return m_isActive;
}

void ViewPort::_UpdateRTSize()
{
	if(m_rt)
	{
		m_rt->Resize(m_absVP.getSize().x,m_absVP.getSize().y);
	}
}
void ViewPort::draw()
{
	Engine::getInstance().getDevice()->setViewport(this);

	if(m_autoUpdateRenderTarget)
	{
		if(m_rt && m_rt->GetSize()!=m_absVP.getSize())
			m_rtSizeDirty=true;
	}
	if(m_rtSizeDirty)
	{
		_UpdateRTSize();
		m_rtSizeDirty=false;
	}
	if(!m_camera)
	{
		onRenderBegin();
		onRenderDone();
		return;
	}
	if(m_camera->isVisible())
		m_camera->getSceneManager()->render(this);
}

void ViewPort::updateViewPort(){
	/*
	if(m_rt){
			math::vector2di size= m_rt->getSize();
			m_absVP.BRPoint=m_vp.BRPoint*size;
			m_absVP.ULPoint=m_vp.ULPoint*size;
		}else */
	if(m_renderWindow )
	{
		bool recreateRT=false;
		const math::vector2di& rc=m_renderWindow->GetSize();

		math::recti newRect;
		newRect.BRPoint=m_vp.BRPoint*rc;
		newRect.ULPoint=m_vp.ULPoint*rc;
		if(newRect!=m_absRenderingVP)
		{
			m_absRenderingVP=newRect;
			recreateRT=true;
		}
		math::vector2d size=m_absRenderingVP.getSize();
		if(recreateRT && m_rt )
		{
			for(int i=0;i<4;++i)
			{
				video::ITexture *tex= m_rt->GetColorTexture(i);
				if(tex)
				{
					tex->createTexture(math::vector3d(size.x,size.y,1),tex->getImageFormat());
				}
			}
		}
	}
	if(m_autoUpdateAbsRect)
	{
		m_absVP=m_absRenderingVP;
	}
	if(m_camera){
		m_camera->SetWindow(m_absVP.ULPoint.x,m_absVP.ULPoint.y, m_absVP.getWidth(),m_absVP.getHeight());
	}

}


void ViewPort::setPostProcessing(const video::IPostProcessingPtr&pp){
	m_postProcessing=pp;
}
void ViewPort::enablePostProcessing(bool e){
	m_enablePostProcessing=e;
}
bool ViewPort::isEnabledPostProcessing(){
	return m_postProcessing;
}


const math::rectf&ViewPort::getRelViewPort()const{
	return m_vp;
}
const math::recti&ViewPort::getAbsViewPort()const{
	return m_absVP;
}

const math::recti&ViewPort::getAbsRenderingViewPort()const{
	return m_absRenderingVP;
}

int ViewPort::getZOrder(){
	return m_zorder;
}
CameraNode* ViewPort::getCamera(){
	return m_camera;
}
video::IRenderTargetCRef ViewPort::getRenderTarget(){
	return m_rt;
}


void ViewPort::setViewPort(const math::rectf&vp){
	m_vp=vp;
	m_autoUpdateAbsRect=true;
	updateViewPort();
}
void ViewPort::setAbsViewPort(const math::rectf&vp){
	m_absVP=vp;
	if(m_camera){
		m_camera->SetWindow(m_absVP.ULPoint.x,m_absVP.ULPoint.y, m_absVP.getWidth(),m_absVP.getHeight());
	}
	m_autoUpdateAbsRect=false;
	if(!m_renderWindow)
		m_absRenderingVP=vp;
}

void ViewPort::setZOrder(int z){
	m_zorder=z;
}
void ViewPort::setCamera(CameraNode* cam)
{
	m_camera=cam;
	updateViewPort();

	FIRE_LISTENR_METHOD(onSetCamera,(this,cam));

}
void ViewPort::setRenderTarget(video::IRenderTargetCRef rt){
	m_rt=rt;
	updateViewPort();

	FIRE_LISTENR_METHOD(onSetRenderTarget,(this,rt));
}

void ViewPort::onRenderBegin(){

	Engine::getInstance().getDevice()->setViewport(this);
	if(m_rt){
		if(m_isAutoClearRT)
			Engine::getInstance().getDevice()->setRenderTarget(m_rt,true,true,m_clearColor);
		else
			Engine::getInstance().getDevice()->setRenderTarget(m_rt,false,false,m_clearColor);
	}else
	{
		Engine::getInstance().getDevice()->setRenderTarget(0,true,true,m_clearColor);
	}
	FIRE_LISTENR_METHOD(onRenderBegin,(this));
}

void ViewPort::onRenderDone(){

	FIRE_LISTENR_METHOD(onRenderDone,(this));

	video::IVideoDevice* device= Engine::getInstance().getDevice();

	if(m_rt){
		device->setRenderTarget(0,false,false,m_clearColor);
	}
	device->set2DMode();
	//Engine::getInstance().getDevice()->set2DMode();
	if(m_postProcessing && m_enablePostProcessing && m_rt)
	{
		m_postProcessing->Setup(this->getAbsViewPort());
		video::IRenderArea* rt= m_postProcessing->render(m_rt);

		//copy to renderTarget

		device->setRenderTarget(m_rt,false,true,m_clearColor);
		m_renderTargetTexUnit->SetTexture(rt->GetColorTexture());
		device->useTexture(0,m_renderTargetTexUnit);
		device->draw2DImage(math::rectf(0,m_absVP.getSize()),video::DefaultColors::White);
		device->setRenderTarget(0,false,false,m_clearColor);
	}

	if(!m_rt.isNull() && !m_onlyRenderTarget ){
		//math::rectf rc=Engine::getInstance().getDevice()->getViewportRect();
		device->setViewport(this);
		m_renderTargetTexUnit->SetTexture(m_rt->GetColorTexture());
		device->useTexture(0,m_renderTargetTexUnit);
		device->draw2DImage(math::rectf(0,m_absRenderingVP.getSize()),video::DefaultColors::White,0,0);
	}
	//Engine::getInstance().getDevice()->set3DMode();
}

math::vector2di ViewPort::GetSize()
{
	return m_absVP.getSize();
}


void ViewPort::Resize(int x, int y)
{

	if (!m_autoUpdateAbsRect)
	{
		m_absVP.BRPoint.x = m_absVP.ULPoint.x + x;
		m_absVP.BRPoint.y = m_absVP.ULPoint.y + y;
	}
}

const video::ITexturePtr& ViewPort::GetColorTexture(int i)
{
	if(m_rt)
		return m_rt->GetColorTexture(i);
	return video::ITexturePtr::Null;
}
int ViewPort::GetColorTextureCount()
{
	if (m_rt)
		return m_rt->GetColorTextureCount();
	return 0;
}
void ViewPort::SetClearColor(const video::SColor& c)
{
	m_clearColor=c;
}

}
}

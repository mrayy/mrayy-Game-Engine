
#include "stdafx.h"

#include "Sun.h"
#include <LightNode.h>
#include <DeviceCapabilites.h>
#include <MathUtil.h>
#include "Engine.h"
#include "GameLevel.h"
#include "ISceneManager.h"



namespace mray{
namespace game{

	IMPLEMENT_RTTI(Sun,IObject)

Sun::Sun(GameLevel*lvl):m_vertical(0),m_horizontal(0),m_light(0),m_attachedNode(0),m_level(lvl)
{
	m_light=m_level->GetSceneManager()->createLightNode(mT("SunLight"));//new scene::LightNode(0,video::SColor(220,214,190,255),true,100,100,0);
	m_light->setCastShadows(true);
	m_light->setDiffuse(video::SColor(220,214,190,255));
	_RecalcPosition();
	SetShadowMapSize(math::vector2di(2048,2048));
	EnableShadow(true);
	SetShadowProjectionParams(80,0.3,10,800);
/*
	addPropertie(new rwProperty<Sun,float>(this,&Sun::SetVertical,&Sun::GetVertical,EPT_FLOAT,mT("Vertical"),mT("Vertical position of the sun")));
	addPropertie(new rwProperty<Sun,float>(this,&Sun::SetHorizontal,&Sun::GetHorizontal,EPT_FLOAT,mT("Horizontal"),mT("Horizontal position of the sun")));
	addPropertie(new rwProperty<Sun,video::SColor>(this,&Sun::SetColor,&Sun::GetColor,EPT_COLOR,mT("Color"),mT("Sun light color")));


	addPropertie(new rwProperty<Sun,bool>(this,&Sun::EnableShadow,&Sun::_isShadowEnable,EPT_BOOLEAN,mT("Shadow"),mT("Enable/Disable Sun shadow map")));
	addPropertie(new rwProperty<Sun,float>(this,&Sun::SetShadowProjFov,&Sun::GetShadowProjFov,EPT_FLOAT,mT("ShadowFov"),mT("Set Fov value for shadow map projection matrix")));
	addPropertie(new rwProperty<Sun,float>(this,&Sun::SetShadowProjAspect,&Sun::GetShadowProjAspect,EPT_FLOAT,mT("ShadowAspect"),mT("Set Aspect ratio value for shadow map projection matrix")));
	addPropertie(new rwProperty<Sun,float>(this,&Sun::SetShadowProjNear,&Sun::GetShadowProjNear,EPT_FLOAT,mT("ShadowZNear"),mT("Set ZNear value for shadow map projection matrix")));
	addPropertie(new rwProperty<Sun,float>(this,&Sun::SetShadowProjFar,&Sun::GetShadowProjFar,EPT_FLOAT,mT("ShadowZFar"),mT("Set ZFar value for shadow map projection matrix")));

	addPropertie(new rwProperty<Sun,math::vector2di>(this,&Sun::SetShadowMapSize,&Sun::GetShadowMapSize,EPT_VECTOR2Di,mT("ShadowMapSize"),mT("Size of the shadow map render target")));*/
}
Sun::~Sun()
{
	/*
	delete getPropertie(mT("Vertical"));
	delete getPropertie(mT("Horizontal"));
	delete getPropertie(mT("Color"));

	delete getPropertie(mT("Shadow"));
	delete getPropertie(mT("ShadowFov"));
	delete getPropertie(mT("ShadowAspect"));
	delete getPropertie(mT("ShadowZNear"));
	delete getPropertie(mT("ShadowZFar"));
	delete getPropertie(mT("ShadowMapSize"));
	*/

} 


void Sun::SetShadowMapSize(const math::vector2di& sz)
{
	video::IVideoDevice*device= Engine::getInstance().getDevice();
	m_shadowMapSize=sz;
	if(device->getCapabilities()->isFeatureSupported(video::EDF_RenderToTarget)){
		GCPtr<video::ITexture> texture=device->createTexture2D(math::vector2d(sz.x,sz.y),video::EPixel_Float16_R,true);
		GCPtr<video::IRenderTarget> shadowRenderTarget=device->createRenderTarget(mT("shadowRenderTarget"),texture,0,0,0);
		m_light->setShadowMap(shadowRenderTarget);
	}
}
void Sun::EnableShadow(const bool &e)
{
	m_enableShadow=e;
	m_light->setCastShadows(e);
}
void Sun::SetShadowProjectionParams(float fov,float aspect,float znear,float zfar)
{
	m_shadowProjFov=fov;
	m_shadowProjAspect=aspect;
	m_shadowProjNear=znear;
	m_shadowProjFar=zfar;
	math::matrix4x4 projection;
	projection =math::MathUtil::CreateProjectionMatrixPerspectiveFov(math::toRad(fov),aspect,znear,zfar);
	m_light->setProjection(projection);
}

const bool& Sun::_isShadowEnable()
{
	return m_enableShadow;
}

void Sun::SetShadowProjFov(const float& v)
{
	SetShadowProjectionParams(v,m_shadowProjAspect,m_shadowProjNear,m_shadowProjFar);
}
void Sun::SetShadowProjAspect(const float& v)
{
	SetShadowProjectionParams(m_shadowProjFov,v,m_shadowProjNear,m_shadowProjFar);
}
void Sun::SetShadowProjNear(const float& v)
{
	SetShadowProjectionParams(m_shadowProjFov,m_shadowProjAspect,v,m_shadowProjFar);
}
void Sun::SetShadowProjFar(const float& v)
{
	SetShadowProjectionParams(m_shadowProjFov,m_shadowProjAspect,m_shadowProjNear,v);
}


const float& Sun::GetShadowProjFov()
{
	return m_shadowProjFov;
}
const float& Sun::GetShadowProjAspect()
{
	return m_shadowProjAspect;
}
const float& Sun::GetShadowProjNear()
{
	return m_shadowProjNear;
}
const float& Sun::GetShadowProjFar()
{
	return m_shadowProjFar;
}

const math::vector2di& Sun::GetShadowMapSize()
{
	return m_shadowMapSize;
}
const video::SColor& Sun::GetColor()
{
	return m_color;
}
void Sun::_RecalcPosition()
{
	m_dir.set(0,0,1);
	math::quaternion q=math::quaternion(m_vertical,math::vector3d::YAxis)*math::quaternion(m_horizontal,math::vector3d::XAxis);
	m_dir=q*m_dir;
}

void Sun::SetHorizontal(const float& v)
{
	m_horizontal=v;
	_RecalcPosition();
}
const float& Sun::GetHorizontal()
{
	return m_horizontal;
}

void Sun::SetVertical(const float& v)
{
	m_vertical=v;
	_RecalcPosition();
}
const float& Sun::GetVertical()
{
	return m_vertical;
}

const math::vector3d& Sun::GetDirection()
{
	return m_dir;
}

void Sun::SetColor(const video::SColor &clr){
	m_color=clr;
	m_light->setDiffuse(m_color);
}

scene::LightNode* Sun::GetLight(){
	return m_light;
}


void Sun::AttachNode(scene::IMovable*node)
{
	m_attachedNode=node;
}

scene::IMovable* Sun::GetAttachedNode()
{
	return m_attachedNode;
}

void Sun::Update(float dt)
{
	m_light->setPosition(m_dir*1000);
	m_light->setTarget(math::vector3d::Zero);
	if(m_attachedNode)
		m_attachedNode->setPosition(m_light->getPosition());
}
xml::XMLElement*  Sun::loadXMLSettings(xml::XMLElement* elem)
{
	return IObject::loadXMLSettings(elem);
}
xml::XMLElement*  Sun::exportXMLSettings(xml::XMLElement* elem)
{
	xml::XMLElement*e=IObject::exportXMLSettings(elem);
	return e;
}

}
}


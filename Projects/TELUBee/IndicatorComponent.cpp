
#include "stdafx.h"
#include "IndicatorComponent.h"
#include "SceneComponent.h"
#include "ISceneNode.h"


namespace mray
{
namespace TBee
{

IMPLEMENT_RTTI(IndicatorComponent,IGameComponent)

IMPLEMENT_SETGET_PROP(IndicatorComponent,NodeName,core::string,m_nodeName,"",,)
IMPLEMENT_SETGET_PROP(IndicatorComponent,Translate,bool,m_translate,false,core::StringConverter::toBool,core::StringConverter::toString)
IMPLEMENT_SETGET_PROP(IndicatorComponent,Rotate,bool,m_rotate,false,core::StringConverter::toBool,core::StringConverter::toString)
IMPLEMENT_SETGET_PROP(IndicatorComponent,Speed,float,m_speed,0,core::StringConverter::toFloat,core::StringConverter::toString)

IndicatorComponent::IndicatorComponent(game::GameEntityManager* mngr)
{
	m_translate=0;
	m_rotate=0;
	m_speed=10;
	m_scenenode=0;
	CPropertieDictionary* dic;
	if(CreateDictionary(&dic))
	{
		dic->addPropertie(&PropertyTypeNodeName::instance);
		dic->addPropertie(&PropertyTypeTranslate::instance);
		dic->addPropertie(&PropertyTypeRotate::instance);
		dic->addPropertie(&PropertyTypeSpeed::instance);
	}
}
IndicatorComponent::~IndicatorComponent()
{
}

bool IndicatorComponent::InitComponent()
{
	if(!IGameComponent::InitComponent())
		return false;
	m_time=0;

	m_scenenode=IGameComponent::RetriveComponent<game::SceneComponent>(GetOwnerComponent(),m_nodeName);

	return true;
}
void IndicatorComponent::Update(float dt)
{
	if(m_scenenode && m_scenenode->GetSceneNode())
	{
		if(m_translate)
			m_scenenode->GetSceneNode()->setPosition(math::vector3d(0,1+sin(m_speed*m_time),0));
		if(m_rotate)
			m_scenenode->GetSceneNode()->setOrintation(math::quaternion(0,m_speed*m_time,0));
	}
	m_time+=dt;
}
IMPLEMENT_GAMECOMPONENT_FACTORY(IndicatorComponent)

}
}

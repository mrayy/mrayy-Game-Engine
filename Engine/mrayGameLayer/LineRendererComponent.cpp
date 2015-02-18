
#include "stdafx.h"
#include "LineRendererComponent.h"
#include "SceneComponent.h"
#include "ISceneNode.h"

#include "ILogManager.h"
#include "MaterialResourceManager.h"

namespace mray
{
namespace game
{


	IMPLEMENT_PROPERTY_TYPE_HEADER(MaterialName,LineRendererComponent,core::string,mT("MaterialName"),EPBT_Basic,mT(""),mT(""));
	IMPLEMENT_PROPERTY_TYPE_GENERIC(MaterialName,LineRendererComponent,core::string,SetMaterialName,GetMaterialName,,,false);

	IMPLEMENT_PROPERTY_TYPE_HEADER(UseWorldSpace,LineRendererComponent,bool,mT("UseWorldSpace"),EPBT_Basic,mT(""),false);
	IMPLEMENT_PROPERTY_TYPE_GENERIC(UseWorldSpace,LineRendererComponent,bool,SetUseWorldSpace,GetUseWorldSpace,core::StringConverter::toString,core::StringConverter::toBool,false);

	IMPLEMENT_PROPERTY_TYPE_HEADER(Width,LineRendererComponent,float,mT("Width"),EPBT_Basic,mT(""),1);
	IMPLEMENT_PROPERTY_TYPE_GENERIC(Width,LineRendererComponent,float,SetWidth,GetWidth,core::StringConverter::toString,core::StringConverter::toFloat,false);


	IMPLEMENT_PROPERTY_TYPE_HEADER(Start,LineRendererComponent,math::vector3d,mT("Start"),EPBT_Basic,mT(""),0);
	IMPLEMENT_PROPERTY_TYPE_GENERIC(Start,LineRendererComponent,math::vector3d,SetStartPoint,GetStartPoint,core::StringConverter::toString,core::StringConverter::toVector3d,false);

	IMPLEMENT_PROPERTY_TYPE_HEADER(End,LineRendererComponent,math::vector3d,mT("End"),EPBT_Basic,mT(""),0);
	IMPLEMENT_PROPERTY_TYPE_GENERIC(End,LineRendererComponent,math::vector3d,SetEndPoint,GetEndPoint,core::StringConverter::toString,core::StringConverter::toVector3d,false);


LineRendererComponent::LineRendererComponent(GameEntityManager*)
{
	m_renderNode=new scene::SBillboardChain();
	m_renderNode->SetMaxPointCount(2);

	CPropertieDictionary* dic;
	if(CreateDictionary(&dic))
	{
		dic->addPropertie(&PropertyTypeMaterialName::instance);
		dic->addPropertie(&PropertyTypeUseWorldSpace::instance);
		dic->addPropertie(&PropertyTypeWidth::instance);
		dic->addPropertie(&PropertyTypeStart::instance);
		dic->addPropertie(&PropertyTypeEnd::instance);
	}
}
LineRendererComponent::~LineRendererComponent()
{
}


bool LineRendererComponent::SetMaterialName(const core::string&n)
{
	m_materialName=n;
	video::RenderMaterial*m= gMaterialResourceManager.getMaterial(m_materialName);
	if(m)
	{
		m_renderNode->setMaterial(m,0);
	}
	return true;
}

bool LineRendererComponent::SetWidth(float width)
{
	//m_renderNode->setWidth(width);
	m_renderNode->SetStartWidth(width);
	m_renderNode->SetEndWidth(width);
	return true;
}
float LineRendererComponent::GetWidth()
{
	return m_renderNode->GetStartWidth();
}

bool LineRendererComponent::SetUseWorldSpace(bool u)
{
//	m_renderNode->SetUseWorldSpace(u);
	return true;
}
bool LineRendererComponent::GetUseWorldSpace()
{
	return false;//m_renderNode->GetUseWorldSpace();
}

bool LineRendererComponent::SetStartPoint(const math::vector3d& start)
{
	m_renderNode->SetPoint(0,start);
	//m_renderNode->setStartPoint(start);
	return true;
}
const math::vector3d& LineRendererComponent::GetStartPoint()
{
	return m_renderNode->GetPoint(0);//m_renderNode->getStartPoint();
}

bool LineRendererComponent::SetEndPoint(const math::vector3d& end)
{
	m_renderNode->SetPoint(1,end);
	//m_renderNode->setEndPoint(end);
	return true;
}
const math::vector3d& LineRendererComponent::GetEndPoint()
{
	return m_renderNode->GetPoint(1);//m_renderNode->getEndPoint();
}

bool LineRendererComponent::InitComponent()
{
	if(!IGameComponent::InitComponent())
		return false;
	SceneComponent*comp=dynamic_cast<SceneComponent*>(m_ownerComponent-> GetFirstComponent(SceneComponent::getClassType()));
	if(comp)
	{
		comp->GetSceneNode()->AttachNode(m_renderNode);
		gLogManager.log("Line Renderer attached to: "+comp->GetName(),ELL_INFO);
	}
	return true;
}


}
}




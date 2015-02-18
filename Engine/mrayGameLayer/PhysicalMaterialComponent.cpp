

#include "stdafx.h"
#include "PhysicalMaterialComponent.h"
#include "IPhysicManager.h"
#include "GameEntity.h"
#include "GameEntityManager.h"


namespace mray
{
namespace game
{
IMPLEMENT_RTTI(PhysicalMaterialComponent,IGameComponent)


IMPLEMENT_PROPERTY_TYPE_HEADER(StaticFriction,PhysicalMaterialComponent,float,mT("StaticFriction"),EPBT_Basic,mT("Static Friction of the material"),0.5);
IMPLEMENT_PROPERTY_TYPE_GENERIC(StaticFriction,PhysicalMaterialComponent,float,SetStaticFriction,GetStaticFriction,core::StringConverter::toString,core::StringConverter::toFloat,false);

IMPLEMENT_PROPERTY_TYPE_HEADER(DynamicFriction,PhysicalMaterialComponent,float,mT("DynamicFriction"),EPBT_Basic,mT("Dynamic Friction of the material"),0.5);
IMPLEMENT_PROPERTY_TYPE_GENERIC(DynamicFriction,PhysicalMaterialComponent,float,SetDynamicFriction,GetDynamicFriction,core::StringConverter::toString,core::StringConverter::toFloat,false);

IMPLEMENT_PROPERTY_TYPE_HEADER(Restitution,PhysicalMaterialComponent,float,mT("Restitution"),EPBT_Basic,mT("Restitution of the material"),0);
IMPLEMENT_PROPERTY_TYPE_GENERIC(Restitution,PhysicalMaterialComponent,float,SetRestitution,GetRestitution,core::StringConverter::toString,core::StringConverter::toFloat,false);

DECLARE_PROPERTY_TYPE(StaticFriction,float,);
DECLARE_PROPERTY_TYPE(DynamicFriction,float,);
DECLARE_PROPERTY_TYPE(Restitution,float,);

PhysicalMaterialComponent::PhysicalMaterialComponent(GameEntityManager*mngr)
{
	m_desc.staticFriction=PropertyTypeStaticFriction::instance.GetDefaultValue();
	m_desc.dynamicFriction=PropertyTypeDynamicFriction::instance.GetDefaultValue();
	m_desc.restitution=PropertyTypeRestitution::instance.GetDefaultValue();
	CPropertieDictionary* dic;
	if(CreateDictionary(&dic))
	{
		dic->addPropertie(&PropertyTypeStaticFriction::instance);
		dic->addPropertie(&PropertyTypeDynamicFriction::instance);
		dic->addPropertie(&PropertyTypeRestitution::instance);
	}

	m_material=0;
}
PhysicalMaterialComponent::~PhysicalMaterialComponent()
{
}


bool PhysicalMaterialComponent::InitComponent()
{
	if(!IGameComponent::InitComponent())
		return false;
	m_desc.name=GetName();
	m_material=GetOwnerEntity()->GetCreator()->GetPhysicsManager()->createMaterial(&m_desc);
	return true;
}

bool PhysicalMaterialComponent::SetDynamicFriction(float v)
{
	m_desc.dynamicFriction=v;
	if(m_material)
		m_material->LoadFromDesc(&m_desc);
	return true;
}
bool PhysicalMaterialComponent::SetStaticFriction(float v)
{
	m_desc.staticFriction=v;
	if(m_material)
		m_material->LoadFromDesc(&m_desc);
	return true;
}
bool PhysicalMaterialComponent::SetRestitution(float v)
{
	m_desc.restitution=v;
	if(m_material)
		m_material->LoadFromDesc(&m_desc);
	return true;
}


IMPLEMENT_GAMECOMPONENT_FACTORY(PhysicalMaterialComponent);

}
}

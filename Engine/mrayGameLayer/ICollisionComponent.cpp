
#include "stdafx.h"
#include "ICollisionComponent.h"
#include "PhysicalShapes.h"
#include "GameEntity.h"
#include "PhysicalMaterialComponent.h"


namespace mray
{
namespace game
{

IMPLEMENT_PROPERTY_TYPE_HEADER(Name,ICollisionComponent,core::string,mT("Name"),EPBT_Basic,mT(""),mT(""));
IMPLEMENT_PROPERTY_TYPE_GENERIC(Name,ICollisionComponent,core::string,SetName,GetName,,,false);

IMPLEMENT_PROPERTY_TYPE_HEADER(PhysicalMaterial,ICollisionComponent,core::string,mT("PhysicalMaterial"),EPBT_Basic,mT(""),mT(""));
IMPLEMENT_PROPERTY_TYPE_GENERIC(PhysicalMaterial,ICollisionComponent,core::string,SetPhysicalMaterial,GetPhysicalMaterial,,,false);

IMPLEMENT_PROPERTY_TYPE_HEADER(Density,ICollisionComponent,float,mT("Density"),EPBT_Basic,mT(""),0);
IMPLEMENT_PROPERTY_TYPE_GENERIC(Density,ICollisionComponent,float,SetDensity,GetDensity,core::StringConverter::toString,core::StringConverter::toFloat,false);

IMPLEMENT_PROPERTY_TYPE_HEADER(LocalSpace,ICollisionComponent,math::matrix4x4,mT("LocalSpace"),EPBT_Basic,mT(""),math::matrix4x4::Identity);
IMPLEMENT_PROPERTY_TYPE_GENERIC(LocalSpace,ICollisionComponent,math::matrix4x4,SetLocalSpace,GetLocalSpace,core::StringConverter::toString,core::StringConverter::toMatrix,false);

IMPLEMENT_PROPERTY_TYPE_HEADER(Mass,ICollisionComponent,float,mT("Mass"),EPBT_Basic,mT(""),0);
IMPLEMENT_PROPERTY_TYPE_GENERIC(Mass,ICollisionComponent,float,SetMass,GetMass,core::StringConverter::toString,core::StringConverter::toFloat,false);

IMPLEMENT_PROPERTY_TYPE_HEADER(GroupID,ICollisionComponent,physics::GroupID,mT("GroupID"),EPBT_Basic,mT(""),0);
IMPLEMENT_PROPERTY_TYPE_GENERIC(GroupID,ICollisionComponent,physics::GroupID,SetCollisionGroup,GetCollisionGroup,core::StringConverter::toString,core::StringConverter::toUInt,false);

IMPLEMENT_PROPERTY_TYPE_HEADER(Flags,ICollisionComponent,uint,mT("Flags"),EPBT_Basic,mT(""),0);
IMPLEMENT_PROPERTY_TYPE_GENERIC(Flags,ICollisionComponent,uint,SetFlags,GetFlags,core::StringConverter::toString,core::StringConverter::toUInt,false);

ICollisionComponent::ICollisionComponent()
{
	CPropertieDictionary* dic;
	if(CreateDictionary(&dic))
	{
		dic->addPropertie(&PropertyTypeName::instance);
		dic->addPropertie(&PropertyTypePhysicalMaterial::instance);
		dic->addPropertie(&PropertyTypeDensity::instance);
		dic->addPropertie(&PropertyTypeLocalSpace::instance);
		dic->addPropertie(&PropertyTypeMass::instance);
		dic->addPropertie(&PropertyTypeGroupID::instance);
		dic->addPropertie(&PropertyTypeFlags::instance);
	}
}
ICollisionComponent::~ICollisionComponent()
{
}

bool ICollisionComponent::SetName(const core::string& n)
{
	GetShapeDesc()->name=n;
	return true;
}
const core::string& ICollisionComponent::GetName()
{
	return GetShapeDesc()->name;
}

bool ICollisionComponent::SetPhysicalMaterial(const core::string& n)
{
	m_physicalMaterial=n;
	PhysicalMaterialComponent* c=RetriveComponent<PhysicalMaterialComponent>(GetOwnerComponent(),m_physicalMaterial);
	if(c)
	{
		if(!c->IsInited())
			c->InitComponent();
		GetShapeDesc()->material=c->GetMaterial();
	}
	return true;
}
const core::string& ICollisionComponent::GetPhysicalMaterial()
{
	return m_physicalMaterial;
}

bool ICollisionComponent::SetDensity(float d)
{
	GetShapeDesc()->density=d;
	return true;
}
float ICollisionComponent::GetDensity()
{
	return GetShapeDesc()->density;
}

bool ICollisionComponent::SetLocalSpace(const math::matrix4x4&m)
{
	GetShapeDesc()->localSpace=m;
	return true;
}
const math::matrix4x4& ICollisionComponent::GetLocalSpace()
{
	return GetShapeDesc()->localSpace;
}

bool ICollisionComponent::SetMaterial(physics::IPhysicMaterial* m)
{
	GetShapeDesc()->material=m;
	return true;
}
physics::IPhysicMaterial* ICollisionComponent::GetMaterial()
{
	return GetShapeDesc()->material;
}

bool ICollisionComponent::SetMass(float m)
{
	GetShapeDesc()->mass=m;
	return true;
}
float ICollisionComponent::GetMass()
{
	return GetShapeDesc()->mass;
}

bool ICollisionComponent::SetCollisionGroup(physics::GroupID g)
{
	GetShapeDesc()->collisionGroup=g;
	return true;
}
physics::GroupID ICollisionComponent::GetCollisionGroup()
{
	return GetShapeDesc()->collisionGroup;
}

bool ICollisionComponent::SetCollisionMask(const physics::CollisionMask& c)
{
	GetShapeDesc()->groupMask=c;
	return true;
}
const physics::CollisionMask& ICollisionComponent::GetCollisionMask()
{
	return GetShapeDesc()->groupMask;
}

bool ICollisionComponent::SetUserData(void* u)
{
	GetShapeDesc()->userData=u;
	return true;
}
void* ICollisionComponent::GetUserData()
{
	return GetShapeDesc()->userData;
}

bool ICollisionComponent::SetFlags(uint f)
{
	GetShapeDesc()->flags=f;
	return true;
}
uint ICollisionComponent::GetFlags()
{
	return GetShapeDesc()->flags;
}
xml::XMLElement*  ICollisionComponent::loadXMLSettings(xml::XMLElement* elem)
{
	xml::XMLElement* ret=IGameComponent::loadXMLSettings(elem);
	if(!ret)
		return ret;
	//physics::PhysicXMLSerializer::Parse(ret,&m_nodeDesc);
	return ret;
}
xml::XMLElement*  ICollisionComponent::exportXMLSettings(xml::XMLElement* elem)
{
	xml::XMLElement* ret=IGameComponent::exportXMLSettings(elem);
	return ret;
}

}
}

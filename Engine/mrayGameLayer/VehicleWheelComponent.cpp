


#include "stdafx.h"
#include "VehicleWheelComponent.h"
#include "VehicleGameComponent.h"
#include "GameEntity.h"

#include "IMovable.h"
#include "IVehicleWheel.h"
#include "PhysicalVehicle.h"
#include "IPhysicalNode.h"

#include "ModelComponent.h"
#include "MeshRenderableNode.h"
#include "BoneNode.h"

namespace mray
{
namespace game
{

VehicleWheelComponent::VehicleWheelComponent(GameEntityManager*)
{
	_Init();
}
VehicleWheelComponent::VehicleWheelComponent(const core::string& wheel,const core::string& bone)
{
	_Init();
	m_wheelName=wheel;
	m_boneName=bone;
	_Rebuild();
}
void VehicleWheelComponent::_Init()
{
	m_angle=0;
	m_wheel=0;
	m_bone=0;
	m_car=0;

	CPropertieDictionary* dic;
	if(CreateDictionary(&dic))
	{
		dic->addPropertie(&PropertyTypeBoneName::instance);
		dic->addPropertie(&PropertyTypeWheelName::instance);
	}
}
VehicleWheelComponent::VehicleWheelComponent(scene::BoneNode* bone,physics::IVehicleWheel* phNode,physics::PhysicalVehicle*v)
{
	m_angle=0;
	m_wheel=phNode;
	m_bone=bone;

	m_car=v;

	CPropertieDictionary* dic;
	if(CreateDictionary(&dic))
	{
		dic->addPropertie(&PropertyTypeBoneName::instance);
		dic->addPropertie(&PropertyTypeWheelName::instance);
	}
}
VehicleWheelComponent::~VehicleWheelComponent()
{
}

bool VehicleWheelComponent::_Rebuild()
{
	if(!m_ownerEntity)
		return false;
	VehicleGameComponent* comp =dynamic_cast<VehicleGameComponent*>(m_ownerComponent);

	game::ModelComponent* modelComp=dynamic_cast<ModelComponent*>(m_ownerEntity->GetFirstComponent(ModelComponent::getClassRTTI()));
	if(!comp || !modelComp)
		return false;
	m_car=comp->GetModel();
	if(!modelComp->GetModel() || !modelComp->GetModel()->getMesh() ||
		!modelComp->GetModel()->getMesh()->getSkeleton())
		return false;
	physics::PhysicalVehicle* veh=comp->GetModel();
	m_wheel= veh->GetWheelByName(m_wheelName);

	
	m_bone=modelComp->GetModel()->getMesh()->getSkeleton()->getBone(m_boneName);

	if(m_wheel && m_bone)
	{
		m_wheel->SetWheelPos(m_bone->getAbsoluteBasePosition()*comp->GetScale());
	}

	return m_wheel!=0 && m_bone!=0;
}

bool VehicleWheelComponent::SetBoneName(const core::string &n)
{
	m_boneName=n;
	return _Rebuild();
}
core::string VehicleWheelComponent::GetBoneName()
{
	return m_boneName;
}
bool VehicleWheelComponent::SetWheelName(const core::string &n)
{
	m_wheelName=n;
	return _Rebuild();
}
core::string VehicleWheelComponent::GetWheelName()
{
	return m_wheelName;
}
void VehicleWheelComponent::Update(float dt)
{
	if(m_wheel && m_bone && m_car)
	{
		math::matrix4x4 parent;
		m_car->GetPhysicalNode()->getGlobalOrintation().toMatrix(parent);
		parent.setTranslation(m_car->GetPhysicalNode()->getGlobalPosition());

		m_angle+=math::toDeg(m_wheel->GetRPM()*dt*math::TwoPI32);

		//m_bone->setPosition(parent*m_wheel->GetWheelPos());
		/*
		math::quaternion q(m_wheel->GetGlobalOrintation());
		q*=math::quaternion(m_wheel->GetSteeringAngle(),vector3d::YAxis);
		q*=math::quaternion(m_angle,vector3d::XAxis);*/
		math::quaternion q=math::quaternion(m_wheel->GetSteeringAngle(),math::vector3d::YAxis)*
			math::quaternion(m_angle,math::vector3d::XAxis);
		m_bone->setOrintation(q);
	}
}


IMPLEMENT_PROPERTY_TYPE_HEADER(BoneName,VehicleWheelComponent,core::string,mT("BoneName"),EPBT_Basic,mT(""),mT(""))
IMPLEMENT_PROPERTY_TYPE_GENERIC(BoneName,VehicleWheelComponent,core::string,SetBoneName,GetBoneName,,,false)

IMPLEMENT_PROPERTY_TYPE_HEADER(WheelName,VehicleWheelComponent,core::string,mT("WheelName"),EPBT_Basic,mT(""),mT(""))
IMPLEMENT_PROPERTY_TYPE_GENERIC(WheelName,VehicleWheelComponent,core::string,SetWheelName,GetWheelName,,,false)


}
}

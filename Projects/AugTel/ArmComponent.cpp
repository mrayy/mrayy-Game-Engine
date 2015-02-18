

#include "stdafx.h"
#include "ArmComponent.h"
#include "IMovableComponent.h"
#include "KinematicJointDOF6Component.h"
#include "CoupledJointComponent.h"
#include "ISceneNode.h"

namespace mray
{
namespace AugTel
{


	IMPLEMENT_SETGET_PROP(ArmComponent, Prefix, core::string, m_prefix, "", , );
	IMPLEMENT_SETGET_PROP(ArmComponent, ShoulderName, core::string, m_shoulderName, "", , );
	IMPLEMENT_SETGET_PROP(ArmComponent, ArmBaseName, core::string, m_armBaseName, "", , );
	IMPLEMENT_SETGET_PROP(ArmComponent, ArmName, core::string, m_armName, "", , );
	IMPLEMENT_SETGET_PROP(ArmComponent, WristName, core::string, m_wristName, "", , );
	IMPLEMENT_SETGET_PROP(ArmComponent, WristBaseName, core::string, m_wristBaseName, "", , );
	
	IMPLEMENT_PROPERTY_TYPE_HEADER(ArmLength, ArmComponent, float, mT("ArmLength"), EPBT_Basic, mT(""), 0.2);
	IMPLEMENT_PROPERTY_TYPE_GENERIC(ArmLength, ArmComponent, float, SetArmLength, GetArmLength, core::StringConverter::toString, core::StringConverter::toFloat, false);
ArmComponent::ArmComponent(game::GameEntityManager*m)
{
	m_armLength = 0.2;
	m_shoulderLength = 0.3;
	CPropertieDictionary* dic;
	if (CreateDictionary(&dic))
	{
		dic->addPropertie(&PropertyTypePrefix::instance);
		dic->addPropertie(&PropertyTypeShoulderName::instance);
		dic->addPropertie(&PropertyTypeArmBaseName::instance);
		dic->addPropertie(&PropertyTypeArmName::instance);
		dic->addPropertie(&PropertyTypeWristName::instance);
		dic->addPropertie(&PropertyTypeWristBaseName::instance);
		dic->addPropertie(&PropertyTypeArmLength::instance);
	}
}
ArmComponent::~ArmComponent()
{
}
void ArmComponent::_load(game::IMovableComponent** m, const core::string&name)
{

	game::IMovableComponent* comp = RetriveComponent<game::IMovableComponent>(GetOwnerComponent(), m_prefix + name);
	if (comp)
	{
		comp->InitComponent();
	/*	scene::IMovablePtr node= new scene::BoneNode(m_prefix+name+"_parent",-1,0,0);
		comp->GetMovableObject()->updateAbsoluteTransformation();
		node->setPosition(comp->GetMovableObject()->getAbsolutePosition());
		scene::IMovable* parent= comp->GetMovableObject()->getParent();
		if (parent)
			parent->addChild(node,true);
		node->addChild(comp->GetMovableObject(),true);
		*m = node;// */
		*m = comp;
	}
	else
		*m = 0;
}
bool ArmComponent::InitComponent()
{
	if (!game::IGameComponent::InitComponent())
		return false;
	_load(&m_shoulder, m_shoulderName);
	_load(&m_foreamBase, m_armBaseName);
	_load(&m_foream, m_armName);
	_load(&m_wrist, m_wristName);
	_load(&m_wristBase, m_wristBaseName);

	SetArmLength(m_armLength);
	
	for (int i = 0; i < 5; ++i)
	{

		core::string pre_name = "Finger_" + core::StringConverter::toString(i+1) + "_";
		for (int j = 0; j < 4; ++j)
		{
			_load(&m_fingers[i][j], pre_name + core::StringConverter::toString(j+1));
		}
	}

	return true;
}
bool ArmComponent::SetArmLength(float l)
{
	m_armLength = l;
	if (m_wristBase && m_wristBase->GetMovableObject() && m_wristBase->GetMovableObject()->getParent())
	{
		scene::IMovable* parent = m_wristBase->GetMovableObject()->getParent();
		math::vector3d pos = parent->getPosition();
		pos.z = m_armLength*0.5f;
		parent->setPosition(pos);
	}
	if (m_foreamBase && m_foreamBase->GetMovableObject() && m_foreamBase->GetMovableObject()->getParent())
	{
		scene::IMovable* parent = m_foreamBase->GetMovableObject()->getParent();
		math::vector3d pos = parent->getPosition();
		pos.z = m_armLength*0.5f;
		parent->setPosition(pos);
	}
	return true;
}

bool ArmComponent::SetShoulderDistance(float l)
{
	m_shoulderLength = l;
	if (m_shoulder && m_shoulder->GetMovableObject() && m_shoulder->GetMovableObject()->getParent())
	{
		scene::IMovable* parent = m_shoulder->GetMovableObject()->getParent();
		math::vector3d pos = parent->getPosition();
		pos.x = m_shoulderLength;
		parent->setPosition(pos);
	}
	return true;
}

void ArmComponent::Update(float dt)
{
	IGameComponent::Update(dt);

}

void ArmComponent::SetForeamAngles(const math::vector3d& angles)
{
	/*
	if (m_foream[0])
		m_foream[0]->SetControlValue(VT::ControlInputValues(angles.x, angles.x), VT::EControl_Realtime);
	if (m_foream[1])
		m_foream[1]->SetControlValue(VT::ControlInputValues(angles.y, angles.y), VT::EControl_Realtime);
		*/
	if (m_foream)
		m_foream->GetMovableObject()->setOrintation(angles);
// 	if (m_foream[2])
// 		m_foream[2]->SetControlValue(VT::ControlInputValues(angles.z, angles.z), VT::EControl_Realtime);
}

void ArmComponent::SetWristAngles(const math::vector3d& angles)
{

	if (m_wrist)
		m_wrist->GetMovableObject()->setOrintation(angles);
	/*
	if (m_wrist[0])
		m_wrist[0]->SetControlValue(VT::ControlInputValues(angles.x, angles.x), VT::EControl_Realtime);
	if (m_wrist[1])
		m_wrist[1]->SetControlValue(VT::ControlInputValues(angles.y, angles.y), VT::EControl_Realtime);
	if (m_foream[2])//from wrist
		m_foream[2]->SetControlValue(VT::ControlInputValues(angles.z, angles.z), VT::EControl_Realtime);
		*/
}




}
}






#include "stdafx.h"
#include "ArmComponent.h"
#include "IMovableComponent.h"
#include "KinematicJointDOF6Component.h"
#include "CoupledJointComponent.h"
#include "ISceneNode.h"

namespace mray
{
namespace game
{


	IMPLEMENT_PROPERTY_TYPE_HEADER(Prefix, ArmComponent, core::string, mT("Prefix"), EPBT_Basic, mT(""), mT(""));
	IMPLEMENT_PROPERTY_TYPE_GENERIC(Prefix, ArmComponent, core::string, SetPrefix, GetPrefix, , , false);

ArmComponent::ArmComponent(game::GameEntityManager*m)
{
	CPropertieDictionary* dic;
	if (CreateDictionary(&dic))
	{
		dic->addPropertie(&PropertyTypePrefix::instance);
	}
}
ArmComponent::~ArmComponent()
{
}
void ArmComponent::_load(VT::CoupledJointComponent** m, const core::string&name)
{

	VT::CoupledJointComponent* comp = RetriveComponent<VT::CoupledJointComponent>(GetOwnerComponent(), m_prefix + name);
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
	_load(&m_shoulder[0], "ShoulderX");
	_load(&m_shoulder[1], "ShoulderY");
	_load(&m_shoulder[2], "ShoulderZ");
	_load(&m_foream[0], "ArmX");
	_load(&m_foream[1], "ArmY");
	_load(&m_foream[2], "ArmZ");
	_load(&m_wrist[0], "HandX");
	_load(&m_wrist[1], "HandY");
	_load(&m_wrist[2], "HandZ");
	/*
	for (int i = 0; i < 5; ++i)
	{

		core::string pre_name = "Finger_" + core::StringConverter::toString(i+1) + "_";
		for (int j = 0; j < 4; ++j)
		{
			_load(&m_fingers[i][j], pre_name + core::StringConverter::toString(j+1));
		}
	}*/

	return true;
}

void ArmComponent::Update(float dt)
{
	IGameComponent::Update(dt);

}

void ArmComponent::SetForeamAngles(const math::vector3d& angles)
{

	if (m_foream[0])
		m_foream[0]->SetControlValue(VT::ControlInputValues(angles.x, angles.x), VT::EControl_Realtime);
	if (m_foream[1])
		m_foream[1]->SetControlValue(VT::ControlInputValues(angles.y, angles.y), VT::EControl_Realtime);
// 	if (m_foream[2])
// 		m_foream[2]->SetControlValue(VT::ControlInputValues(angles.z, angles.z), VT::EControl_Realtime);
}

void ArmComponent::SetWristAngles(const math::vector3d& angles)
{

	if (m_wrist[0])
		m_wrist[0]->SetControlValue(VT::ControlInputValues(angles.x, angles.x), VT::EControl_Realtime);
	if (m_wrist[1])
		m_wrist[1]->SetControlValue(VT::ControlInputValues(angles.y, angles.y), VT::EControl_Realtime);
	if (m_foream[2])//from wrist
		m_foream[2]->SetControlValue(VT::ControlInputValues(angles.z, angles.z), VT::EControl_Realtime);
}



IMPLEMENT_RTTI(ArmComponent, IGameComponent)
IMPLEMENT_GAMECOMPONENT_FACTORY(ArmComponent);


}
}




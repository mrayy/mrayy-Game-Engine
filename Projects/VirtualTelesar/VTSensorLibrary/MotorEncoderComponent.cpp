
#include "stdafx.h"
#include "MotorEncoderComponent.h"
#include "PhysicalJointDOF6Component.h"

namespace mray
{
namespace VT
{

IMPLEMENT_RTTI(MotorEncoderComponent,IVTComponent)
MotorEncoderComponent::MotorEncoderComponent(game::GameEntityManager*mngr)
:IVTComponent()
{
	m_joint=0;
	m_encoderVal=0;
	m_precision=PropertyTypePrecision::instance.GetDefaultValue();
	CPropertieDictionary* dic;
	if(CreateDictionary(&dic))
	{
		dic->addPropertie(&PropertyTypePrecision::instance);
		dic->addPropertie(&PropertyTypeJointName::instance);
	}
}

MotorEncoderComponent::~MotorEncoderComponent()
{
}


bool MotorEncoderComponent::InitComponent()
{
	if(!IVTComponent::InitComponent())
		return false;
	m_joint=RetriveComponent<game::PhysicalJointDOF6Component>(m_ownerComponent,m_jointName);
	return true;
}

IMPLEMENT_SETGET_PROP(MotorEncoderComponent,JointName,core::string,m_jointName,"",core::StringConverter::toString,core::StringConverter::toString);
IMPLEMENT_SETGET_PROP(MotorEncoderComponent,Precision,float,m_precision,1,core::StringConverter::toFloat,core::StringConverter::toString);


void MotorEncoderComponent::Update(float dt)
{
	IVTComponent::Update(dt);
	if(m_joint)
	{
		m_encoderVal=m_joint->GetCurrentAngle(0);
	}
}

IMPLEMENT_GAMECOMPONENT_FACTORY(MotorEncoderComponent);


}
}


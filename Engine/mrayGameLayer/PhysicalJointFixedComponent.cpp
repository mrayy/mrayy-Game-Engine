
#include "stdafx.h"
#include "PhysicalJointFixedComponent.h"


#include "IPhysicManager.h"
#include "IPhysicalJoint.h"
#include "GameEntityManager.h"
#include "PhysicXMLSerializer.h"
#include "PhysicsComponent.h"



namespace mray
{
namespace game
{


PhysicalJointFixedComponent::PhysicalJointFixedComponent(GameEntityManager*mngr):m_joint(0),m_removeNodeOnDestroy(0)
{
	m_phManager=mngr->GetPhysicsManager();
}
PhysicalJointFixedComponent::PhysicalJointFixedComponent(physics::IPhysicManager* phManager):m_joint(0),m_removeNodeOnDestroy(0)
{
	m_phManager=phManager;
	InitComponent();
}
PhysicalJointFixedComponent::~PhysicalJointFixedComponent()
{
	if(m_removeNodeOnDestroy)
	{
		m_joint->GetCreator()->removeJoint(m_joint);
		//delete m_node;
	}
}

void PhysicalJointFixedComponent::_OnTargetChanged()
{
	if(!m_inited)
		return;

	IPhysicalJointComponent::_OnTargetChanged();

	if(m_targetComp1 && m_targetComp2)
	{
		//make sure to be inited
		if(!m_targetComp1->IsInited())
			m_targetComp1->InitComponent();
		if(!m_targetComp2->IsInited())
			m_targetComp2->InitComponent();
		
		PhysicsComponent* a=dynamic_cast<PhysicsComponent*>(m_targetComp1);
		PhysicsComponent* b=dynamic_cast<PhysicsComponent*>(m_targetComp2);
		if(a && b)
		{
			m_jointDesc.body1=a->GetPhysicalNode();
			m_jointDesc.body2=b->GetPhysicalNode();

			if(m_joint)
				m_joint->GetCreator()->removeJoint(m_joint);
			physics::IPhysicalJointFixedDesc d;
			d.body1=m_jointDesc.body1;
			d.body2=m_jointDesc.body2;

			for (int i=0;i<2;++i)
			{
				d.localAxis[i]=m_jointDesc.localAxis[i];
				d.localNormal[i]=m_jointDesc.localNormal[i];
				d.localAnchor[i]=m_jointDesc.localAnchor[i];
			}
			m_joint=dynamic_cast<physics::IFixedJoint3D*>(m_phManager->createJoint(&m_jointDesc));
		}
	}
}



physics::IPhysicalJoint* PhysicalJointFixedComponent::GetJoint()
{
	return m_joint;
}
void PhysicalJointFixedComponent::SetJoint(physics::IFixedJoint3D* node)
{
	m_joint =node;
}

math::vector3d PhysicalJointFixedComponent::GetCurrentAngles()
{
	if(!m_joint)
		return 0;
	physics::IPhysicalNode* n1= m_joint->getNode1();
	physics::IPhysicalNode* n2= m_joint->getNode2();

	if(!n1 || !n2)
		return 0;

	math::quaternion q1= n1->getGlobalOrintation();
	math::quaternion q2= n2->getGlobalOrintation();
	math::quaternion childFrame=q1.inverse()*q2;
	math::vector3d angles;
	childFrame.toEulerAngles(angles);
	return angles;
}
float PhysicalJointFixedComponent::GetCurrentAngle(int axis)
{
	math::vector3d angles=GetCurrentAngles();
	float v=0;
	switch (axis)
	{
	case 0:
		v=angles.x;
		break;
	case 1:
		v=angles.y;
		break;
	case 2:
		v=angles.z;
		break;
	}
	return v;
}
float PhysicalJointFixedComponent::GetCurrentLinearDistance(int axis)
{
	if(!m_joint)
		return 0;
	physics::IPhysicalNode* n1= m_joint->getNode1();
	physics::IPhysicalNode* n2= m_joint->getNode2();

	if(!n1 || !n2)
		return 0;
	math::quaternion parentQ= n1->getGlobalOrintation();

	math::vector3d localPos=parentQ.inverse()*(n2->getGlobalPosition()-n1->getGlobalPosition());
	switch(axis)
	{
	case 0:
		return localPos.x;
	case 1:
		return localPos.y;
	case 2:
		return localPos.z;
	}
	return 0;
}

void PhysicalJointFixedComponent::SetRotationDrive(const math::vector3d& angles)
{
}
void PhysicalJointFixedComponent::SetTwistDrive(float v)
{
}
void PhysicalJointFixedComponent::SetSwing1Drive(float v)
{
}
void PhysicalJointFixedComponent::SetSwing2Drive(float v)
{
}

void PhysicalJointFixedComponent::SetPositionDrive(const math::vector3d& angles)
{
}
void PhysicalJointFixedComponent::SetXDrive(float v)
{
}
void PhysicalJointFixedComponent::SetYDrive(float v)
{
}
void PhysicalJointFixedComponent::SetZDrive(float v)
{
}



xml::XMLElement*  PhysicalJointFixedComponent::loadXMLSettings(xml::XMLElement* elem)
{
	xml::XMLElement* ret=IGameComponent::loadXMLSettings(elem);
	if(!ret)
		return 0;
	physics::PhysicXMLSerializer::Parse(ret,&m_jointDesc);
	return ret;
}
xml::XMLElement*  PhysicalJointFixedComponent::exportXMLSettings(xml::XMLElement* elem)
{
	xml::XMLElement* ret=IGameComponent::exportXMLSettings(elem);

	if(m_joint)
	{
		m_joint->SaveToDesc(&m_jointDesc);
		physics::PhysicXMLSerializer::Save(ret,&m_jointDesc);

	}

	return ret;

}

}
}
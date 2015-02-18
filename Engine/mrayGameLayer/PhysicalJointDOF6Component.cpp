
#include "stdafx.h"
#include "PhysicalJointDOF6Component.h"
#include "IPhysicManager.h"
#include "IPhysicalJoint.h"
#include "GameEntityManager.h"
#include "PhysicXMLSerializer.h"
#include "PhysicsComponent.h"



namespace mray
{
namespace physics
{
class IPhysicManager;
class IPhysicalJoint;
}
namespace game
{
PhysicalJointDOF6Component::PhysicalJointDOF6Component(GameEntityManager*mngr):m_joint(0),m_removeNodeOnDestroy(0)
{
	m_phManager=mngr->GetPhysicsManager();
}
PhysicalJointDOF6Component::PhysicalJointDOF6Component(physics::IPhysicManager* phManager):m_joint(0),m_removeNodeOnDestroy(0)
{
	m_phManager=phManager;
	InitComponent();
}
PhysicalJointDOF6Component::~PhysicalJointDOF6Component()
{
	if(m_removeNodeOnDestroy)
	{
		m_joint->GetCreator()->removeJoint(m_joint);
		//delete m_node;
	}
}

void PhysicalJointDOF6Component::_OnChangeAngularDrive()
{
	if(!m_joint)
		return;

	math::quaternion q(m_rotationDrive);
	m_joint->setDriveOrientation(q);
}

void PhysicalJointDOF6Component::_OnChangePositionDrive()
{
	if(!m_joint)
		return;
	m_joint->setDrivePosition(m_positionDrive);
}

void PhysicalJointDOF6Component::_OnTargetChanged()
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
			m_joint=dynamic_cast<physics::IDOF6Joint3D*>(m_phManager->createJoint(&m_jointDesc));
		}
	}
}



physics::IPhysicalJoint* PhysicalJointDOF6Component::GetJoint()
{
	return m_joint;
}
void PhysicalJointDOF6Component::SetJoint(physics::IDOF6Joint3D* node)
{
	m_joint =node;
}

math::vector3d PhysicalJointDOF6Component::GetCurrentAngles()
{

	if(!m_joint)
		return 0;
	physics::IPhysicalNode* n1= m_joint->getNode1();
	physics::IPhysicalNode* n2= m_joint->getNode2();

	if(!n1 || !n2)
		return 0;
//	math::quaternion parentQ= n1->getGlobalOrintation();

	math::quaternion q1= m_joint->getInitialNode2Orintation();
	math::quaternion q2= m_joint->getCurrentNode2Orintation();
	math::quaternion childFrame=q1.inverse()*q2;
	math::vector3d angles;
	childFrame.toEulerAngles(angles);
	return angles;
}
float PhysicalJointDOF6Component::GetCurrentAngle(int axis)
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
float PhysicalJointDOF6Component::GetCurrentLinearDistance(int axis)
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
math::vector3d PhysicalJointDOF6Component::TransformToJointSpaceAngles(const math::vector3d& angles)
{

	math::vector3d v;
	if(m_jointDesc.twistMotion==physics::ED6Joint_Motion_Limited)
		v.x=math::clamp(angles.x,m_jointDesc.twistLimit.low.value,m_jointDesc.twistLimit.high.value);
	else if(m_jointDesc.twistMotion==physics::ED6Joint_Motion_Locked)
		v.x=0;
	else
		v.x=angles.x;

	if(m_jointDesc.swing1Motion==physics::ED6Joint_Motion_Limited)
		v.y=math::clamp(angles.y,-m_jointDesc.swing1Limit.value,m_jointDesc.swing1Limit.value);
	else if(m_jointDesc.swing1Motion==physics::ED6Joint_Motion_Locked)
		v.y=0;
	else
		v.y=angles.y;

	if(m_jointDesc.swing2Motion==physics::ED6Joint_Motion_Limited)
		v.z=math::clamp(angles.z,-m_jointDesc.swing2Limit.value,m_jointDesc.swing2Limit.value);
	else if(m_jointDesc.swing2Motion==physics::ED6Joint_Motion_Locked)
		v.z=0;
	else
		v.z=angles.z;

	return v;
}


void PhysicalJointDOF6Component::SetRotationDrive(const math::vector3d& angles)
{
	math::vector3d v=TransformToJointSpaceAngles(angles);
	if(v!=m_rotationDrive)
	{
		m_rotationDrive=v;
		_OnChangeAngularDrive();
	}

}

void PhysicalJointDOF6Component::SetTwistDrive(float v)
{
	if(m_jointDesc.twistMotion==physics::ED6Joint_Motion_Limited)
		m_rotationDrive.x=math::clamp(v,m_jointDesc.twistLimit.low.value,m_jointDesc.twistLimit.high.value);
	else
		m_rotationDrive.x=v;

	_OnChangeAngularDrive();
}
void PhysicalJointDOF6Component::SetSwing1Drive(float v)
{
	if(m_jointDesc.swing1Motion==physics::ED6Joint_Motion_Limited)
		m_rotationDrive.y=math::clamp(v,-m_jointDesc.swing1Limit.value,m_jointDesc.swing1Limit.value);
	else
		m_rotationDrive.y=v;
	_OnChangeAngularDrive();
}
void PhysicalJointDOF6Component::SetSwing2Drive(float v)
{
	if(m_jointDesc.swing2Motion==physics::ED6Joint_Motion_Limited)
		m_rotationDrive.z=math::clamp(v,-m_jointDesc.swing2Limit.value,m_jointDesc.swing2Limit.value);
	else
		m_rotationDrive.z=v;
	_OnChangeAngularDrive();

}

void PhysicalJointDOF6Component::SetPositionDrive(const math::vector3d& v)
{
	if(m_jointDesc.xMotion==physics::ED6Joint_Motion_Limited)
		m_positionDrive.x=math::clamp(v.x,-m_jointDesc.linearLimit.value,m_jointDesc.linearLimit.value);
	else
		m_positionDrive.x=v.x;

	if(m_jointDesc.yMotion==physics::ED6Joint_Motion_Limited)
		m_positionDrive.y=math::clamp(v.y,-m_jointDesc.linearLimit.value,m_jointDesc.linearLimit.value);
	else
		m_positionDrive.y=v.y;

	if(m_jointDesc.zMotion==physics::ED6Joint_Motion_Limited)
		m_positionDrive.z=math::clamp(v.z,-m_jointDesc.linearLimit.value,m_jointDesc.linearLimit.value);
	else
		m_positionDrive.z=v.z;

	_OnChangePositionDrive();

}
void PhysicalJointDOF6Component::SetXDrive(float v)
{
	if(m_jointDesc.xMotion==physics::ED6Joint_Motion_Limited)
		m_positionDrive.x=math::clamp(v,-m_jointDesc.linearLimit.value,m_jointDesc.linearLimit.value);
	else
		m_positionDrive.x=v;

	_OnChangePositionDrive();

}
void PhysicalJointDOF6Component::SetYDrive(float v)
{

	if(m_jointDesc.xMotion==physics::ED6Joint_Motion_Limited)
		m_positionDrive.y=math::clamp(v,-m_jointDesc.linearLimit.value,m_jointDesc.linearLimit.value);
	else
		m_positionDrive.y=v;

	_OnChangePositionDrive();
}
void PhysicalJointDOF6Component::SetZDrive(float v)
{
	if(m_jointDesc.zMotion==physics::ED6Joint_Motion_Limited)
		m_positionDrive.z=math::clamp(v,-m_jointDesc.linearLimit.value,m_jointDesc.linearLimit.value);
	else
		m_positionDrive.z=v;

	_OnChangePositionDrive();

}
xml::XMLElement*  PhysicalJointDOF6Component::loadXMLSettings(xml::XMLElement* elem)
{
	xml::XMLElement* ret=IGameComponent::loadXMLSettings(elem);
	if(!ret)
		return 0;
	physics::PhysicXMLSerializer::Parse(ret,&m_jointDesc);
	m_jointDesc.twistDrive.spring*=20;
 	m_jointDesc.twistDrive.damping*=10;
  	m_jointDesc.twistLimit.high.damping*=10;
  	m_jointDesc.twistLimit.low.damping*=10;
 	m_jointDesc.swing1Limit.damping=10;
 	m_jointDesc.swingDrive.damping=10;
	m_jointDesc.projectionMode=physics::EJPM_LinearMindist;
	m_jointDesc.projectionDistance=0.001;
	m_jointDesc.projectionAngle=0.1;
	return ret;
}	
xml::XMLElement*  PhysicalJointDOF6Component::exportXMLSettings(xml::XMLElement* elem)
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
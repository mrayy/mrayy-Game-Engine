

#include "stdafx.h"
#include "KinematicJointDOF6Component.h"

#include "PhysicsComponent.h"
#include "NullPhysicsComponent.h"
#include "PhysicXMLSerializer.h"
#include "HierarchyComponent.h"
#include "GameEntity.h"
#include "ISceneNode.h"

namespace mray
{
namespace game
{
KinematicJointDOF6Component::KinematicJointDOF6Component(GameEntityManager*mngr)
{
	m_parent=m_child=0;
}
KinematicJointDOF6Component::~KinematicJointDOF6Component()
{
}


void KinematicJointDOF6Component::_OnChangeAngularDrive()
{
	if(m_child)
		m_child->SetRotation(m_mat.transformNormal(m_rotationDrive));
}
void KinematicJointDOF6Component::_OnChangePositionDrive()
{
}

void KinematicJointDOF6Component::_OnTargetChanged()
{
	if(!m_inited)
		return;

	IPhysicalJointComponent::_OnTargetChanged();

	HierarchyComponent* hComp=(HierarchyComponent*) GetOwnerEntity()->GetFirstComponent(HierarchyComponent::getClassRTTI());
	if(!hComp)
	{
		hComp=new HierarchyComponent(m_ownerEntity->GetCreator());
		hComp->SetInited();
		GetOwnerEntity()->AddComponent(hComp);
	}

	if(m_targetComp1)
	{
		//make sure to be inited
		if(!m_targetComp1->IsInited())
			m_targetComp1->InitComponent();
	}
	if(m_targetComp2)
	{
		if(!m_targetComp2->IsInited())
			m_targetComp2->InitComponent();

	}
	if(m_targetComp1 && m_targetComp2)
	{
		m_parent=hComp->InsertPair(this);
		m_child=(m_parent==m_targetComp1)?m_targetComp2:m_targetComp1;
	}

	if(m_targetComp1 )
	{
		PhysicsComponent* a=dynamic_cast<PhysicsComponent*>(m_targetComp1);
		if(a)
		{
			physics::IPhysicalNode* node= a->GetPhysicalNode();
			if(node)
			{
				node->SetBodyFlag(physics::EBF_Kinematic,true);
			}
		}
		
	}
	if(m_targetComp2 )
	{
		PhysicsComponent* a=dynamic_cast<PhysicsComponent*>(m_targetComp2);
		if(a)
		{
			physics::IPhysicalNode* node= a->GetPhysicalNode();
			if(node)
			{
				node->SetBodyFlag(physics::EBF_Kinematic,true);
			}
		}
	}
	{
		NullPhysicsComponent* n1=dynamic_cast<NullPhysicsComponent*>(m_targetComp1);
		NullPhysicsComponent* n2=dynamic_cast<NullPhysicsComponent*>(m_targetComp2);
		if(n1 && n2)
		{
			if(m_parent==n1)
			{
				n1->GetRootNode()->addChild(n2->GetRootNode(),true);
				//n2->GetRootNode()->setPosition(m_jointDesc.localAnchor[1]);
				n2->GetRootNode()->updateAbsoluteTransformation();
			}else if(m_parent==n2)
			{
				n2->GetRootNode()->addChild(n1->GetRootNode(),true);
				n1->GetRootNode()->updateAbsoluteTransformation();
			}
		}
	}
}
bool KinematicJointDOF6Component::InitComponent()
{
	if(!IPhysical6DOFJointComponent::InitComponent())
		return false;


/*	m_desc->localAxis[i].Normalize();
	desc->localNormal[i].Normalize();
	*/
	//math::matrix4x4 mat[2];

	//for(int i=0;i<2;++i)
	int i=0;
	{
		//calculate parent matrix
		math::vector3d tang=m_jointDesc.localAxis[i].crossProduct(m_jointDesc.localNormal[i]);
		m_mat.setComponent(m_jointDesc.localAxis[i],m_jointDesc.localNormal[i],tang);
		//	mat[i]=mat[i].getTransposed();
		m_mat.setTranslation(m_jointDesc.localAnchor[i]);

	}
	NullPhysicsComponent* n1=dynamic_cast<NullPhysicsComponent*>(m_child);
	if(n1)
	{
		n1->GetRootNode()->setPosition(m_mat.getTranslation());
	}
	return true;
}

math::vector3d KinematicJointDOF6Component::GetCurrentAngles()
{

	return m_child->GetRotation();
}

float KinematicJointDOF6Component::GetCurrentAngle(int axis)
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
float KinematicJointDOF6Component::GetCurrentLinearDistance(int axis)
{

	PhysicsComponent* a=dynamic_cast<PhysicsComponent*>(m_targetComp1);
	PhysicsComponent* b=dynamic_cast<PhysicsComponent*>(m_targetComp2);
	if(a && b)
	{
		physics::IPhysicalNode* n1= a->GetPhysicalNode();
		physics::IPhysicalNode* n2= b->GetPhysicalNode();

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
	}
	return 0;

}

math::vector3d KinematicJointDOF6Component::TransformToJointSpaceAngles(const math::vector3d& angles)
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


void KinematicJointDOF6Component::SetRotationDrive(const math::vector3d& angles)
{
	math::vector3d v=TransformToJointSpaceAngles(angles);

	if(m_rotationDrive!=v)
	{
		m_rotationDrive=v;
		_OnChangeAngularDrive();
	}

}

void KinematicJointDOF6Component::SetTwistDrive(float v)
{
	float a;
	if(m_jointDesc.twistMotion==physics::ED6Joint_Motion_Limited)
		a=math::clamp(v,m_jointDesc.twistLimit.low.value,m_jointDesc.twistLimit.high.value);
	else
		a=v;
	if(m_rotationDrive.x==a)
		return;
	m_rotationDrive.x=a;
	_OnChangeAngularDrive();
}
void KinematicJointDOF6Component::SetSwing1Drive(float v)
{
	float a;
	if(m_jointDesc.swing1Motion==physics::ED6Joint_Motion_Limited)
		a=math::clamp(v,-m_jointDesc.swing1Limit.value,m_jointDesc.swing1Limit.value);
	else
		a=v;
	if(m_rotationDrive.y==a)
		return;
	m_rotationDrive.y=a;
	_OnChangeAngularDrive();
}
void KinematicJointDOF6Component::SetSwing2Drive(float v)
{
	float a;
	if(m_jointDesc.swing2Motion==physics::ED6Joint_Motion_Limited)
		a=math::clamp(v,-m_jointDesc.swing2Limit.value,m_jointDesc.swing2Limit.value);
	else
		a=v;
	if(m_rotationDrive.z==a)
		return;
	m_rotationDrive.z=a;
	_OnChangeAngularDrive();

}

void KinematicJointDOF6Component::SetPositionDrive(const math::vector3d& v)
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
void KinematicJointDOF6Component::SetXDrive(float v)
{
	if(m_jointDesc.xMotion==physics::ED6Joint_Motion_Limited)
		m_positionDrive.x=math::clamp(v,-m_jointDesc.linearLimit.value,m_jointDesc.linearLimit.value);
	else
		m_positionDrive.x=v;

	_OnChangePositionDrive();

}
void KinematicJointDOF6Component::SetYDrive(float v)
{

	if(m_jointDesc.xMotion==physics::ED6Joint_Motion_Limited)
		m_positionDrive.y=math::clamp(v,-m_jointDesc.linearLimit.value,m_jointDesc.linearLimit.value);
	else
		m_positionDrive.y=v;

	_OnChangePositionDrive();
}
void KinematicJointDOF6Component::SetZDrive(float v)
{
	if(m_jointDesc.zMotion==physics::ED6Joint_Motion_Limited)
		m_positionDrive.z=math::clamp(v,-m_jointDesc.linearLimit.value,m_jointDesc.linearLimit.value);
	else
		m_positionDrive.z=v;

	_OnChangePositionDrive();

}

void KinematicJointDOF6Component::Update(float dt)
{
}
xml::XMLElement* KinematicJointDOF6Component::loadXMLSettings(xml::XMLElement* elem)
{
	xml::XMLElement* ret=IGameComponent::loadXMLSettings(elem);
	if(!ret)
		return 0;
	physics::PhysicXMLSerializer::Parse(ret,&m_jointDesc);
	return ret;

}

}
}


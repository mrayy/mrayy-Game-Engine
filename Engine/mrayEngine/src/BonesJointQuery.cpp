#include "stdafx.h"
 
#include "BonesJointQuery.h"
#include "BoneNode.h"


namespace mray{
namespace animation{

BonesJointQuery::BonesJointQuery()
{
	m_dofs=0;
	m_endEffector=0;
}
BonesJointQuery::~BonesJointQuery()
{
}

IJointQuery::JointDOF* BonesJointQuery::addBone(scene::IMovable*bone, EIKJointType type, const math::vector3d& axis)
{
	if(!bone)
		return 0;

	JointDOF j;
	j.node = bone;
	j.type = type;
	j.jointAxis = axis;
		
	m_bones.push_back(j);
	m_values.push_back(0);
	m_jointsMap[bone].push_back(m_values.size() - 1);

	m_dofs++;
	return &m_bones[m_bones.size() - 1];
}
void BonesJointQuery::SetEndEffector(scene::IMovable*bone)
{
	m_endEffector = bone;
}

void BonesJointQuery::clearBones()
{
	m_bones.clear();
	m_values.clear();
	m_endEffector=0;
	m_dofs=0;
}
void BonesJointQuery::OnIKBegin()
{
}

void BonesJointQuery::OnIKEnd()
{
}

std::vector<IJointQuery::JointDOF>&  BonesJointQuery::getJointsDOF()
{
	
	return m_bones;
	/*
	dof.resize(m_dofs);
	math::vector3d ef=getEndEffectorPos();
	
	for(int i=0;i<m_bones.size();++i)
	{
		//scene::IMovable* bone = m_bones[i].bone;
		//tmp.pivot=bone->getAbsolutePosition();
// 		dof[i].node=bone;
// 		dof[i].isRotation=true;
	}*/
	/*
	for(int i=0,j=0;j<m_bones.size();i+=3,++j)
	{
		scene::BoneNode* bone=m_bones[j];
		tmp.pivot=bone->getAbsolutePosition();
		const math::matrix4x4&m=bone->getAbsoluteTransformation();
		//3 rotation Axis
		tmp.axis.x=m(0,0);
		tmp.axis.y=m(1,0);
		tmp.axis.z=m(2,0);
		dof[i]=tmp;
		tmp.axis.x=m(0,1);
		tmp.axis.y=m(1,1);
		tmp.axis.z=m(2,1);
		dof[i+1]=(tmp);
		tmp.axis.x=m(0,2);
		tmp.axis.y=m(1,2);
		tmp.axis.z=m(2,2);
		dof[i+2]=(tmp);
	}*/
}

std::vector<float>& BonesJointQuery::getJointVector()
{
	return m_values;
}

int BonesJointQuery::getJointsDOFCount()
{
	return m_dofs;
}

math::vector3d BonesJointQuery::getEndEffectorPos()
{
	FATAL_ERROR(!m_endEffector,mT("End-Effector not set"));
	return m_endEffector->getAbsolutePosition();
}
math::quaternion BonesJointQuery::getEndEffectorOri()
{
	FATAL_ERROR(!m_endEffector,mT("End-Effector not set"));
	return m_endEffector->getAbsoluteOrintation();
}

math::quaternion Align(const math::quaternion &q, const math::vector3d& v1, const math::vector3d& v2)
{
	math::vector3d bisec = v1 + v2;
	bisec.Normalize();
	float cosHalfAng = v1.dotProduct(bisec);
	math::vector3d cross;
	math::quaternion ret;
	ret.w = cosHalfAng;
	if (cosHalfAng != 0)
	{
		cross = v1.crossProduct(bisec);
		ret.x = cross.x;
		ret.y = cross.y;
		ret.z = cross.z;
	}
	else
	{
		float invLen;
		if (fabs(v1.x) > fabs(v2.x))
		{
			invLen = sqrt(v1.x*v1.x + v1.z*v1.z);
			if (invLen > 0)invLen = 1.0f / invLen;
			ret.x = -v1.z*invLen;
			ret.y = 0;
			ret.z = v1.x*invLen;
		}else
		{
			invLen = sqrt(v1.x*v1.x + v1.z*v1.z);
			if (invLen > 0)invLen = 1.0f / invLen;
			ret.x = 0;
			ret.y = v1.z*invLen;
			ret.z = v1.y*invLen;
		}
	}
	return ret;
}
void DecomposeQuatSwingTwist(const math::quaternion &q, const math::vector3d& axis, math::quaternion& swing, math::quaternion& twist)
{
	math::vector3d v2 = q*axis;
	swing = Align(q, axis, v2);
	twist = q*swing.inverse();
}

void BonesJointQuery::applyChange(const std::vector<float> &change)
{
	for(int i=0;i<change.size();++i)
	{
		m_values[i]+=change[i];
	}
	JointMap::iterator it = m_jointsMap.begin();
	math::quaternion angles;
	math::vector3d translate;
	math::quaternion swing, twist;
	float a;
	math::vector3d axis;
	math::vector3d baxis;
	for (; it != m_jointsMap.end(); ++it)
	{
		std::vector<int>& lst=it->second;
		angles = math::quaternion::Identity;
		translate = 0;
		for (int i=0;i<lst.size();++i)
		{
			int idx = lst[i];
		//	float v = m_bones[idx].limitValue(change[idx]);

			if (m_bones[idx].type == EIKJointType::Linear)
			{
				translate += m_bones[idx].GetAxis()*change[idx];
			}else 
			{
				angles *= math::quaternion(change[idx], m_bones[idx].GetAxis());
		
			}
		}
		it->first->translate(translate, scene::TS_Parent);

		it->first->rotate(angles, scene::TS_Parent);
		it->first->updateAbsoluteTransformation();
#if 0
		angles = math::quaternion::Identity;
		math::quaternion bangles = it->first->getOrintation();
		math::vector3d euler;
		bangles.toEulerAngles(euler);
		//apply limits
		for (int i = 0; i < lst.size(); ++i)
		{
			int idx = lst[i];
			if (m_bones[idx].type == EIKJointType::Prismatic)
			{
				switch (m_bones[idx].jointAxis)
				{
				case EIKJointAxis::X:
					euler.x = m_bones[idx].limitValue(euler.x);
					break;
				case EIKJointAxis::Y:
					euler.y = m_bones[idx].limitValue(euler.y);
					break;
				case EIKJointAxis::Z:
					euler.z = m_bones[idx].limitValue(euler.z);
					break;
				}
				
			}
		}
		it->first->setOrintation(euler);
#endif
	}
	m_endEffector->updateAbsoluteTransformation();
#if 0
	for(int i=0;i<m_bones.size();++i)
	{
		scene::IMovable* bone = m_bones[i].node;

		//math::quaternion q = math::quaternion(change[i].x, change[i].y, change[i].z);
		bone->rotate(change[i], scene::TS_Parent);/*
		math::vector3d angles;
		bone->getOrintation().toEulerAngles(angles);
		m_bones[i].limitAngles(angles);
		bone->setOrintation(math::quaternion(angles.x,angles.y,angles.z));*/
		//	bone->setOrintation(q);
		bone->updateAbsoluteTransformation();
	}

	m_endEffector->updateAbsoluteTransformation();
	/*
	for(int i=0,j=0;j<m_bones.size();i+=3,++j)
	{
		scene::BoneNode* bone=m_bones[j];
		bone->rotate(math::quaternion(change[i+0],dof[i+0].axis),scene::TS_World);
		bone->rotate(math::quaternion(change[i+1],dof[i+1].axis),scene::TS_World);
		bone->rotate(math::quaternion(change[i+2],dof[i+2].axis),scene::TS_World);

		bone->updateAbsoluteTransformation();
	}*/
#endif
}

}
}
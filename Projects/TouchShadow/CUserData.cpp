

#include "stdafx.h"
#include "CUserData.h"
#include "AppCommon.h"
#include "UserDebugData.h"

namespace mray
{

CUserData::CUserData(const nite::UserData& state):m_userData(state)
{
	m_debugString=new UserDebugData();
}

CUserData::~CUserData()
{
	delete m_debugString;
}


nite::UserId CUserData::GetID()const
{	
	return m_userData.getId();
}
bool CUserData::IsLost()const
{
	return m_userData.isLost();
}
bool CUserData::IsVisible()const
{
	return m_userData.isVisible();
}

math::vector3d CUserData::GetCenterOfMass()const
{
	return ToVectro3d(m_userData.getCenterOfMass());
}
const nite::BoundingBox& CUserData::GetBoundingBox()const
{
	return m_userData.getBoundingBox();
}

void CUserData::UpdateState(const nite::UserData& state)
{
	m_userData=state;
	m_transformedSkeleton=state.getSkeleton();
}

void CUserData::TransformByMatrix(const math::matrix4x4& m)
{
	for(int i=0;i<NITE_JOINT_COUNT;++i)
	{
		NiteSkeletonJoint* j= &((NiteSkeleton*)&m_transformedSkeleton)->joints[i];
		math::vector3d v=m*ToVectro3d(j->position);
		j->position.x=v.x;
		j->position.y=v.y;
		j->position.z=v.z;
	}
}


}



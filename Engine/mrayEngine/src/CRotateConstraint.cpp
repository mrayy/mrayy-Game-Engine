#include "stdafx.h"

#include "CRotateConstraint.h"
#include "ISceneNode.h"


namespace mray{
using namespace math;
namespace scene{

CRotateConstraint::CRotateConstraint(math::vector3d min,math::vector3d max,Point3d<bool> emin,Point3d<bool> emax)
{
	m_Min=min;
	m_Max=max;
	m_eMin=emin;
	m_eMax=emax;
	if(m_Min.x>m_Max.x && m_eMin.x && m_eMax.x)Swap<float>(m_Min.x,m_Max.x);
	if(m_Min.y>m_Max.y && m_eMin.y && m_eMax.y)Swap<float>(m_Min.y,m_Max.y);
	if(m_Min.z>m_Max.z && m_eMin.z && m_eMax.z)Swap<float>(m_Min.z,m_Max.z);
}

void CRotateConstraint::animate(scene::IMovable *node,float dt)
{
	if(!node)return;
	math::vector3d finalRot;
	bool needUpdate=false;
	math::vector3d angle=0;
	if(m_eMin.x || m_eMax.x){
		angle.x=node->getOrintation().getPitch();
	}
	if(m_eMin.y || m_eMax.y){
		angle.y=node->getOrintation().getYaw();
	}
	if(m_eMin.z || m_eMax.z){
		angle.z=node->getOrintation().getRoll();
	}

	if(m_eMin.x){
		finalRot.x=math::Max<float>(angle.x,m_Min.x);
		if(angle.x!=finalRot.x)
			needUpdate=true;
	}
	if(m_eMin.y){
		finalRot.y=math::Max<float>(angle.y,m_Min.y);
		if(angle.y!=finalRot.y)
			needUpdate=true;
	}
	if(m_eMin.z){
		finalRot.z=math::Max<float>(angle.z,m_Min.z);
		if(angle.z!=finalRot.z)
			needUpdate=true;
	}
	if(m_eMax.x){
		finalRot.x=math::Max<float>(angle.x,m_Max.x);
		if(angle.x!=finalRot.x)
			needUpdate=true;
	}
	if(m_eMax.y){
		finalRot.y=math::Max<float>(angle.y,m_Max.y);
		if(angle.y!=finalRot.y)
			needUpdate=true;
	}
	if(m_eMax.z){
		finalRot.z=math::Max<float>(angle.z,m_Max.z);
		if(angle.z!=finalRot.z)
			needUpdate=true;
	}

	if(needUpdate){
		node->setOrintation(math::quaternion(finalRot));
	}
	
}

}
}




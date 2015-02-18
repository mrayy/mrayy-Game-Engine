


#include "stdafx.h"
#include "ErrorCalculatorComponent.h"
#include "CoupledJointComponent.h"
#include "PhysicalJointDOF6Component.h"




namespace mray
{
namespace VT
{


ErrorCalculatorComponent::ErrorCalculatorComponent(game::GameEntityManager* mngr)
{
}
ErrorCalculatorComponent::~ErrorCalculatorComponent()
{
}

bool ErrorCalculatorComponent::InitComponent()
{
	if(!IVTComponent::InitComponent())
		return false;
	if(!m_ownerComponent)
		return false;
	const std::list<IObjectComponent*>& lst= m_ownerComponent->GetComponentsList();
	std::list<IObjectComponent*>::const_iterator it=lst.begin();
	for(;it!=lst.end();++it)
	{
		CoupledJointComponent* c=dynamic_cast<CoupledJointComponent*>(*it);
		if(c)
		{
			m_joints.push_back(c);
			const std::vector<CoupledJointInfo*>& lst2=c->GetAttachedJoints();
			for(int i=0;i<lst2.size();++i)
			{
				ContorlComponentErrorInfo ifo;
				ifo.comp=(CoupledJointInfo*)lst2[i];
				m_errorValues.push_back(ifo);
			}
		}
	}

	return true;
}

void ErrorCalculatorComponent::Update(float dt)
{
	IVTComponent::Update(dt);
	float totalError=0;
	for(int i=0;i<m_errorValues.size();++i)
	{
		game::IPhysicalJointComponent* joint=m_errorValues[i].comp->GetJoint();
		if(!joint)
			continue;

		float v=m_errorValues[i].comp->GetValue().KinValue;
		math::vector3d angles=joint->GetCurrentAngles();
		float error=abs(angles.x)+abs(angles.y)+abs(angles.z)-v;
		/*
		std::vector<float> contVals= it->second->GetControlValue();
		std::vector<float> currentVals= it->second->GetCurrentValue();
		m_errorValues[i].comp=it->second;
		m_errorValues[i].errorValues.resize(contVals.size());
		for(int j=0;j<contVals.size();++j)
		{
			float e=contVals[j]-currentVals[j];
			m_errorValues[i].errorValues[j]=e;
			totalError+=e*e;
		}
*/
		totalError+=error;
	}
	totalError=sqrt(totalError);

}

}
}

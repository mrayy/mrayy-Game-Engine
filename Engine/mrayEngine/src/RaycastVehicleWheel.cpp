
#include "stdafx.h"
#include "RaycastVehicleWheel.h"

#include "PhysicalShapes.h"
#include "IPhysicalWheelShape.h"
#include "IPhysicalNode.h"
#include "IPhysicManager.h"

namespace mray
{
namespace physics
{

RaycastVehicleWheel::RaycastVehicleWheel(IPhysicalNode* node,const VehicleWheelDesc&desc,float scale)
{
	WheelShapeDesc wheelShapeDesc;
	math::quaternion q;
	//q.fromAngle(90,math::vector3d::YAxis);
	//q.toMatrix(wheelShapeDesc.localSpace);
	wheelShapeDesc.localSpace.setTranslation(desc.position);

	SetName(desc.name);

	m_flags=desc.WheelFlags;

	static IPhysicMaterial* mat=0;
	if(!mat)
	{
		PhysicMaterialDesc mdesc;
		mdesc.dynamicFriction=0.7;
		mdesc.staticFriction=0.7;
		//mdesc.flags[EMF_DisableFriction]=true;
		mat=node->GetCreator()->createMaterial(&mdesc);
	}

	wheelShapeDesc.material=mat;

	wheelShapeDesc.radius=desc.radius*scale;
	float heightModifier=(desc.suspension+wheelShapeDesc.radius)/desc.suspension;
	wheelShapeDesc.suspension.spring=desc.springRestitution*heightModifier;
	wheelShapeDesc.suspension.damper=desc.springDamping*heightModifier;
	wheelShapeDesc.suspension.targetValue=desc.springBias*heightModifier;

	wheelShapeDesc.inverseWheelMass=1.0f/desc.wheelMass;
	wheelShapeDesc.suspensionTravel=desc.suspension;

	wheelShapeDesc.lateralTireForceFunction.stiffnessFactor*=desc.frictionToSide;
	wheelShapeDesc.longitudalTireForceFunction.stiffnessFactor*=desc.frictionToFront;

	m_wheel= dynamic_cast<IPhysicalWheelShape*>(node->CreateShape(&wheelShapeDesc));
}
RaycastVehicleWheel::~RaycastVehicleWheel()
{
}


IPhysicalNode* RaycastVehicleWheel::GetTouchNode()const
{
	WheelContactData contact;
	IPhysicalShape* s= m_wheel->GetContact(contact);
	return s?s->getPhysicalNode():0;
}
void RaycastVehicleWheel::SetWheelPos(const math::vector3d& pos)
{
	m_wheel->GetShape()->setLocalTranslation(pos);
}

math::vector3d RaycastVehicleWheel::GetWheelPos()const
{
	return m_wheel->GetShape()-> getLocalTranslation();
}
math::quaternion RaycastVehicleWheel::GetLocalOrintation()const
{
	return m_wheel->GetShape()->getLocalOrintation();
}
math::quaternion RaycastVehicleWheel::GetGlobalOrintation()const
{
	return m_wheel->GetShape()->getGlobalOrintation();
}
float RaycastVehicleWheel::GetSteeringAngle()const
{
	return m_wheel->GetSteerAngle();
}


void RaycastVehicleWheel::SetAngle(float a)
{
	m_wheel->SetSteerAngle(-a);
}
float RaycastVehicleWheel::GetRPM()const
{
	float rpm=(m_wheel->GetAxleSpeed()*60.0f)/(math::TwoPI32);
	//printf("wrpm=%2.3f ",rpm);
	return fabsf(rpm);
}
math::vector3d RaycastVehicleWheel::GetGroundContactPos()
{
	return GetWheelPos()+math::vector3d(0,-GetRadius(),0);
}
float RaycastVehicleWheel::GetRadius()
{
	return m_wheel->GetRadius(); 
}

void RaycastVehicleWheel::Update(bool handBrake,float motorTorque,float brakeTorque,float dt)
{
	if(IsFlagUp(EVW_AffectedByHandbrake) && handBrake)
	{
		brakeTorque=1000;
	}
	if(IsFlagUp(EVW_Accelerated))
	{
		m_wheel->SetMotorTorque(motorTorque);
	}
	m_wheel->SetBrakeTorque(100*brakeTorque);
}


}
}



#include "stdafx.h"
#include "PhysicalVehicle.h"


#include "IVehicleWheel.h"
#include "VehicleMotor.h"
#include "VehicleGear.h"

#include "IPhysicalNode.h"
#include "IPhysicMaterial.h"
#include "VehicleManager.h"

#include "IPhysicManager.h"
#include "IPhysicMaterial.h"

#include "VehicleDesc.h"

namespace mray
{
namespace physics
{

PhysicalVehicle::PhysicalVehicle(VehicleManager*creator)
:m_creator(creator),m_braking(false),m_releaseBraking(false),m_maxVelocity(0),m_userData(0),
m_motor(0),m_gear(0),m_bodyNode(0),m_steeringWheelState(0),m_accelerationPedal(0),m_brakePedal(0),
m_brakePedalChanged(0),m_handBrake(0),m_acceleration(0),m_steeringMaxAngle(0),m_motorForce(0),
m_transmissionEfficiency(0),m_differentialRatio(0),m_mostTouchedNode(0)
{
}
bool PhysicalVehicle::LoadFromDesc(const VehicleDesc& desc,IPhysicalNode* node,float scale)
{
	m_userData=desc.userData;
/*
	{
		PhysicMaterialDesc matDesc;
		matDesc.dynamicFriction=0.4;
		matDesc.staticFriction=0.4;
		matDesc.restitution=0;
		matDesc.friction_combineMode=ECM_Multiply;
		m_material= m_creator->GetPhysicsManager()->createMaterial(&matDesc);
	}

	PhysicalNodeDesc nodeDesc;
	PhysicalBodyDesc bodyDesc;
	for(int i=0;i<desc.shapes.size();++i)
	{
		nodeDesc.shapes.push_back(desc.shapes[i]);
		if(nodeDesc.shapes[i]->material==0)
		{
			nodeDesc.shapes[i]->material=m_material;
		}
	}

	bodyDesc.mass=desc.mass;
	bodyDesc.sleepEnergyThreshold=0.05f;
	nodeDesc.bodyDesc=&bodyDesc;
	nodeDesc.globalPos.setTranslation(desc.position);

	m_bodyNode=m_creator->GetPhysicsManager()->createNode(&nodeDesc);
*/
	m_type=desc.vehicleTypeName;

	m_bodyNode=node;
	if(!m_bodyNode)
		return false;

	m_bodyNode->setMass(desc.mass);
	m_bodyNode->setSleepEnergyThreshold(0.05);

	m_bodyNode->SetUserData(this);

	if(desc.motorDesc)
	{
		m_motor=new VehicleMotor(*desc.motorDesc);
		m_motorForce=0;
	}else
	{
		m_motor=0;
		m_motorForce=desc.motorForce;
	}

	if(desc.gearDesc)
	{
		m_gear=new VehicleGear(*desc.gearDesc);
	}else
	{
		m_gear=0;
	}

	int indexCounter=0;
	for(int i=0;i<desc.wheels.size();++i)
	{
		IVehicleWheel* wheel=m_creator->CreateWheel(m_bodyNode,*desc.wheels[i],scale);
		if(!wheel)
			return false;
		m_wheels.push_back(wheel);

		core::string wheelName=wheel->GetName();
		WheelsMap::iterator it= m_wheelsMap.find(wheelName);
		while (it!=m_wheelsMap.end()) 
		{
			wheelName=wheel->GetName()+core::StringConverter::toString(++indexCounter);
			it= m_wheelsMap.find(wheelName);
		} 
		wheel->SetName(wheelName);
		m_wheelsMap[wheelName]=m_wheels.size()-1;
	}

	m_digitalSteeringDelta=desc.digitalSteeringDelta;
	m_steeringSteerPoint=desc.steeringSteerPoint;
	m_steeringTurnPoint=desc.steeringTurnPoint;
	m_steeringMaxAngle=math::toRad(desc.steeringMaxAngle);
	m_transmissionEfficiency=desc.transmissionEfficiency;
	m_differentialRatio=desc.differentialRatio;
	m_maxVelocity=desc.maxVelocity;
	m_bodyNode->SetCMassOffsetLocalPosition(desc.centerOfMass);

	Control(0,true,0,true,false);

	return true;
}
PhysicalVehicle::~PhysicalVehicle()
{
	delete m_gear;
	delete m_motor;
	for(int i=0;i<m_wheels.size();++i)
	{
		delete m_wheels[i];
	}
}
void PhysicalVehicle::_ControlSteering(float steering,bool analogSteering)
{
	if(analogSteering)
	{
		m_steeringWheelState=steering;
	}else if(fabsf(steering)>0.0001f)
	{
		m_steeringWheelState+=math::sign(steering)*m_digitalSteeringDelta;
	}else if(fabsf(m_steeringWheelState)>0.0001f)
	{
		m_steeringWheelState-=math::sign(m_steeringWheelState)*m_digitalSteeringDelta;
	}
	m_steeringWheelState=math::clamp(m_steeringWheelState,-1.0f,1.0f);
}

void PhysicalVehicle::_ControlAcceleration(float acceleration,bool analogAcc)
{
	if(fabsf(acceleration)<0.001f)
	{
		m_releaseBraking=true;
	}
	if(!m_braking)
	{
		m_accelerationPedal=math::clamp(acceleration,-1.0f,1.0f);
		m_brakePedalChanged=(m_brakePedal == 0);
		m_brakePedal=0;
	}else
	{
		m_accelerationPedal=0;
		float newV=math::Min(fabsf(acceleration),1.0f);
		m_brakePedalChanged=(m_brakePedal==newV);
		m_brakePedal=newV;
	}
}

void PhysicalVehicle::_ComputeMostTouchedNode()
{
	std::map<IPhysicalNode*,uchar> actorCounter;
	std::map<IPhysicalNode*,uchar>::iterator it;
	for(int i=0;i<m_wheels.size();++i)
	{
		IPhysicalNode* node= m_wheels[i]->GetTouchNode();
		if(!node)
			continue;
		it=actorCounter.find(node);
		if(it==actorCounter.end())
			actorCounter[node]=1;
		else 
			it->second++;
	}
	uchar bestCnt=0;
	m_mostTouchedNode=0;
	for(it=actorCounter.begin();it!=actorCounter.end();++it)
	{
		if(it->second>bestCnt)
		{
			bestCnt=it->second;
			m_mostTouchedNode=it->first;
		}
	}
}

void PhysicalVehicle::_ComputeLocalVelocity()
{
	_ComputeMostTouchedNode();
	math::vector3d relVel;
	if(!m_mostTouchedNode || !m_mostTouchedNode->isDynamic())
	{
		relVel=m_bodyNode->getLinearVelocity();
	}else
	{
		relVel=m_bodyNode->getLinearVelocity() - m_mostTouchedNode->getLinearVelocity();
	}
	math::quaternion rot=m_bodyNode->getGlobalOrintation();

	m_localVelocity=rot.inverse()*relVel;

}

float PhysicalVehicle::_ComputeAxisTorque()
{
	if(m_motor)
	{
		float rpm=_ComputeRPMFromWheels();
		float motorRPM=_ComputeMotorRPM(rpm);
		m_motor->SetRPM(motorRPM);

		float torque=m_accelerationPedal * m_motor->GetTorque();
		//float v=m_bodyNode->getLinearVelocity().Length();
		
		return torque*_GetGearRatio()*m_differentialRatio*m_transmissionEfficiency;
	}else
	{
		_ComputeRPMFromWheels();
		return m_accelerationPedal*m_motorForce;
	}
}


float PhysicalVehicle::_ComputeRPMFromWheels()
{
	float wheelsRPM=0;
	int nbWheels=0;
	for(int i=0;i<m_wheels.size();++i)
	{
		IVehicleWheel* wheel=m_wheels[i];
		if(wheel->IsFlagUp(EVW_Accelerated))
		{
			wheelsRPM+=wheel->GetRPM();
			++nbWheels;
		}
	}
	if(!nbWheels)
		return 0;
	return wheelsRPM/(float)nbWheels;

}
float PhysicalVehicle::_ComputeMotorRPM(float rpm)
{
	float motorRPM;
	if(m_motor)
	{
		VehicleMotor::EGearChangeResult change;
		if(m_gear &&
			(change = m_motor->ChangeGears(m_gear,0.2f)))
		{
			if(change==VehicleMotor::EGearUp)
				GearUp();
			else if(change==VehicleMotor::EGearDown)
				GearDown();
		}

		float t=_GetGearRatio()*m_differentialRatio;
		motorRPM=math::Max<float>(rpm*t,m_motor->GetDesc().minRPM);
	}else
	{
		motorRPM=rpm*_GetGearRatio()*m_differentialRatio;
	}
	return motorRPM;
}

void PhysicalVehicle::_UpdateRPM()
{
	if(m_motor)
	{
		float rpm=_ComputeRPMFromWheels();
		float motorRpm=_ComputeMotorRPM(rpm);
		m_motor->SetRPM(motorRpm);
	}
}
int PhysicalVehicle::_GetGearRatio()
{
	if(!m_gear)
		return 1;
	return m_gear->GetCurrRatio();
}
IVehicleWheel* PhysicalVehicle::GetWheelByName(const core::string&name)
{
	WheelsMap::iterator it= m_wheelsMap.find(name);
	if(it==m_wheelsMap.end())
		return 0;
	return m_wheels[it->second];
}
void PhysicalVehicle::Control(float steering,bool analogSteering,
							  float acceleration,bool analogAcc,bool handBrake)
{
	if(steering!=0 || acceleration!=0 || handBrake)
		m_bodyNode->wakeUp(0.05f);
	_ControlSteering(steering,analogSteering);
	_ComputeLocalVelocity();

	if(!m_braking || m_releaseBraking)
	{
		//m_braking=(m_localVelocity.x*acceleration)<(-0.1f)*math::sign(-acceleration) ? true:false;
		m_releaseBraking=false;
	}

	if(handBrake!=m_handBrake)
	{
		m_handBrake=handBrake;
		//m_brakePedalChanged=true;
	}
	_ControlAcceleration(acceleration,analogAcc);

}

void PhysicalVehicle::GearUp()
{
	if(m_gear)
		m_gear->GearUp();
}
void PhysicalVehicle::GearDown()
{
	if(m_gear)
		m_gear->GearDown();
}

math::vector3d PhysicalVehicle::GetPosition()const
{
	return m_bodyNode->getGlobalPosition();
}

void PhysicalVehicle::Update(float dt)
{
	float distSteeringAxisCarTurnAxis= m_steeringSteerPoint.x - m_steeringTurnPoint.x;
	float distance2=0;
	if(fabsf(m_steeringWheelState)>0.01f)
		distance2=distSteeringAxisCarTurnAxis/(tanf(m_steeringWheelState*m_steeringMaxAngle));

	int handBrakeCount=0;
	int touchCount=0;
	int notTouchCount=0;

	for(int i=0;i<m_wheels.size();++i)
	{
		IVehicleWheel* wheel=m_wheels[i];
		if(wheel->IsFlagUp(EVW_Steerable_Input))
		{
			if(distance2!=0)
			{
				float xpos=wheel->GetWheelPos().x;
				float zpos=wheel->GetWheelPos().z;
				float dx=xpos - m_steeringTurnPoint.x;
				float dz=-zpos + distance2;
				wheel->SetAngle(tanf(dx/dz));
			}else
			{
				wheel->SetAngle(0);
			}
		}else if(wheel->IsFlagUp(EVW_Steerable_Auto))
		{
			math::vector3d localVel=m_bodyNode->getLocalPointVelocity(wheel->GetWheelPos());
			math::quaternion local2Glob=m_bodyNode->getGlobalOrintation();
			localVel=local2Glob.inverse()*localVel;
			localVel.y=0;
			if(localVel.LengthSQ()<0.01f)
			{
				wheel->SetAngle(0);
			}else
			{
				localVel.Normalize();
				if(localVel.x<0)
				{
					localVel=-localVel;
				}
				float angle=math::clamp((float)atan(localVel.z/localVel.x),-0.3f,0.3f);
				wheel->SetAngle(angle);
			}
		}

		if(!wheel->IsFlagUp(EVW_Accelerated))
			continue;
		if(m_handBrake && wheel->IsFlagUp(EVW_AffectedByHandbrake))
		{
			++handBrakeCount;
		}else
		{
			if(wheel->HasGroundContact())
			{
				++touchCount;
			}else
				++notTouchCount;
		}
	}

	float motorTorque=0;

	if(touchCount && fabsf(m_accelerationPedal)>0.01f)
	{
		float axisTorque=_ComputeAxisTorque();
		float wheelTorque=axisTorque/((float)m_wheels.size()-handBrakeCount);
		float wheelTorqueNotTouch=(notTouchCount>0 ? wheelTorque*pow(0.5f,(float)notTouchCount):0);
		float wheelTorqueTouch=wheelTorque-wheelTorqueNotTouch;
		motorTorque=wheelTorqueTouch/(float)touchCount;
		//printf("torque=%2.3f ",motorTorque);
	}else
		_UpdateRPM();

	for(int i=0;i<m_wheels.size();++i)
	{
		m_wheels[i]->Update(m_handBrake,motorTorque,m_brakePedal,dt);
	}

	math::vector3d centerOfMass;
	m_bodyNode->GetCMassLocalPosition(centerOfMass);

	centerOfMass.y=-fabsf(m_localVelocity.Length()*0.4f);

	m_bodyNode->SetCMassOffsetLocalPosition(centerOfMass);

}


}
}
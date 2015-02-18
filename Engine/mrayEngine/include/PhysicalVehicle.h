

/********************************************************************
	created:	2012/01/02
	created:	2:1:2012   13:07
	filename: 	d:\Development\mrayEngine\Engine\mrayEngine\include\PhysicalVehicle.h
	file path:	d:\Development\mrayEngine\Engine\mrayEngine\include
	file base:	PhysicalVehicle
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __PhysicalVehicle__
#define __PhysicalVehicle__


#include "compileConfig.h"
#include "mstring.h"
#include "IObject.h"


namespace mray
{
namespace physics
{
	class IVehicleWheel;
	class VehicleMotor;
	class VehicleGear;
	class VehicleDesc;

	class IPhysicalNode;
	class IPhysicMaterial;

	class VehicleManager;

class MRAY_DLL PhysicalVehicle:public IObject
{
	DECLARE_RTTI
private:
protected:
	std::vector<IVehicleWheel*> m_wheels;
	typedef std::map<core::string,int> WheelsMap;
	WheelsMap m_wheelsMap;

	IPhysicalNode* m_bodyNode;
	//IPhysicMaterial* m_material;

	VehicleMotor* m_motor;
	VehicleGear* m_gear;

	core::string m_type;

	float m_steeringWheelState;
	float m_accelerationPedal;
	float m_brakePedal;
	bool  m_brakePedalChanged;
	bool  m_handBrake;
	float m_acceleration;

	float m_digitalSteeringDelta;

	math::vector3d m_steeringTurnPoint;
	math::vector3d m_steeringSteerPoint;
	float m_steeringMaxAngle;
	float m_motorForce;
	float m_transmissionEfficiency;
	float m_differentialRatio;

	math::vector3d m_localVelocity;
	bool m_braking;
	bool m_releaseBraking;
	float m_maxVelocity;


	void* m_userData;

	IPhysicalNode* m_mostTouchedNode;

	VehicleManager* m_creator;

	void _ControlSteering(float steering,bool analogSteering);
	void _ControlAcceleration(float acceleration,bool analogAcc);
	
	void _ComputeLocalVelocity();
	void _ComputeMostTouchedNode();
	float _ComputeAxisTorque();
	float _ComputeRPMFromWheels();
	float _ComputeMotorRPM(float rpm);
	void _UpdateRPM();

	int _GetGearRatio();
public:
	PhysicalVehicle(VehicleManager* creator);
	virtual~PhysicalVehicle();

	bool LoadFromDesc(const VehicleDesc& desc,IPhysicalNode* node,float scale);

	void Control(float steering,bool analogSteering,float acceleration,bool analogAcc,bool handBrake);

	void GearUp();
	void GearDown();

	VehicleMotor* GetMotor()const{return m_motor;}
	VehicleGear* GetGears()const{return m_gear;}

	IPhysicalNode* GetPhysicalNode()const{return m_bodyNode;}

	IVehicleWheel* GetWheelByName(const core::string&name);

	float GetDriveVelocity()const{return (m_localVelocity.x);}
	float GetMaxVelocity()const{return m_maxVelocity;}
	const std::vector<IVehicleWheel*>& GetWheels()const{return m_wheels;}
	math::vector3d GetPosition()const;

	void Update(float dt);

};


}
}

#endif

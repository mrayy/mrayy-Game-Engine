


/********************************************************************
	created:	2011/12/30
	created:	30:12:2011   15:22
	filename: 	d:\Development\mrayEngine\Engine\mrayEngine\include\IVehicleWheel.h
	file path:	d:\Development\mrayEngine\Engine\mrayEngine\include
	file base:	IVehicleWheel
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __IVehicleWheel__
#define __IVehicleWheel__

#include "VehicleWheelDesc.h"

namespace mray
{
namespace physics
{
	class IPhysicalNode;

class IVehicleWheel
{
private:
protected:
	void* m_userData;
	uint m_flags;
	core::string m_name;
public:
	IVehicleWheel():m_userData(0),m_flags(0)
	{}
	virtual~IVehicleWheel(){}

	virtual void SetName(const core::string&name){m_name=name;}
	virtual const core::string& GetName()const{return m_name;}

	virtual IPhysicalNode* GetTouchNode()const=0;
	virtual math::vector3d GetWheelPos()const=0;
	virtual void SetWheelPos(const math::vector3d& pos)=0;
	virtual math::quaternion GetLocalOrintation()const=0;
	virtual math::quaternion GetGlobalOrintation()const=0;
	virtual float GetSteeringAngle()const=0;
	virtual void SetAngle(float a)=0;
	virtual float GetRPM()const=0;
	virtual math::vector3d GetGroundContactPos()=0;
	virtual float GetRadius()=0;

	virtual void Update(bool handBrake,float motorTorque,float brakeTorque,float dt)=0;

	bool HasGroundContact()const{return GetTouchNode()!=0;}
	bool IsFlagUp(EVehicleWheelFlags flag){return (m_flags & flag)!=0;}
	uint GetFlags()const{return m_flags;}
};


}
}

#endif

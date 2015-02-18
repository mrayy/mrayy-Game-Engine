


/********************************************************************
	created:	2011/12/30
	created:	30:12:2011   23:37
	filename: 	d:\Development\mrayEngine\Engine\mrayEngine\include\RaycastVehicleWheel.h
	file path:	d:\Development\mrayEngine\Engine\mrayEngine\include
	file base:	RaycastVehicleWheel
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __RaycastVehicleWheel__
#define __RaycastVehicleWheel__

#include "IVehicleWheel.h"
#include "compileconfig.h"

namespace mray
{
namespace physics
{
	class IPhysicalWheelShape;

class MRAY_DLL RaycastVehicleWheel:public IVehicleWheel
{
private:
protected:
	IPhysicalWheelShape* m_wheel;
public:
	RaycastVehicleWheel(IPhysicalNode* node, const VehicleWheelDesc&desc,float scale);
	virtual~RaycastVehicleWheel();


	IPhysicalNode* GetTouchNode()const;
	void SetWheelPos(const math::vector3d& pos);
	math::vector3d GetWheelPos()const;
	math::quaternion GetLocalOrintation()const;
	math::quaternion GetGlobalOrintation()const;
	float GetSteeringAngle()const;

	void SetAngle(float a);
	float GetRPM()const;
	math::vector3d GetGroundContactPos();
	float GetRadius();

	void Update(bool handBrake,float motorTorque,float brakeTorque,float dt);
};

}
}

#endif

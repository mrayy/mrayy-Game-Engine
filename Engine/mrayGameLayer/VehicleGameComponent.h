


/********************************************************************
	created:	2012/02/27
	created:	27:2:2012   10:29
	filename: 	d:\Development\mrayEngine\Engine\mrayGameLayer\VehicleGameComponent.h
	file path:	d:\Development\mrayEngine\Engine\mrayGameLayer
	file base:	VehicleGameComponent
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __VehicleGameComponent__
#define __VehicleGameComponent__

#include "IGameComponent.h"

namespace mray
{
namespace physics
{
	class VehicleManager;
	class PhysicalVehicle;
}
namespace game
{
	class VehicleWheelComponent;

class MRAY_GAME_DLL VehicleGameComponent:public IGameComponent
{
	DECLARE_RTTI;
protected:
	core::string m_vehiclePath;
	physics::PhysicalVehicle* m_vehicle;
	physics::VehicleManager* m_vehManager;
	float m_scale;

	std::vector<VehicleWheelComponent*> m_wheels;
public:
	DECLARE_PROPERTY_TYPE(VehiclePath,core::string,);
public:
	VehicleGameComponent(GameEntityManager*m);
	VehicleGameComponent(physics::VehicleManager* cm);
	~VehicleGameComponent();

	bool SetScale(float scale);
	float GetScale();

	physics::PhysicalVehicle* GetModel();
	virtual IObject* GetAttachedObject();
	bool SetVehiclePath(const core::string& path);

	const core::string& GetVehiclePath();

};
DECLARE_GAMECOMPONENT_FACTORY(VehicleGameComponent);

}
}

#endif

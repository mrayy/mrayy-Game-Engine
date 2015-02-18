


/********************************************************************
	created:	2012/01/02
	created:	2:1:2012   16:14
	filename: 	d:\Development\mrayEngine\Engine\mrayEngine\include\VehicleManager.h
	file path:	d:\Development\mrayEngine\Engine\mrayEngine\include
	file base:	VehicleManager
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __VehicleManager__
#define __VehicleManager__

#include "compileconfig.h"

namespace mray
{
namespace physics
{
	class PhysicalVehicle;
	class VehicleDesc;

	class IVehicleWheel;
	class VehicleWheelDesc;

	class IPhysicManager;
	class IPhysicalNode;

class MRAY_DLL VehicleManager
{
private:
protected:
	IPhysicManager* m_phManager;

	std::vector<PhysicalVehicle*> m_vehicles;

public:
	VehicleManager(IPhysicManager* phManager);
	virtual~VehicleManager();

	void Clear();

	const std::vector<PhysicalVehicle*>& GetVehicles()const{return m_vehicles;}

	IVehicleWheel* CreateWheel(IPhysicalNode* node, const VehicleWheelDesc& desc,float scale);

	PhysicalVehicle* CreateVehicle(const VehicleDesc& desc,IPhysicalNode* node,float scale);

	IPhysicManager* GetPhysicsManager()const{return m_phManager;}

	void Update(float dt);

};


}
}

#endif

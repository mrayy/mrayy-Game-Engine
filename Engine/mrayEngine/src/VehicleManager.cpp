
#include "stdafx.h"
#include "VehicleManager.h"
#include "PhysicalVehicle.h"
#include "VehicleDesc.h"
#include "IVehicleWheel.h"
#include "VehicleWheelDesc.h"
#include "IPhysicManager.h"
#include "RaycastVehicleWheel.h"

namespace mray
{
namespace physics
{

VehicleManager::VehicleManager(IPhysicManager* phManager)
:m_phManager(phManager)
{
}
VehicleManager::~VehicleManager()
{
	Clear();
}

void VehicleManager::Clear()
{
	for(int i=0;i<m_vehicles.size();++i)
	{
		delete m_vehicles[i];
	}
	m_vehicles.clear();
}

IVehicleWheel* VehicleManager::CreateWheel(IPhysicalNode* node,const VehicleWheelDesc& desc,float scale)
{
	return new RaycastVehicleWheel(node,desc,scale);
}

PhysicalVehicle* VehicleManager::CreateVehicle(const VehicleDesc& desc,IPhysicalNode* node,float scale)
{
	PhysicalVehicle* v=new PhysicalVehicle(this);
	if(!v->LoadFromDesc(desc,node,scale))
	{
		delete v;
		return 0;
	}

	m_vehicles.push_back(v);
	return v;
}

void VehicleManager::Update(float dt)
{
	for(int i=0;i<m_vehicles.size();++i)
	{
		m_vehicles[i]->Update(dt);
	}
}

}
}


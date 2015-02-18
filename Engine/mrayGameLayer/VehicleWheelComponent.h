


/********************************************************************
	created:	2012/02/27
	created:	27:2:2012   10:33
	filename: 	d:\Development\mrayEngine\Engine\mrayGameLayer\VehicleWheelComponent.h
	file path:	d:\Development\mrayEngine\Engine\mrayGameLayer
	file base:	VehicleWheelComponent
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __VehicleWheelComponent__
#define __VehicleWheelComponent__

#include "IGameComponent.h"

namespace mray
{
	namespace scene
	{
		class BoneNode;
	}
	namespace physics
	{
		class IVehicleWheel;
		class PhysicalVehicle;
	}
namespace game
{

class MRAY_GAME_DLL VehicleWheelComponent:public IGameComponent
{
	DECLARE_RTTI;
protected:
	physics::PhysicalVehicle* m_car;

	core::string m_wheelName;
	core::string m_boneName;

	physics::IVehicleWheel* m_wheel;
	scene::BoneNode* m_bone;

	float m_angle;

	bool _Rebuild();
	void _Init();
public:
	DECLARE_PROPERTY_TYPE(BoneName,core::string,)
	DECLARE_PROPERTY_TYPE(WheelName,core::string,)
public:

	VehicleWheelComponent(GameEntityManager*);
	VehicleWheelComponent(const core::string& wheel,const core::string& bone );

	VehicleWheelComponent(scene::BoneNode* bone,physics::IVehicleWheel* phNode,physics::PhysicalVehicle*v);
	virtual ~VehicleWheelComponent();

	bool SetBoneName(const core::string &n);
	core::string GetBoneName();
	bool SetWheelName(const core::string &n);
	core::string GetWheelName();
	virtual void Update(float dt);

};

DECLARE_GAMECOMPONENT_FACTORY(VehicleWheelComponent);

}
}

#endif

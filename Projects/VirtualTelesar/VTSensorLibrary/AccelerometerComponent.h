


/********************************************************************
	created:	2012/07/22
	created:	22:7:2012   23:19
	filename: 	d:\Development\mrayEngine\Projects\VirtualTelesar\VTLib\AccelerometerComponent.h
	file path:	d:\Development\mrayEngine\Projects\VirtualTelesar\VTLib
	file base:	AccelerometerComponent
	file ext:	h
	author:		MHD YAMEN SARAIJI
	
	purpose:	
*********************************************************************/
#ifndef ___AccelerometerComponent___
#define ___AccelerometerComponent___

#include "MountableComponent.h"


namespace mray
{
	namespace game
	{
		class PhysicsComponent;
	}
	namespace physics
	{
		class IPhysicManager;
	}
namespace VT
{

BEGIN_DECLARE_VTCOMPONENT(AccelerometerComponent,MountableComponent)
protected:
	float m_noiseAmount;
	math::vector3d m_accelValues;
	float m_refreshFreq;
	float m_invRefFreq;
	float m_timeAcc;
	math::vector3d m_vel[2];

	game::PhysicsComponent* m_phComp;
	physics::IPhysicManager* m_phMngr;
public:
	DECLARE_VTPROPERTY(NoiseAmount,float,);
	DECLARE_VTPROPERTY(RefreshFrequency,float,);

	math::vector3d GetAccelerometerValues();

END_DECLARE_VTCOMPONENT(AccelerometerComponent);


}
}

#endif


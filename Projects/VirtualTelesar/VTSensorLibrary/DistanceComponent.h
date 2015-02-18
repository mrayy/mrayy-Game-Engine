

/********************************************************************
	created:	2012/07/22
	created:	22:7:2012   15:00
	filename: 	d:\Development\mrayEngine\Projects\VirtualTelesar\VTLib\DistanceComponent.h
	file path:	d:\Development\mrayEngine\Projects\VirtualTelesar\VTLib
	file base:	DistanceComponent
	file ext:	h
	author:		MHD YAMEN SARAIJI
	
	purpose:	
*********************************************************************/
#ifndef ___DistanceComponent___
#define ___DistanceComponent___

#include "MountableComponent.h"

namespace mray
{
namespace VT
{

BEGIN_DECLARE_VTCOMPONENT(DistanceComponent,MountableComponent)
protected:
	physics::IPhysicManager* m_phManager;

	physics::PhysicsRaycaseHit m_hitData;
	bool m_hasHit;
	float m_distance;

public:
	DECLARE_PROPERTY_TYPE(Distance,float,);
public:

	bool SetNodeName(const core::string &name);
	const core::string& GetNodeName();

	bool SetOffset(const math::vector3d &v);
	const math::vector3d& GetOffset();
	
	bool SetAngles(const math::vector3d &v);
	const math::vector3d& GetAngles();

	bool SetDistance(float v);
	float GetDistance();


	const physics::PhysicsRaycaseHit& GetLastHitData()const{return m_hitData;}
	bool HasHit(){return m_hasHit;}

	virtual void DebugRender(scene::IDebugDrawManager* renderer);
END_DECLARE_VTCOMPONENT(DistanceComponent);

}
}

#endif

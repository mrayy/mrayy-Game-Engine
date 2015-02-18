

/********************************************************************
	created:	2012/07/16
	created:	16:7:2012   14:40
	filename: 	d:\Development\mrayEngine\Projects\VirtualTelesar\PhysicsTransformComponent.h
	file path:	d:\Development\mrayEngine\Projects\VirtualTelesar
	file base:	PhysicsTransformComponent
	file ext:	h
	author:		MHD YAMEN SARAIJI
	
	purpose:	
*********************************************************************/
#ifndef ___PhysicsTransformComponent___
#define ___PhysicsTransformComponent___

#include "IVTComponent.h"

namespace mray
{
namespace VT
{

class PhysicsTransformComponent:public IVTComponent
{
	DECLARE_RTTI;

protected:

	math::vector3d m_pos;
	math::vector3d m_rotation;

public:
	DECLARE_PROPERTY_TYPE(Position,math::vector3d,);
	DECLARE_PROPERTY_TYPE(Rotation,math::vector3d,);

public:
	PhysicsTransformComponent(game::GameEntityManager*mngr);
	virtual~PhysicsTransformComponent();
	bool InitComponent();


	bool SetPosition(const math::vector3d& v){m_pos=v;return true;}
	const math::vector3d& GetPosition(){return m_pos;}

	bool SetRotation(const math::vector3d& v){m_rotation=v;return true;}
	const math::vector3d& GetRotation(){return m_rotation;}
};
DECLARE_GAMECOMPONENT_FACTORY(PhysicsTransformComponent);

}
}

#endif



/********************************************************************
	created:	2013/06/05
	created:	5:6:2013   16:45
	filename: 	C:\Development\mrayEngine\Projects\VirtualTelesar\VTLib\NullPhysicsComponent.h
	file path:	C:\Development\mrayEngine\Projects\VirtualTelesar\VTLib
	file base:	NullPhysicsComponent
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __NullPhysicsComponent__
#define __NullPhysicsComponent__


#include "IPhysicsComponent.h"

namespace mray
{
	namespace scene
	{
		class ISceneNode;
	}
namespace game
{

class NullPhysicsComponent:public game::IPhysicsComponent
{
	DECLARE_RTTI;
protected:
	scene::ISceneNode* m_root;
	math::vector3d m_position;
	math::vector3d m_rotation;

	DECLARE_PROPERTY_TYPE(Position,math::vector3d,);
	DECLARE_PROPERTY_TYPE(Rotation,math::vector3d,);
public:
	NullPhysicsComponent(game::GameEntityManager*);
	virtual~NullPhysicsComponent();

	virtual bool InitComponent();
	virtual void AttachNode(game::PhysicalComponentAttachment*a);

	bool SetPosition(const math::vector3d& v);
	const math::vector3d& GetPosition();

	bool SetRotation(const math::vector3d& v);
	const math::vector3d& GetRotation();

	scene::ISceneNode* GetRootNode(){return m_root;}
};

DECLARE_GAMECOMPONENT_FACTORY(NullPhysicsComponent);


}
}


#endif

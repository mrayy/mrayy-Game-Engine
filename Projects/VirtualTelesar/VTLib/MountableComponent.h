

/********************************************************************
	created:	2012/07/22
	created:	22:7:2012   23:20
	filename: 	d:\Development\mrayEngine\Projects\VirtualTelesar\VTLib\MountableComponent.h
	file path:	d:\Development\mrayEngine\Projects\VirtualTelesar\VTLib
	file base:	MountableComponent
	file ext:	h
	author:		MHD YAMEN SARAIJI
	
	purpose:	
*********************************************************************/
#ifndef ___MountableComponent___
#define ___MountableComponent___

#include "IVTComponent.h"

namespace mray
{
namespace VT
{

class MountableComponent:public IVTComponent
{
	DECLARE_RTTI;
protected:

	math::vector3d m_offset;
	math::vector3d m_angles;
	float m_distance;

	math::quaternion m_orientation;

	core::string m_targetNode;
	scene::IMovable* m_node;

public:
	DECLARE_PROPERTY_TYPE(TargetNode,core::string,);
	DECLARE_PROPERTY_TYPE(Offset,math::vector3d,);
	DECLARE_PROPERTY_TYPE(Angles,math::vector3d,);
public:
	MountableComponent(game::GameEntityManager*m);
	virtual~MountableComponent();

	virtual bool InitComponent();

	bool SetNodeName(const core::string &name);
	const core::string& GetNodeName();

	bool SetOffset(const math::vector3d &v);
	const math::vector3d& GetOffset();

	bool SetAngles(const math::vector3d &v);
	const math::vector3d& GetAngles();

	bool SetDistance(float v);
	float GetDistance();
	
	scene::IMovable* GetNode(){ return m_node; }

	math::vector3d GetAbsolutePosition();
	math::quaternion GetAbsoluteOrientation();

	virtual void DebugRender(scene::IDebugDrawManager* renderer);
};
DECLARE_GAMECOMPONENT_FACTORY(MountableComponent);


}
}

#endif




/********************************************************************
	created:	2014/02/17
	created:	17:2:2014   16:09
	filename: 	C:\Development\mrayEngine\Projects\AugTel\TriggerObjectComponent.h
	file path:	C:\Development\mrayEngine\Projects\AugTel
	file base:	TriggerObjectComponent
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __TriggerObjectComponent__
#define __TriggerObjectComponent__


#include "IGameComponent.h"
#include "IMovable.h"

namespace mray
{
namespace AugTel
{
	class RobotSpaceComponent;

class TriggerObjectComponent:public game::IGameComponent
{
	DECLARE_RTTI;
protected:
	core::string m_targetName;
	core::string m_type;
	core::string m_grabName;
	core::string m_triggerName;
	math::vector3d m_position;
	bool m_isMounted;

	scene::ISceneNode* m_targetNode;
	scene::IMovable* m_grabNode;
	scene::IMovable* m_triggerNode;

	RobotSpaceComponent* m_robotSpace;

	DECLARE_PROPERTY_TYPE(TargetNode, core::string, );
	DECLARE_PROPERTY_TYPE(Type, core::string, );
	DECLARE_PROPERTY_TYPE(GrabNode, core::string, );
	DECLARE_PROPERTY_TYPE(TriggerNode, core::string, );
	DECLARE_PROPERTY_TYPE(Position, math::vector3d, );

public:
	TriggerObjectComponent(game::GameEntityManager*m);
	virtual~TriggerObjectComponent();

	DECLARE_SETGET_PROP(TargetNode, core::string);
	DECLARE_SETGET_PROP(Type, core::string);
	DECLARE_SETGET_PROP(GrabNode, core::string);
	DECLARE_SETGET_PROP(TriggerNode, core::string);
	DECLARE_SETGET_PROP(Position, math::vector3d);

	virtual bool InitComponent();

	virtual void Update(float dt);

	virtual void DebugRender(scene::IDebugDrawManager* renderer);
	virtual void OnGUIRender(GUI::IGUIRenderer* renderer, const math::rectf& vp);
};

DECLARE_GAMECOMPONENT_FACTORY(TriggerObjectComponent);
}
}


#endif


/********************************************************************
	created:	2014/02/17
	created:	17:2:2014   17:35
	filename: 	C:\Development\mrayEngine\Projects\AugTel\RobotSpaceComponent.h
	file path:	C:\Development\mrayEngine\Projects\AugTel
	file base:	RobotSpaceComponent
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __RobotSpaceComponent__
#define __RobotSpaceComponent__

#include "IGameComponent.h"


namespace mray
{
namespace AugTel
{

class RobotSpaceComponent:public game::IGameComponent
{
	DECLARE_RTTI;
protected:
	core::string m_eyeNodeName;
	math::vector3d m_pos;
	math::quaternion m_ori;
	bool m_isVisible;
	int m_trackID;
	scene::IMovable* m_eyeNode;

	DECLARE_PROPERTY_TYPE(EyeNode, core::string, );
	DECLARE_PROPERTY_TYPE(TrackableID, int, );
public:
	RobotSpaceComponent(game::GameEntityManager*m);
	virtual~RobotSpaceComponent();

	DECLARE_SETGET_PROP(EyeNode, core::string);
	DECLARE_SETGET_PROP(TrackableID, int);

	math::vector3d TransformToWorld(const math::vector3d& pos);
	math::vector3d TransformToEye(const math::vector3d& pos);

	bool IsVisible(){ return m_isVisible; }


	virtual bool InitComponent();

	virtual void Update(float dt);

	virtual void DebugRender(scene::IDebugDrawManager* renderer);
	virtual void OnGUIRender(GUI::IGUIRenderer* renderer, const math::rectf& vp);
};

DECLARE_GAMECOMPONENT_FACTORY(RobotSpaceComponent);
}
}


#endif

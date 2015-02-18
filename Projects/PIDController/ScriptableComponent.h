

/********************************************************************
	created:	2013/07/25
	created:	25:7:2013   12:41
	filename: 	C:\Development\mrayEngine\Projects\PIDController\ScriptableComponent.h
	file path:	C:\Development\mrayEngine\Projects\PIDController
	file base:	ScriptableComponent
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __ScriptableComponent__
#define __ScriptableComponent__


#include "IGameComponent.h"
#include "luabind/wrapper_base.hpp"

namespace mray
{
namespace game
{

class ScriptableComponent:public IGameComponent,public luabind::wrap_base
{
	static RTTI s_scriptRtti;
protected:
	RTTI m_rttiObject;
public:
	ScriptableComponent(const core::string& type);
	virtual~ScriptableComponent();

	virtual bool InitComponent();

	virtual void OnReset();

	virtual void PreUpdate();
	virtual void Update(float dt);
	virtual void DebugRender(scene::IDebugDrawManager* renderer);

	virtual void OnDestroy();
	
	virtual const core::string &getObjectType()const{return m_rttiObject.getTypeName();}
	virtual const RTTI*getObjectRTTI()const{return &m_rttiObject;}


	static void UpdateDefault(ScriptableComponent* s,float dt)
	{
		s->IGameComponent::Update(dt);
	}
	static void DebugRenderDefault(ScriptableComponent* s,scene::IDebugDrawManager* renderer)
	{
		s->IGameComponent::DebugRender(renderer);
	}
};

}
}


#endif

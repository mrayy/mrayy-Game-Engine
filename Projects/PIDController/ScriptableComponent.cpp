

#include "stdafx.h"
#include "ScriptableComponent.h"
#include "ILogManager.h"
#include "IDebugDrawManager.h"


namespace mray
{
namespace game
{

	RTTI ScriptableComponent::s_scriptRtti("ScriptableComponent",&IGameComponent::ms_RTTI_def);

ScriptableComponent::ScriptableComponent(const core::string& type):
m_rttiObject(type.c_str(),&s_scriptRtti)
{
}

ScriptableComponent::~ScriptableComponent()
{
}


bool ScriptableComponent::InitComponent()
{
	if(!IGameComponent::InitComponent())
		return false;
	bool ret=false;
	try
	{
		ret=call<bool>("InitComponent");
	}
	catch (luabind::error& e)
	{
		gLogManager.log(e.what(),ELL_WARNING);
	}

	return ret;
}


void ScriptableComponent::OnReset()
{
	try
	{
		call<void>("OnReset");
	}
	catch (luabind::error& e)
	{
		gLogManager.log(e.what(),ELL_WARNING);
	}

}


void ScriptableComponent::PreUpdate()
{
	try
	{
		call<void>("PreUpdate");
	}
	catch (luabind::error& e)
	{
		gLogManager.log(e.what(),ELL_WARNING);
	}
}

void ScriptableComponent::Update(float dt)
{
	try
	{
		call<void>("Update",dt);
	}
	catch (luabind::error& e)
	{
		gLogManager.log(e.what(),ELL_WARNING);
	}
}

void ScriptableComponent::DebugRender(scene::IDebugDrawManager* renderer)
{
	try
	{
		call<void>("DebugRender");
	}
	catch (luabind::error& e)
	{
		gLogManager.log(e.what(),ELL_WARNING);
	}
}


void ScriptableComponent::OnDestroy()
{
	try
	{
		call<bool>("OnDestroy");
	}
	catch (luabind::error& e)
	{
		gLogManager.log(e.what(),ELL_WARNING);
	}
}


}
}




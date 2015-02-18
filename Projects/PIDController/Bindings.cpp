

#include "stdafx.h"

#include "luabind/luabind.hpp"
#include "luabind/operator.hpp"
#include "PIDApplication.h"

#include "luamacros.h"
#include "GameEntity.h"
#include "IPhysicManager.h"

#include "ScriptableComponent.h"


using namespace luabind;

#define LUA_DeclareProperty(C,name,getter,setter) .property(#name,&C::getter,&C::setter)
#define LUA_DeclareMethod(C,name) .def(#name,&C::name)

namespace mray
{
	using namespace game;

	class A
	{
	public:
		A(){x=0;}
		int x;
	};
	class B
	{
		A m_a;
	public:
		A& getA(){return m_a;}
	}*g_b=new B();
	static A& B_GetA(B&b){return b.getA();}
	void ExportApplicationToLua(lua_State* L)
	{
		module(L)[
			class_<A>("A")
				.def(constructor<>())
				.def_readwrite("x",&A::x)
				,
			class_<B>("B")
				.def(constructor<>())
				.def("getA",(A&(B::*)())&B::getA),
				def("B_GetA",&B_GetA)
		];
		luabind::globals(L)["g_B"]=(g_b);
		module(L)[
			class_<PIDApplication>("PIDApplication")
				LUA_DeclareMethod(PIDApplication,getSceneManager)
				LUA_DeclareMethod(PIDApplication,GetRenderWindow)
				LUA_DeclareMethod(PIDApplication,getCamera)
				LUA_DeclareMethod(PIDApplication,getDevice)
				LUA_DeclareMethod(PIDApplication,GetGameManager)
				.def("CreateScene",(void(PIDApplication::*)())&PIDApplication::CreateScene)
				,
			def( "getApplication", &PIDApplication::getInstance )
		];

		module(L)[
			class_<RTTI>("RTTI")
				LUA_DeclareMethod(RTTI,getTypeName)
				LUA_DeclareMethod(RTTI,getParent)
				.def("isKindOf",(bool (RTTI::*)(const core::string& )const)&RTTI::isKindOf)
				.def("isKindOf",(bool (RTTI::*)(const RTTI* )const)&RTTI::isKindOf)

		];
		module(L)[
			class_<IObject>("IObject")
				LUA_DeclareMethod(IObject,toString)
				LUA_DeclareMethod(IObject,parse)
				LUA_DeclareMethod(IObject,getObjectRTTI)
				LUA_DeclareMethod(IObject,getObjectType)

		];
		module(L)[
			class_<IObjectComponent,IObject>("IObjectComponent")
				LUA_DeclareMethod(IObjectComponent,AddComponent)
				LUA_DeclareMethod(IObjectComponent,CanAddComponent)
				LUA_DeclareMethod(IObjectComponent,RemoveComponent)
				LUA_DeclareMethod(IObjectComponent,GetComponents)
				LUA_DeclareMethod(IObjectComponent,ClearComponents)
				LUA_DeclareMethod(IObjectComponent,OnAttachedToComponent)
				LUA_DeclareMethod(IObjectComponent,OnRemovedFromComponent)
				LUA_DeclareMethod(IObjectComponent,GetOwnerComponent)
				.def("GetComponent",(const std::list<IObjectComponent*>& (IObjectComponent::*)(const core::string& )const)&IObjectComponent::GetComponent)
				.def("GetComponent",(const std::list<IObjectComponent*>& (IObjectComponent::*)(const RTTI* )const)&IObjectComponent::GetComponent)
				.def("GetFirstComponent",(IObjectComponent* (IObjectComponent::*)(const core::string& )const)&IObjectComponent::GetFirstComponent)
				.def("GetFirstComponent",(IObjectComponent* (IObjectComponent::*)(const RTTI* )const)&IObjectComponent::GetFirstComponent)

		];
		module(L)[
			class_<IGameComponent,IObjectComponent,ScriptableComponent>("GameComponent")
			.def(constructor<const core::string&>())
			LUA_DeclareProperty(IGameComponent,ID,GetID,SetID)
			LUA_DeclareProperty(IGameComponent,enable,IsEnabled,SetEnabled)
			LUA_DeclareProperty(IGameComponent,name,GetName,SetName)
			LUA_DeclareMethod(IGameComponent,GetOwnerEntity)
			LUA_DeclareMethod(IGameComponent,InvokeMessage)
			LUA_DeclareMethod(IGameComponent,IsInited)
				.def("DebugRender",&IGameComponent::DebugRender,&ScriptableComponent::DebugRenderDefault)
				.def("DebugRender",&IGameComponent::Update,&ScriptableComponent::UpdateDefault)
		];

		module(L)[
			class_<GameEntityManager>("GameEntityManager")
				LUA_DeclareMethod(GameEntityManager,AddGameEntity)
				LUA_DeclareMethod(GameEntityManager,CreateGameEntity)
				LUA_DeclareMethod(GameEntityManager,RemoveGameEntity)
				LUA_DeclareMethod(GameEntityManager,GetGameEntity)
				LUA_DeclareMethod(GameEntityManager,Clear)
		];
		module(L)[
			class_<GameEntity,IGameComponent>("GameEntity")
				.def(constructor<const core::string&,GameEntityManager*>())
				LUA_DeclareMethod(GameEntity,AddTag)
				LUA_DeclareMethod(GameEntity,RemoveTag)
				LUA_DeclareMethod(GameEntity,ClearTags)
				LUA_DeclareMethod(GameEntity,HasTag)
				LUA_DeclareMethod(GameEntity,GetTags)
		];

		using namespace physics;
		module(L)[
			class_<IPhysicManager>("PhysicsManager")
		];
	}
}

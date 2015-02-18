// SimpleGameMod.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "SimpleGameMod.h"
#include "ILogManager.h"
#include "IGameAction.h"
#include "IGameActionCreator.h"
#include "GameActionFactory.h"
#include "GenericValues.h"

#ifdef _MANAGED
#pragma managed(push, off)
#endif

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
    return TRUE;
}

#ifdef _MANAGED
#pragma managed(pop)
#endif



namespace mray{
namespace game{


class SimpleAction:public IGameAction
{
public:
	SimpleAction():IGameAction(mT("SimpleAction"))
	{
		AddAttribute(new FloatValue(mT("Health"),100));
		AddAttribute(new IntValue(mT("ApplesCount"),10));
		AddAttribute(new IntValue(mT("Ammo"),50));
		AddAttribute(new IntValue(mT("TargetEntity"),0));
	}

	virtual void AttachToEntity(GameEntity*ent)
	{
	}

	virtual void Update(float dt)
	{
	}
};

class SimpleActionCreator:public IGameActionCreator
{
public:
	SimpleActionCreator():IGameActionCreator(mT("SimpleAction"))
	{
	}
	virtual core::string GetDescription()
	{
		return mT("This is for test only");
	}

	virtual IGameAction* CreateObject()
	{
		return new SimpleAction();
	}
};

SimpleGameMod::SimpleGameMod()
{
}
SimpleGameMod::~SimpleGameMod()
{
}

const mchar* SimpleGameMod::getName()
{
	return mT("SimpleGameMod");
}

void SimpleGameMod::install()
{
	if(isInstalled())return;
	IPlugin::install();
	gLogManager.log(mT("Installing SimpleGameMod"),ELL_INFO);
	GameActionFactory::getInstance().RegisterFactory(new SimpleActionCreator());;
}
void SimpleGameMod::uninstall()
{
	if(!isInstalled())return;
	IPlugin::uninstall();
	gLogManager.log(mT("Uninstalling SimpleGameMod"),ELL_INFO);
	GameActionFactory::getInstance().UnregisterFactory(mT("SimpleAction"));;
}



extern "C" SIMPLEGAMEMOD_API IPlugin* dllCreatePlugin(){
return new SimpleGameMod();

}

}
}
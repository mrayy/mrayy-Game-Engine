// CharacterDemoGM.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "CompileConfig.h"
#include <IPlugin.h>
#include <ILogManager.h>
#include <GameActionFactory.h>

#include "CharacterAction.h"
#include "FoodAction.h"
#include "ObstacleAction.h"
#include "PhysicalObjectAction.h"
#include "CollisionModelAction.h"
#include "SelectionSurfaceAction.h"
#include "CD_SelectTargetMain.h"
#include "PhysicalCharacterAction.h"
#include "CameraStartAction.h"

#include "GoalWait.h"
#include "GoalFindFood.h"
#include "GoalChatToFriend.h"
#include "GoalGoToObject.h"
#include "GoalPlayAnimation.h"
#include "SoundAction.h"

#include "GoalFactoryManager.h"


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
namespace GameMod{


class CHARACTERDEMOGM_API CharacterDemoGM:public IPlugin
{
private:
protected:
public:
	CharacterDemoGM();
	virtual ~CharacterDemoGM();

	virtual const mchar*getName();

	virtual void install();
	virtual void uninstall();
};

CharacterDemoGM::CharacterDemoGM()
{
}
CharacterDemoGM::~CharacterDemoGM()
{
}

const mchar* CharacterDemoGM::getName()
{
	return mT("CharacterDemoGM");
}

void CharacterDemoGM::install()
{
	if(isInstalled())return;
	IPlugin::install();
	gLogManager.log(mT("Installing CharacterDemo Game Mod"),ELL_INFO);


	game::GameActionFactory::getInstance().RegiesterCreater(new CharacterActionCreator());;
	game::GameActionFactory::getInstance().RegiesterCreater(new FoodActionCreator());;
	game::GameActionFactory::getInstance().RegiesterCreater(new ObstacleActionCreator());;
	game::GameActionFactory::getInstance().RegiesterCreater(new PhysicalObjectActionCreator());;
	game::GameActionFactory::getInstance().RegiesterCreater(new CollisionModelActionCreator());;
	game::GameActionFactory::getInstance().RegiesterCreater(new SelectionSurfaceActionCreator());;
	game::GameActionFactory::getInstance().RegiesterCreater(new CD_SelectTargetMainCreator());;
	game::GameActionFactory::getInstance().RegiesterCreater(new PhysicalCharacterActionCreator());;
	game::GameActionFactory::getInstance().RegiesterCreater(new SoundActionCreator());;
	game::GameActionFactory::getInstance().RegiesterCreater(new CameraStartActionCreator());;


	AI::GoalFactoryManager::getInstance().AddFactory(new GoalWaitFactory());
	AI::GoalFactoryManager::getInstance().AddFactory(new GoalFindFoodFactory());
	AI::GoalFactoryManager::getInstance().AddFactory(new GoalChatToFriendFactory());
	AI::GoalFactoryManager::getInstance().AddFactory(new GoalGoToObjectFactory());
	AI::GoalFactoryManager::getInstance().AddFactory(new GoalPlayAnimationFactory());

}
void CharacterDemoGM::uninstall()
{
	if(!isInstalled())return;
	IPlugin::uninstall();
	gLogManager.log(mT("Uninstalling CharacterDemo Game Mod"),ELL_INFO);
	if(game::GameActionFactory::isExist()){
		game::GameActionFactory::getInstance().UnregiesterCreater(mT("CharacterAction"));;
		game::GameActionFactory::getInstance().UnregiesterCreater(mT("FoodAction"));;
		game::GameActionFactory::getInstance().UnregiesterCreater(mT("ObstacleAction"));;
		game::GameActionFactory::getInstance().UnregiesterCreater(mT("PhysicalObjectAction"));;
		game::GameActionFactory::getInstance().UnregiesterCreater(mT("CollisionModelAction"));;
		game::GameActionFactory::getInstance().UnregiesterCreater(mT("SelectionSurfaceAction"));;
		game::GameActionFactory::getInstance().UnregiesterCreater(mT("CD_SelectTargetMain"));;
		game::GameActionFactory::getInstance().UnregiesterCreater(mT("PhysicalCharacterAction"));;
		game::GameActionFactory::getInstance().UnregiesterCreater(mT("SoundAction"));;
		game::GameActionFactory::getInstance().UnregiesterCreater(mT("CameraStartAction"));;
	}

	if(AI::GoalFactoryManager::isExist()){
		AI::GoalFactoryManager::getInstance().RemoveFactory(mT("Wait"));
		AI::GoalFactoryManager::getInstance().RemoveFactory(mT("FindFood"));
		AI::GoalFactoryManager::getInstance().RemoveFactory(mT("ChatToFriend"));
		AI::GoalFactoryManager::getInstance().RemoveFactory(mT("GoToObject"));
		AI::GoalFactoryManager::getInstance().RemoveFactory(mT("PlayAnimation"));
	}
}



extern "C" CHARACTERDEMOGM_API IPlugin* dllCreatePlugin(){
	return new CharacterDemoGM();

}

}
}
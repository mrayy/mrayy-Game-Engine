
#include "stdafx.h"
#include "EditorGlobals.h"
#include "EditorEventManager.h"
#include "EditorEventMessages.h"

namespace mray
{


std::list<IToolBox*> EditorGlobals::Toolboxes;
bool EditorGlobals::IsEngineInitiated=false;

void EditorGlobals::RegisterToolBox(IToolBox*toolBox)
{
	Toolboxes.push_back(toolBox);
	EditorEventManager::getInstance().FireEvent(EEE_OnToolboxRegistered,toolBox);
}
void EditorGlobals::UnregisterToolBox(IToolBox*toolBox)
{
	std::list<IToolBox*>::iterator it=Toolboxes.begin();
	for(;it!=Toolboxes.end();++it){
		if(*it==toolBox){
			Toolboxes.remove(it);
			EditorEventManager::getInstance().FireEvent(EEE_OnToolboxUnregistered,toolBox);
			return;
		}
	}
}


}
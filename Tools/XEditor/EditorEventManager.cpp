
#include "stdafx.h"
#include "EditorEventManager.h"



namespace mray
{


EditorEventManager::EditorEventManager()
{
}

EditorEventManager::~EditorEventManager()
{
}


void EditorEventManager::FireEvent(uint e,PVOID data)
{
	m_events[e](data);
}


void EditorEventManager::AddHandler(uint event,EditorEventHandler h)
{
	m_events[event]+=h;
}


}

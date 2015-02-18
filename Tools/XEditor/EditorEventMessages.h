

/********************************************************************
	created:	2010/06/10
	created:	10:6:2010   16:18
	filename: 	i:\Programing\GameEngine\mrayEngine\Tools\Editor\EditorEventMessages.h
	file path:	i:\Programing\GameEngine\mrayEngine\Tools\Editor
	file base:	EditorEventMessages
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef EditorEventMessages_h__
#define EditorEventMessages_h__


enum EEditorEvents
{
	EEE_OnEngineInitiated,
	EEE_OnToolboxRegistered,
	EEE_OnToolboxUnregistered,
	EEE_OnModelSelected,
	EEE_OnLevelLoaded,
	EEE_OnCleanLevel,
	EEE_OnNodeAdded,
	EEE_OnNodeTrasnformingDone
};

#endif // EditorEventMessages_h__

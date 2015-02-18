

/********************************************************************
	created:	2010/06/10
	created:	10:6:2010   16:07
	filename: 	i:\Programing\GameEngine\mrayEngine\Tools\Editor\EditorEventManager.h
	file path:	i:\Programing\GameEngine\mrayEngine\Tools\Editor
	file base:	EditorEventManager
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef EditorEvent_h__
#define EditorEvent_h__

#include "IDelegate.h"

namespace mray
{

	typedef IDelegate1<void,PVOID>* EditorEventHandler;

class EditorEventManager:public ISingleton<EditorEventManager>
{
private:
protected:
	typedef std::map<uint,DelegateEvent1<PVOID>> EventMap;
	EventMap m_events;
public:
	EditorEventManager();
	virtual ~EditorEventManager();

	void FireEvent(uint e,PVOID data=0);
	
	void AddHandler(uint event,EditorEventHandler h);
};

}
#endif // EditorEvent_h__

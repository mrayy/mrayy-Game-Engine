
/********************************************************************
	created:	2009/06/19
	created:	19:6:2009   13:45
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\ScheduleManager.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	ScheduleManager
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___ScheduleManager___
#define ___ScheduleManager___

#include "CompileConfig.h"
#include "ISingleton.h"
#include "ScheduleItems.h"

#include "EventTimer.h"

namespace mray{

class MRAY_DLL ScheduleManager:public ISingleton<ScheduleManager>
{
private:
protected:
	typedef std::list<IScheduleItem*> ItemsList;
	typedef std::list<EventTimer*> TimerList;

	ItemsList m_items;
	TimerList m_timers;

	void addItem(IScheduleItem*item);

public:
	ScheduleManager();
	virtual~ScheduleManager();

	void schedule(float time,ScheduledFunction*func){
		addItem(new ScheduledFunctionItem(time,func));
	}

	template <class T>
	void schedule(float time,T* object,void(T::*e)()){
		addItem(new ScheduledMethodItem(time,object,e));
	}

	void ClearItems();

	void update(float dt);

	void registerTimer(EventTimer* timer);
	void removeTimer(EventTimer* timer);
};

}


#endif //___ScheduleManager___

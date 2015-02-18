
/********************************************************************
	created:	2009/06/19
	created:	19:6:2009   13:50
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\ScheduleItems.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	ScheduleItems
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___ScheduleItems___
#define ___ScheduleItems___

#include "GCPtr.h"

namespace mray{


	typedef void(* ScheduledFunction)();

class IScheduleItem
{
	float m_time;
public:
	
	IScheduleItem(float t){
		m_time =t;
	}
	virtual~IScheduleItem(){}

	virtual float timeToFire(){
		return m_time;
	}

	virtual void fireEvent()=0;

	virtual bool update(float dt){
		m_time-=dt;
		if(m_time<=0){
			m_time=0;
			return true;
		}
		return false;
	}
};

MakeSharedPtrType(IScheduleItem)

class ScheduledFunctionItem:public IScheduleItem
{
	ScheduledFunction* m_event;
public:
	ScheduledFunctionItem(float t,ScheduledFunction* e):IScheduleItem(t)
	{
		m_event=e;
	}
	virtual void fireEvent(){
		if(m_event){
			(*m_event)();
		}
	}
};

template <class T>
class ScheduledMethodItem:public IScheduleItem
{
public:
	typedef void(T::*TScheduledFunction)();

	ScheduledMethodItem(float t,T* object,TScheduledFunction* e):IScheduleItem(t)
	{
		m_object=object;
		m_event=e;
	}
	virtual void fireEvent(){
		if(m_object && m_event){
			m_object->(*m_event)();
		}
	}
protected:
	T* m_object;
	TScheduledFunction* m_event;
};

}


#endif //___ScheduleItems___


/********************************************************************
	created:	2009/03/28
	created:	28:3:2009   0:10
	filename: 	i:\Programing\GameEngine\mrayEngine\EaglesProject\IScenarioNode.h
	file path:	i:\Programing\GameEngine\mrayEngine\EaglesProject
	file base:	IScenarioNode
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___IScenarioNode___
#define ___IScenarioNode___

#include <mString.h>
#include <GCPtr.h>
#include <IEventReciver.h>

namespace mray{
namespace gameMod{

class IScenarioNode:public IEventReciver
{
private:
protected:
public:
	IScenarioNode()
	{
	}
	virtual~IScenarioNode(){
	}
	/*
	inline const core::string& getName()const
	{
		return m_name;
	}
	inline int getId()const
	{
		int m_id;
	}*/

	virtual void onEnter()=0;
	virtual void onExit()=0;

	virtual void onEvent(GCPtr<Event> event)=0;

	virtual void update(float dt)=0;
	
	// return the exit code !=0
	virtual int isDone()=0;
};

}
}


#endif //___IScenarioNode___

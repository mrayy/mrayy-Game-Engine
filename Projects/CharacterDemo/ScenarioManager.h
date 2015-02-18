
/********************************************************************
	created:	2009/03/28
	created:	28:3:2009   0:09
	filename: 	i:\Programing\GameEngine\mrayEngine\EaglesProject\ScenarioManager.h
	file path:	i:\Programing\GameEngine\mrayEngine\EaglesProject
	file base:	ScenarioManager
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	this class controls the updating of the active sub-node in run-time, and the transitions between them
*********************************************************************/

#ifndef ___ScenarioManager___
#define ___ScenarioManager___

#include <ISingleton.h>
#include <GCPtr.h>
#include <mrayFSM.h>
#include <map>
#include <IEventReciver.h>

#include "IScenarioNode.h"

namespace mray{
namespace gameMod{


class ScenarioManager:public ISingleton<ScenarioManager>,public IEventReciver
{
private:
protected:
	typedef std::map<core::string,GCPtr<IScenarioNode>> SenarioNodeMap;
	
	GCPtr<IScenarioNode> m_activeState;

	SenarioNodeMap m_senarioNodes;
	GCPtr<StateMachine> m_fsm;

public:
	ScenarioManager();
	virtual~ScenarioManager();

	void draw();
	void update(float dt);

	void addNode(GCPtr<IScenarioNode> node,const core::string&name);

	GCPtr<IScenarioNode> getActiveNode();

	virtual void onEvent(GCPtr<Event> event);

	void setInitialState(const core::string&name);
	void addTransition(const core::string&a,const core::string&b,int code);
};

}
}


#endif //___ScenarioManager___


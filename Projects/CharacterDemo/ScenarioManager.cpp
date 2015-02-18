
#include "stdafx.h"
#include "ScenarioManager.h"
#include "SenarioState.h"


namespace mray{
namespace gameMod{

	class COnDoneCondition:public ICondition
	{
	protected:
		int m_code;
	public:
		COnDoneCondition(const core::string&name,int code):ICondition(name)
		{
			m_code=code;
		}
		virtual ~COnDoneCondition(){

		}
		virtual bool checkCondition(){
			if(ScenarioManager::getInstance().getActiveNode())
				return ScenarioManager::getInstance().getActiveNode()->isDone()==m_code;
			return false;
		}
	};

	//////////////////////////////////////////////////////////////////////////

ScenarioManager::ScenarioManager(){
	m_fsm=new StateMachine();
	if(EventQueue::isExist()){
		EventQueue::getInstance().addEventHandler(this,
			Event::getEventMaskingValue(ET_Mouse)|
			Event::getEventMaskingValue(ET_Keyboard)|
			Event::getEventMaskingValue(ET_GuiEvent));
	}
}
ScenarioManager::~ScenarioManager(){
	if(EventQueue::isExist()){
		EventQueue::getInstance().removeEventHandler(this);
	}
}

void ScenarioManager::draw()
{
	IState*s=m_fsm->getActiveState();
	if(!s)
		return;
	SenarioState*st=dynamic_cast<SenarioState*>(s);
	if(st)
		st->getNode()->draw();
}
void ScenarioManager::update(float dt){
	m_fsm->onUpdate();
	IState*s=m_fsm->getActiveState();
	if(!s)
		return;
	SenarioState*st=dynamic_cast<SenarioState*>(s);
	if(st)
		m_activeState=st->getNode();
	else
		m_activeState=0;
}
GCPtr<IScenarioNode> ScenarioManager::getActiveNode(){
	return m_activeState;
}

void ScenarioManager::addNode(GCPtr<IScenarioNode> node,const core::string&name){
	if(!node)
		return;
	m_senarioNodes[name]=node;
	m_fsm->addState(new SenarioState(node,name));
}

void ScenarioManager::setInitialState(const core::string&name){
	m_fsm->setActiveState(name);
	IState*s=m_fsm->getActiveState();
	if(!s)
		return;
	SenarioState*st=dynamic_cast<SenarioState*>(s);
	if(st)
		m_activeState=st->getNode();
	else
		m_activeState=0;
}
void ScenarioManager::addTransition(const core::string&a,const core::string&b,int code){
	core::string name=mT("Trans");
	name+=code;
	COnDoneCondition*cond= new COnDoneCondition(name,code);
	if(!m_fsm->addCondition(cond))
		delete cond;
	m_fsm->addTransition(a,b,name);
}

void ScenarioManager::onEvent(GCPtr<Event> event){
	if(m_activeState){
		m_activeState->onEvent(event);
		if(event->getType()==ET_GuiEvent){
			bool ok=1;
		}
	}
}

}
}


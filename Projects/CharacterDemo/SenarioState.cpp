
#include "stdafx.h"
#include "SenarioState.h"



namespace mray{
namespace gameMod{

SenarioState::SenarioState(GCPtr<IScenarioNode> node,const core::string&name):
	IState(name)
{
	m_node=node;
}
SenarioState::~SenarioState(){
	m_node=0;
}

void SenarioState::onEnter(IState*prev){
	if(m_node)
		m_node->onEnter();
}
void SenarioState::onUpdate(){
	if(m_node){
		m_node->update(gFPS.dt());
	}
}
void SenarioState::onExit(){
	if(m_node)
		m_node->onExit();
}

}
}
#include "stdafx.h"

#include "TriggerSystem.h"
#include "AISystem.h"


namespace mray{
namespace AI{


TriggerSystem::TriggerSystem(AISystem *sys):m_AiSystem(sys)
{
}
TriggerSystem::~TriggerSystem()
{
}

void TriggerSystem::SetAISystem(AISystem*system)
{
	m_AiSystem=system;
}
AISystem* TriggerSystem::GetAISystem()
{
	return m_AiSystem;
}

void TriggerSystem::AddTrigger(GCPtr<ITrigger> t)
{
	m_triggers.push_back(t);
}
void TriggerSystem::RemoveTrigger(ITrigger* t)
{
	TriggerList::iterator it=m_triggers.begin();
	for(;it!=m_triggers.end();++it){
		ITrigger* tr=*it;
		if(tr==t){
			m_triggers.erase(it);
			return;
		}
	}
}

void TriggerSystem::Update()
{
	TriggerList::iterator it=m_triggers.begin();
	while(it!=m_triggers.end()){
		ITrigger* t=*it;
		if(t->ShouldBeRemoved()){
			it=m_triggers.erase(it);
		}else{
			if(t->IsActive())
				t->TestTrigger();
			++it;
		}
	}
}


}
}


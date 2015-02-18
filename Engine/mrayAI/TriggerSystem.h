
/********************************************************************
	created:	2010/03/17
	created:	17:3:2010   16:53
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayAI\TriggerSystem.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayAI
	file base:	TriggerSystem
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___TriggerSystem___
#define ___TriggerSystem___

#include "ITrigger.h"
#include "GCPtr.h"
#include <vector>

namespace mray{
namespace AI{

class AISystem;

class TriggerSystem
{
private:
protected:
	AISystem *m_AiSystem;
	typedef std::vector<GCPtr<ITrigger>> TriggerList;
	TriggerList m_triggers;

public:
	TriggerSystem(AISystem *sys);
	virtual~TriggerSystem();

	void SetAISystem(AISystem*system);
	AISystem* GetAISystem();

	void AddTrigger(GCPtr<ITrigger> t);
	void RemoveTrigger(ITrigger* t);

	void Update();

};

}
}


#endif //___TriggerSystem___

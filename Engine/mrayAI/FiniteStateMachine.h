

/********************************************************************
	created:	2010/04/28
	created:	28:4:2010   18:00
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayAI\FiniteStateMachine.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayAI
	file base:	FiniteStateMachine
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef FiniteStateMachine_h__
#define FiniteStateMachine_h__

#include "CompileConfig.h"
#include "IAIComponent.h"
#include "IDelegate.h"
#include <map>
#include <vector>

namespace mray
{
namespace AI
{

	typedef IDelegate1<void,core::string>* FSMStateTriggerDelegate;

struct FSMState
{
	FSMState():id(-1),isDirty(true)
	{}

	int id;
	core::string name;
	bool isDirty;//need to refresh transition list right side (states ids)

	typedef std::map<core::string,int> TransitionIDMap;
	typedef std::map<core::string,core::string> TransitionMap;
	TransitionMap transitions;
	TransitionIDMap transitions_id;
};

class MRAY_AI_DLL FiniteStateMachine:public IAIComponent
{
	DECLARE_RTTI
private:
protected:


	typedef std::map<core::string,int> StringIntMap;


	typedef std::map<core::string,int> StatesIDMap;
	typedef std::vector<FSMState> StatesList;
	StatesIDMap m_statesIDs;
	StatesList m_states;
	int m_activeState;

	void UpdateStateTransitions(FSMState& s);

public:
	DelegateEvent1<core::string> OnTransition;

	FiniteStateMachine();
	virtual ~FiniteStateMachine();

	void SetActiveState(const core::string& name);
	core::string GetCurrentState();

	virtual void Update(float dt);

	void AddState(const FSMState& s);

	void Tick(const core::string& symbol);

	void loadXMLTransElem(xml::XMLElement* elem,FSMState&s);
	bool loadXMLStateElem(xml::XMLElement* elem);
	virtual xml::XMLElement* loadXMLSettings(xml::XMLElement* elem);
};

}
}

#endif // FiniteStateMachine_h__


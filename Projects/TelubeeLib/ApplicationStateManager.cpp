


#include "Stdafx.h"
#include "ApplicationStateManager.h"
#include "IApplicationState.h"
#include "BaseAppState.h"

#include <StateMachine.h>



namespace mray
{
class StateMachine;

namespace TBee
{

class CAppOnDoneCondition :public ICondition
{
protected:
	int m_code;
	StateMachine* m_machine;
public:
	CAppOnDoneCondition(StateMachine*m, const core::string&name, int code) :ICondition(name)
	{
		m_machine = m;
		m_code = code;
	}
	virtual ~CAppOnDoneCondition(){

	}
	virtual bool checkCondition()
	{
		BaseAppState* s = (BaseAppState*)m_machine->getActiveState();
		if (s && s->GetState())
			return s->GetState()->GetExitCode() == m_code;
		return false;
	}
};

ApplicationStateManager::ApplicationStateManager()
{
	m_stateMachine = new StateMachine();
	m_stateMachine->AddListener(this);
}

ApplicationStateManager::~ApplicationStateManager()
{
	delete m_stateMachine;
}

void ApplicationStateManager::OnStateChanged(StateMachine*, IState* oldS, IState* newS)
{
	FIRE_LISTENR_METHOD(OnChangeState, ((IApplicationState*)newS));
}

void ApplicationStateManager::AddState(IApplicationState* st,const core::string&name)
{
	if(!st)
		return;
	m_stateMachine->addState(new BaseAppState(name,st));
}


IApplicationState* ApplicationStateManager::GetActiveState()
{
	BaseAppState* s=(BaseAppState*) m_stateMachine->getActiveState();
	if(!s)
		return 0;

	return s->GetState();
}

bool ApplicationStateManager::OnEvent(Event* e, const math::rectf& rc)
{
	BaseAppState* s=(BaseAppState*)m_stateMachine->getActiveState();
	if(!s)
		return false;


	return s->GetState()->OnEvent(e,rc);
}


void ApplicationStateManager::SetInitialState(const core::string&name)
{
	m_stateMachine->setActiveStateByName(name);
}

void ApplicationStateManager::AddTransition(const core::string&a,const core::string&b,int code)
{
	core::string name=mT("Trans@")+a+"_"+b+"#";
	name+=core::StringConverter::toString(code);
	CAppOnDoneCondition*cond= new CAppOnDoneCondition(m_stateMachine,name,code);
	if(!m_stateMachine->addCondition(cond))
		delete cond;
	m_stateMachine->addTransitionByName(a,b,name);
}



void ApplicationStateManager::Update(float dt)
{
	BaseAppState* s=(BaseAppState*)m_stateMachine->getActiveState();
	if(!s)
		return;

	s->GetState()->OnUpdate(dt);

	m_stateMachine->onUpdate();
}

void ApplicationStateManager::Draw(const math::rectf& rc,video::IRenderTarget* rt,ETargetEye eye)
{
	BaseAppState* s=(BaseAppState*)m_stateMachine->getActiveState();
	if(!s)
		return;
	s->GetState()->OnDraw(rc,rt,eye);
}

}
}

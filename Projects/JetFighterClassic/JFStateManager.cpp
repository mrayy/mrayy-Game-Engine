


#include "stdafx.h"
#include "JFStateManager.h"

#include "IGameState.h"
#include "StateMachine.h"
#include "JFBaseState.h"

namespace mray
{
namespace game
{

	class COnDoneCondition:public ICondition
	{
	protected:
		int m_code;
		JFStateManager* m_mngr;
	public:
		COnDoneCondition(JFStateManager*m,const core::string&name,int code):ICondition(name)
		{
			m_mngr=m;
			m_code=code;
		}
		virtual ~COnDoneCondition(){

		}
		virtual bool checkCondition(){
			if(m_mngr->GetActiveState())
				return m_mngr->GetActiveState()->GetExitCode()==m_code;
			return false;
		}
	};

JFStateManager::JFStateManager()
{
	m_machine=new StateMachine();
}

JFStateManager::~JFStateManager()
{
	delete m_machine;
}


void JFStateManager::Update(float dt)
{
	m_machine->onUpdate();
	IGameState* st=GetActiveState();
	if(st)
		st->Update(dt);
}


void JFStateManager::AddState(IGameState* st,const core::string&name)
{
	m_machine->addState(new JFBaseState(name,st));
}


IGameState* JFStateManager::GetActiveState()
{
	JFBaseState* st=(JFBaseState*)m_machine->getActiveState();
	if(!st)
		return 0;
	 return st->GetState();
}


void JFStateManager::OnEvent(Event* e)
{
	IGameState* st=GetActiveState();
	if(st)
		st->OnEvent(e);
}


void JFStateManager::SetInitialState(const core::string&name)
{
	m_machine->setActiveState(name);
}

void JFStateManager::AddTransition(const core::string&a,const core::string&b,int code)
{
	core::string name=mT("Trans@");
	name+=core::StringConverter::toString(code);
	COnDoneCondition*cond= new COnDoneCondition(this,name,code);
	if(!m_machine->addCondition(cond))
		delete cond;
	m_machine->addTransition(a,b,name);
}



}
}
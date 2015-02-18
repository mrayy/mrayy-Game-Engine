

/********************************************************************
	created:	2012/10/10
	created:	10:10:2012   10:55
	filename: 	d:\Development\mrayEngine\Projects\TelubeeLib\ApplicationStateManager.h
	file path:	d:\Development\mrayEngine\Projects\TelubeeLib
	file base:	ApplicationStateManager
	file ext:	h
	author:		MHD YAMEN SARAIJI
	
	purpose:	
*********************************************************************/
#ifndef ___ApplicationStateManager___
#define ___ApplicationStateManager___

#include "IRenderingState.h"
#include "StateMachine.h"


namespace mray
{

namespace TBee
{

	class IApplicationState;

	class IApplicationStateManagerListener
	{
	public:
		virtual void OnChangeState(IApplicationState*state){}
	};

class ApplicationStateManager:public ListenerContainer<IApplicationStateManagerListener*>,public IStateMachineListener
{
protected:
	StateMachine* m_stateMachine;

	DECLARE_FIRE_METHOD(OnChangeState, (IApplicationState*state), (state));

	virtual void OnStateChanged(StateMachine*, IState* oldS, IState* newS);
public:
	ApplicationStateManager();
	virtual~ApplicationStateManager();
	
	void AddState(IApplicationState* st,const core::string&name);

	IApplicationState* GetActiveState();

	virtual bool OnEvent(Event* e, const math::rectf& rc);

	void SetInitialState(const core::string&name);
	void AddTransition(const core::string&a,const core::string&b,int code);

	void Update(float dt);
	void Draw(const math::rectf& rc,video::IRenderTarget* rt,ETargetEye eye);

	
};

}
}

#endif

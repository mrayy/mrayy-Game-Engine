


/********************************************************************
	created:	2012/05/17
	created:	17:5:2012   11:57
	filename: 	d:\Development\mrayEngine\Projects\JetFighterClassic\JFStateManager.h
	file path:	d:\Development\mrayEngine\Projects\JetFighterClassic
	file base:	JFStateManager
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __JFStateManager__
#define __JFStateManager__


namespace mray
{
	class StateMachine;
namespace game
{
	class IGameState;


class JFStateManager
{
private:
protected:
	StateMachine* m_machine;
public:
	JFStateManager();
	virtual~JFStateManager();

	void Update(float dt);

	void AddState(IGameState* st,const core::string&name);

	IGameState* GetActiveState();

	virtual void OnEvent(Event* e);

	void SetInitialState(const core::string&name);
	void AddTransition(const core::string&a,const core::string&b,int code);
};


}
}

#endif

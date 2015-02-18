


/********************************************************************
	created:	2011/06/06
	created:	6:6:2011   13:23
	filename: 	d:\Development\mrayEngine\Projects\BallanceBall\ScenarioState.h
	file path:	d:\Development\mrayEngine\Projects\BallanceBall
	file base:	ScenarioState
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __ScenarioState__
#define __ScenarioState__

#include "IState.h"

namespace mray
{
namespace Ball
{

	class IGameState;

class ScenarioState:public IState
{
private:
protected:
	IGameState* m_state;
public:
	ScenarioState(const core::string& name,IGameState* s);
	virtual~ScenarioState();

	IGameState* GetGameState(){return m_state;}

	virtual void onEnter(IState*prev);
	virtual void onUpdate();
	virtual void onExit();

};


}
}

#endif

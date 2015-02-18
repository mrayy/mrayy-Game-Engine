

/********************************************************************
	created:	2012/05/17
	created:	17:5:2012   11:52
	filename: 	d:\Development\mrayEngine\Projects\JetFighterClassic\JFBaseState.h
	file path:	d:\Development\mrayEngine\Projects\JetFighterClassic
	file base:	JFBaseState
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __JFBaseState__
#define __JFBaseState__

#include "IState.h"

namespace mray
{
namespace game
{

	class IGameState;

class JFBaseState:public IState
{
private:
protected:
	IGameState* m_state;
public:
	JFBaseState(const core::string&name,IGameState* st):IState(name),m_state(st){}
	virtual~JFBaseState();

	IGameState* GetState(){return m_state;}

	virtual void onEnter(IState*prev);
	virtual void onUpdate();
	virtual void onExit();
};

}
}

#endif


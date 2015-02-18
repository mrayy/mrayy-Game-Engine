



/********************************************************************
	created:	2012/05/15
	created:	15:5:2012   19:19
	filename: 	d:\Development\mrayEngine\Projects\JetFighterClassic\IGameState.h
	file path:	d:\Development\mrayEngine\Projects\JetFighterClassic
	file base:	IGameState
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __IGameState__
#define __IGameState__

namespace mray
{
namespace game
{

class IGameState
{
private:
protected:
public:
	IGameState()
	{}
	virtual~IGameState(){}

	virtual core::string GetStateName()=0;

	virtual void OnEvent(Event* e){}

	virtual void OnEnter(){}
	virtual void Update(float dt){}
	virtual void OnExit(){}

	virtual uint GetExitCode(){return 0;}

};


}
}

#endif

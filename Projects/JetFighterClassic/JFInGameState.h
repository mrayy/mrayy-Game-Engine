


/********************************************************************
	created:	2012/05/15
	created:	15:5:2012   19:40
	filename: 	d:\Development\mrayEngine\Projects\JetFighterClassic\JFInGameState.h
	file path:	d:\Development\mrayEngine\Projects\JetFighterClassic
	file base:	JFInGameState
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __JFInGameState__
#define __JFInGameState__

#include "IGameState.h"

namespace mray
{
namespace game
{
	class JFGameLevelManager;

class JFInGameState:public IGameState
{
private:
protected:
	JFGameLevelManager* m_gameManager;

	int m_exitCode;

public:

	static const core::string StateName;

	virtual core::string GetStateName(){return StateName;}


	JFInGameState();
	virtual~JFInGameState();

	virtual void OnEvent(Event* e);

	virtual void OnEnter();
	virtual void Update(float dt);
	virtual void OnExit();

	virtual uint GetExitCode();

};

}
}

#endif

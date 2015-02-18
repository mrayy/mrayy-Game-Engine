


/********************************************************************
	created:	2012/05/17
	created:	17:5:2012   11:31
	filename: 	d:\Development\mrayEngine\Projects\JetFighterClassic\JFMainMenuState.h
	file path:	d:\Development\mrayEngine\Projects\JetFighterClassic
	file base:	JFMainMenuState
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __JFMainMenuState__
#define __JFMainMenuState__

#include "IGameState.h"

namespace mray
{
namespace GUI
{
	class GUIManager;
}
namespace game
{

class JFMainMenuState:public IGameState
{
private:
protected:
	int m_exitCode;

	GUI::GUIManager* m_guiManager;

	void _initLayout();

	void OnStartGameBtn(const PVOID& sender,const PVOID& arg);
public:
	static const core::string StateName;

	virtual core::string GetStateName(){return StateName;}

	JFMainMenuState();
	virtual~JFMainMenuState();

	virtual void OnEvent(Event* e);

	virtual void OnEnter();
	virtual void Update(float dt);
	virtual void OnExit();

	virtual uint GetExitCode();

};

}
}

#endif

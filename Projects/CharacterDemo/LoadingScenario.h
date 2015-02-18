
/********************************************************************
	created:	2009/04/09
	created:	9:4:2009   23:56
	filename: 	i:\Programing\GameEngine\mrayEngine\EaglesProject\src\LoadingScenario.h
	file path:	i:\Programing\GameEngine\mrayEngine\EaglesProject\src
	file base:	LoadingScenario
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___LoadingScenario___
#define ___LoadingScenario___

#include "IScenarioNode.h"
#include <GUIManager.h>
#include <ISoundManager.h>
#include <IResLoadListener.h>
#include <GUILoadingBar.h>
#include <GUIStaticText.h>
#include <GUIButton.h>

namespace mray{
namespace gameMod{

class LoadingScenario:public IScenarioNode,public IResLoadListener
{
private:
protected:
	GCPtr<GUI::GUIManager> m_guiMngr;
	GCPtr<GUI::GUIManager> m_oldGuiMngr;

	GCPtr<sound::ISoundManager> m_sndMngr;
	GCPtr<sound::ISoundManager> m_oldSndMngr;

	GCPtr<sound::ISound> m_bgSound;

	GCPtr<GUI::GUILoadingBar> m_loadingBar;
	GCPtr<GUI::GUIStaticText> m_info;
	GCPtr<GUI::GUIButton> m_proceedBtn;

	bool m_done;

public:
	LoadingScenario();
	virtual~LoadingScenario();

	virtual void update(float dt);

	virtual void onEvent(GCPtr<Event> event);

	virtual int isDone();

	virtual void onEnter();

	virtual void onExit();


	virtual void beginLoadResources(const core::string& category,int count);

	virtual void onResourceLoadStart(const core::string&name);
	virtual void onResourceLoadDone(const core::string&name);

	// called when there is an info need to be sayed
	virtual void resourceInfo(const core::string&info);

	virtual void loadDone();
};

}
}


#endif //___LoadingScenario___

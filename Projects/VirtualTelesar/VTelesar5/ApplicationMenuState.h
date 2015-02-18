


/********************************************************************
	created:	2012/10/29
	created:	29:10:2012   18:26
	filename: 	d:\Development\mrayEngine\Projects\VirtualTelesar\VTelesar5\ApplicationMenuState.h
	file path:	d:\Development\mrayEngine\Projects\VirtualTelesar\VTelesar5
	file base:	ApplicationMenuState
	file ext:	h
	author:		MHD YAMEN SARAIJI
	
	purpose:	
*********************************************************************/
#ifndef ___ApplicationMenuState___
#define ___ApplicationMenuState___


#include "IApplicationState.h"
#include "IGUIManager.h"


namespace mray
{
	namespace TBee
	{
		class RenderingStateManager;
	}

	class GUIMainMenu;
namespace VT
{

class ApplicationMenuState :public TBee::IApplicationState, public IDelegateContainer
{
protected:
	TBee::RenderingStateManager* m_renderingManager;
	GCPtr<GUI::IGUIManager> m_guiManager;


	GUIMainMenu* m_mainMenu;

	DECLARE_OBJECT_DELEGATE(StartDemoEvent);

 public:
	ApplicationMenuState();
	virtual~ApplicationMenuState();

	void LoadScenarioFile(const core::string& fname);

	void OnEvent(Event* e);
	void OnEnter(TBee::IApplicationState* prev);
	void OnExit();

	void OnUpdate(float dt);
	void OnDraw(bool left,const math::rectf& rc,video::IRenderTarget* rt);

};

}
}

#endif

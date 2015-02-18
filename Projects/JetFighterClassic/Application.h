


/********************************************************************
	created:	2012/05/15
	created:	15:5:2012   13:47
	filename: 	d:\Development\mrayEngine\Projects\JetFighterClassic\Application.h
	file path:	d:\Development\mrayEngine\Projects\JetFighterClassic
	file base:	Application
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __Application__
#define __Application__


namespace mray
{

namespace game
{
	class JFStateManager;
}

class Application:public CMRayApplication
{
private:
protected:
	game::JFStateManager* m_stateManager;
	GCPtr<GUI::GUIBatchRenderer> m_guiRender;

	GUI::IGUIManager* m_activeGUIManager;

protected:
	void _initStateMachine();
	void _initResources();
	void _initDefaultViewport();
public:
	Application();
	virtual~Application();

	virtual void init(const OptionContainer &extraOptions);

	virtual void onEvent(Event* event);

	virtual void draw(scene::ViewPort* vp);
	virtual void update(float dt);

	virtual void WindowPostRender(video::RenderWindow* wnd);

	void SetActiveGUIManager(GUI::IGUIManager* m)
	{
		m_activeGUIManager=m;
	}
};


}

#endif



/********************************************************************
	created:	2014/03/24
	created:	24:3:2014   1:38
	filename: 	C:\Development\mrayEngine\Projects\TEDxTokyo\TwitterVisualizer\Application.h
	file path:	C:\Development\mrayEngine\Projects\TEDxTokyo\TwitterVisualizer
	file base:	Application
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __Application__
#define __Application__


#include <cmrayapplication.h>
#include "IRenderingScene.h"
#include "ISoundManager.h"

namespace mray
{
class Application :public CMRayApplication
{
protected:
	ted::IRenderingScene* m_scene;
	scene::ViewPort* m_mainVP;

	GCPtr<sound::ISoundManager> m_soundManager;
public:
	Application();
	virtual~Application();

	virtual void onEvent(Event* event);

	virtual void init(const OptionContainer &extraOptions);

	virtual void update(float dt);

	virtual void WindowPostRender(video::RenderWindow* wnd);

	virtual void onDone();


};

}


#endif

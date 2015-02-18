

/********************************************************************
	created:	2009/09/29
	created:	29:9:2009   19:18
	filename: 	i:\Programing\GameEngine\mrayEngine\testFrameWork\TestProject\Application.h
	file path:	i:\Programing\GameEngine\mrayEngine\testFrameWork\TestProject
	file base:	Application
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___Application___
#define ___Application___

#include <CMRayApplication.h>

namespace mray{

	class GameEntityManager;
	class IExampleScene;

class Application:public CMRayApplication
{
private:

	static const int BaseExampleButtonID=0x100;

protected:

	typedef std::vector<GCPtr<IExampleScene>> ExampleList;

	ExampleList m_examples;
	IExampleScene* m_activeExample;

	GCPtr<GUI::GUIElement> m_rootGUI;
	GCPtr<GUI::GUIWindow> m_exampleSelectWnd;

	GCPtr<GUI::GUIStaticText> m_descriptionTxt;

	GCPtr<GUI::IFont> m_font;
	GCPtr<scene::SCameraNode> m_camera;
	GCPtr<scene::ViewPort> m_viewPort;

	GCPtr<GameEntityManager> m_gameEntManager;
	GCPtr<physics::IPhysicManager> m_phManager;

	void createGUI();

public:
	Application();
	virtual~Application();

	virtual void onEvent(GCPtr<Event> event);

	virtual void init(core::array<SOptionElement> *extraOptions);
	virtual void update(float dt);
	virtual void draw();
	virtual void onDone();

	GameEntityManager* getEntityManager();
	scene::SCameraNode* getCamera();
};

}


#endif //___Application___

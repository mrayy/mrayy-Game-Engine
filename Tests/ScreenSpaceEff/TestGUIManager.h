

/********************************************************************
	created:	2011/01/09
	created:	9:1:2011   21:31
	filename: 	i:\Programing\GameEngine\mrayEngine\testFrameWork\ScreenSpaceEff\TestGUIManager.h
	file path:	i:\Programing\GameEngine\mrayEngine\testFrameWork\ScreenSpaceEff
	file base:	TestGUIManager
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef TestGUIManager_h__
#define TestGUIManager_h__

#include "IGUIManager.h"

#include <list>

namespace mray
{
namespace GUI
{

class TestGUIManager:public IGUIManager
{
private:
protected:
	IGUIRenderer* m_renderQueue;
public:
	TestGUIManager(video::IVideoDevice*dev);
	virtual ~TestGUIManager();


	virtual void		SetActiveTheme(IGUITheme* theme){}
	virtual IGUITheme*	GetActiveTheme(){return 0;}

	virtual LanguageFontSpecification* GetLanguageSpecification(){return 0;}

	virtual void RegisterBenchmark(){}
	virtual void UnregisterBenchmark(){}

	virtual IGUIRenderer* GetRenderQueue(){return m_renderQueue;}

	virtual IGUIElement*	GetElementFromPoint(float x,float y){return 0;}

	virtual void OnEvent(Event* event);
	virtual void DrawAll();

	virtual void Update(float dt);

	virtual IGUIElement* CreateElement(const GUID& type);

	virtual void SetEventListener(IEventReciver*l){}
};

}
}

#endif // TestGUIManager_h__

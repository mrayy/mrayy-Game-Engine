

/********************************************************************
	created:	2013/01/28
	created:	28:1:2013   19:55
	filename: 	C:\Development\mrayEngine\Projects\TELUBee\InMapRenderState.h
	file path:	C:\Development\mrayEngine\Projects\TELUBee
	file base:	InMapRenderState
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __InMapRenderState__
#define __InMapRenderState__


#include "IRenderingState.h"
#include "IRenderTarget.h"
#include "MapObject.h"

namespace mray
{
	namespace GUI
	{
		class GUIPageManager;
		class IGUIManager;
		class GUIWebPageControl;
		class GUIRobotPanelImpl;

	}
	namespace scene
	{
		class GUIManagerRT;	
		class ISceneManager;
		class ViewPort;
	}
	namespace game
	{
		class GameEntityManager;
	}
	class GUIMapScreen;
namespace TBee
{
	class MapObject;
	class IMapController;

#define BackToTile_Code 0x200
#define ConnectToRobot_Code 0x201

class InMapRenderState:public IRenderingState,public IMapListener
{
protected:
	GUI::GUIPageManager* m_pageManager;
	GUI::GUIWebPageControl* m_mapWebpage;
	GUI::IGUIManager* m_mapGUIManager;
	scene::GUIManagerRT* m_mapGUIRT;
	game::GameEntityManager* m_entManager;

	GUI::IGUIManager* m_guiManager;
	GUIMapScreen* m_mapScreenGUI;

	scene::ISceneManager* m_sceneManager;

	scene::ViewPort* m_viewport;

	MapObject* m_mapObject;
	IMapController* m_mapController;

	GUI::GUIRobotPanelImpl* m_robotPanel;

	std::vector<scene::ISceneNode*> m_indicatorObjs;
	bool m_indicatorVisible;

	void SwitchIndicatorVisibility();

	void PanMap(int dx,int dy);

	float m_time;
	bool m_dropedMarkers;

public:
	InMapRenderState();
	virtual~InMapRenderState();

	virtual void InitState();

	virtual void OnEvent(Event* e);
	virtual void OnEnter(IRenderingState*prev);
	virtual void OnExit();
	virtual void Update(float dt);
	virtual video::IRenderTarget* Render(const math::rectf& rc,ETargetEye eye);
	virtual video::IRenderTarget* GetLastFrame(ETargetEye eye);

	virtual void OnMarkerlClicked(MapObject* map,float x,float y,int id);
	virtual void OnMarkerlHovered(MapObject* map,float x,float y,int id);
	virtual void OnMarkerlOut(MapObject* map,float x,float y,int id);

	virtual void LoadFromXML(xml::XMLElement* e);
};

}
}

#endif

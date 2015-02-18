#ifndef __GUIScreenLayout_V2__
#define __GUIScreenLayout_V2__
#include "IGUISchemeBase.h"
#include "GUICommentsLayer.h"
#include "GUIFacultyList.h"
#include "GUILayersContainer.h"
#include "GUIPanel.h"
#include "GUISceneSpacePanel.h"
#include "GUISessionSidePanel.h"
#include "GUISpeakerDetailsPanel.h"
#include "GUIStatusLayer.h"
namespace mray{

using namespace GUI;
class GUIScreenLayout_V2:public GUI::IGUISchemeBase
{

public:
	GUIPanel* Root;
	GUILayersContainer* LayersRoot;
	GUIPanel* WallpaperLayer;
	GUISceneSpacePanel* ScenePanel;
	GUIStatusLayer* StatusPanel;
	GUICommentsLayer* CommentsPanel;
	GUISessionSidePanel* SessionsBar;
	GUISpeakerDetailsPanel* SessionDetails;
	GUIFacultyList* FacultyList;

public:

	GUIScreenLayout_V2():Root(0),LayersRoot(0),WallpaperLayer(0),ScenePanel(0),StatusPanel(0),CommentsPanel(0),SessionsBar(0),SessionDetails(0),FacultyList(0)
	{		
		m_elementsMap["Root"]=(IGUIElement**)&Root;
		m_elementsMap["LayersRoot"]=(IGUIElement**)&LayersRoot;
		m_elementsMap["WallpaperLayer"]=(IGUIElement**)&WallpaperLayer;
		m_elementsMap["ScenePanel"]=(IGUIElement**)&ScenePanel;
		m_elementsMap["StatusPanel"]=(IGUIElement**)&StatusPanel;
		m_elementsMap["CommentsPanel"]=(IGUIElement**)&CommentsPanel;
		m_elementsMap["SessionsBar"]=(IGUIElement**)&SessionsBar;
		m_elementsMap["SessionDetails"]=(IGUIElement**)&SessionDetails;
		m_elementsMap["FacultyList"]=(IGUIElement**)&FacultyList;

	}

};
}
#endif

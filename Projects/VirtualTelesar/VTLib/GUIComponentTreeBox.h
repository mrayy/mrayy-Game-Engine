


/********************************************************************
	created:	2013/05/08
	created:	8:5:2013   16:22
	filename: 	C:\Development\mrayEngine\Projects\VirtualTelesar\VTLib\GUIComponentTreeBox.h
	file path:	C:\Development\mrayEngine\Projects\VirtualTelesar\VTLib
	file base:	GUIComponentTreeBox
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __GUIComponentTreeBox__
#define __GUIComponentTreeBox__

#include "GUITreeBox.h"


namespace mray
{
namespace game
{
	class GameEntity;
	class IGameComponent;
}
namespace GUI
{

class GUIComponentTreeBox:public GUITreeBox
{
	DECLARE_RTTI;
protected:
	std::vector<video::ImageSetPtr> m_imageSets;
	game::GameEntity* m_ent;
	void _AddNode(GUITreeNode* node,game::IGameComponent* c);
public:
	GUIComponentTreeBox(IGUIManager*mngr);
	virtual~GUIComponentTreeBox();

	void SetEntity(game::GameEntity* ent);

	void AddImageSet(video::ImageSetCRef imageSet);
};

}
}


#endif

#ifndef ___GUIMenu___
#define ___GUIMenu___


#include "mString.h"
#include "mArray.h"
#include "GUIContextMenu.h"
#include "GUIElement.h"

namespace mray{
namespace GUI{

class MRAY_DLL GUIMenu:public GUIElement
{
#define SEPERATOR_WIDTH 2
	struct SMenu{
		core::string Caption;
		GCPtr<GUIContextMenu> menu;
		int posX;
		math::vector2d size;
		bool isSeperator;
		bool enabled;
	};
	std::vector<SMenu> menus;
	int selectedMenu;
	bool selecting;
	FontAttributes m_FontAttributs;

	int getMenuFromPos(math::vector2d p);
	void recalcSize();
public:
	DECLARE_RTTI;
	
	GUIMenu();
	GUIMenu(GUIManager* mngr,IEventReciver* event=0,
		GCPtr<GUIElement>  parent=0,video::SColor color=240,int id=0);

	virtual ~GUIMenu();

	virtual void draw(float dt);
	virtual void onEvent(const EventPtr&  event);

	int addMenu(const  mchar*Caption,bool seperator=0,bool enabled=1,
		GUIContextMenu*subMenu=0);
	void removeMenu(int i);
	void clearAllMenus();
	
	int getMenusCount();

	int getSelectedMenu();
	void setMenuCaption(int i,const mchar*Caption);

	void setSubMenu(int i,GCPtr<GUIContextMenu>subMenu);
	GCPtr<GUIContextMenu>getSubMenu(int i);

	void setMenuEnable(int i,bool enabled);
	bool isMenuEnable(int i);
	virtual FontAttributes* getFontAttributes();

};

}
}


#endif













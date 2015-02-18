
#ifndef ___GUIContextMenu___
#define ___GUIContextMenu___

#include "mString.h"
#include "mArray.h"
#include "GUIElement.h"

namespace mray{
namespace GUI{

class MRAY_DLL GUIContextMenu:public GUIElement
{
#define SEPERATOR_HEIGHT 2
	struct SMenuItem{
		core::string Caption;
		bool isSeperator;
		bool enabled;
		bool isSubMenu;
		int posY;
		math::Point2df size;
		GUIContextMenu*subMenu;
		int itemId;
	};
	std::vector<SMenuItem> items;
	int selectedItem;
	FontAttributes m_FontAttributes;

	int getItemFromPos(math::vector2d p);
	void recalcSize();
public:
	DECLARE_RTTI;

	GUIContextMenu();
	GUIContextMenu(GUIManager*mngr,IEventReciver* event=0,
		GCPtr<GUIElement>  parent=0,video::SColor color=240,int id=0);

	virtual ~GUIContextMenu();

	virtual void draw(float dt);
	virtual void onEvent(const EventPtr&  event);

	int addItem(const  mchar*Caption,int itemID=0,bool seperator=0,bool enabled=1,bool isSubMenu=0,
		GUIContextMenu*subMenu=0);
	void removeItem(int i);
	void removeAllItems();
	int getItemCount();

	int getSelectedItem();

	void setItemCaption(int i,const mchar*Caption);

	void setSubMenu(int i,GUIContextMenu*subMenu);
	GUIContextMenu*getSubMenu(int i);

	const SMenuItem *getItem(int i);

	void setItemEnable(int i,bool enabled);
	bool isItemEnable(int i);
	
	int getItemID(int i);
	void setItemID(int i,int itemID);

	virtual FontAttributes* getFontAttributes();

};


}
}

#endif




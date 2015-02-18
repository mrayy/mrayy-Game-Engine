
#ifndef ___GUICOMBOLIST___
#define ___GUICOMBOLIST___

#include "compileconfig.h"
#include "IGUIComboList.h"
#include "SColor.h"

#include "FontAttributes.h"
#include "GUIElementFactoryDef.h"


namespace mray
{
namespace GUI
{
	class GUIComboListComponent;

class MRAYGUI_API GUIComboList:public IGUIComboList
{
	DECLARE_RTTI
protected:
	GUIComboListComponent* m_component;
	FontAttributes m_FontAttributes;

	bool _OnKeyboardEvent(KeyboardEvent* e);
	bool _OnMouseEvent(MouseEvent* e);

	void _notifyLostFocus();
public:

	GUIComboList(IGUIManager* manager);
	virtual~GUIComboList();

	int GetItemsCount()const;
	IListItem* GetItem(int index)const;
	int FindItem(const core::UTFString& txt)const;
	int AddItem(IListItem* item);
	int InsertItem(int index,IListItem* item);
	void DeleteItem(int index);
	const ItemList& GetItems()const;
	void SortItems();
	void ClearItems();

	int GetSelectedItem();
	void SetSelectedItem(int i);


	void SetShowItemsCount(int c);
	int GetShowItemsCount();

	int GetItemFromPos(const math::vector2d& y);

	void SetSelectionColor(const video::SColor& clr);
	const video::SColor& GetSelectionColor()const;

	virtual void Draw(const math::rectf*vp);
	virtual void PostDraw(const math::rectf*vp);

	virtual IGUIElement* Duplicate();
	virtual FontAttributes* GetFontAttributes();
};
DECLARE_ELEMENT_FACTORY(GUIComboList);

}
}



#endif










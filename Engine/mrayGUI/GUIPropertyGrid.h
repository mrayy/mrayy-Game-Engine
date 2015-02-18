
#ifndef ___GUIPropertyGrid___
#define ___GUIPropertyGrid___

#include "compileConfig.h"
#include "IGUIPropertyGrid.h"
#include "FontAttributes.h"
#include "GUIElementFactoryDef.h"

namespace mray{
namespace GUI{

	class IGUIPropGridItem;
	class GUICatagoryPanel;
	class GUIPropGridStack;
	class GUIStackPanel;
	class GUISliderbarComponent;

class MRAYGUI_API GUIPropertyGrid:public IGUIPropertyGrid
{
	DECLARE_RTTI;
protected:
	struct CatagoryObj
	{
		CatagoryObj()
		{
		}
		CatagoryObj(GUICatagoryPanel* t,GUIPropGridStack* s):tab(t),stack(s)
		{
		}
		GUICatagoryPanel* tab;
		GUIPropGridStack* stack;
	};
	typedef std::map<core::string,CatagoryObj> CategoryList;
	CategoryList m_categories;
	FontAttributes m_FontAttributes;
	TPropertieList m_objProps;

	GUIElementRegion* m_scrollRegion;
	GUIElementRegion* m_catagRegion;

	GUIStackPanel* m_stackPanel;

	GUICatagoryPanel* m_activeTab;

	GUISliderbarComponent* m_slider;

// 	virtual bool _OnMouseEvent(MouseEvent*evt);
// 	virtual bool _OnKeyboardEvent(KeyboardEvent*evt);

	virtual bool _UpdateRegion(const math::rectf*vp);

public:

	GUIPropertyGrid(IGUIManager* manager);
	virtual~GUIPropertyGrid();

	virtual void SetPropertyObject(CPropertieSet* obj);

	void ClearTabs();

	virtual bool OnEvent(Event*e);

	virtual void Draw(const math::rectf*vp);

	virtual IGUIElement* Duplicate();
	virtual FontAttributes* GetFontAttributes();
};

DECLARE_ELEMENT_FACTORY(GUIPropertyGrid);

}
}




#endif








/********************************************************************
	created:	2011/12/14
	created:	14:12:2011   13:53
	filename: 	d:\Development\mrayEngine\Engine\mrayGUI\GUICatagoryPanel.h
	file path:	d:\Development\mrayEngine\Engine\mrayGUI
	file base:	GUICatagoryPanel
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __GUICatagoryPanel__
#define __GUICatagoryPanel__

#include "CompileConfig.h"
#include "GUIElementFactoryDef.h"
#include "IGUICatagoryPanel.h"
#include "FontAttributes.h"

namespace mray
{
namespace GUI
{

	class GUIStackPanel;

class MRAYGUI_API GUICatagoryPanel:public IGUICatagoryPanel
{
	DECLARE_RTTI;
private:
protected:

	GUIStackPanel* m_panel;
	FontAttributes m_FontAttributes;

	//math::rectf m_tabRC;
	//math::rectf m_frameRect;
	GUIElementRegion* m_tabRegion;
	GUIElementRegion* m_frameRegion;
	//GUIElementRegion* m_scrollRegion;
	
	float m_tabHeight;
	float m_contentHeight;
	float m_currentHeight;
	float m_speed;

	bool m_isOpen;

	virtual bool _UpdateRegion(const math::rectf*vp);
	virtual bool _OnMouseEvent(MouseEvent*evt);
public:
	GUICatagoryPanel(IGUIManager*mngr);
	virtual~GUICatagoryPanel();

	virtual void AddElement(IGUIElementCRef elem);
	virtual void RemoveElement(IGUIElement* elem);
	virtual IGUIElementCRef GetElementByID(uint id)const;
	virtual void ClearElements();

	//virtual const math::rectf& GetUnclippedRect();
	//virtual const math::rectf& GetClippedRect();
	virtual void Draw(const math::rectf*vp);

	virtual IGUIElement* Duplicate();
	virtual FontAttributes* GetFontAttributes();

};
DECLARE_ELEMENT_FACTORY(GUICatagoryPanel);


}
}

#endif
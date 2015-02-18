

/********************************************************************
	created:	2013/05/07
	created:	7:5:2013   20:58
	filename: 	C:\Development\mrayEngine\Engine\mrayGUI\GUITreeBox.h
	file path:	C:\Development\mrayEngine\Engine\mrayGUI
	file base:	GUITreeBox
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __GUITreeBox__
#define __GUITreeBox__


#include "CompileConfig.h"
#include "IGUITreeBox.h"
#include "SColor.h"
#include "IFont.h"
#include "IDelegate.h"

#include "TypedProperty.h"
#include "FontAttributes.h"
#include "GUIElementFactoryDef.h"

namespace mray
{

class KeyboardEvent;
class MouseEvent;

namespace GUI
{

class MRAYGUI_API GUITreeBox:public IGUITreeBox
{
	DECLARE_RTTI
protected:

	FontAttributes m_FontAttributes;
	GUITreeNodeList m_nodes;

	virtual void fillProperties();

	bool _OnKeyboardEvent(KeyboardEvent* e);
	bool _OnMouseEvent(MouseEvent* e);

public:
	GUITreeBox(IGUIManager*mngr);
	virtual~GUITreeBox();



	virtual GUITreeNode* AddItem(const core::string& text);
	virtual void RemoveItem(GUITreeNode* node);
	virtual void ClearItems();

	virtual void ExpandAll();
	virtual void CollapseAll();

	virtual GUITreeNodeList& GetNodes();

	virtual void Draw(const math::rectf*vp);
	virtual void Update(float dt);


	virtual IGUIElement* Duplicate();
	virtual FontAttributes* GetFontAttributes();

};

DECLARE_ELEMENT_FACTORY(GUITreeBox);

}
}

#endif
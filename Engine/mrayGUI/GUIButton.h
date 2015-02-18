
#ifndef ___GUIBUTTON___
#define ___GUIBUTTON___

#include "CompileConfig.h"
#include "IGUIButton.h"
#include "IDelegate.h"
#include "FontAttributes.h"

#include "GUIElementFactoryDef.h"

namespace mray
{
namespace GUI
{
	class GUIButtonComponent;

class MRAYGUI_API GUIButton:public IGUIButton
{
	DECLARE_RTTI
protected:
	FontAttributes m_FontAttributs;

	GUIButtonComponent* m_component;

	virtual void fillProperties();
	bool _OnKeyboardEvent(KeyboardEvent* e);
	bool _OnMouseEvent(MouseEvent* e);
public:

	GUIButton(IGUIManager* creator);
	virtual~GUIButton();
	virtual void Draw(const math::rectf*vp);
	virtual bool SetText(const core::UTFString&t);
	virtual const core::UTFString& GetText()const;
	
	virtual IGUIElement* Duplicate();
	virtual FontAttributes* GetFontAttributes();
protected:

	virtual void _notifyMouseEnter();
	virtual void _notifyMouseLeave();

	virtual void _notifyReceiveFocus();
	virtual void _notifyLostFocus();


};

DECLARE_ELEMENT_FACTORY(GUIButton);

}
}
#endif
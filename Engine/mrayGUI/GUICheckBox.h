

#ifndef ___GUICHECKBOX___
#define ___GUICHECKBOX___

#include "CompileConfig.h"
#include "IGUICheckBox.h"
#include "IDelegate.h"
#include "FontAttributes.h"

#include "GUIElementFactoryDef.h"

namespace mray{
namespace GUI{

	class GUICheckBoxComponent;

class MRAYGUI_API GUICheckBox:public IGUICheckBox
{
	DECLARE_RTTI;
protected:
	FontAttributes m_FontAttributes;

	GUICheckBoxComponent* m_component;
protected:
	bool _HandleMouseEvent(MouseEvent*e);
	bool _HandleKeyboardEvent(KeyboardEvent*e);

public:

	GUICheckBox(IGUIManager* creator);
	virtual~GUICheckBox();

	virtual void Draw(const math::rectf*vp);
	virtual bool OnEvent(Event*  event);

	void ChangeState();

	const bool& IsChecked();
	bool SetChecked(const bool& val);

	const bool& IsBackgroundOpaque();
	bool SetBGOpaque(const bool& val);

	const bool& IsPushButton();
	bool SetPushButton(const bool&val);


	virtual IGUIElement* Duplicate();
	virtual FontAttributes* GetFontAttributes();

protected:
};

DECLARE_ELEMENT_FACTORY(GUICheckBox);


}
}

#endif












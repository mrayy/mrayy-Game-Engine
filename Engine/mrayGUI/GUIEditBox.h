

#ifndef ___GUIEDITTEXT___
#define ___GUIEDITTEXT___

#include "CompileConfig.h"
#include "IGUIEditBox.h"
#include "SColor.h"
#include "IFont.h"
#include "IDelegate.h"

#include "TypedProperty.h"
#include "FontAttributes.h"
#include "GUIElementFactoryDef.h"
#include "GUIEditBoxComponent.h"

namespace mray
{
	class KeyboardEvent;
	class MouseEvent;

namespace GUI
{

	//class GUIEditBoxComponent;

class MRAYGUI_API GUIEditBox:public IGUIEditBox,public GUIEditBoxComponent::IListener
{
	DECLARE_RTTI
protected:

	GUIEditBoxComponent* m_component;
	FontAttributes m_FontAttributes;


	virtual void fillProperties();

	bool _OnKeyboardEvent(KeyboardEvent* e);
	bool _OnMouseEvent(MouseEvent* e);

	void _RemoveSelectedText();

public:
	GUIEditBox(IGUIManager*mngr);
	virtual~GUIEditBox();
	
	virtual void Draw(const math::rectf*vp);
	virtual void Update(float dt);

	virtual bool SetText(const core::UTFString&t);
	virtual const core::UTFString& GetText()const ;
	bool CheckIsValideText(const core::UTFString&t);

	virtual void OnTextChangeCallback(GUIEditBoxComponent*caller);


	core::string getSelectedText();


	virtual IGUIElement* Duplicate();
	virtual FontAttributes* GetFontAttributes();

	const bool& getBackground() ;
	bool setBackground(const bool &val) ;

	const bool& getBorder() ;
	bool setBorder(const bool &val);

	const core::string& getHideChar();
	bool setHideChar(const core::string &val);

	const bool& getHideText();
	bool setHideText(const bool &val);

	const core::string& getRegex();
	bool setRegex(const core::string&val) ;

};

DECLARE_ELEMENT_FACTORY(GUIEditBox);

}
}

#endif

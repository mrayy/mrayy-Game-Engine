

#ifndef ___GUISTATICTEXT___
#define ___GUISTATICTEXT___

#include "CompileConfig.h"
#include "IGUIStaticText.h"
#include "IDelegate.h"
#include "FontAttributes.h"

#include "GUIElementFactoryDef.h"


namespace mray{
namespace GUI{

	class GUIStaticTextComponent;

class MRAYGUI_API GUIStaticText:public IGUIStaticText
{
	DECLARE_RTTI
protected:

	FontAttributes m_FontAttributs;
	GUIStaticTextComponent* m_component;
public:
	DECLARE_PROPERTY_TYPE(Background, bool, MRAYGUI_API);
	DECLARE_PROPERTY_TYPE(Borders, bool, MRAYGUI_API);

	GUIStaticText(IGUIManager* creator);
	virtual~GUIStaticText();

	virtual bool SetText(const core::UTFString&t);
	virtual void Draw(const math::rectf*vp);

	const bool& GetBackground() ;
	bool SetBackground(const bool& val);

	const bool& GetBorder();
	bool SetBorder(const bool& val);

	virtual IGUIElement* Duplicate();
	virtual FontAttributes* GetFontAttributes();
};


DECLARE_ELEMENT_FACTORY(GUIStaticText);

}
}

#endif

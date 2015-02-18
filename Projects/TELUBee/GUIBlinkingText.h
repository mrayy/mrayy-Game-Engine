

/********************************************************************
	created:	2013/02/07
	created:	7:2:2013   16:10
	filename: 	C:\Development\mrayEngine\Projects\TELUBee\GUIBlinkingText.h
	file path:	C:\Development\mrayEngine\Projects\TELUBee
	file base:	GUIBlinkingText
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __GUIBlinkingText__
#define __GUIBlinkingText__


#include "IGUIElement.h"
#include "GUIElementFactoryDef.h"

namespace mray
{
namespace GUI
{

class GUIBlinkingText:public IGUIElement
{
	DECLARE_RTTI
protected:
	FontAttributes m_fontAttrs;
	float m_time;
	float m_blinkSpeed;
public:
	static const GUID ElementType;
	DECLARE_PROPERTY_TYPE(BlinkSpeed,float,);
public:
	GUIBlinkingText(IGUIManager* mngr);
	virtual~GUIBlinkingText();

	DECLARE_SETGET_PROP(BlinkSpeed,float);

	virtual void Draw(const math::rectf*vp);
	virtual void Update(float dt);


	virtual FontAttributes* GetFontAttributes();
};
DECLARE_ELEMENT_FACTORY(GUIBlinkingText);

}
}

#endif



/********************************************************************
	created:	2013/02/07
	created:	7:2:2013   14:13
	filename: 	C:\Development\mrayEngine\Projects\TELUBee\GUIDateElement.h
	file path:	C:\Development\mrayEngine\Projects\TELUBee
	file base:	GUIDateElement
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __GUIDateElement__
#define __GUIDateElement__


#include "IGUIElement.h"
#include "FontAttributes.h"
#include "GUIElementFactoryDef.h"

namespace mray
{
namespace GUI
{

class GUIDateElement:public IGUIElement
{
	DECLARE_RTTI
protected:
	bool m_displayTime;
	bool m_displayDate;
	FontAttributes m_fontAttrs;
	core::string m_text;
public:
	static const GUID ElementType;
public:
	GUIDateElement(IGUIManager* mngr);
	virtual~GUIDateElement();


	virtual void Draw(video::IRenderArea*vp);
	virtual void Update(float dt);


	virtual FontAttributes* GetFontAttributes();
};
DECLARE_ELEMENT_FACTORY(GUIDateElement);



}
}


#endif

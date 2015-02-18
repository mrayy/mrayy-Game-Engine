

/********************************************************************
	created:	2013/02/01
	created:	1:2:2013   13:14
	filename: 	C:\Development\mrayEngine\Projects\TELUBee\GUILoadingElement.h
	file path:	C:\Development\mrayEngine\Projects\TELUBee
	file base:	GUILoadingElement
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __GUILoadingElement__
#define __GUILoadingElement__


#include "IGUIElement.h"
#include "GUIElementFactoryDef.h"

namespace mray
{
namespace GUI
{

class GUILoadingElement:public IGUIElement
{
protected:
	video::TextureUnit m_layer1;
	video::TextureUnit m_layer2;
	video::TextureUnit m_layer3;

	float m_time;
public:
	static const GUID ElementType;
public:
	GUILoadingElement(IGUIManager* mngr);
	virtual~GUILoadingElement();

	virtual void Draw(const math::rectf*vp);
	virtual void Update(float dt);
};

DECLARE_ELEMENT_FACTORY(GUILoadingElement);


}
}

#endif

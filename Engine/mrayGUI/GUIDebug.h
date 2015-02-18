

#ifndef ___GUIDebug___
#define ___GUIDebug___


#include "GUIElement.h"
#include "SColor.h"
#include "IFont.h"

namespace mray{
namespace GUI{

class MRAY_DLL GUIDebug:public GUIElement
{
	GCPtr<CPropertieSet>m_watches;
	FontAttributes m_FontAttributes;
public:
	DECLARE_RTTI;

	GUIDebug(GUIManager*mngr,math::Point2df pos,math::Point2df size);
	virtual~GUIDebug();

	void addWatch(IProperty*prop);

	virtual void draw(float dt);

	virtual FontAttributes* getFontAttributes();
};

}
}



#endif




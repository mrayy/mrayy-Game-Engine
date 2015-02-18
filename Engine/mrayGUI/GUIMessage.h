

#ifndef ___GUIMessage___
#define ___GUIMessage___


#include "GUIWindow.h"
#include "GUIStaticText.h"
#include "GUIButton.h"


namespace mray{
namespace GUI{

class GUIMessage:public GUIWindow
{
	class COkBtnEvent;

	GCPtr<COkBtnEvent>okBtnEvent;

	GCPtr<GUIStaticText>m_message;
	GCPtr<GUIButton>m_ok;
public:
	DECLARE_RTTI;

	GUIMessage(GUIManager*manager,math::vector2d pos);
	virtual~GUIMessage();

	virtual void draw(float dt);
	virtual void onEvent(const EventPtr& event);

	GCPtr<GUIStaticText>getMessageText();

	void showMessage(const  mchar*message,const  mchar*Caption,video::SColor msgColor=video::SColor(0,0,0,255));

	virtual FontAttributes* getFontAttributes();
};


}
}



#endif








#ifndef ___GUIToolBar___
#define ___GUIToolBar___

#include "GUIElement.h"
#include "SColor.h"
#include "IFont.h"

namespace mray{
namespace GUI{

class MRAY_DLL GUIToolBar:public GUIElement
{
	DECLARE_RTTI;
protected:
	int width;
	EAlignment align;
public:

	GUIToolBar();
	GUIToolBar(GUIManager* manager,int Width,EAlignment Align,IEventReciver* event=0,
		GCPtr<GUIElement>  parent=0,video::SColor color=240,int id=0);
	virtual~GUIToolBar();
	virtual void draw(float dt);
	virtual void onEvent(const EventPtr&  event);
	int getBarWidth(){return width;}
	void setBarWidth(int w){width=w;}
	EAlignment getAlignment(){return align;}
	void setAlignment(EAlignment a){align=a;}
	
	virtual GCPtr<GUIElement> getDuplicate();

};

}
}


#endif
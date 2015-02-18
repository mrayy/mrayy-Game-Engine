

#ifndef ___GUITabPanel___
#define ___GUITabPanel___

#include "GUIElement.h"
#include "SColor.h"
#include "IFont.h"


namespace mray{
namespace GUI{

class MRAY_DLL GUITabPanel:public GUIElement
{
	struct S_TAB
	{
		std::vector<GCPtr<GUIElement> > children;
		core::string Caption;
	};
	int width;
	EAlignment align;
	std::vector<S_TAB> tabs;
	int currTab;
	FontAttributes m_FontAttributs;
public:
	DECLARE_RTTI;

	GUITabPanel();
	GUITabPanel(GUIManager* manager,int tabCount,math::Point2df pos,math::Point2df size,IEventReciver* event=0,
		GUIElement*  parent=0,video::SColor color=240,int id=0);
	virtual~GUITabPanel();
	virtual void draw(float dt);
	virtual void onEvent(const EventPtr&  event);


	std::vector<GCPtr<GUIElement> >*getTabChildren(int index);
	void setTabCount(int count);
	void setTabCaption(const  mchar*str,int index);
	void addTab(const  mchar*str,int index=-1);
	void delTab(int index=-1);
	int getTabCount(){return tabs.size();}
	void addChildToTab(GCPtr<GUIElement> elem,int index);
	void removeChild(GCPtr<GUIElement> elem);
	void checkTabs(math::Point2df pos);
	

	virtual GCPtr<GUIElement> getDuplicate();

	virtual void addChild(GCPtr<GUIElement> elem);
	virtual FontAttributes* getFontAttributes();
};

}
}


#endif
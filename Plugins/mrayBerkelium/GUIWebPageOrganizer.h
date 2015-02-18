

/********************************************************************
created:	2012/05/22
created:	22:5:2012   15:26
filename: 	d:\Development\mrayEngine\Tests\TestOpenCV\GUIWebPageOrganizer.h
file path:	d:\Development\mrayEngine\Tests\TestOpenCV
file base:	GUIWebPageOrganizer
file ext:	h
author:		MHD Yamen Saraiji

purpose:	
*********************************************************************/


#ifndef __GUIWebPageOrganizer__
#define __GUIWebPageOrganizer__

#include "IGUIPanelElement.h"

namespace mray
{

namespace GUI
{
	class GUIWebPageControl;

class GUIWebPageOrganizer:public IGUIPanelElement
{
protected:

	bool m_fullScreen;
	std::vector<GUIWebPageControl*> m_pages;
	int m_activePage;

	void _SwitchFullscreen();
public:
	static const GUID ElementType;
	GUIWebPageOrganizer(IGUIManager* creator);

	~GUIWebPageOrganizer();

	GUIWebPageControl* GetActivePage();

	void toFront(IGUIElementPtr e);

	bool OnEvent(Event*e);
	void AddNewTab(GCPtr<GUIWebPageControl> ctrl);

	virtual void Update(float dt);
	virtual IGUIElement* Duplicate();
	virtual FontAttributes* GetFontAttributes();

};

}
}

#endif




/********************************************************************
	created:	2012/05/22
	created:	22:5:2012   15:21
	filename: 	d:\Development\mrayEngine\Tests\TestOpenCV\GUIWebPageControl.h
	file path:	d:\Development\mrayEngine\Tests\TestOpenCV
	file base:	GUIWebPageControl
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __GUIWebPageControl__
#define __GUIWebPageControl__

#include "IGUIElement.h"

namespace mray
{
	namespace web
	{
		class BrowserWindow;
	}
namespace GUI
{
	class GUIWebPageControlImpl;

class GUIWebPageControl:public IGUIElement
{
protected:
	GUIWebPageControlImpl* m_impl;


 	bool _OnKeyboardEvent(KeyboardEvent* e);
 	bool _OnMouseEvent(MouseEvent* e);

	int m_tabID;
public:
	static const GUID ElementType;
	GUIWebPageControl(IGUIManager* creator,void* context);
	~GUIWebPageControl();


	web::BrowserWindow* GetBrowserWindow();

	void SetResolution(int x,int y);

	bool NavigateTo(const core::string& url);
	void ExecuteJavascript(const core::stringw& script);

	virtual void Draw(video::IRenderArea* vp);
	virtual IGUIElement* Duplicate();
	virtual FontAttributes* GetFontAttributes();

};
}
}

#endif




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
#include "BrowserWindow.h"
#include "ParsedShaderPP.h"

namespace mray
{
namespace GUI
{

class GUIWebPageControl:public IGUIElement
{
protected:
	GCPtr<BrowserWindow> m_window;
	video::TextureUnit m_texUnit;


	bool _OnKeyboardEvent(KeyboardEvent* e);
	bool _OnMouseEvent(MouseEvent* e);

	int m_tabID;
	GCPtr<video::ParsedShaderPP> m_shader;
	video::IRenderTargetPtr m_inputRT;
	video::ITexturePtr m_rt;
	video::TextureUnit m_rtUnit;

	math::rectf m_texCoords;
public:
	static const GUID ElementType;
	GUIWebPageControl(IGUIManager* creator);
	~GUIWebPageControl();

	void SetWindow(const GCPtr<BrowserWindow>& w);
	BrowserWindow* GetWindow();
	virtual void Draw(video::IRenderArea* vp);
	virtual IGUIElement* Duplicate();
	virtual FontAttributes* GetFontAttributes();

};
}
}

#endif

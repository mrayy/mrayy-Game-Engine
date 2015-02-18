

/********************************************************************
	created:	2012/01/15
	created:	15:1:2012   15:15
	filename: 	d:\Development\mrayEngine\Tools\XEditorV2\DeviceWidget.h
	file path:	d:\Development\mrayEngine\Tools\XEditorV2
	file base:	DeviceWidget
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __DeviceWidget__
#define __DeviceWidget__

#include <RenderWindow.h>

namespace mray
{

	class DeviceWidget;
	class DrawTimer:public wxTimer
	{
	protected:
		DeviceWidget* m_owner;
	public:
		DrawTimer(DeviceWidget* owner);
		virtual void Notify();
	};

class DeviceWidget:public wxWindow,public video::IRenderWindowListener
{
private:
protected:
	HDC m_hDC;
	video::RenderWindow* m_window;
	scene::ViewPort* m_viewport;
	math::vector2d startpos;
	video::TextureUnitPtr m_texture;
	float m_angle;
	math::vector2d m_pos;
	
	core::string m_name;
	DrawTimer m_timer;

	bool m_selecting;
	math::rectf m_selectRC;
public:
	DeviceWidget(const core::string& name,wxWindow *parent,const wxPoint& pos,const wxSize& sz);
	virtual~DeviceWidget();


	virtual void WindowPostViweportUpdate(video::RenderWindow* wnd,scene::ViewPort* vp);

	void OnPaint(wxPaintEvent& WXUNUSED(event));
	void OnResize(wxSizeEvent& WXUNUSED(event));
	void OnMouse(wxMouseEvent& WXUNUSED(event));

	void OnInit(wxInitDialogEvent& WXUNUSED(event));
	
	DECLARE_EVENT_TABLE()
};


}

#endif

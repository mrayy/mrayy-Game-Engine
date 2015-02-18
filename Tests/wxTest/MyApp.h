/********************************************************************
	created:	2012/11/09
	created:	9:11:2012   19:23
	filename: 	d:\Development\mrayEngine\Tests\WxTest\app.h
	file path:	d:\Development\mrayEngine\Tests\WxTest
	file base:	app
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __app__
#define __app__


#include <wx/wxprec.h>

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

namespace mray
{

	class Engine;
class MyApp:public wxApp
{
protected:
	Engine* m_engine;
	video::IVideoDevice* m_device;
public:
	MyApp();
	virtual~MyApp(){}

	virtual bool OnInit();
	virtual int OnExit();
};

}

#endif
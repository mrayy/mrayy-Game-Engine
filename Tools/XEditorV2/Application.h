


/********************************************************************
	created:	2012/01/15
	created:	15:1:2012   14:20
	filename: 	d:\Development\mrayEngine\Tools\XEditorV2\Application.h
	file path:	d:\Development\mrayEngine\Tools\XEditorV2
	file base:	Application
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __Application__
#define __Application__


namespace mray
{
class Application : public wxApp
{
public:
	Application();

	// virtual wxApp methods
	virtual bool OnInit();
	virtual int OnExit();

private:
	video::IVideoDevice* m_device;
};

}

#endif

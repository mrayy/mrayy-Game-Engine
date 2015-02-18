


/********************************************************************
	created:	2012/01/17
	created:	17:1:2012   15:34
	filename: 	d:\Development\mrayEngine\Engine\mrayEngine\include\RenderWindowUtils.h
	file path:	d:\Development\mrayEngine\Engine\mrayEngine\include
	file base:	RenderWindowUtils
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __RenderWindowUtils__
#define __RenderWindowUtils__


#include "mstring.h"
//#include "OptionContainer.h"


namespace mray
{
	class OptionContainer;
namespace video
{
	class RenderWindow;
	class IRenderWindowListener;


class MRAY_DLL RenderWindowUtils
{
public:
	typedef std::map<core::string,RenderWindow*> RenderWindowMap;
	typedef std::multimap<RenderWindow*,IRenderWindowListener*> RenderWindowListenerMap;
private:
protected:
	static RenderWindowMap m_renderWindows;
	static RenderWindowListenerMap m_listeners;
public:
	RenderWindowUtils(){}
	virtual~RenderWindowUtils(){}

	static ulong HandleWindowEvent(const OptionContainer& params);

	static RenderWindow* GetRenderWindow(const core::string& name);
	static void AddRenderWindow(RenderWindow*wnd);
	static void RemoveRenderWindow(RenderWindow*wnd);

	static void AddListener(RenderWindow*wnd,IRenderWindowListener*l);
	static void RemoveListener(RenderWindow*wnd,IRenderWindowListener*l);

	static RenderWindowListenerMap& GetListners(){return m_listeners;}
};

}
}

#endif

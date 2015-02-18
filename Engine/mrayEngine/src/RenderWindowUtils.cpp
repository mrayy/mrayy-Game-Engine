
#include "stdafx.h"
#include "RenderWindowUtils.h"
#include "RenderWindow.h"
#include "VideoLoggerSystem.h"
#include "IOSystem.h"


namespace mray
{
namespace video
{
RenderWindowUtils::RenderWindowMap RenderWindowUtils::m_renderWindows;
RenderWindowUtils::RenderWindowListenerMap RenderWindowUtils::m_listeners;

ulong RenderWindowUtils::HandleWindowEvent(const OptionContainer& params)
{
	return gOSystem.HandleWindowEvent(params);
}

RenderWindow* RenderWindowUtils::GetRenderWindow(const core::string& name)
{
	RenderWindowMap::iterator it= m_renderWindows.find(name);
	if(it!=m_renderWindows.end())
		return it->second;
	return 0;
}
void RenderWindowUtils::AddRenderWindow(RenderWindow*wnd)
{
	if(m_renderWindows.find(wnd->GetName())!=m_renderWindows.end())
		gVideoLoggerSystem.log(mT("Render Window with name :")+wnd->GetName()+mT(" Already exists"),ELL_WARNING);
	m_renderWindows[wnd->GetName()]=wnd;
}
void RenderWindowUtils::RemoveRenderWindow(RenderWindow*wnd)
{
	m_renderWindows.erase(wnd->GetName());
}

void RenderWindowUtils::AddListener(RenderWindow*wnd,IRenderWindowListener*l)
{
	m_listeners.insert(std::make_pair(wnd,l));
}
void RenderWindowUtils::RemoveListener(RenderWindow*wnd,IRenderWindowListener*l)
{
	RenderWindowListenerMap::iterator it = m_listeners.begin(), e = m_listeners.end();

	for( ; it != e; ++it )
	{
		if( it->first == wnd && it->second == l )
		{
			m_listeners.erase(it);
			break;
		}
	}
}

}
}


#include "stdafx.h"
#include "RenderWindow.h"
#include "VideoLoggerSystem.h"
#include "ViewPort.h"


namespace mray
{
namespace video
{

RenderWindow::RenderWindow(const core::string&name):m_name(name),m_Active(false),m_isPrimary(false),
	m_isFullscreen(false),m_autoDeactivate(false),m_inputManager(0)
{
}
RenderWindow::~RenderWindow()
{

	ViewPortMultiList::iterator it=m_viewports.begin();
	for(;it!=m_viewports.end();++it)
	{
		ViewPortList::iterator it2=(*it).begin();
		for(;it2!=(*it).end();++it2)
		{
			scene::ViewPort* vp=*it2;
			delete vp;
		}
	}
	m_viewports.clear();
}

bool RenderWindow::_AddViewport(scene::ViewPort*vp)
{
	if(GetViewport(vp->getName())!=0){
		gVideoLoggerSystem.log(mT("RenderWindow::addViewport()- Viewport with name: ")+vp->getName()+mT(" is already exist."),ELL_WARNING);
		return false;
	}
	int zorder=vp->getZOrder();
	ViewPortMultiList::iterator it=m_viewports.begin();
	ViewPortMultiList::iterator it2=m_viewports.end();
	for (;it!=m_viewports.end();++it)
	{
		ViewPortList::iterator vpIt=(*it).begin();
		if(vpIt!=(*it).end()){
			scene::ViewPort*  vp2=*vpIt;
			if(vp2->getZOrder()==zorder){
				(*it).push_back(vp);
				return true;
			}else if(vp2->getZOrder()>zorder){
				ViewPortList lst;
				lst.push_front(vp);
				if(it2==m_viewports.end()){
					m_viewports.push_front(lst);
				}else{
					m_viewports.insert(it2,lst);
				}
				return true;
			}
		}
		it2=it;
	}
	//lists are empty
	ViewPortList lst;
	lst.push_front(vp);
	m_viewports.push_front(lst);
	return true;
}

scene::ViewPort* RenderWindow::CreateViewport(const core::string&name,scene::CameraNode* camera,video::IRenderTarget* rt,
	const math::rectf&vprect,int zorder)
{
	if(GetViewport(name)!=0){
		gVideoLoggerSystem.log(mT("RenderWindow::createViewport()- Viewport with name: ")+name+mT(" is already exist."),ELL_WARNING);
		return 0;
	}
	scene::ViewPort*  vp=new scene::ViewPort(name,camera,rt,this,vprect,zorder);
	
	_AddViewport(vp);
	FIRE_LISTENR_METHOD(WindowViweportAdd,(this,vp));

	return vp;
}

scene::ViewPort* RenderWindow::GetViewport(const core::string&name)
{
	ViewPortMultiList::iterator it=m_viewports.begin();
	for (;it!=m_viewports.end();++it)
	{
		ViewPortList&lst=(*it);
		ViewPortList::iterator vpIt=lst.begin();
		for (;vpIt!=lst.end();++vpIt)
		{
			if((*vpIt)->getName()==name)
				return *vpIt;
		}

	}
	return 0;
}
scene::ViewPort* RenderWindow::GetViewportFromPoint(int x,int y,bool includeInactive)
{
	if(m_viewports.size()==0)
		return 0;
	math::vector2di pt(x,y);
	ViewPortMultiList::iterator it=m_viewports.end();
	do
	{
		it--;
		if((*it).size()==0)
			continue;
		ViewPortList::iterator it2=(*it).end();
		do
		{
			it2--;
			scene::ViewPort* vp=*it2;
			if(!vp->isActive() && !includeInactive)
				continue;
			if(vp->getAbsViewPort().IsPointInside(pt))
				return vp;
		}while(it2!=(*it).begin());
	}while(it!=m_viewports.begin());
	return 0;
}
void RenderWindow::ClearViewports()
{

	ViewPortMultiList::iterator it=m_viewports.begin();
	for(;it!=m_viewports.end();++it)
	{
		ViewPortList::iterator it2=(*it).begin();
		for(;it2!=(*it).end();++it2)
		{
			scene::ViewPort* vp=*it2;
			FIRE_LISTENR_METHOD(WindowViweportRemoved,(this,vp));
			delete vp;
		}
	}
	m_viewports.clear();
}
void RenderWindow::Render(bool swap)
{
	FIRE_LISTENR_METHOD(WindowPreRender,(this));

	ViewPortMultiList::iterator it=m_viewports.begin();
	for(;it!=m_viewports.end();++it)
	{
		ViewPortList::iterator it2=(*it).begin();
		for(;it2!=(*it).end();++it2)
		{
			scene::ViewPort* vp=*it2;
			if(!vp->isActive())
				continue;

			FIRE_LISTENR_METHOD(WindowPreViweportUpdate,(this,vp));
			vp->draw();
			FIRE_LISTENR_METHOD(WindowPostViweportUpdate,(this,vp));
		}
	}

	FIRE_LISTENR_METHOD(WindowPostRender,(this));
	if (swap)
	{
		// Swap buffers
		SwapBuffers(m_vsync);
	}
}



}
}


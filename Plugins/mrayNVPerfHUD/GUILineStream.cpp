
#include "stdafx.h"
#include "GUILineStream.h"
#include "GUIElementRegion.h"
#include "DefaultColors.h"
#include "FontResourceManager.h"
#include "IGUIManager.h"

#include "Engine.h"

namespace mray
{
namespace GUI
{

const GUID GUILineStream::ElementType(mT("GUILineStream"));

IMPLEMENT_RTTI(GUILineStream,IGUIElement)

GUILineStream::GUILineStream(IGUIManager*mngr)
:IGUIElement(ElementType,mngr)
{
	m_currentTraceSample=0;
	m_dirty=false;
	SetMaxSamplesCount(1000);

	m_fontAttrs.fontSize=10;
	m_fontAttrs.hasShadow=false;
	m_fontAttrs.fontAligment=GUI::EFA_TopLeft;
}
GUILineStream::~GUILineStream()
{
	ClearTraces();
}

void GUILineStream::SetMaxSamplesCount(int scount)
{
	m_maxSamplesCount=scount;
	TracteMap::iterator it=m_traces.begin();
	for(;it!=m_traces.end();++it)
	{
		it->second->points.resize(scount);
		it->second->samples.resize(scount);
	}

	m_dirty=true;
}
int	 GUILineStream::GetMaxSamplesCount()const
{
	return m_maxSamplesCount;
}

void GUILineStream::AddTrace(int idx,const video::SColor& clr,const core::UTFString& title)
{
	if(!HasTrace(idx))
	{
		TraceData* td=new TraceData();
		td->clr=clr;
		td->index=idx;
		td->samples.resize(m_maxSamplesCount);
		td->points.resize(m_maxSamplesCount);
		td->title=title;
		m_traces[idx]=td;
		m_dirty=true;
	}
}

void GUILineStream::RemoveTrace(int idx)
{
	TracteMap::iterator it=m_traces.find(idx);
	if(it==m_traces.end())
		return ;
	delete it->second;
	m_traces.erase(it);
}

bool GUILineStream::HasTrace(int idx)
{
	TracteMap::iterator it=m_traces.find(idx);
	if(it==m_traces.end())
		return false;
	return true;
}
void GUILineStream::ClearTraces()
{
	TracteMap::iterator it=m_traces.begin();
	for(;it!=m_traces.end();++it)
		delete it->second;
	m_traces.clear();

}

void GUILineStream::SetTraceColor(int idx,const video::SColor& clr)
{
	TracteMap::iterator it=m_traces.find(idx);
	if(it==m_traces.end())
		return ;
	it->second->clr=clr;
}

void GUILineStream::SetTraceTitle(int idx,const core::UTFString& title)
{
	TracteMap::iterator it=m_traces.find(idx);
	if(it==m_traces.end())
		return ;
	it->second->title=title;
}

void GUILineStream::SetTraceValue(int idx,float v)
{
	TracteMap::iterator it=m_traces.find(idx);
	if(it==m_traces.end())
		return ;
	it->second->samples[m_currentTraceSample]=v;
	m_dirty=true;
}

void GUILineStream::Draw()
{
	if(m_dirty)
	{
		_UpdatePoints();
		m_currentTraceSample=(m_currentTraceSample+1)%m_maxSamplesCount;
		m_dirty=false;
	}

	if(m_currentTraceSample>0)
	{
		video::IVideoDevice* device= Engine::getInstance().getDevice();

		const math::rectf& rc=GetDefaultRegion()->GetClippedRect();
		device->draw2DRectangle(rc,video::SColor(0.5));
		IFont* f= gFontResourceManager.getFontByName(m_fontAttrs.fontName);
		math::rectf textRc=rc;

		textRc.ULPoint.y=textRc.BRPoint.y-m_traces.size()*m_fontAttrs.fontSize;
		if(textRc.ULPoint.y<rc.ULPoint.y)
			textRc.ULPoint.y=rc.ULPoint.y;
		textRc.BRPoint.y=textRc.ULPoint.y+m_fontAttrs.fontSize;
		TracteMap::iterator it=m_traces.begin();
		for(;it!=m_traces.end();++it)
		{
			device->draw2DLine(&it->second->points[0],m_currentTraceSample-1,it->second->clr);

			if(f)
			{
				m_fontAttrs.fontColor=it->second->clr;
				f->print(textRc,&m_fontAttrs,&rc,it->second->title,GetCreator()->GetRenderQueue());
				textRc.ULPoint.y+=m_fontAttrs.fontSize;
				textRc.BRPoint.y+=m_fontAttrs.fontSize;
			}
		}


	}
}

void GUILineStream::_UpdatePoints()
{
	float maxValue=0;
	TracteMap::iterator it=m_traces.begin();
	for(;it!=m_traces.end();++it)
	{
		TraceData* td= it->second;
		for(int j=0;j<m_currentTraceSample;++j)
		{
			if(maxValue<td->samples[j])
				maxValue=td->samples[j];
		}
	}
	if(maxValue==0)
	{
		maxValue=1;
	}

	const math::rectf& rc=GetDefaultRegion()->GetClippedRect();


	float step=rc.getWidth()/(float)m_maxSamplesCount;
	float yMul=rc.getHeight()/maxValue;

	it=m_traces.begin();
	for(int i=0;it!=m_traces.end();++it,++i)
	{
		TraceData* td= it->second;
		for(int j=0;j<m_currentTraceSample;++j)
		{
			td->points[j].x=rc.ULPoint.x+step*(float)j;
			td->points[j].y=rc.ULPoint.y+td->samples[j]*yMul;
		}
	}
}

IGUIElement* GUILineStream::Duplicate()
{
	return new GUILineStream(GetCreator());
}


}
}




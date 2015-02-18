
#include "stdafx.h"
#include "GUIGraphPlotElement.h"
#include "GUIElementRegion.h"
#include "IGUIRenderer.h"
#include "IGUIManager.h"
#include "FontResourceManager.h"

namespace mray
{
namespace GUI
{

	const GUID GUIGraphPlotElement::ElementType("GraphPlot");

GUIGraphPlotElement::GUIGraphPlotElement(GUI::IGUIManager* crtr):GUI::IGUIElement(ElementType,crtr)
{
	m_maxSamplesCount=100;
	m_font.fontSize=14;
	m_font.hasShadow=false;
	m_font.fontAligment=GUI::EFA_TopLeft;

	m_min=math::Infinity;
	m_max=-math::Infinity;
}

GUIGraphPlotElement::~GUIGraphPlotElement()
{
	Clear();
}


void GUIGraphPlotElement::AddGraph(const core::string &name,const video::SColor& clr)
{
	GraphInfo*g=new GraphInfo();
	g->name=name;
	g->color=clr;
	m_graphs[name]=g;
	g->points.resize(m_maxSamplesCount);
	g->values.resize(m_maxSamplesCount);
}
bool GUIGraphPlotElement::HasGraph(const core::string& name)
{
	GraphList::iterator it= m_graphs.find(name);
	if(it!=m_graphs.end())
		return true;
	return false;
}

void GUIGraphPlotElement::RemoveGraph(const core::string &name)
{
	GraphList::iterator it= m_graphs.find(name);
	if(it!=m_graphs.end())
	{
		delete it->second;
		m_graphs.erase(it);
	}
}

void GUIGraphPlotElement::AddValue(const core::string &name,float v)
{
	GraphList::iterator it= m_graphs.find(name);
	if(it!=m_graphs.end())
	{
		GraphInfo* g=it->second;
		if(g->values.size()==0)
		{
			g->minVal=g->minVal=v;
		}else
		{
			g->minVal=math::Min(v,g->minVal);
			g->maxVal=math::Max(v,g->maxVal);
		}
		m_min=math::Min(m_min,g->minVal);
		m_max=math::Max(m_max,g->maxVal);
		g->lastVal=v;
		g->values[g->currSample]=(v);
		if(g->currSample<g->values.size()-1)
			++g->currSample;
		else
		{
			for(int i=0;i<g->values.size()-1;++i)
				g->values[i]=g->values[i+1];
		}

	}

}

void GUIGraphPlotElement::Clear()
{
	GraphList::iterator it=m_graphs.begin();
	for (;it!=m_graphs.end();++it)
	{
		delete it->second;
	}
	m_graphs.clear();
	m_min=math::Infinity;
	m_max=-math::Infinity;
}


void GUIGraphPlotElement::ResetValues()
{
	GraphList::iterator it=m_graphs.begin();
	for (;it!=m_graphs.end();++it)
	{
		it->second->values.clear();
	}
}


void GUIGraphPlotElement::SetMaxSamplesCount(int cnt)
{
	m_maxSamplesCount=cnt;
	GraphList::iterator it=m_graphs.begin();
	for (;it!=m_graphs.end();++it)
	{
		it->second->points.resize(m_maxSamplesCount);
		it->second->values.resize(m_maxSamplesCount);
	}

}

void GUIGraphPlotElement::Update(float dt)
{
	GUI::IGUIElement::Update(dt);

	math::rectf rc=GetDefaultRegion()->GetClippedRect();

	float stepSize=rc.getWidth()/(float)m_maxSamplesCount;
	GraphList::iterator it=m_graphs.begin();
	float h=(m_max-m_min);
	if(h==0)
		h=1;
	h=rc.getHeight()/h;
	for (;it!=m_graphs.end();++it)
	{
		GraphInfo* g=it->second;
		for(int i=0;i<g->currSample;++i)
		{
			g->points[i].x=rc.ULPoint.x + stepSize*i;
			g->points[i].y=rc.BRPoint.y - (g->values[i]-m_min)*h;
		}
	}
}

void GUIGraphPlotElement::Draw(const math::rectf*vp)
{
	if(!IsVisible())
		return;
	GUI::IGUIElement::Draw(vp);
	float alpha=0.5;
	math::rectf rc=GetDefaultRegion()->GetClippedRect();
	GetCreator()->GetActiveTheme()->drawSizableBox(GetCreator()->GetRenderQueue(),rc,0,mT("Frame"),video::SColor(1,1,1,alpha));
	GetCreator()->GetRenderQueue()->Flush();


	if(!m_graphs.size())
		return;

	GUI::IFont*font=gFontResourceManager.getFontByName(m_font.fontName);
	GraphList::iterator it=m_graphs.begin();
	math::rectf r;
	r.ULPoint=rc.ULPoint;
	r.BRPoint.x=rc.BRPoint.x;
	r.BRPoint.y=r.ULPoint.y+m_font.fontSize;
	float fontSpacing=rc.getHeight()/(m_graphs.size()+1);
	fontSpacing=math::Min(20.0f,fontSpacing);
	m_font.fontSize=fontSpacing;
	for (;it!=m_graphs.end();++it)
	{
		GraphInfo* g=it->second;
		g->color.A=alpha;
		GetCreator()->GetDevice()->draw2DLine(&g->points[0],g->currSample-1,g->color);
		if(font)
		{
			m_font.fontColor=g->color;
			m_font.fontColor.A=1;
			core::string msg=g->name+mT("@")+core::StringConverter::toString(g->lastVal,4);
			font->print(r,&m_font,&r,msg,GetCreator()->GetRenderQueue());
			r.ULPoint.y+=fontSpacing;
			r.BRPoint.y+=fontSpacing;
		}
	}
	GetCreator()->GetRenderQueue()->Flush();
}


}
}


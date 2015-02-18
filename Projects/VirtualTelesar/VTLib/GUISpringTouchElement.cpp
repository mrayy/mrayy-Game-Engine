#include "stdafx.h"
#include "GUISpringTouchElement.h"
#include "SensorCurve.h"
#include "IGUIManager.h"
#include "GUIElementRegion.h"
#include "SensorCurve.h"
#include "SpringTouchSensorImpl.h"

namespace mray
{
namespace GUI
{

IMPLEMENT_RTTI(GUISpringTouchElement,IGUIElement)
const GUID GUISpringTouchElement::ElementType("GUISpringTouchElement");

GUISpringTouchElement::GUISpringTouchElement(IGUIManager* creator)
	:IGUIElement(ElementType,creator)
{
	m_elem=0;
}
GUISpringTouchElement::~GUISpringTouchElement()
{
}

void GUISpringTouchElement::SetElement(VT::SpringTouchSensorComponentImpl* c)
{
	m_elem=c;
}
VT::SpringTouchSensorComponentImpl* GUISpringTouchElement::GetElement()
{
	return m_elem;
}
void GUISpringTouchElement::Draw(video::IRenderArea* vp)
{
	IGUIManager* creator= GetCreator();
	video::IVideoDevice* device=creator->GetDevice();
	math::rectf rc=GetDefaultRegion()->GetClippedRect();
	math::rectf urc=GetDefaultRegion()->GetRect();
	device->draw2DRectangle(rc,video::SColor(0.5,0.5,0.5,0.5));
	std::vector<math::vector2d> points;
	int w=GetSize().x;
	int h=GetSize().y;
	points.push_back(math::vector2d(urc.ULPoint.x,urc.BRPoint.y));
	points.push_back(math::vector2d(urc.BRPoint.x,urc.ULPoint.y));
	device->draw2DLine(&points[0],points.size(),video::SColor(0.8,0.8,0.8,1));
	points.clear();
	for(int i=0;i<w;++i)
	{
		float t=i/(float)w;
		float v= m_elem->modulator->SampleValue01(t);
		points.push_back(math::vector2d(t,1-v)*h+GetPosition());
	}
	device->draw2DLine(&points[0],points.size(),video::SColor(1,0,0,1));
	const std::vector<float>& keys= m_elem->modulator->GetKeys();
	for(int i=0;i<keys.size();++i)
	{
		float x=(w*i)/(float)(keys.size()-1);
		float y=(1-keys[i])*h;
		math::rectf knot;
		knot.ULPoint=urc.ULPoint+math::vector2d(x-1,y-1);
		knot.BRPoint=urc.ULPoint+math::vector2d(x+1,y+1);
		device->draw2DRectangle(knot,video::SColor(1,1,1,1));
	}
	math::rectf bar;
	bar.ULPoint.x=bar.BRPoint.x=math::clamp<float>(w*m_elem->resForce.value/(m_elem->modulator->GetInLimit().y-m_elem->modulator->GetInLimit().x),0,w);
	bar.ULPoint.x-=1;
	bar.BRPoint.x+=1;
	bar.ULPoint.y=GetPosition().y;
	bar.BRPoint.y=bar.ULPoint.y+h;
	device->draw2DRectangle(bar,video::SColor(0,1,0,1));
}
	
}
}
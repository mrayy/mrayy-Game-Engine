#include "stdafx.h"

#include "AIMovmentTracker.h"
#include "IAIPhysics.h"
#include "XMLElement.h"
#include "mmath.h"
#include "StringConverter.h"
#include "AIComponentTypes.h"
#include "IRenderDevice.h"
#include "DefaultColors.h"


namespace mray
{
namespace AI
{

AIMovmentTracker::AIMovmentTracker(IAIPhysics*ph):
	m_maxSamples(50),m_physics(ph)
{
	m_componentTriggerPeriod=0.1;
}
AIMovmentTracker::~AIMovmentTracker()
{
}

void AIMovmentTracker::Update(float dt)
{
	m_samples.push_back(m_physics->GetGlobalPosition());
	while(m_samples.size()>m_maxSamples)
	{
		m_samples.erase(m_samples.begin());
	}
}

void AIMovmentTracker::Visualize(IRenderDevice*dev)
{
	math::line3d l;
	std::list<math::vector3d>::iterator it=m_samples.begin();
	if(it==m_samples.end())
		return;
/*
	for (;it!=m_samples.end();)
	{
		l.pStart=*it;
		++it;
		if(it!=m_samples.end()){
			l.pEnd=*it;
			++it;
			dev->drawLine(l,video::SColor(255,0,0,128));
		}
		}*/
	l.pStart=*it;
	float alpha=m_maxSamples-m_samples.size();
	float invSize=1.0f/m_maxSamples;
	++it;
	for (;it!=m_samples.end();++it)
	{
		l.pEnd=*it;
		dev->drawLine(l.pStart,l.pEnd,video::SColor(255,0,0,255*alpha*invSize));
		l.pStart=l.pEnd;
		++alpha;
	}
	l.pEnd=m_physics->GetGlobalPosition();
	dev->drawLine(l.pStart,l.pEnd,video::DefaultColors::Red);
}
xml::XMLElement* AIMovmentTracker::loadXMLSettings(xml::XMLElement* elem)
{
	IAIComponent::loadXMLSettings(elem);
	xml::XMLAttribute*attr=elem->getAttribute(mT("MaxSamples"));
	if(attr)
		m_maxSamples=core::StringConverter::toFloat(attr->value);
	return elem;
}


}
}
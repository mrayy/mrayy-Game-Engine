#include "stdafx.h"

#include "CurveNode.h"
#include "CSpline.h"
#include "XMLElement.h"
#include "StringConverter.h"


namespace mray
{
namespace scene
{

CurveNode::CurveNode()
{
	m_curve=new math::CSpline<math::vector3d>(0,0,1);
}
CurveNode::~CurveNode()
{
	delete m_curve;
}

void CurveNode::SetName(const core::string&name)
{
	m_name=name;
}
const core::string& CurveNode::GetName()
{
	return m_name;
}

void CurveNode::BuildCurve(math::vector3d *points,int count,int curveSubD)
{
	m_curve->createCurve(points,count,curveSubD);
}

math::ICurve<math::vector3d>* CurveNode::GetCurve()
{
	return m_curve;
}

void CurveNode::LoadXML(xml::XMLElement* elem)
{
	xml::XMLAttribute*attr=0;
	xml::XMLElement*e=0;

	attr=elem->getAttribute(mT("name"));
	if(attr)
		m_name=attr->value;
	
	std::vector<math::vector3d> points;

	xml::xmlSubElementsMapIT it=elem->getElementsBegin();
	xml::xmlSubElementsMapIT end=elem->getElementsEnd();
	for (;it!=end;++it)
	{
		if((*it)->GetType()!=xml::ENT_Element)continue;
		e=dynamic_cast<xml::XMLElement*>(*it);
		if(e->getName().equals_ignore_case(mT("cv")))
		{
			attr=e->getAttribute(mT("pos"));
			if(attr)
			{
				points.push_back(core::StringConverter::toVector3d(attr->value));
			}
		}
	}
	BuildCurve(&points[0],points.size(),3);
}


}
}


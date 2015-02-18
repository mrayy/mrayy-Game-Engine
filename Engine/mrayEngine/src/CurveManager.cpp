#include "stdafx.h"

#include "CurveManager.h"
#include "XMLTree.h"
#include "IStream.h"
#include "CurveNode.h"
#include "ILogManager.h"

namespace mray
{
namespace scene
{

CurveManager::CurveManager()
{
}
CurveManager::~CurveManager()
{
	Clear();
}

void CurveManager::AddCurve(CurveNode*c)
{
	//TODO:check for conflicts
	m_curves[c->GetName()]=c;
}
CurveNode* CurveManager::GetCurve(const core::string&name)
{
	CurvesMap::iterator it= m_curves.find(name);
	if(it==m_curves.end())
		return 0;
	return it->second;
}
void CurveManager::RemoveCurve(const core::string&name)
{
	CurvesMap::iterator it= m_curves.find(name);
	if(it==m_curves.end())
		return ;

	m_curves.erase(it);
}

void CurveManager::Clear()
{
	CurvesMap::iterator it= m_curves.begin();
	for(;it!=m_curves.end();++it)
	{
		delete it->second;
	}
	m_curves.clear();
}

void CurveManager::LoadCurves(xml::XMLElement*elem)
{
	xml::XMLElement*e;
	xml::xmlSubElementsMapIT it=elem->getElementsBegin();
	xml::xmlSubElementsMapIT end=elem->getElementsEnd();
	for (;it!=end;++it)
	{
		if((*it)->GetType()!=xml::ENT_Element)continue;
		e=dynamic_cast<xml::XMLElement*>(*it);
		if(e->getName().equals_ignore_case(mT("Curve")))
		{
			CurveNode* node=new CurveNode();
			node->LoadXML(e);
			AddCurve(node);
		}
	}
}
void CurveManager::LoadCurves(OS::IStream*file)
{
	xml::XMLTree t;
	if(!t.load(file))
	{
		gLogManager.log(mT("Couldn't load Curves file:") + file->getStreamName(),ELL_WARNING);
		return;
	}
	xml::XMLElement*e= t.getSubElement(mT("Curves"));
	if(e)
		LoadCurves(e);
}


}
}
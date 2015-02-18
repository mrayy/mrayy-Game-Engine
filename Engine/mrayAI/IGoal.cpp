#include "stdafx.h"


#include "IGoal.h"

#include "IRenderDevice.h"


namespace mray{
namespace AI{

void IGoal::AddAttribute(IValue*v)
{
	m_parameters[v->getName()]=v;
}
const ValuesMap& IGoal::GetAttributes()
{
	return m_parameters;
}

IValue* IGoal::GetAttribute(const core::string&name)
{
	ValuesMap::iterator it= m_parameters.find(name);
	if(it==m_parameters.end())
		return 0;
	return it->second;
}


bool IGoal::OnMessageTelegram(const AIMessageTelegram&msg){
	return false;
}

void IGoal::Visualize(IRenderDevice*dev,const core::string&prefix)
{
	dev->addTextLine(prefix+getType());
}

void IGoal::loadXMLSettings(xml::XMLElement* elem)
{
	xml::xmlSubElementsMapIT it= elem->getElementsBegin();
	xml::xmlSubElementsMapIT end=elem->getElementsEnd();
	for(;it!=end;++it)
	{
		if((*it)->GetType()!=xml::ENT_Element)continue;
		xml::XMLElement* e=dynamic_cast<xml::XMLElement*>(*it);
		if(e->getName().equals_ignore_case(mT("Value")))
		{
			xml::XMLAttribute*attr= e->getAttribute(mT("Name"));
			if(!attr)
				continue;
			IValue*v= GetAttribute(attr->value);
			if(v)
			{
				v->loadXMLSettings(e);
			}
		}
	}
}

}
}


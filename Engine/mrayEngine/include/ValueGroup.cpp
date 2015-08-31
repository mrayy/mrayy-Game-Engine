

#include "stdafx.h"
#include "ValueGroup.h"
#include "XMLElement.h"
#include "StringUtil.h"


namespace mray
{


ValueGroup::ValueGroup(const core::string&name)
{
	m_name = name;
}

ValueGroup::~ValueGroup()
{
	Clear();
}


IValue* ValueGroup::AddValue(IValue* v)
{
	m_values[v->getName()] = v;
	return v;
}

IValue* ValueGroup::GetValue(const core::string& name)
{
	std::vector<core::string> splits = core::StringUtil::Split(name, ".");
	if (splits.size() > 1)
	{
		ValueGroup* v = this;
		for (int i = 0; i < splits.size() - 1; ++i)
		{
			v = v->GetValueGroup(splits[i]);
			if (!v)
				return 0;
		}

		return v->GetValue(splits[splits.size() - 1]);
	}
	else
	{
		ValueMap::iterator it = m_values.find(name);
		if (it == m_values.end())
			return 0;
		return it->second;
	}

}


ValueGroup* ValueGroup::AddValueGroup(ValueGroup* grp)
{
	m_subGroups[grp->GetName()]=grp;
	return grp;
}

ValueGroup* ValueGroup::GetValueGroup(const core::string& name)
{
	std::vector<core::string> splits = core::StringUtil::Split(name, ".");
	ValueGroup* v = this;
	for (int i = 0; i < splits.size(); ++i)
	{
		SubGroupMap::iterator it = v->m_subGroups.find(splits[i]);
		if (it == v->m_subGroups.end())
			return 0;
		v = it->second;
		if (!v)
			return 0;
	}
	return v;
}


void ValueGroup::Clear()
{
	for (SubGroupMap::iterator it = m_subGroups.begin(); it != m_subGroups.end(); ++it)
	{
		delete it->second;
	}
	for (ValueMap::iterator it = m_values.begin(); it != m_values.end(); ++it)
	{
		delete it->second;
	}
	m_values.clear();
	m_subGroups.clear();
}



void ValueGroup::loadXMLSettings(xml::XMLElement* elem)
{
	xml::XMLElement* e;
	
	e = elem->getSubElement("Value");
	while (e)
	{
		IValue* v = GetValue(e->getAttribute("Name")->value);
		if (v)
			v->parse(e->getAttribute("Value")->value);

		e = e->nextSiblingElement("Value");
	}
	e = elem->getSubElement("ValueGroup");
	while (e)
	{
		ValueGroup* v = GetValueGroup(e->getAttribute("Name")->value);
		if (v)
			v->loadXMLSettings(e);

		e = e->nextSiblingElement("ValueGroup");
	}
}

xml::XMLElement*  ValueGroup::exportXMLSettings(xml::XMLElement* elem)
{
	xml::XMLElement* e = new xml::XMLElement("ValueGroup");
	e->addAttribute("Name", m_name);

	for (SubGroupMap::iterator it = m_subGroups.begin(); it != m_subGroups.end(); ++it)
	{
		it->second->exportXMLSettings(e);
	}
	for (ValueMap::iterator it = m_values.begin(); it != m_values.end(); ++it)
	{
		it->second->exportXMLSettings(e);
	}
	elem->addSubElement(e);
	return e;
}

}

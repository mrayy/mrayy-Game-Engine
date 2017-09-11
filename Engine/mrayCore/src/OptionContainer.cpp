#include "stdafx.h"

#include "OptionContainer.h"


namespace mray{


OptionContainer::OptionContainer(){
}
OptionContainer::~OptionContainer(){
	Clear();
}

void OptionContainer::AddOption(const SOptionElement&elem)
{
	core::string str=elem.name;
	str.make_lower();
	m_options[str]=elem;
}
SOptionElement* OptionContainer::GetOptionByName(const core::string &name)
{
	core::string str=name;
	str.make_lower();
	OptionsMap::iterator it= m_options.find(str);
	if(it==m_options.end())
		return 0;
	return &it->second;
}
const SOptionElement* OptionContainer::GetOptionByName(const core::string &name)const
{
	core::string str=name;
	str.make_lower();
	OptionsMap::const_iterator it= m_options.find(str);
	if(it==m_options.end())
		return 0;
	return &it->second;
}
const core::string& OptionContainer::GetOptionValue(const core::string &name)const
{
	core::string str=name;
	str.make_lower();
	OptionsMap::const_iterator it= m_options.find(str);
	if(it==m_options.end())
		return core::string::Empty;
	
	return it->second.getValue();
}

SOptionElement& OptionContainer::operator[](const core::string& name)
{
	SOptionElement* ret=0;
	core::string str=name;
	str.make_lower();
	OptionsMap::iterator it= m_options.find(str);
	if(it==m_options.end())
	{
		m_options[str]=SOptionElement(name,mT(""));
		ret=&m_options[str];
	}else
		ret= &it->second;
	return *ret;
}

const SOptionElement* OptionContainer::operator[](const core::string& name)const
{
	core::string str=name;
	str.make_lower();
	OptionsMap::const_iterator it= m_options.find(str);
	if(it==m_options.end())
	{
		return 0;
	}else
		return &it->second;
}
void OptionContainer::Clear(){
	m_options.clear();
}


}



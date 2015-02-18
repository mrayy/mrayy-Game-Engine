

#include "stdafx.h"
#include "Localization.h"
#include "XMLElement.h"

namespace mray
{

const core::string& LocalizationLanguage::GetKeyword(const core::string& k)const
{
	KeywordsMap::const_iterator it=m_keywords.find(k);
	if(it!=m_keywords.end())
		return core::string::Empty;
	return it->second;
}

void LocalizationLanguage::InsertKeyword(const core::string& k,const core::string& v)
{
	m_keywords[k]=v;
}

void LocalizationLanguage::ClearKeywords()
{
	m_keywords.clear();
}


void LocalizationLanguage::LoadXML(xml::XMLElement* elem)
{
	xml::XMLAttribute* attr;

	attr=elem->getAttribute(mT("Name"));
	if(attr)
		m_name=attr->value;

	xml::XMLElement* e=elem->getSubElement(mT("Keyword"));
	while(e)
	{
		core::string k,v;
		attr=e->getAttribute(mT("Name"));
		if(attr)
			k=attr->value;
		attr=e->getAttribute(mT("Value"));
		if(attr)
			v=attr->value;
		m_keywords[k]=v;

		e=e->nextSiblingElement(mT("Keyword"));
	}
}

void LocalizationLanguage::SaveXML(xml::XMLElement* elem)
{
}


Localization::Localization()
{
}

Localization::~Localization()
{
	LanguageMap::iterator it= m_languages.begin();
	for(;it!=m_languages.end();++it)
		delete it->second;
}



LocalizationLanguage* Localization::GetLanguage(const core::string& name)
{
	LanguageMap::iterator it= m_languages.find(name);
	if(it==m_languages.end())
		return 0;
	return it->second;
}

bool Localization::SetActiveLanguage(const core::string& name)
{
	LocalizationLanguage* l=GetLanguage(name);
	if(!l)
		return false;

	FIRE_LISTENR_METHOD(OnLanguageChanged,(l));
	return true;
}


void Localization::LoadFromFile(const core::string& file)
{
}


void Localization::LoadXML(xml::XMLElement* elem)
{
}

void Localization::SaveXML(xml::XMLElement* elem)
{
}


}


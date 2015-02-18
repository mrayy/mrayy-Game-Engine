

/********************************************************************
	created:	2012/10/09
	created:	9:10:2012   22:59
	filename: 	d:\Development\mrayEngine\Engine\mrayEngine\include\Localization.h
	file path:	d:\Development\mrayEngine\Engine\mrayEngine\include
	file base:	Localization
	file ext:	h
	author:		MHD YAMEN SARAIJI
	
	purpose:	
*********************************************************************/
#ifndef ___Localization___
#define ___Localization___


#include "mstring.h"
#include "ListenerContainer.h"


namespace mray
{
	namespace xml
	{
		class XMLElement;
	}


class LocalizationLanguage
{
public:
	typedef std::map<core::string,core::string> KeywordsMap;
protected:
	core::string m_name;
	KeywordsMap m_keywords;
public:

	const core::string& GetName()const{return m_name;}
	const KeywordsMap& GetKeywords()const{return m_keywords;}

	const core::string& GetKeyword(const core::string& k)const;
	void InsertKeyword(const core::string& k,const core::string& v);
	void ClearKeywords();

	void LoadXML(xml::XMLElement* elem);
	void SaveXML(xml::XMLElement* elem);
};

class ILocalizationListener
{
public:
	virtual void OnLanguageChanged(LocalizationLanguage* language)=0;
};

class Localization:public ListenerContainer<ILocalizationListener*>
{
public:
	typedef std::map<core::string,LocalizationLanguage*> LanguageMap;
protected:

	LanguageMap m_languages;

	DECLARE_FIRE_METHOD(OnLanguageChanged,(LocalizationLanguage* language),(language));
public:
	Localization();
	virtual~Localization();

	const LanguageMap& GetAvaliableLanguages(){return m_languages;}

	LocalizationLanguage* GetLanguage(const core::string& name);
	bool SetActiveLanguage(const core::string& name);
	
	void LoadFromFile(const core::string& file);

	void LoadXML(xml::XMLElement* elem);
	void SaveXML(xml::XMLElement* elem);
	
};

}

#endif

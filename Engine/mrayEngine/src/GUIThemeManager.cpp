#include "stdafx.h"


#include "GUIThemeManager.h"
#include "GUILoggerSystem.h"
#include "TraceManager.h"
#include "XML_GUITheme.h"

namespace mray{
namespace GUI{


GUIThemeManager::GUIThemeManager()
{
	m_activeTheme=0;
}
GUIThemeManager::~GUIThemeManager(){
	clearThemes();
}

IGUITheme* GUIThemeManager::getActiveTheme(){
	return m_activeTheme; 
}

IGUITheme*  GUIThemeManager::loadTheme(OS::IStream* stream){
	traceFunction(eGUI);
	IGUITheme* t=XML_GUITheme::loadXML(stream);
	if(!t)return IGUIThemePtr::Null;
	addTheme(t);
	
	return t;
}

bool GUIThemeManager::setActiveTheme(const core::string &type){
	ThemesMap::iterator it=m_Themes.find(type);
	if(it==m_Themes.end())return 0;
	m_activeTheme=it->second;
	return true;
}

void GUIThemeManager::addTheme(IGUITheme*  Theme){
	if(!Theme)
		return;
	m_Themes.insert(ThemesMap::value_type(Theme->GetName(),Theme));
	if(!m_activeTheme)
		m_activeTheme=Theme;
	gGUILoggerSystem.log(core::string(mT("Theme \""))+Theme->GetName()+mT("\" been added"),ELL_INFO);
}
IGUITheme* GUIThemeManager::getTheme(const core::string &type){
	ThemesMap::iterator it=m_Themes.find(type);
	if(it==m_Themes.end())return 0;
	return it->second;
}
void GUIThemeManager::removeTheme(const core::string &type){
	m_Themes.erase(type);
	if(m_activeTheme && type.equals_ignore_case(m_activeTheme->GetName()))
	{
		if(m_Themes.begin()!=m_Themes.end())
			m_activeTheme=m_Themes.begin()->second;
		else
			m_activeTheme=0;
	}
}

void GUIThemeManager::clearThemes()
{
	ThemesMap::iterator it=m_Themes.begin();
	for(;it!=m_Themes.end();++it){
		delete it->second;
	}
	m_Themes.clear();
}

void GUIThemeManager::listThemes(std::vector<core::string>&outList){
	ThemesMap::iterator it=m_Themes.begin();
	for(;it!=m_Themes.end();++it){
		outList.push_back(it->first);
	}
}



}
}


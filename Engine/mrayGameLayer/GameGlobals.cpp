#include "stdafx.h"

#include "GameGlobals.h"
#include "IFileSystem.h"
#include "PluginManager.h"


namespace mray
{
namespace game
{

GameGlobals::GameGlobals()
{
}
GameGlobals::~GameGlobals()
{
	ClearPlugins();
	ClearSearchPaths();
}

void GameGlobals::AddRequiredPlugin(const core::string&plugin)
{
	m_requiredPlugins.push_back(plugin);
}
void GameGlobals::AddSearchPath(const core::string&path)
{
	m_searchPath.push_back(path);
}

void GameGlobals::SetupSearchPaths()
{
	for (int i=0;i<m_searchPath.size();++i)
	{
		gFileSystem.addPath(m_searchPath[i]);
	}
}
void GameGlobals::InstallPlugins()
{
	for (int i=0;i<m_requiredPlugins.size();++i)
	{
		int idx=PluginManager::getInstance().addPlugin(m_requiredPlugins[i]);
		if(idx!=-1)
			m_installedPlugins.push_back(idx);
	}
}
void GameGlobals::UninstallPlugins()
{
	for (int i=0;i<m_installedPlugins.size();++i)
	{
		PluginManager::getInstance().removePlugin(m_installedPlugins[i]);
	}
	m_installedPlugins.clear();
}
void GameGlobals::ClearPlugins()
{
	UninstallPlugins();
	m_requiredPlugins.clear();
}
void GameGlobals::ClearSearchPaths()
{
	for (int i=0;i<m_searchPath.size();++i)
	{
		gFileSystem.removePath(m_searchPath[i]);
	}
	m_searchPath.clear();
}

void GameGlobals::loadXMLSettings(xml::XMLElement* elem)
{
	xml::XMLAttribute*attr;
	xml::XMLElement* e=elem->getSubElement(mT("SearchPath"));
	if(e)
	{
		xml::xmlSubElementsMapIT it= e->getElementsBegin();
		xml::xmlSubElementsMapIT end= e->getElementsEnd();
		for(;it!=end;++it)
		{
			if((*it)->GetType()!=xml::ENT_Element)continue;
			xml::XMLElement* ee=dynamic_cast<xml::XMLElement*>(*it);
			if(ee->getName().equals_ignore_case(mT("Path")))
			{
				attr=ee->getAttribute(mT("Value"));
				if(attr)
					AddSearchPath(attr->value);
			}
		}
		SetupSearchPaths();
	}
	e=elem->getSubElement(mT("RequiredPlugins"));
	if(e)
	{
		xml::xmlSubElementsMapIT it= e->getElementsBegin();
		xml::xmlSubElementsMapIT end= e->getElementsEnd();
		for(;it!=end;++it)
		{
			if((*it)->GetType()!=xml::ENT_Element)continue;
			xml::XMLElement* ee=dynamic_cast<xml::XMLElement*>(*it);
			if(ee->getName().equals_ignore_case(mT("Plugin")))
			{
				attr=ee->getAttribute(mT("Value"));
				if(attr)
					AddRequiredPlugin(attr->value);
			}
		}
		InstallPlugins();
	}
}
xml::XMLElement*  GameGlobals::exportXMLSettings(xml::XMLElement* elem)
{
	xml::XMLElement* e=new xml::XMLElement(mT("GameGlobals"));
	elem->addSubElement(e);
	if(m_searchPath.size()){
		xml::XMLElement* paths=new xml::XMLElement(mT("SearchPath"));
		for (int i=0;i<m_searchPath.size();++i)
		{
			xml::XMLElement* e2=new xml::XMLElement(mT("Path"));
			e2->addAttribute(mT("Value"),m_searchPath[i]);
			paths->addSubElement(e2);
		}
		e->addSubElement(paths);
	}
	if(m_requiredPlugins.size()){
		xml::XMLElement* paths=new xml::XMLElement(mT("RequiredPlugins"));
		for (int i=0;i<m_requiredPlugins.size();++i)
		{
			xml::XMLElement* e2=new xml::XMLElement(mT("Plugin"));
			e2->addAttribute(mT("Value"),m_requiredPlugins[i]);
			paths->addSubElement(e2);
		}
		e->addSubElement(paths);
	}
	return e;
}


}
}

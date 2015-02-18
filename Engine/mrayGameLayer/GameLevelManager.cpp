#include "stdafx.h"
#include "stdafx.h"

#include "GameLevelManager.h"

#include "IFileSystem.h"
#include "XMLTree.h"

namespace mray
{
namespace game
{

GameLevelManager::GameLevelManager()
{
}
GameLevelManager::~GameLevelManager()
{
}

void GameLevelManager::LoadLevelsFile(const core::string& path)
{
	GCPtr<OS::IStream> stream=gFileSystem.openFile(path,OS::TXT_READ);
	if(!stream)
		return;

	xml::XMLTree tree;
	if(!tree.load(stream))
	{
		stream->close();
		return;
	}
	xml::XMLElement*e=tree.getSubElement(mT("Levels"));
	if(!e){
		stream->close();
		return;
	}
	xml::xmlSubElementsMapIT it= e->getElementsBegin();
	for (;it!=e->getElementsEnd();++it)
	{
		if((*it)->GetType()!=xml::ENT_Element)continue;
		xml::XMLElement* elem=dynamic_cast<xml::XMLElement*>(*it);
		xml::XMLAttribute*attr;
		attr=elem->getAttribute(mT("Path"));
		if(attr)
		{
			LoadInfo(attr->value);
		}
	}

	stream->close();
}

void GameLevelManager::LoadInfo(const core::string&path)
{
	GCPtr<OS::IStream> stream=gFileSystem.openFile(path,OS::TXT_READ);
	if(!stream)
		return;

	xml::XMLTree tree;
	if(!tree.load(stream))
	{
		stream->close();
		return;
	}
	xml::XMLElement*e=tree.getSubElement(mT("GameLevel"));
	if(!e){
		stream->close();
		return;
	}
	xml::XMLAttribute*attr;
	GameLevelInfo info;
	info.path=path;
	attr=e->getAttribute(mT("Name"));
	if(attr)
	{
		info.name=attr->value;
	}
	attr=e->getAttribute(mT("Description"));
	if(attr)
	{
		info.description=attr->value;
	}
	attr=e->getAttribute(mT("Thumbnail"));
	if(attr)
	{
		info.thumbnail=attr->value;
	}
	m_gameLevelsInfo.push_back(info);

	stream->close();
}

std::vector<GameLevelInfo>& GameLevelManager::GetLoadedLevelsInfo()
{
	return m_gameLevelsInfo;
}
void GameLevelManager::ClearLoadedLevelsInfo()
{
	m_gameLevelsInfo.clear();
}

void GameLevelManager::AddLevel(GCPtr<GameLevel> level)
{
	if(!level)
		return;
	m_levels[level->GetLevelName()]=level;
}

void GameLevelManager::RemoveLevel(GCPtr<GameLevel> level)
{
	if(!level)
		return;
	m_levels.erase(level->GetLevelName());
}
void GameLevelManager::SetActiveLevel(GCPtr<GameLevel> level)
{
	m_activeLevel=level;
}
GCPtr<GameLevel> GameLevelManager::GetActiveLevel()
{
	return m_activeLevel;
}

}
}


#include "stdafx.h"
#include "CharacterManager.h"
#include "Character.h"
#include "Environment.h"

namespace mray
{
namespace PhysicsBasedCharacter
{

CharacterManager::CharacterManager()
{
	m_phmngr=0;
	m_environment=new Environment();
}
CharacterManager::~CharacterManager()
{
	ClearCharacters();
	delete m_environment;
}

void CharacterManager::SetPhysicsManager(physics::IPhysicManager*mngr)
{
	m_phmngr=mngr;
	m_environment->OnSetPhysicsManager(m_phmngr);
}
physics::IPhysicManager* CharacterManager::GetPhysicsManager()
{
	return m_phmngr;
}
CharacterPtr CharacterManager::CreateCharacter(const core::string& descriptionFile, scene::Skeleton* skeleton,float scale,const math::vector3d &pos,IObject*data)
{
	CharacterPtr c=new Character(this);
	if(!c->Create(descriptionFile,skeleton,scale,pos,m_phmngr,data))
	{
		return CharacterPtr::Null;
	}
	m_characters.push_back(c);
	return c;
}
void CharacterManager::RemoveCharacter(Character*c)
{
	CharacterList::iterator it=m_characters.begin();
	for(;it!=m_characters.end();++it)
	{
		if((*it).pointer()==c)
		{
			delete *it;
			m_characters.erase(it);
			return;
		}
	}
}

Environment* CharacterManager::GetEnvironment()
{
	return m_environment;
}

void CharacterManager::ClearCharacters()
{
// 	CharacterList::iterator it=m_characters.begin();
// 	for(;it!=m_characters.end();++it)
// 	{
// 		delete *it;
// 	}
	m_characters.clear();
}

void CharacterManager::Update(float dt)
{
	CharacterList::iterator it=m_characters.begin();
	for(;it!=m_characters.end();++it)
	{
		(*it)->update(dt);
	}
	m_environment->update(dt);
}

}
}


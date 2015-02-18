

#include "stdafx.h"
#include "JFGameLevelManager.h"

#include "JFIBullet.h"
#include "JFObject.h"


namespace mray
{
namespace game
{

class JFObject;
class JFIBullet;


JFGameLevelManager::JFGameLevelManager()
{
}
JFGameLevelManager::~JFGameLevelManager()
{
}

void JFGameLevelManager::AddObject(JFObject* obj)
{
	m_objects.push_back(obj);
}
void JFGameLevelManager::RemoveObject(JFObject* obj)
{
	m_toRemoveObjects.push_back(obj);
}

void JFGameLevelManager::AddBullet(JFIBullet* b)
{
	m_bullets.push_back(b);
}
void JFGameLevelManager::RemoveBullet(JFIBullet* b)
{
	m_toRemoveBullets.push_back(b);
}

void JFGameLevelManager::Update(float dt)
{

	for(std::list<JFObject*>::iterator it= m_objects.begin();
		it!=m_objects.end();++it)
	{
		(*it)->Update(dt);
	}
	for(std::list<JFIBullet*>::iterator it= m_bullets.begin();
		it!=m_bullets.end();++it)
	{
		(*it)->Update(dt);
	}
	_ProcessRemoveRequests();
}

void JFGameLevelManager::_ProcessRemoveRequests()
{
	//process remove requests
	_ProcessRemoveRequests<JFObject>(m_toRemoveObjects,m_objects);
	_ProcessRemoveRequests<JFIBullet>(m_toRemoveBullets,m_bullets);
}

}
}



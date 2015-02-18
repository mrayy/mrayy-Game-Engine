
#include "stdafx.h"
#include "CHouse.h"

#include "SceneHelper.h"

#include "Environment.h"

namespace mray
{
namespace GameMod
{

CHouse::CHouse()
{
	m_selector=new scene::CTriangleSelector();
	m_characterManager=0;
	m_collisionMtrl=0;
	m_camera=0;
}
CHouse::~CHouse()
{
}

void CHouse::SetCharacterManager(PhysicsBasedCharacter::CharacterManager*charMngr)
{
	m_characterManager=charMngr;
	if(!m_characterManager)
		return;
	for(std::list<physics::IPhysicalNode*>::iterator it=m_collisionGrounds.begin();it!=m_collisionGrounds.end();++it)
		m_characterManager->getEnvironment()->addGroundNode(*it);

}

void CHouse::AddObject(const core::string& type,game::GameEntity*ent)
{
	m_objects[type].push_back(ent);
}
void CHouse::RemoveObject(const core::string& type,game::GameEntity*ent)
{
	std::list<game::GameEntity*>& objects=m_objects[type];
	std::list<game::GameEntity*>::iterator it=objects.begin();
	for (;it!=objects.end();++it)
	{
		if(*it==ent)
		{
			objects.erase(it);
			return;
		}
	}
}
const std::list<game::GameEntity*>& CHouse::GetObjectsByType(const core::string& type)
{
	return m_objects[type];
}
void CHouse::AddFood(CFood*f)
{
	m_food.push_back(f);
}
const FoodList& CHouse::GetFoods()
{
	return m_food;
}

void CHouse::AddCollisionGround(physics::IPhysicalNode* node)
{
	m_collisionGrounds.push_back(node);
	if(m_collisionMtrl)
		node->setMaterialIndex(m_collisionMtrl->getMaterialIndex());
	if(m_characterManager)
		m_characterManager->getEnvironment()->addGroundNode(node);
}
void CHouse::RemoveCollisionGround(physics::IPhysicalNode* node)
{
	if(m_characterManager)
		m_characterManager->getEnvironment()->removeGroundNode(node);
	std::list<physics::IPhysicalNode*>::iterator it=m_collisionGrounds.begin();
	for (;it!=m_collisionGrounds.end();++it)
	{
		if(*it==node)
		{
			m_collisionGrounds.erase(it);
			return;
		}
	}
}
const std::list<physics::IPhysicalNode*>& CHouse::GetCollisionGrounds()
{
	return m_collisionGrounds;
}
void CHouse::AddCharacter(CharacterObject*c)
{
	m_characters.push_back(c);
}
void CHouse::RemoveCharacter(CharacterObject*c)
{
	CharactersList::iterator it=m_characters.begin();
	for (;it!=m_characters.end();++it)
	{
		if(*it==c)
		{
			m_characters.erase(it);
			return;
		}
	}
}
const CharactersList& CHouse::GetCharacters()
{
	return m_characters;
}

void CHouse::SetSelectorMesh(scene::SMesh*mesh,const math::matrix4x4*transform)
{
	m_selector->setTriangles(mesh,transform);
}
bool CHouse::CastRay(const math::vector3d& start,const math::vector3d& end,math::vector3d& out)
{
	math::triangle3d t;
	float dist=scene::SceneHelper::getInstance().getCollisionTriangle(m_selector,math::line3d(start,end),t,out);

	if(dist>0)
		return true;
	return false;
}


}
}

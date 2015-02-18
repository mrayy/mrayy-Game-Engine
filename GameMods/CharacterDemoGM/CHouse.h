

/********************************************************************
	created:	2010/07/01
	created:	1:7:2010   13:33
	filename: 	i:\Programing\GameEngine\mrayEngine\gameMods\CharacterDemoGM\House.h
	file path:	i:\Programing\GameEngine\mrayEngine\gameMods\CharacterDemoGM
	file base:	House
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef House_h__
#define House_h__

#include "ISingleton.h"
#include "CFood.h"
#include "CTriangleSelector.h"
#include "CharacterManager.h"
#include <list>

namespace mray
{
namespace GameMod
{

	class CharacterObject;

typedef std::list<CFood*> FoodList;
typedef std::list<CharacterObject*> CharactersList;

class CHARACTERDEMOGM_API CHouse:public ISingleton<CHouse>
{
private:
protected:
	typedef std::map<core::string,std::list<game::GameEntity*>> ObjectsMap;

	FoodList m_food;
	CharactersList m_characters;
	GCPtr<scene::CTriangleSelector> m_selector;
	ObjectsMap m_objects;

	physics::IPhysicMaterial* m_collisionMtrl;

	scene::SCameraNode* m_camera;

	PhysicsBasedCharacter::CharacterManager* m_characterManager;

	std::list<physics::IPhysicalNode*> m_collisionGrounds;
public:
	CHouse();
	virtual ~CHouse();

	scene::SCameraNode* GetCamera(){return m_camera;}
	void SetCamera(scene::SCameraNode*cam){m_camera=cam;}

	physics::IPhysicMaterial* GetCollisionMtrl(){return m_collisionMtrl;}
	void SetCollisionMtrl(physics::IPhysicMaterial*mtrl){m_collisionMtrl=mtrl;}

	void SetCharacterManager(PhysicsBasedCharacter::CharacterManager*charMngr);
	PhysicsBasedCharacter::CharacterManager* GetCharacterManager(){return m_characterManager;}

	void AddObject(const core::string& type,game::GameEntity*ent);
	void RemoveObject(const core::string& type,game::GameEntity*ent);
	const std::list<game::GameEntity*>& GetObjectsByType(const core::string& type);

	void AddCollisionGround(physics::IPhysicalNode* node);
	void RemoveCollisionGround(physics::IPhysicalNode* node);
	const std::list<physics::IPhysicalNode*>& GetCollisionGrounds();

	void AddFood(CFood*f);
	const FoodList& GetFoods();

	void AddCharacter(CharacterObject*c);
	void RemoveCharacter(CharacterObject*c);
	const CharactersList& GetCharacters();

	void SetSelectorMesh(scene::SMesh*mesh,const math::matrix4x4*transform);
	bool CastRay(const math::vector3d& start,const math::vector3d& end,math::vector3d& out);
};

}
}

#endif // House_h__

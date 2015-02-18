


/********************************************************************
	created:	2011/07/14
	created:	14:7:2011   14:42
	filename: 	d:\Development\mrayEngine\Engine\PhCharacterLib\CharacterManager.h
	file path:	d:\Development\mrayEngine\Engine\PhCharacterLib
	file base:	CharacterManager
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __CharacterManager__
#define __CharacterManager__

#include "compileConfig.h"
#include "Character.h"

namespace mray
{
namespace PhysicsBasedCharacter
{
	class Environment;

class MRAY_PH_CHARACTER_DLL CharacterManager
{
private:
protected:

	typedef std::list<CharacterPtr> CharacterList;
	CharacterList m_characters;
	Environment* m_environment;
	physics::IPhysicManager* m_phmngr;

public:
	CharacterManager();
	virtual~CharacterManager();

	void SetPhysicsManager(physics::IPhysicManager*mngr);
	physics::IPhysicManager* GetPhysicsManager();

	CharacterPtr CreateCharacter(const core::string& descriptionFile, scene::Skeleton* skeleton,
		float scale,const math::vector3d &pos,IObject*data=0);
	void RemoveCharacter(Character*c);

	Environment* GetEnvironment();

	void ClearCharacters();

	void Update(float dt);


};


}
}

#endif

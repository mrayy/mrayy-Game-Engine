


/********************************************************************
	created:	2010/07/08
	created:	8:7:2010   19:23
	filename: 	i:\Programing\GameEngine\mrayEngine\gameMods\CharacterDemoGM\ShotingObjectAction.h
	file path:	i:\Programing\GameEngine\mrayEngine\gameMods\CharacterDemoGM
	file base:	ShotingObjectAction
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ShotingObjectAction_h__
#define ShotingObjectAction_h__

#include "CompileConfig.h"
#include "AIPhysicsProxy.h"
#include "IGameAction.h"
#include "IPhysicalNode.h"

namespace mray
{
namespace GameMod
{

	enum EShotingObjectType
	{
		ESOT_Ball,
		ESOT_Crate
	};

class ShotingObjectAction:public game::IGameAction
{
private:
protected:
	game::GameEntity* m_owner;
	physics::IPhysicalNode* m_phNode;

	float m_radius;
	math::vector3d m_force;

	EShotingObjectType m_type;

	float m_age;
	void deleteBall();
public:
	ShotingObjectAction(float radius,const math::vector3d& force,EShotingObjectType type);
	virtual ~ShotingObjectAction();

	virtual void AttachToEntity(game::GameEntity*ent);

	virtual void Update(float dt);
};

}
}


#endif // ShotingObjectAction_h__

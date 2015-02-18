
/********************************************************************
	created:	2009/09/30
	created:	30:9:2009   0:58
	filename: 	i:\Programing\GameEngine\mrayEngine\testFrameWork\TestProject\GameEntity.h
	file path:	i:\Programing\GameEngine\mrayEngine\testFrameWork\TestProject
	file base:	GameEntity
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___GameEntity___
#define ___GameEntity___

#include <GCPtr.h>
#include <IMovable.h>
#include <IAIPhysics.h>

namespace mray{
	namespace scene
	{
		class IMovable;
	}

class GameEntity
{
private:
protected:
	scene::IMovable* m_sceneNode;
	AI::IAIPhysics* m_physicNode;

	bool m_isPhysicsControlled;
public:
	GameEntity();
	GameEntity(scene::IMovable* sceneNode,AI::IAIPhysics* physicNode);
	virtual~GameEntity();

	void setNode(scene::IMovable* n);
	scene::IMovable* getNode();


	void setPhysicalNode(AI::IAIPhysics* n);
	AI::IAIPhysics* getPhysicalNode();

	void setPhysicsControlled(bool s);
	bool isPhysicsControlled();

	void update(float dt);
};

}


#endif //___GameEntity___

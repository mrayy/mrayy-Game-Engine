

/********************************************************************
	created:	2009/09/30
	created:	30:9:2009   0:59
	filename: 	i:\Programing\GameEngine\mrayEngine\testFrameWork\TestProject\GameEntityManager.h
	file path:	i:\Programing\GameEngine\mrayEngine\testFrameWork\TestProject
	file base:	GameEntityManager
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___GameEntityManager___
#define ___GameEntityManager___


#include <list>

namespace mray{
	class GameEntity;

class GameEntityManager
{
private:
protected:
	typedef std::list<GCPtr<GameEntity>> EntityList;
	EntityList m_entities;
public:
	GameEntityManager();
	virtual~GameEntityManager();

	void addEntity(GCPtr<GameEntity> e);
	void removeEntity(GCPtr<GameEntity> e);

	void clear();

	void update(float dt);
};

}


#endif //___GameEntityManager___

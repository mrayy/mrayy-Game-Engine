

/********************************************************************
	created:	2009/05/14
	created:	14:5:2009   19:06
	filename: 	i:\Programing\GameEngine\mrayEngine\EaglesProject\src\CollisionManager.h
	file path:	i:\Programing\GameEngine\mrayEngine\EaglesProject\src
	file base:	CollisionManager
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___CollisionManager___
#define ___CollisionManager___

#include "GameEntity.h"


namespace mray{
namespace gameMod{

class CollisionManager
{
private:
protected:
public:
	CollisionManager();
	virtual~CollisionManager();

	void doCollision(core::IteratorPair<EntityList> nodes);
};

}
}


#endif //___CollisionManager___

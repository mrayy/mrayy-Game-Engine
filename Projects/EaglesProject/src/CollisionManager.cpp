
#include "CollisionManager.h"



namespace mray{
namespace gameMod{

CollisionManager::CollisionManager(){
}
CollisionManager::~CollisionManager(){
}

void CollisionManager::doCollision(core::IteratorPair<EntityList> nodes){
	core::IteratorPair<EntityList> p1(nodes);
	core::IteratorPair<EntityList> p2(nodes);
	for (;!p1.done();p1++)
	{
		if((*p1)->isPassable())
			continue;
		if((*p1)->isStaticEntity())
			continue;
		math::box3d bb=(*p1)->getBoundingBox();
		for (p2=nodes;!p2.done();p2++)
		{
			if((*p2)->isPassable())
				continue;
			if(((*p1)->getCollisionGroup() & (*p2)->getCollisionGroup())!=0)
				continue;
			if(bb.intersectWithBox((*p2)->getBoundingBox())){
				(*p1)->hasCollide();
				(*p2)->hasCollide();
				break;
			}
		}
	}
}

}
}

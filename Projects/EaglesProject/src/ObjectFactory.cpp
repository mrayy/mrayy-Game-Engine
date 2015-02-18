
#include "ObjectFactory.h"
#include <IteratorPair.h>


namespace mray{
namespace gameMod{


ObjectFactory::ObjectFactory(){
}
ObjectFactory::~ObjectFactory(){
	m_creators.clear();
}

void ObjectFactory::addObjectCreator(GCPtr<IObjectCreator> creator){
	m_creators.push_back(creator);
}

GCPtr<GameEntity> ObjectFactory::createObject(const core::string&objClass,int type){
	core::IteratorPair<CreatorList> p(m_creators);
	for (;!p.done();p++)
	{
		if((*p)->getObjectClass().equals_ignore_case(objClass)){
			return (*p)->createEntity(type);
		}
	}
	return 0;
}

}
}

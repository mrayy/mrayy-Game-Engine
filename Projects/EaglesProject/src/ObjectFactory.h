
/********************************************************************
	created:	2009/04/09
	created:	9:4:2009   22:01
	filename: 	i:\Programing\GameEngine\mrayEngine\EaglesProject\ObjectFactory.h
	file path:	i:\Programing\GameEngine\mrayEngine\EaglesProject
	file base:	ObjectFactory
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___ObjectFactory___
#define ___ObjectFactory___

#include "GameEntity.h"
#include "IObjectCreator.h"

#include <ISingleton.h>

namespace mray{
namespace gameMod{


class ObjectFactory:public ISingleton<ObjectFactory>
{
private:
protected:
	typedef std::list<GCPtr<IObjectCreator>> CreatorList;
	CreatorList m_creators;
public:
	ObjectFactory();
	virtual~ObjectFactory();

	void addObjectCreator(GCPtr<IObjectCreator> creator);

	GCPtr<GameEntity> createObject(const core::string&objClass,int type);
};

}
}


#endif //___ObjectFactory___

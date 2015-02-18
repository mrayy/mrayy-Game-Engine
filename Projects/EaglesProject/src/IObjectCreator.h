
/********************************************************************
	created:	2009/04/09
	created:	9:4:2009   22:09
	filename: 	i:\Programing\GameEngine\mrayEngine\EaglesProject\src\IObjectCreator.h
	file path:	i:\Programing\GameEngine\mrayEngine\EaglesProject\src
	file base:	IObjectCreator
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___IObjectCreator___
#define ___IObjectCreator___

#include "GameEntity.h"
#include <GCPtr.h>

namespace mray{
namespace gameMod{

class IObjectCreator
{
private:
protected:
public:
	IObjectCreator(){}
	virtual~IObjectCreator(){}

	virtual const core::string& getObjectClass()=0;
	virtual GCPtr<GameEntity> createEntity(int type)=0;
};

}
}


#endif //___IObjectCreator___

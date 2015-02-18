

/********************************************************************
	created:	2009/05/15
	created:	15:5:2009   11:33
	filename: 	i:\Programing\GameEngine\mrayEngine\EaglesProject\src\BuildingEntity.h
	file path:	i:\Programing\GameEngine\mrayEngine\EaglesProject\src
	file base:	BuildingEntity
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___BuildingEntity___
#define ___BuildingEntity___

#include "StaticEntity.h"

namespace mray{
namespace gameMod{

class BuildingEntity:public StaticEntity
{
private:
protected:
	core::string m_base;
	int m_type;
public:
	BuildingEntity(GCPtr<scene::ISceneNode> shape,const core::string&base,int type);
	virtual~BuildingEntity();

	virtual void hasCollide();
};

}
}


#endif //___BuildingEntity___



/********************************************************************
created:	2009/05/15
created:	15:5:2009   11:33
filename: 	i:\Programing\GameEngine\mrayEngine\EaglesProject\src\TreeEntity.h
file path:	i:\Programing\GameEngine\mrayEngine\EaglesProject\src
file base:	TreeEntity
file ext:	h
author:		Mohamad Yamen Saraiji

purpose:	
*********************************************************************/

#ifndef ___TreeEntity___
#define ___TreeEntity___

#include "StaticEntity.h"

namespace mray{
namespace gameMod{

class TreeEntity:public StaticEntity
{
private:
protected:
public:
	TreeEntity(GCPtr<scene::ISceneNode> shape);
	virtual~TreeEntity();

	virtual void hasCollide();
};

}
}


#endif //___TreeEntity___

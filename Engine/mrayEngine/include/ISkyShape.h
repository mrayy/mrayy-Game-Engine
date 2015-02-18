
/********************************************************************
	created:	2009/04/23
	created:	23:4:2009   19:26
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\src\ISkyShape.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\src
	file base:	ISkyShape
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___ISkyShape___
#define ___ISkyShape___

#include "MeshRenderableNode.h"

namespace mray{
namespace scene{

class MRAY_DLL ISkyShape:public MeshRenderableNode
{
private:
protected:
public:
	ISkyShape();
	virtual~ISkyShape();
};

}
}


#endif //___ISkyShape___

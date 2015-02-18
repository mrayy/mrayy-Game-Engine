
/********************************************************************
	created:	2009/03/08
	created:	8:3:2009   16:56
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\ScreenNodeSelector.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	ScreenNodeSelector
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___ScreenNodeSelector___
#define ___ScreenNodeSelector___

#include "RayNodeSelector.h"


namespace mray{
namespace scene{

	class CameraNode;

class MRAY_DLL ScreenNodeSelector:public RayNodeSelector
{
public:
	ScreenNodeSelector();
	virtual~ScreenNodeSelector();

	void setup(const math::vector2di&coord,CameraNode*camera,const MovableNodeList& nodes,uint idMask);
};

}
}


#endif //___ScreenNodeSelector___

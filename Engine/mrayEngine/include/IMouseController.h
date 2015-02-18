
/********************************************************************
	created:	2008/12/30
	created:	30:12:2008   23:10
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\IMouseController.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine
	file base:	IMouseController
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___IMouseController___
#define ___IMouseController___


//#include "ISingleton.h"
#include "compileconfig.h"
#include <point3d.h>

namespace mray{
namespace controllers{


enum EMouseButton{
	EMB_Left,
	EMB_Right,
	EMB_Middle,
	EMB_Buttons_Count
};

class MRAY_DLL IMouseController//:public ISingleton<IMouseController>
{
public:
	IMouseController(){};
	virtual ~IMouseController(){};

	virtual void setButton(EMouseButton b)=0;
	virtual void setPressed(EMouseButton button,bool p)=0;
	virtual EMouseButton getButton()=0;
	virtual bool isPressed(EMouseButton button)=0;

	virtual int getX()=0;
	virtual int getY()=0;
	virtual int getZ()=0;

	virtual float getDX()=0;
	virtual float getDY()=0;
	virtual float getDZ()=0;


	virtual void setPos(math::vector3d p)=0;
	virtual void setDPos(math::vector3d p)=0;
	virtual math::vector3d getPos()=0;
	virtual math::vector3d getDPos()=0;
};

//#define gMouseController mray::controllers::IMouseController::getInstance()

}
}


#endif //___IMouseController___


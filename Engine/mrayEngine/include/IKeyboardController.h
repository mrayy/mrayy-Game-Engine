

/********************************************************************
	created:	2008/12/30
	created:	30:12:2008   23:12
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\IKeyboardController.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine
	file base:	IKeyboardController
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___IKeyboardController___
#define ___IKeyboardController___

//#include "ISingleton.h"
#include "compileconfig.h"
#include "KeyCode.h"

namespace mray{

namespace controllers{


class MRAY_DLL IKeyboardController//:public ISingleton<IKeyboardController>
{
public:
	IKeyboardController(){};
	virtual ~IKeyboardController(){};

	virtual bool isLShiftPress()=0;
	virtual bool isRShiftPress()=0;

	virtual bool isLAltPress()=0;
	virtual bool isRAltPress()=0;

	virtual bool isLCtrlPress()=0;
	virtual bool isRCtrlPress()=0;

	virtual bool getKeyState(EKEY_CODE key)=0;
	virtual bool getCharState(mchar key)=0;

	virtual EKEY_CODE getLastKeyPressed()=0;
	virtual mchar getLastKeyPressedCode()=0;

	virtual void setShiftState(bool p,bool left)=0;
	virtual void setAltState(bool p,bool left)=0;
	virtual void setCtrlState(bool p,bool left)=0;

	virtual void setKeyState(EKEY_CODE key,mchar code,bool s)=0;

};
//#define gKeyboardController mray::controllers::IKeyboardController::getInstance()
}

}


#endif //___IKeyboardController___

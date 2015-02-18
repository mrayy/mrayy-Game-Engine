
#ifndef ___CKeyboardController___
#define ___CKeyboardController___



#include "Keycode.h"
#include "IKeyboardController.h"

namespace mray{
namespace controllers{


class MRAY_DLL CKeyboardController:public IKeyboardController
{

	bool m_keys[KEY_CODES_COUNT];
	bool m_keysChar[sizeof(mchar)];
	EKEY_CODE m_lastKey;
	 mchar m_lastkeyCode;

	bool m_shift;
	bool m_lshift;
	bool m_alt;
	bool m_lalt;
	bool m_ctrl;
	bool m_lctrl;
	
public:
	CKeyboardController();
	virtual ~CKeyboardController();

	bool isLShiftPress();
	bool isRShiftPress();

	bool isLAltPress();
	bool isRAltPress();

	bool isLCtrlPress();
	bool isRCtrlPress();

	bool getKeyState(EKEY_CODE key);
	bool getCharState(mchar key);

	EKEY_CODE getLastKeyPressed();
	 mchar getLastKeyPressedCode();

	 //set

	 void setShiftState(bool p,bool left);
	 void setAltState(bool p,bool left);
	 void setCtrlState(bool p,bool left);

	 void setKeyState(EKEY_CODE key,mchar code,bool s);
};

}
}

#endif



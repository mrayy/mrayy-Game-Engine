#include "stdafx.h"


#include "CKeyboardController.h"
#include "mraySystem.h"

namespace mray{


namespace controllers{


CKeyboardController::CKeyboardController(){
	mraySystem::memSet(m_keys,0,sizeof(m_keys));
	mraySystem::memSet(m_keysChar,0,sizeof(m_keysChar));
	
	m_shift=0;
	m_lshift=0;
	m_alt=0;
	m_lalt=0;
	m_ctrl=0;
	m_lctrl=0;
	m_lastkeyCode=0;
}
CKeyboardController::~CKeyboardController(){
}

bool CKeyboardController::isLShiftPress(){
	return m_shift && m_lshift;
}
bool CKeyboardController::isRShiftPress(){
	return m_shift && !m_lshift;
}

bool CKeyboardController::isLAltPress(){
	return m_alt && m_lalt;
}
bool CKeyboardController::isRAltPress(){
	return m_alt && !m_lalt;
}

bool CKeyboardController::isLCtrlPress(){
	return m_ctrl && m_lctrl;
}
bool CKeyboardController::isRCtrlPress(){
	return m_ctrl && !m_lctrl;
}

bool CKeyboardController::getKeyState(EKEY_CODE key){
	return m_keys[key];
}

bool CKeyboardController::getCharState(mchar key){
	return m_keysChar[key];
}

EKEY_CODE CKeyboardController::getLastKeyPressed(){
	return m_lastKey;
}

 mchar CKeyboardController::getLastKeyPressedCode(){
	return m_lastkeyCode;
}


 void CKeyboardController::setShiftState(bool p,bool left){
	 m_shift =p;
	 m_lshift=left;
 }

 void CKeyboardController::setAltState(bool p,bool left){
	 m_alt=p;
	 m_lalt=left;
 }

 void CKeyboardController::setCtrlState(bool p,bool left){
	 m_ctrl=p;
	 m_lctrl=left;

 }
 void CKeyboardController::setKeyState(EKEY_CODE key,mchar code,bool s){
	 m_keys[key]=s;
	 m_keysChar[code]=s;
	 if(s){
		 m_lastKey=key;
		 m_lastkeyCode=code;
	 }
 }

}
}



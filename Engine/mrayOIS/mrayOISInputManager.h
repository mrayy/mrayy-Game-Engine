
/********************************************************************
	created:	2009/03/14
	created:	14:3:2009   23:24
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayOIS\mrayOISInputManager.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayOIS
	file base:	mrayOISInputManager
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___mrayOISInputManager___
#define ___mrayOISInputManager___

#include <InputManager.h>
#include <marray.h>
#include <GCPtr.h>
#include <IEventReciver.h>

#include <OIS\includes\OIS.h>

namespace mray{
	class OISEventHandler;

class mrayOISInputManager:public InputManager,public IEventReciver
{
private:
protected:
	OISEventHandler*m_oisHandler;

	video::RenderWindow* m_renderWindow;
	OIS::InputManager*	m_inputManagerOIS;
	OIS::Mouse*			m_mouseOIS;
	OIS::Keyboard*		m_keyboardOIS;

	std::vector<OIS::JoyStick*>	m_joysticsOIS;
	std::vector<controllers::IJoysticController*> m_joystics;

	controllers::IMouseController*m_mouse;
	controllers::IKeyboardController*m_keyboard;

	int m_currentJoystick;
public:
	mrayOISInputManager(InputCreationPack&pack);
	virtual~mrayOISInputManager();

	virtual void onEvent(Event* e);


	video::RenderWindow* GetRenderWindow(){return m_renderWindow;}

	virtual OIS::Mouse* getOISMouse();
	virtual OIS::Keyboard* getOISKeyboard();
	virtual OIS::JoyStick* getOISJoystick(int i);

	virtual controllers::IMouseController* getMouse();
	virtual controllers::IKeyboardController* getKeyboard();
	virtual controllers::IJoysticController* getJoystick(int i);
	virtual int getNumberOfJoysticks();

	int getCurrentCaptureJoystick();

	virtual void capture();
	void OnWindowSizeChange(int x,int y);
};

}


#endif //___mrayOISInputManager___

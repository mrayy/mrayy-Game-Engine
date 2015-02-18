

#include "stdafx.h"

#include "mrayOISInputManager.h"

#include <EventMemoryManager.h>
#include <MouseEvent.h>
#include <KeyboardEvent.h>
#include <JoystickEvent.h>
#include <EventQueue.h>
#include <TraceManager.h>
#include <StringConverter.h>
#include <LogManager.h>
#include <sstream>


#include <CJoysticController.h>
#include <CMouseController.h>
#include <CKeyboardController.h>

#include <RenderWindow.h>

namespace mray{


class OISEventHandler:public OIS::MouseListener,public OIS::JoyStickListener,public OIS::KeyListener
{
	mrayOISInputManager*m_mngr;
public:

	OISEventHandler(mrayOISInputManager*mngr){
		m_mngr=mngr;
	}
	bool keyPressed( const OIS::KeyEvent &arg ) {
		GCPtr<KeyboardEvent>e=EventMemoryManager::getInstance().createEvent(ET_Keyboard);

		e->press=true;
		e->shift=m_mngr->getOISKeyboard()->isModifierDown(OIS::Keyboard::Shift);//(arg.key==OIS::KC_LSHIFT || arg.key==OIS::KC_RSHIFT);
		if(e->shift){
			e->lshift=(arg.key==OIS::KC_LSHIFT );
		}

		e->alt=m_mngr->getOISKeyboard()->isModifierDown(OIS::Keyboard::Alt);//(arg.key==OIS::KC_LMENU || arg.key==OIS::KC_RMENU);
		if(e->alt){
			e->lalt=(arg.key==OIS::KC_LMENU );
		}

		e->ctrl=m_mngr->getOISKeyboard()->isModifierDown(OIS::Keyboard::Ctrl);//(arg.key==OIS::KC_LCONTROL || arg.key==OIS::KC_RCONTROL);
		if(e->ctrl){
			e->lctrl=(arg.key==OIS::KC_LCONTROL );
		}

		//		ToAscii(wParam,lParam,keys,&keyAsc,0);

		e->SetOwnerRenderWindow(m_mngr->GetRenderWindow());
		e->Char=( mchar)arg.text;
		e->key=(EKEY_CODE)arg.key;
		if(EventQueue::isExist())
			EventQueue::getInstance().pushEvent(e);
		return true;
	}
	bool keyReleased( const OIS::KeyEvent &arg ) {
		GCPtr<KeyboardEvent>e=EventMemoryManager::getInstance().createEvent(ET_Keyboard);
		e->press=false;
		e->shift=(arg.key==OIS::KC_LSHIFT || arg.key==OIS::KC_RSHIFT);
		if(e->shift){
			e->lshift=(arg.key==OIS::KC_LSHIFT );
		}

		e->alt=(arg.key==OIS::KC_LMENU || arg.key==OIS::KC_RMENU);
		if(e->alt){
			e->lalt=(arg.key==OIS::KC_LMENU );
		}

		e->ctrl=(arg.key==OIS::KC_LCONTROL || arg.key==OIS::KC_RCONTROL);
		if(e->ctrl){
			e->lctrl=(arg.key==OIS::KC_LCONTROL );
		}

		//		ToAscii(wParam,lParam,keys,&keyAsc,0);

		e->SetOwnerRenderWindow(m_mngr->GetRenderWindow());
		e->Char=( mchar)arg.text;
		e->key=(EKEY_CODE)arg.key;
		if(EventQueue::isExist())
			EventQueue::getInstance().pushEvent(e);
		return true;
	}
	bool mouseMoved( const OIS::MouseEvent &arg ) {
		GCPtr<MouseEvent>e=EventMemoryManager::getInstance().createEvent(ET_Mouse);
		e->event=MET_MOVED;
		const OIS::MouseState& s = arg.state;
		e->pos.set(s.X.abs,s.Y.abs);
		e->rel.set(s.X.rel,s.Y.rel);
		e->MouseWheel=s.Z.abs;
		e->MouseWheelRel=s.Z.rel;
		e->vpSize.set(s.width,s.height);

		e->SetOwnerRenderWindow(m_mngr->GetRenderWindow());
		if(EventQueue::isExist())
			EventQueue::getInstance().pushEvent(e);
		return true;
	}
	bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id ) {
		GCPtr<MouseEvent>e=EventMemoryManager::getInstance().createEvent(ET_Mouse);
		switch(id){
		case OIS::MB_Left:
			e->event=MET_LEFTDOWN;
			break;
		case OIS::MB_Right:
			e->event=MET_RIGHTDOWN;
			break;
		case OIS::MB_Middle:
			e->event=MET_MIDDLEDOWN;
			break;
		}
		const OIS::MouseState& s = arg.state;
		e->pos.set(s.X.abs,s.Y.abs);
		e->rel.set(s.X.rel,s.Y.rel);
		e->MouseWheel=s.Z.abs;
		e->MouseWheelRel=s.Z.rel;
		e->vpSize.set(s.width,s.height);

		e->SetOwnerRenderWindow(m_mngr->GetRenderWindow());
		if(EventQueue::isExist())
			EventQueue::getInstance().pushEvent(e);
		return true;
	}
	bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id ) {
		GCPtr<MouseEvent>e=EventMemoryManager::getInstance().createEvent(ET_Mouse);
		switch(id){
		case OIS::MB_Left:
			e->event=MET_LEFTUP;
			break;
		case OIS::MB_Right:
			e->event=MET_RIGHTUP;
			break;
		case OIS::MB_Middle:
			e->event=MET_MIDDLEUP;
			break;
		}
		const OIS::MouseState& s = arg.state;
		e->pos.set(s.X.abs,s.Y.abs);
		e->rel.set(s.X.rel,s.Y.rel);
		e->MouseWheel=s.Z.abs;
		e->MouseWheelRel=s.Z.rel;
		e->vpSize.set(s.width,s.height);

		e->SetOwnerRenderWindow(m_mngr->GetRenderWindow());
		if(EventQueue::isExist())
			EventQueue::getInstance().pushEvent(e);
		return true;
	}
	bool buttonPressed( const OIS::JoyStickEvent &arg, int button ) {
		GCPtr<JoystickEvent>e=EventMemoryManager::getInstance().createEvent(ET_Joystick);
		e->event=JET_BUTTON_PRESSED;
		e->button=button;
		e->id=m_mngr->getCurrentCaptureJoystick();

		e->SetOwnerRenderWindow(m_mngr->GetRenderWindow());
		if(EventQueue::isExist())
			EventQueue::getInstance().pushEvent(e);

		return true;
	}
	bool buttonReleased( const OIS::JoyStickEvent &arg, int button ) {
		GCPtr<JoystickEvent>e=EventMemoryManager::getInstance().createEvent(ET_Joystick);
		e->event=JET_BUTTON_RELEASED;
		e->button=button;
		e->id=m_mngr->getCurrentCaptureJoystick();
		e->SetOwnerRenderWindow(m_mngr->GetRenderWindow());

		if(EventQueue::isExist())
			EventQueue::getInstance().pushEvent(e);

		return true;
	}
	bool axisMoved( const OIS::JoyStickEvent &arg, int axis )
	{
		GCPtr<JoystickEvent>e=EventMemoryManager::getInstance().createEvent(ET_Joystick);
		e->event=JET_AXIS_MOVED;
		OIS::Axis a= arg.state.mAxes[axis];
		//if(a.abs>2500 || a.abs<-2500)
			e->axis.abs=(a.abs/32768.0f);
	//	else
	//		e->axis.abs=0;
		e->axis.rel=a.rel;
		e->axis.absOnly=a.absOnly;
		e->axisIndex=axis;
		e->id=m_mngr->getCurrentCaptureJoystick();

		e->SetOwnerRenderWindow(m_mngr->GetRenderWindow());
		if(EventQueue::isExist())
			EventQueue::getInstance().pushEvent(e);
		return true;
	}
	bool povMoved( const OIS::JoyStickEvent &arg, int pov )
	{
		GCPtr<JoystickEvent>e=EventMemoryManager::getInstance().createEvent(ET_Joystick);
		e->event=JET_AXIS_MOVED;

		OIS::Pov p= arg.state.mPOV[pov];
		e->pov.direction=p.direction;
		e->povIndex=pov;
		e->id=m_mngr->getCurrentCaptureJoystick();

		e->SetOwnerRenderWindow(m_mngr->GetRenderWindow());
		if(EventQueue::isExist())
			EventQueue::getInstance().pushEvent(e);
		return true;
	}

	bool vector3Moved( const OIS::JoyStickEvent &arg, int index)
	{
		GCPtr<JoystickEvent>e=EventMemoryManager::getInstance().createEvent(ET_Joystick);
		e->event=JET_AXIS_MOVED;

		OIS::Vector3 v= arg.state.mVectors[index];
		e->vector.x=v.x;
		e->vector.y=v.y;
		e->vector.z=v.z;
		e->vectorIndex=index;
		e->id=m_mngr->getCurrentCaptureJoystick();

		e->SetOwnerRenderWindow(m_mngr->GetRenderWindow());
		if(EventQueue::isExist())
			EventQueue::getInstance().pushEvent(e);
		return true;
	}
};


mrayOISInputManager::mrayOISInputManager(InputCreationPack&pack){
	traceFunction(eEngine);

	
	m_currentJoystick=0;
	OIS::ParamList pl;
	
	m_renderWindow=pack.Wnd;
	if(!m_renderWindow)
		gLogManager.log("mrayOISInputManager() - Fatal Error: Trying to create InputManager without window",ELL_ERROR);
	m_renderWindow->SetInputManager(this);

	m_mouse=0;
	m_keyboard=0;

	gLogManager.startSection(mT("OIS Input Manager"));

	std::ostringstream wnd;
	size_t hWnd;
	m_renderWindow->GetCustomParam("WINDOW",&hWnd);
	wnd << (size_t)hWnd;

	pl.insert(std::make_pair( std::string("WINDOW"), wnd.str() ));

	if(pack.createMouse){
		pl.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_FOREGROUND" )));
		
		if(pack.exclusiveMouse)
			pl.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_EXCLUSIVE")));
		else
			pl.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_NONEXCLUSIVE")));
	}
	if(pack.createKeyboard){
		pl.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_FOREGROUND" )));
		if(pack.exclusiveKeyboard)
			pl.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_EXCLUSIVE")));
		else
			pl.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_NONEXCLUSIVE")));
	}
	if(pack.createJoystic){
		pl.insert(std::make_pair(std::string("w32_joystick"), std::string("DISCL_FOREGROUND" )));
		if(pack.exclusiveJoystic)
			pl.insert(std::make_pair(std::string("w32_joystick"), std::string("DISCL_EXCLUSIVE")));
		else
			pl.insert(std::make_pair(std::string("w32_joystick"), std::string("DISCL_NONEXCLUSIVE")));
	}

	try
	{
		m_inputManagerOIS = OIS::InputManager::createInputSystem(pl);
		m_inputManagerOIS->enableAddOnFactory(OIS::InputManager::AddOn_All);

		m_oisHandler=new OISEventHandler(this);

		unsigned int v = m_inputManagerOIS->getVersionNumber();
		core::string str=  mT("OIS Version: ") ;
		str+=core::StringConverter::toString((int)(v>>16 ));
		str+= mT(".") ;
		str+= core::StringConverter::toString((int)((v>>8) & 0x000000FF));
		str+= mT(".") ;
		str+= core::StringConverter::toString((int)(v & 0x000000FF));

		gLogManager.log(mT("Verion Name:")+core::StringConverter::toString(m_inputManagerOIS->getVersionName().c_str()),ELL_INFO);
		gLogManager.log(str.c_str(),ELL_INFO);

		m_mouse=new controllers::CMouseController();
		m_keyboard=new controllers::CKeyboardController();

		if(pack.createMouse){
			m_mouseOIS = (OIS::Mouse*)m_inputManagerOIS->createInputObject( OIS::OISMouse, pack.bufferedMouse );
			m_mouseOIS->setEventCallback( (OIS::MouseListener*)m_oisHandler );


			const OIS::MouseState &ms = m_mouseOIS->getMouseState();
			ms.width = pack.WinSize.x;
			ms.height = pack.WinSize.y;
		}
		if(pack.createKeyboard){
			m_keyboardOIS = (OIS::Keyboard*)m_inputManagerOIS->createInputObject( OIS::OISKeyboard, pack.bufferedKeyboard );
			m_keyboardOIS->setEventCallback( (OIS::KeyListener*)m_oisHandler );
		}

		if(pack.createJoystic){
			int numSticks = m_inputManagerOIS->getNumberOfDevices(OIS::OISJoyStick);
			m_joysticsOIS.resize(numSticks);
			m_joystics.resize(numSticks);
			for( int i = 0; i < numSticks; ++i )
			{
				m_joysticsOIS[i] = (OIS::JoyStick*)m_inputManagerOIS->createInputObject( OIS::OISJoyStick, pack.bufferedJoystic );
				m_joysticsOIS[i]->setEventCallback( (OIS::JoyStickListener*)m_oisHandler );

				controllers::CJoysticController*js = new controllers::CJoysticController();
				js->m_buttons.resize(m_joysticsOIS[i]->getJoyStickState().mButtons.size());
				js->m_Axis.resize(m_joysticsOIS[i]->getJoyStickState().mAxes.size());
				js->m_Vectors.resize(m_joysticsOIS[i]->getJoyStickState().mVectors.size());
				m_joystics[i]=js;

				m_joystics[i]->reset();
			}
		}
	}
	catch (std::exception& e)
	{
		core::string w;
		core::char_to_string(e.what(),w);
		gLogManager.log(w,ELL_ERROR);
	}

	if(EventQueue::isExist()){

		std::set<uint> eventSet;
		eventSet.insert(ET_ResizeEvent.ID());
		eventSet.insert(ET_Mouse.ID());
		eventSet.insert(ET_Keyboard.ID());
		eventSet.insert(ET_Joystick.ID());
		EventQueue::getInstance().addEventHandler(this,eventSet); 
	}
	gLogManager.endSection(1);
	gLogManager.flush();

}
mrayOISInputManager::~mrayOISInputManager(){
	traceFunction(eEngine);

	if(m_renderWindow)
		m_renderWindow->SetInputManager(0);
	if(EventQueue::isExist()){
		EventQueue::getInstance().removeEventHandler(this);
	}

	if (m_mouse)
		delete m_mouse;
	if (m_keyboard)
		delete m_keyboard;
	for (int i = 0; i < m_joystics.size(); ++i)
		delete m_joystics[i];
	m_joystics.clear();

	delete m_oisHandler;
	if(!m_inputManagerOIS)
		return;
	if(m_mouseOIS){
		m_inputManagerOIS->destroyInputObject(m_mouseOIS);
		m_mouseOIS=0;
	}
	if(m_keyboardOIS){
		m_inputManagerOIS->destroyInputObject(m_keyboardOIS);
		m_keyboardOIS=0;
	}
	for(int i=0;i<m_joysticsOIS.size();++i){
		m_inputManagerOIS->destroyInputObject(m_joysticsOIS[i]);
	}
	m_joysticsOIS.clear();
	OIS::InputManager::destroyInputSystem(m_inputManagerOIS);
	m_inputManagerOIS=0;
}


OIS::Mouse* mrayOISInputManager::getOISMouse(){
	return m_mouseOIS;
}
OIS::Keyboard* mrayOISInputManager::getOISKeyboard(){
	return m_keyboardOIS;
}
OIS::JoyStick* mrayOISInputManager::getOISJoystick(int i){
	if(i>=m_joystics.size())return 0;
	return m_joysticsOIS[i];
}


int mrayOISInputManager::getCurrentCaptureJoystick()
{
	return m_currentJoystick;
}

controllers::IMouseController* mrayOISInputManager::getMouse(){
	return m_mouse;
}
controllers::IKeyboardController* mrayOISInputManager::getKeyboard(){
	return m_keyboard;
}
controllers::IJoysticController* mrayOISInputManager::getJoystick(int i){
	if(i>=m_joystics.size())return 0;
	return m_joystics[i];
}
int mrayOISInputManager::getNumberOfJoysticks(){
	return m_joystics.size();
}

void mrayOISInputManager::onEvent(Event*event){
	if(event->getType()==ET_Joystick)
	{
		JoystickEvent*e=dynamic_cast<JoystickEvent*>(event);
		switch(e->event){
		case JET_BUTTON_PRESSED:
			m_joystics[e->id]->setButtonState(e->button,true);
			break;
		case JET_BUTTON_RELEASED:
			m_joystics[e->id]->setButtonState(e->button,false);
			break;
		case JET_AXIS_MOVED:
			m_joystics[e->id]->setAxisState(e->axisIndex,e->axis);
			break;
		case JET_POV_MOVED:
			m_joystics[e->id]->setPovState(e->povIndex,e->pov);
			break;
		case JET_VECTOR3_MOVED:
			m_joystics[e->id]->setVectorState(e->vectorIndex,e->vector);
			break;
		}
	}else if(event->getType()== ET_Mouse)
	{
		MouseEvent*e=dynamic_cast<MouseEvent*>(event);
		math::vector3di newPos(e->pos.x,e->pos.y,e->MouseWheel);
		m_mouse->setPos(newPos);
		switch(e->event){
		case MET_LEFTDOWN:
			m_mouse->setPressed(controllers::EMB_Left,true);
			break;
		case MET_LEFTUP:
			m_mouse->setPressed(controllers::EMB_Left,false);
			break;
		case MET_RIGHTDOWN:
			m_mouse->setPressed(controllers::EMB_Right,true);
			break;
		case MET_RIGHTUP:
			m_mouse->setPressed(controllers::EMB_Right,false);
			break;
		case MET_MIDDLEDOWN:
			m_mouse->setPressed(controllers::EMB_Middle,true);
			break;
		case MET_MIDDLEUP:
			m_mouse->setPressed(controllers::EMB_Middle,false);
			break;
		}
	}
	else if(event->getType()==ET_Keyboard)
	{
		KeyboardEvent*e=dynamic_cast<KeyboardEvent*>(event);
		m_keyboard->setAltState(e->alt,e->lalt);
		m_keyboard->setCtrlState(e->ctrl,e->lctrl);
		m_keyboard->setShiftState(e->shift,e->lshift);
		m_keyboard->setKeyState(e->key,e->Char,e->press);
	}
	else if(event->getType()==ET_ResizeEvent)
	{
		OnWindowSizeChange(m_renderWindow->GetSize().x,m_renderWindow->GetSize().y);
	}
}

void mrayOISInputManager::capture(){
	if(m_mouseOIS){
		m_mouse->setDPos(0);
		m_mouseOIS->capture();
	}
	if(m_keyboardOIS){
		m_keyboardOIS->capture();
	}
	for(m_currentJoystick=0;m_currentJoystick<m_joysticsOIS.size();m_currentJoystick++){
		m_joysticsOIS[m_currentJoystick]->capture();
	}
}

void mrayOISInputManager::OnWindowSizeChange(int x,int y)
{

	if(m_mouseOIS){
		m_mouseOIS->getMouseState().width=x;
		m_mouseOIS->getMouseState().height=y;
	}
}

}

/********************************************************************
	created:	2009/03/14
	created:	14:3:2009   23:21
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\InputManager.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	InputManager
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___InputManager___
#define ___InputManager___

//#include "ISingleton.h"

#include "IMouseController.h"
#include "IKeyboardController.h"
#include "IJoysticController.h"

namespace mray
{
	namespace video
	{
		class RenderWindow;
	}


struct InputCreationPack
{
	InputCreationPack(video::RenderWindow* hWnd){
		Wnd=hWnd;
		createKeyboard=true;
		createMouse=true;
		createJoystic=false;

		exclusiveKeyboard=false;
		exclusiveMouse=true;
		exclusiveJoystic=true;

		bufferedKeyboard=true;
		bufferedMouse=true;
		bufferedJoystic=true;

	}
	video::RenderWindow* Wnd;

	math::Point2di WinSize;

	bool createKeyboard;
	bool createMouse;
	bool createJoystic;

	bool exclusiveKeyboard;
	bool exclusiveMouse;
	bool exclusiveJoystic;

	bool bufferedKeyboard;
	bool bufferedMouse;
	bool bufferedJoystic;
};

class InputManager//:public ISingleton<InputManager>
{
private:
protected:
public:
	InputManager(){}
	virtual~InputManager(){}

	virtual controllers::IMouseController* getMouse()=0;
	virtual controllers::IKeyboardController* getKeyboard()=0;
	virtual controllers::IJoysticController* getJoystick(int i)=0;
	virtual int getNumberOfJoysticks()=0;

	virtual void capture()=0;

	virtual video::RenderWindow* GetRenderWindow()=0;

	virtual bool usingInputManagment(){return true;}

	virtual void OnWindowSizeChange(int x,int y)=0;
};

}


#endif //___InputManager___

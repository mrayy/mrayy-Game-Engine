
#ifndef ___Win32WindowUtils___
#define ___Win32WindowUtils___

#define WIN32_LEAN_AND_MEAN
#include <windows.h>


#include <mMath.h>
#include <Point2d.h>
#include <IEventReciver.h>
#include <InputManager.h>
#include <ICursorControl.h>

#include <GCPtr.h>

#include "compileConfig.h"

namespace mray{
	class Event;




/*!
   API window class
*/
class MRAY_Win32_DLL Win32WindowUtils:public IEventReciver
{
	GCPtr<math::MTrigTable> m_mathTrigTable;

	GCPtr<ICursorControl> m_cursorControl;

	//GCPtr<InputManager> m_inputManager;

	void windowResized();
	void resizeDevice();
	
public:

	Win32WindowUtils();
	virtual ~Win32WindowUtils();

	/*!
		run the application
		if it returned 0 thats mean the window closed
	 */
	static bool doMessagePump();

	static void shutdown();

	//! freeze execution tell we get API message
	static bool waitForMessage(){return WaitMessage();}
	
	void onEvent(Event* event);

};


};//mray

#endif
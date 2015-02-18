
/********************************************************************
	created:	2009/03/14
	created:	14:3:2009   21:03
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayGLDevice\Win32Context.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayGLDevice
	file base:	Win32Context
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___Win32Context___
#define ___Win32Context___

#include "GLContext.h"
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>

namespace mray{
namespace video{

	class GLDev;

class Win32Context:public GLContext
{
private:
protected:
	HDC m_hDc;
	HGLRC m_hRc;
	GLDev* m_device;
public:
	Win32Context(HDC hDc,HGLRC hRc,GLDev*device);
	virtual~Win32Context();


	void setCurrent();
	void endCurrent();
	void deleteContext();
	GLContext*duplicate();
};

}
}


#endif //___Win32Context___

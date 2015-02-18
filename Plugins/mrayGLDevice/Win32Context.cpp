#include "stdafx.h"


#include "Engine.h"
#include "Win32Context.h"
#include "Win32GLSupport.h"
#include "GLDev.h"
#include "VideoLoggerSystem.h"


namespace mray{
namespace video{

Win32Context::Win32Context(HDC hDc,HGLRC hRc,GLDev*device)
{
	m_device=device;
	m_hDc=hDc;
	m_hRc=hRc;
}
Win32Context::~Win32Context()
{
	m_device->removeContext(this);
}


void Win32Context::setCurrent(){
	wglMakeCurrent(m_hDc,m_hRc);
}
void Win32Context::endCurrent(){
	wglMakeCurrent(0,0);
}
void Win32Context::deleteContext(){
	if(m_hRc)
	{
		wglDeleteContext(m_hRc);
		m_hRc=0;
		m_hDc=0;
	}
}
GLContext*Win32Context::duplicate()
{
	HGLRC newContext=wglCreateContext(m_hDc);

	DWORD err=0;

	HGLRC oldrc = wglGetCurrentContext();
	HDC oldhdc = wglGetCurrentDC();
	wglMakeCurrent(0, 0);
	// Share lists with old context
	if (!wglShareLists(m_hRc, newContext))
	{
		err= GetLastError();

		wglDeleteContext(newContext);
		core::string errMsg=mT("wglShareLists() failed: ");
		errMsg+=Win32GLSupport::TranslateGLErrorString(err);
		gVideoLoggerSystem.log(errMsg,ELL_ERROR);
	}
	// restore old context
	wglMakeCurrent(oldhdc, oldrc);
	return new Win32Context(m_hDc,newContext,m_device);
}


}
}

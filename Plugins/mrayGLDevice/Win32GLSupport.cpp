

#include "stdafx.h"
#include "Win32GLSupport.h"
#include "GLWin32RenderWindow.h"
#include "Win32MonitorEnum.h"

#include <gl\ExtGL.h>
#include <GL/gl.h>
#include <GL/wglext.h>
#include <mstring.h>
#include <StringConverter.h>
#include <VideoLoggerSystem.h>
#include <sstream>
#include <algorithm>
#include <OptionContainer.h>

namespace mray
{
namespace video
{


Win32GLSupport::Win32GLSupport()
{
	m_HasPixelFormatARB=false;
	m_HasMultisample=false;
	m_HasHardwareGamma=false;
	m_device=0;

	m_InitialWindow=0;

	new Win32MonitorEnumManager();
}

Win32GLSupport::~Win32GLSupport()
{
	delete Win32MonitorEnumManager::getInstancePtr();
}


void Win32GLSupport::getConfigParams(OptionContainer& params)
{
	SOptionElement& videoMode= params[mT("Resolution")];
	SOptionElement& fullScreen= params[mT("Fullscreen")];
	SOptionElement& colorDepth= params[mT("Bits")];
	SOptionElement& vsync= params[mT("VSync")];
	SOptionElement& FSAA= params[mT("Multisample")];
	SOptionElement& Monitor= params[mT("Monitor")];

	fullScreen.valueSet.insert(mT("True"));
	fullScreen.valueSet.insert(mT("False"));

	vsync.valueSet.insert(mT("True"));
	vsync.valueSet.insert(mT("False"));


	DEVMODE DevMode;
	DevMode.dmSize = sizeof(DEVMODE);
	for (DWORD i = 0; EnumDisplaySettings(NULL, i, &DevMode); ++i)
	{
		if (DevMode.dmBitsPerPel < 16 || DevMode.dmPelsHeight < 480)
			continue;
		m_deviceModes.push_back(DevMode);
		core::string value=core::StringConverter::toString(math::vector2di(DevMode.dmPelsWidth,DevMode.dmPelsHeight));
		videoMode.valueSet.insert(value);
	}

	const std::vector<IMonitorDevice*>& monitors=Win32MonitorEnumManager::getInstance().GetMonitors();
	for(int i=0;i<monitors.size();++i)
	{
		Monitor.valueSet.insert(core::StringConverter::toString(i));//monitors[i]->GetName());
	}

	colorDepth.valueSet.insert(mT("16"));
	colorDepth.valueSet.insert(mT("24"));
	colorDepth.valueSet.insert(mT("32"));


	FSAA.valueSet.insert(mT("0"));
	FSAA.valueSet.insert(mT("1"));
	FSAA.valueSet.insert(mT("2"));
	FSAA.valueSet.insert(mT("4"));
	FSAA.valueSet.insert(mT("8"));
	FSAA.valueSet.insert(mT("16"));
}


LRESULT CALLBACK Win32GLSupport::dummyWndProc(HWND hwnd, UINT umsg, WPARAM wp, LPARAM lp)
{
	return DefWindowProc(hwnd, umsg, wp, lp);
}

void Win32GLSupport::InitaliseWGL()
{
#ifdef UNICODE
	LPCWSTR dummyText = mT("mrayWGLDummy");
#else 
	LPCSTR dummyText = mT("mrayWGLDummy");
#endif
#ifdef MRAY_GLDEVICE_LIB
	HINSTANCE hinst = GetModuleHandle( NULL );
#else
	HINSTANCE hinst = GetModuleHandle(mT("mrayGLDevice.dll"));
#endif
	WNDCLASS dummyClass;
	memset(&dummyClass, 0, sizeof(WNDCLASS));
	dummyClass.style = CS_OWNDC;
	dummyClass.hInstance = hinst;
	dummyClass.lpfnWndProc = dummyWndProc;
	dummyClass.lpszClassName = dummyText;
	RegisterClass(&dummyClass);

	HWND hwnd = CreateWindow(dummyText, dummyText,
		WS_POPUP | WS_CLIPCHILDREN,
		0, 0, 32, 32, 0, 0, hinst, 0);

	// if a simple CreateWindow fails, then boy are we in trouble...
	if (hwnd == NULL)
		gVideoLoggerSystem.log(mT("couldn't create Dummy Window for GLDevice!"),ELL_ERROR);


	// no chance of failure and no need to release thanks to CS_OWNDC
	HDC hdc = GetDC(hwnd); 

	// assign a simple OpenGL pixel format that everyone supports
	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.cColorBits = 16;
	pfd.cDepthBits = 15;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER | PFD_STEREO;
	pfd.iPixelType = PFD_TYPE_RGBA;

	// if these fail, wglCreateContext will also quietly fail
	int format;
	if ((format = ChoosePixelFormat(hdc, &pfd)) != 0)
		SetPixelFormat(hdc, format, &pfd);

	HGLRC hrc = wglCreateContext(hdc);
	if (hrc)
	{
		HGLRC oldrc = wglGetCurrentContext();
		HDC oldhdc = wglGetCurrentDC();
		// if wglMakeCurrent fails, wglGetProcAddress will return null
		wglMakeCurrent(hdc, hrc);

		PFNWGLGETEXTENSIONSSTRINGARBPROC _wglGetExtensionsStringARB =
			(PFNWGLGETEXTENSIONSSTRINGARBPROC)
			wglGetProcAddress("wglGetExtensionsStringARB");

		// check for pixel format and multisampling support
		if (_wglGetExtensionsStringARB)
		{
			std::istringstream wglexts(_wglGetExtensionsStringARB(hdc));
			std::string ext;
			while (wglexts >> ext)
			{
				if (ext == "WGL_ARB_pixel_format")
					m_HasPixelFormatARB = true;
				else if (ext == "WGL_ARB_multisample")
					m_HasMultisample = true;
				else if (ext == "WGL_EXT_framebuffer_sRGB")
					m_HasHardwareGamma = true;
			}
		}

		if (m_HasPixelFormatARB && m_HasMultisample)
		{
			// enumerate all formats w/ multisampling
			static const int iattr[] = {
				WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
				WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
				WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
				WGL_SAMPLE_BUFFERS_ARB, GL_TRUE,
				WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
				/* We are no matter about the colour, depth and stencil buffers here
				WGL_COLOR_BITS_ARB, 24,
				WGL_ALPHA_BITS_ARB, 8,
				WGL_DEPTH_BITS_ARB, 24,
				WGL_STENCIL_BITS_ARB, 8,
				*/
				WGL_SAMPLES_ARB, 2,
				0
			};
			int formats[256];
			unsigned int count;
			// cheating here.  wglChoosePixelFormatARB procc address needed later on
			// when a valid GL context does not exist and glew is not initialized yet.
			__wglewChoosePixelFormatARB =
				(PFNWGLCHOOSEPIXELFORMATARBPROC)wglGetProcAddress("wglChoosePixelFormatARB");
			if (__wglewChoosePixelFormatARB(hdc, iattr, 0, 256, formats, &count))
			{
				// determine what multisampling levels are offered
				int query = WGL_SAMPLES_ARB, samples;
				for (unsigned int i = 0; i < count; ++i)
				{
					PFNWGLGETPIXELFORMATATTRIBIVARBPROC _wglGetPixelFormatAttribivARB =
						(PFNWGLGETPIXELFORMATATTRIBIVARBPROC)
						wglGetProcAddress("wglGetPixelFormatAttribivARB");
					if (_wglGetPixelFormatAttribivARB(hdc, formats[i], 0, 1, &query, &samples))
					{
						m_fsaaSupport.push_back(samples);
					}
				}
				{

					std::sort(m_fsaaSupport.begin(), m_fsaaSupport.end());
					std::vector<int>::iterator p = std::unique(m_fsaaSupport.begin(), m_fsaaSupport.end());
					m_fsaaSupport.erase(p, m_fsaaSupport.end());
				}
			}
		}

		wglMakeCurrent(oldhdc, oldrc);
		wglDeleteContext(hrc);
	}

	// clean up our dummy window and class
	DestroyWindow(hwnd);
	UnregisterClass(dummyText, hinst);
}
bool Win32GLSupport::selectPixelFormat(HDC hdc, int colourDepth, int multisample, bool hwGamma)
{
	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(pfd));
	pfd.nSize = sizeof(pfd);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER | PFD_STEREO;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = (colourDepth > 16)? 24 : colourDepth;
	pfd.cAlphaBits = (colourDepth > 16)? 8 : 0;
	pfd.cDepthBits = 24;
	pfd.cStencilBits = 8;

	int format = 0;

	int useHwGamma = hwGamma? GL_TRUE : GL_FALSE;

	if (multisample && (!m_HasMultisample || !m_HasPixelFormatARB))
		return false;

	if (hwGamma && !m_HasHardwareGamma)
		return false;

	if ((multisample || hwGamma) && __wglewChoosePixelFormatARB)
	{

		// Use WGL to test extended caps (multisample, sRGB)
		std::vector<int> attribList;
		attribList.push_back(WGL_DRAW_TO_WINDOW_ARB); attribList.push_back(GL_TRUE);
		attribList.push_back(WGL_SUPPORT_OPENGL_ARB); attribList.push_back(GL_TRUE);
		attribList.push_back(WGL_DOUBLE_BUFFER_ARB); attribList.push_back(GL_TRUE);
		attribList.push_back(WGL_SAMPLE_BUFFERS_ARB); attribList.push_back(GL_TRUE);
		attribList.push_back(WGL_ACCELERATION_ARB); attribList.push_back(WGL_FULL_ACCELERATION_ARB);
		attribList.push_back(WGL_COLOR_BITS_ARB); attribList.push_back(pfd.cColorBits);
		attribList.push_back(WGL_ALPHA_BITS_ARB); attribList.push_back(pfd.cAlphaBits);
		attribList.push_back(WGL_DEPTH_BITS_ARB); attribList.push_back(24);
		attribList.push_back(WGL_STENCIL_BITS_ARB); attribList.push_back(8);
		attribList.push_back(WGL_STEREO_ARB); attribList.push_back(GL_TRUE);
		attribList.push_back(WGL_SAMPLES_ARB); attribList.push_back(multisample);
		if (useHwGamma && CheckExtension(mT("WGL_EXT_framebuffer_sRGB")))
		{
			attribList.push_back(WGL_FRAMEBUFFER_SRGB_CAPABLE_EXT); attribList.push_back(GL_TRUE);
		}
		// terminator
		attribList.push_back(0);


		UINT nformats;
		// ChoosePixelFormatARB proc address was obtained when setting up a dummy GL context in initialiseWGL()
		// since glew hasn't been initialized yet, we have to cheat and use the previously obtained address
		if (!__wglewChoosePixelFormatARB(hdc, &(attribList[0]), NULL, 1, &format, &nformats) || nformats <= 0)
			return false;
	}
	else
	{
		format = ChoosePixelFormat(hdc, &pfd);
	}


	return (format && SetPixelFormat(hdc, format, &pfd));
}


core::string Win32GLSupport::TranslateGLErrorString()
{
	return TranslateGLErrorString(GetLastError());
}
core::string Win32GLSupport::TranslateGLErrorString(int code)
{
	char errDesc[255];
	int i;

	// Try windows errors first
	i = FormatMessage(
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		code,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
		(LPTSTR) errDesc,
		255,
		NULL
		);

	core::string str;
	core::char_to_string(errDesc,str);
	return str;
}


void Win32GLSupport::InitExtensions()
{
	GLSupport::InitExtensions();
	initGLExt();

	// Check for W32 specific extensions probe function
	PFNWGLGETEXTENSIONSSTRINGARBPROC _wglGetExtensionsStringARB = 
		(PFNWGLGETEXTENSIONSSTRINGARBPROC)wglGetProcAddress("wglGetExtensionsStringARB");
	if(!_wglGetExtensionsStringARB)
		return;
	const char *wgl_extensions = _wglGetExtensionsStringARB(m_InitialWindow->GetHDC());
	core::string extStr;
	core::char_to_string(wgl_extensions ,extStr);
	gVideoLoggerSystem.log(mT("Supported WGL extensions: ")+extStr,ELL_INFO,EVL_Heavy);

	std::stringstream ext;
	std::string instr;
	ext << wgl_extensions;
	while(ext >> instr)
	{
		core::char_to_string( instr.c_str(),extStr);
		m_extensionList.insert(extStr);
	}
}



RenderWindow* Win32GLSupport::CreateRenderWindow(const core::string& name,const math::vector2di& size,
										 bool fullScreen,const OptionContainer&params,ulong hWND)
{
	GLWin32RenderWindow* ret=new GLWin32RenderWindow(name,this,m_device);
	ret->Create(size,fullScreen,params,hWND);
	if(!m_InitialWindow)
	{
		m_InitialWindow=ret;
	}
	return ret;
}


}
}

#include "stdafx.h"

#include "GLPBuffer.h"
#include "ILogManager.h"
#include "Win32Context.h"
#include "Engine.h"
#include "GLDev.h"


namespace mray{
namespace video{

GLPBuffer::GLPBuffer(GLDev*device,bool floatingPoint,bool depth,int auxBuffers,EPBufferTexType type,
					 int numComponents,int bitPerChannel):
	m_shareObjects(0),m_isBound(0),m_isActive(0),m_oldHDC(0),m_oldGlRC(0),
	m_shareContexts(0),m_hasTexture(0),m_hPBuffer(0),m_hDC(0),m_glRC(0),m_buffersCount(auxBuffers),
	m_device(device)
{

	m_buffers.resize(m_buffersCount);
	for(int i=0;i<m_buffersCount;i++){
		m_buffers[i]=GL_AUX0+i;
	}

	m_fattributes.push_back(WGL_DRAW_TO_PBUFFER_ARB);
	m_fattributes.push_back(true);
	m_fattributes.push_back(WGL_SUPPORT_OPENGL_ARB);
	m_fattributes.push_back(true);

	m_fattributes.push_back(WGL_PIXEL_TYPE_ARB);
	if(floatingPoint){
		m_fattributes.push_back(WGL_TYPE_RGBA_FLOAT_ARB);
	}
	else{
		m_fattributes.push_back(WGL_TYPE_RGBA_ARB);
	}

	//if(depth){
		m_fattributes.push_back(WGL_DEPTH_BITS_ARB);
		m_fattributes.push_back(15);
	//	}
	m_fattributes.push_back(WGL_STENCIL_BITS_ARB);
	m_fattributes.push_back(1);
	m_fattributes.push_back(WGL_AUX_BUFFERS_ARB);
	m_fattributes.push_back(auxBuffers);


	m_battributes.push_back(WGL_PBUFFER_LARGEST_ARB);
	m_battributes.push_back(true);

	m_battributes.push_back(WGL_TEXTURE_TARGET_ARB);
	switch(type){
		case EPBT_RECT:
			m_battributes.push_back(WGL_TEXTURE_RECTANGLE_NV);
			break;
		case EPBT_Cube:
			m_battributes.push_back(WGL_TEXTURE_CUBE_MAP_ARB);
			break;
		default:
			m_battributes.push_back(WGL_TEXTURE_2D_ARB);
	}
/**/
	if(!floatingPoint )
	{
		m_fattributes.push_back(WGL_BIND_TO_TEXTURE_RGBA_ARB);
		m_fattributes.push_back(true);

		m_battributes.push_back(WGL_TEXTURE_FORMAT_ARB);
		m_battributes.push_back(WGL_TEXTURE_RGBA_ARB);
	}

	m_fattributes.push_back(WGL_RED_BITS_ARB);
	m_fattributes.push_back(bitPerChannel);
	m_fattributes.push_back(WGL_GREEN_BITS_ARB);
	m_fattributes.push_back(bitPerChannel);
 	m_fattributes.push_back(WGL_BLUE_BITS_ARB);
 	m_fattributes.push_back(bitPerChannel);
 	m_fattributes.push_back(WGL_ALPHA_BITS_ARB);
 	m_fattributes.push_back(bitPerChannel);

	if(depth){

		m_fattributes.push_back(WGL_BIND_TO_TEXTURE_DEPTH_NV);
		m_fattributes.push_back(true);

		m_battributes.push_back(WGL_DEPTH_TEXTURE_FORMAT_NV);
		m_battributes.push_back(WGL_TEXTURE_DEPTH_COMPONENT_NV);
	}

	m_fattributes.push_back(0);
	m_battributes.push_back(0);

}
GLPBuffer::~GLPBuffer(){
}

bool GLPBuffer::initialize(int width,int height,bool shareContexts,bool shareObjects){
	HDC hdc=wglGetCurrentDC();
	HGLRC hglrc = wglGetCurrentContext();
	int format = 0;
	int nfattribs = 0;
	int niattribs = 0;

	m_size.x=width;
	m_size.y=height;

	m_shareObjects=shareObjects;
	m_shareContexts=shareContexts;

	if(m_shareContexts){
		format=GetPixelFormat(hdc);
		if(format==0){
			gLogManager.log(mT("GLPBuffer::initialize() - GetPixelFormat() Failed!"),ELL_ERROR);
			return false;
		}
	}else{
		uint nformats=0;
		wglChoosePixelFormatARB(hdc, &m_fattributes[0], NULL, 1, &format, &nformats);
		if(nformats==0){
			gLogManager.log(mT("GLPBuffer::initialize() - Couldn't find a suitable pixel format!"),ELL_ERROR);
			return false;
		}
	}
	const int piAttributes[]={
		WGL_RED_BITS_ARB,WGL_GREEN_BITS_ARB,WGL_BLUE_BITS_ARB,WGL_ALPHA_BITS_ARB,
		WGL_DEPTH_BITS_ARB,WGL_STENCIL_BITS_ARB
	};
	int piValues[sizeof(piAttributes)/sizeof(const int)];

	wglGetPixelFormatAttribivARB(hdc,format,0,
		sizeof(piAttributes)/sizeof(const int),piAttributes,piValues);

	int pattrib_default[] = { 
		0
	};
	m_hPBuffer=wglCreatePbufferARB(hdc, format, m_size.x, m_size.y, &m_battributes[0]);
	if (!m_hPBuffer)
	{
		DWORD err = GetLastError();
		core::string msg=mT("GLPBuffer::initialize() - wglCreatePbufferARB() Failed: ");
		switch(err){
		case ERROR_INVALID_PIXEL_FORMAT:
		{
			msg+=mT("ERROR_INVALID_PIXEL_FORMAT");
		}break;
		case ERROR_NO_SYSTEM_RESOURCES:
		{
			msg+=mT("ERROR_NO_SYSTEM_RESOURCES");
		}break;
		case ERROR_INVALID_DATA:
		{
			msg+=mT("ERROR_INVALID_DATA");
		}break;
		case ERROR_OUTOFMEMORY:
		{
			msg+=mT("ERROR_OUTOFMEMORY");
		}break;
		}
		gLogManager.log(msg.c_str(),ELL_ERROR);

		return false;
	}

	m_hDC=wglGetPbufferDCARB(m_hPBuffer);
	if(!m_hDC){
		gLogManager.log(mT("GLPBuffer::initialize() - wglGetPbufferDCARB() Failed!"),ELL_ERROR);
		return false;
	}
	if(m_shareContexts){
		m_glRC=hglrc;
	}else{
		m_glRC=wglCreateContext(m_hDC);
		if(!m_glRC){
			gLogManager.log(mT("GLPBuffer::initialize() - wglCreateContext() Failed!"),ELL_ERROR);
			return false;
		}
		if(m_shareObjects){
			if(!wglShareLists(hglrc ,m_glRC)){
				gLogManager.log(mT("GLPBuffer::initialize() - wglShareLists() Failed!"),ELL_ERROR);
				return false;
			}
		}
	}

	GLint texFormat=WGL_NO_TEXTURE_ARB;
	wglQueryPbufferARB(m_hPBuffer,WGL_TEXTURE_FORMAT_ARB, &texFormat);

	if(texFormat!=WGL_NO_TEXTURE_ARB)
		m_hasTexture=true;


	wglQueryPbufferARB(m_hPBuffer, WGL_PBUFFER_WIDTH_ARB, &m_size.x);
	wglQueryPbufferARB(m_hPBuffer, WGL_PBUFFER_HEIGHT_ARB, &m_size.y);


	m_context=new Win32Context(m_hDC,m_glRC,m_device);

	return true;
}

void GLPBuffer::destroy()
{
	if(m_hPBuffer){
		if(!m_shareContexts)
			wglDeleteContext(m_glRC);
		wglReleasePbufferDCARB(m_hPBuffer,m_hDC);
		wglDestroyPbufferARB(m_hPBuffer);
		m_hPBuffer=0;
	}
}

void GLPBuffer::handleModeSwitch(){
	int lost=0;

	wglQueryPbufferARB(m_hPBuffer, WGL_PBUFFER_LOST_ARB, &lost);

	if (lost)
	{
		destroy();
		initialize(m_size.x, m_size.y, m_shareContexts, m_shareObjects);
	}
}


int GLPBuffer::bind(int ibuffer){
	if(!m_hasTexture)
		return 0;

	int ret=wglBindTexImageARB(m_hPBuffer,ibuffer+WGL_AUX0_ARB);

	m_isBound=true;
	return ret;
}
int GLPBuffer::release(int ibuffer){
	if(!m_hasTexture)
		return 0;
	int ret=wglReleaseTexImageARB(m_hPBuffer,ibuffer+WGL_AUX0_ARB);

	m_isBound=false;
	return ret;
}

void GLPBuffer::clear(const video::SColor&c,bool clearBackbuffer,bool clearDepthBuffer){

	GLbitfield mask = 0;
	if (clearBackbuffer)
	{
		//glClearColor(c.R , c.G ,c.B , c.A );

		m_device->setClearColor(c);

		mask |= GL_COLOR_BUFFER_BIT;
	}
	if (clearDepthBuffer)
	{
		glDepthMask(GL_TRUE);
		mask |= GL_DEPTH_BUFFER_BIT;
	}

	for(int i=0;i<m_buffersCount;i++){
		glDrawBuffer(GL_AUX0+i);
		glClear(mask);
	}
}

void GLPBuffer::setDrawBuffers(){
	glDrawBuffersATI(m_buffers.size(), &m_buffers[0]);
}

void GLPBuffer::activate(GLPBuffer*current){
	if(current==this)
		return;


	if(!current || !current->m_isActive){
		if(m_isActive)
			return;
		m_oldHDC=wglGetCurrentDC();
		m_oldGlRC=wglGetCurrentContext();
	}else{
		m_oldHDC=current->m_hDC;
		m_oldGlRC=current->m_glRC;

		current->m_oldHDC=0;
		current->m_oldGlRC=0;
		current->m_isActive=0;
	}

	bool ret=wglMakeCurrent(m_hDC,m_glRC);
	m_isActive=true;
}
void GLPBuffer::deactivate(){
	if (!m_isActive)
		return;
	bool ret=wglMakeCurrent(m_oldHDC, m_oldGlRC);

	glDrawBuffer(GL_BACK);

	m_oldGlRC = 0;
	m_oldHDC = 0;
	m_isActive = false;
}

}
}



/********************************************************************
	created:	2009/07/18
	created:	18:7:2009   0:34
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayGLDevice\GLPBuffer.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayGLDevice
	file base:	GLPBuffer
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___GLPBuffer___
#define ___GLPBuffer___

#include <gl/ExtGL.h>
#include <vector>
#include "CompileConfig.h"
#include "Point2d.h"
#include "SColor.h"
#include "marray.h"
#include "GLContext.h"
#include "GCPtr.h"

namespace mray{
namespace video{
	class GLDev;

class MRAY_GL_DLL GLPBuffer
{
private:
protected:

	HPBUFFERARB m_hPBuffer;
	HDC m_hDC;
	HGLRC m_glRC;

	HDC m_oldHDC;
	HGLRC m_oldGlRC;

	math::vector2di m_size;

	int m_buffersCount;
	std::vector<GLenum> m_buffers ;

	bool m_isBound;
	bool m_isActive;

	bool m_hasTexture;

	bool m_shareContexts;
	bool m_shareObjects;

	std::vector<int> m_fattributes;
	std::vector<int> m_battributes;

	GCPtr<GLContext> m_context;

	GLDev* m_device;

public:

	enum EPBufferTexType{
		EPBT_RECT,
		EPBT_Cube,
		EPBT_2D,
	};

	GLPBuffer(GLDev*device,bool floatingPoint,bool depth,int auxBuffers,EPBufferTexType type,
		int numComponents=4,int bitPerChannel=8);
	virtual~GLPBuffer();

	const math::vector2di&getSize(){return m_size;}

	bool initialize(int width,int height,bool shareContexts,bool shareObjects);

	void destroy();

	void handleModeSwitch();

	void clear(const video::SColor&c,bool clearBackbuffer,bool clearDepthBuffer);

	void setDrawBuffers();

	int bind(int ibuffer);
	int release(int ibuffer);
	void activate(GLPBuffer*current=0);
	void deactivate();

	GLContext* GetContext(){return m_context;}
};

}
}


#endif //___GLPBuffer___

/********************************************************************
	created:	2009/07/18
	created:	18:7:2009   1:22
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayGLDevice\GLPBufferTexture.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayGLDevice
	file base:	GLPBufferTexture
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___GLPBufferTexture___
#define ___GLPBufferTexture___


#include "GLTexture.h"
#include "GLPBuffer.h"

#include "GLDev.h"

namespace mray{
namespace video{

class GLPBufferTexture:public GLTexture
{
private:
protected:
	GLPBuffer*m_pbuff;
	int m_ibuffer;
	GLDev*m_device;

	void create();
public:
	GLPBufferTexture(GLDev*dev,GLPBuffer*pbuff,int ibuffer);
	virtual~GLPBufferTexture();

	void setPBuffer(GLPBuffer*pbuff,int ibuffer);

	virtual void onUse();
	virtual void unuse();


	virtual bool isPBuffer(){return true;}
};

}
}


#endif //___GLPBufferTexture___

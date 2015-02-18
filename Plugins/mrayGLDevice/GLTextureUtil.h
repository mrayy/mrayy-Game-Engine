
/********************************************************************
	created:	2009/05/21
	created:	21:5:2009   21:17
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayGLDevice\GLTextureUtil.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayGLDevice
	file base:	GLTextureUtil
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___GLTextureUtil___
#define ___GLTextureUtil___

#include "CompileConfig.h"

#include <gl/glew.h>
#include <videoCommon.h>
#include <mTypes.h>

namespace mray{
namespace video{


class MRAY_GL_DLL GLTextureUtil
{
public:

	static GLenum getGLOrginalFormat(EPixelFormat f);
	static GLenum getGLDataType(EPixelFormat f);	
	static GLenum getGLInternalFormat(EPixelFormat f);
	static GLenum getClosestFormat(EPixelFormat f);
	static GLenum getTextureTarget(ETextureType t);

	static int getMaxMipmaps(int width,int height,int depth);

	static EPixelFormat getClosestPixelFormat(GLenum f);
};

}
}


#endif //___GLTextureUtil___



/********************************************************************
	created:	2013/06/09
	created:	9:6:2013   23:19
	filename: 	C:\Development\mrayEngine\Engine\mrayEngine\include\TextureSourceFactory.h
	file path:	C:\Development\mrayEngine\Engine\mrayEngine\include
	file base:	TextureSourceFactory
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __TextureSourceFactory__
#define __TextureSourceFactory__


#include "ObjectFactoryManager.h"

#include "ITextureSource.h"

namespace mray
{
namespace video
{

class  MRAY_DLL TextureSourceFactory:public ISingleton<TextureSourceFactory>, public ObjectFactoryManager<ITextureSource>
{
protected:
	typedef ObjectFactory<ITextureSource> FactoryType;
public:
	TextureSourceFactory();
	virtual~TextureSourceFactory();
};

}
}


#endif

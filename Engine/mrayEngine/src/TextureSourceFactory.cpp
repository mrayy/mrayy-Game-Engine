

#include "stdafx.h"
#include "TextureSourceFactory.h"

#include "FileTextureSource.h"
#include "VideoTextureSource.h"

namespace mray
{
namespace video
{

TextureSourceFactory::TextureSourceFactory()
{
	RegisterFactory(new TextureSourceFactory_FileTextureSource());
	RegisterFactory(new TextureSourceFactory_VideoTextureSource());
}
TextureSourceFactory::~TextureSourceFactory()
{
}


}
}

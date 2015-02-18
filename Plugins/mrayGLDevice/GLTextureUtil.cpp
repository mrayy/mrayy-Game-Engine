#include "stdafx.h"

#include "GLTextureUtil.h"



namespace mray{
namespace video{


GLenum GLTextureUtil::getGLOrginalFormat(EPixelFormat f){
	switch (f)
	{
	case EPixel_LUMINANCE8:
	case EPixel_LUMINANCE16:
		return GL_LUMINANCE;
	case EPixel_Alpha8:
		return GL_ALPHA;
	case EPixel_Alpha4Luminance4:
	case EPixel_Alpha8Luminance8:
		return GL_LUMINANCE_ALPHA;

	case EPixel_B8G8R8:
		return GL_BGR;
	case EPixel_R5G6B5:
	case EPixel_R8G8B8:
		return GL_RGB;

	case EPixel_X8B8G8R8:
	case EPixel_B8G8R8A8:
		return GL_BGRA;
	case EPixel_X8R8G8B8:
	case EPixel_R8G8B8A8:
		return GL_RGBA;

	case EPixel_Float16_R:
		return GL_LUMINANCE_ALPHA;
	case EPixel_Float16_RGB:
		return GL_RGB;
	case EPixel_Float16_RGBA:
		return GL_RGBA;
	case EPixel_Float16_GR:
		return GL_LUMINANCE_ALPHA;

	case EPixel_Float32_R:
		return GL_LUMINANCE;
	case EPixel_Float32_RGB:
		return GL_RGB;
	case EPixel_Float32_RGBA:
		return GL_RGBA;
	case EPixel_Float32_GR:
		return GL_LUMINANCE_ALPHA;

	case EPixel_Depth:
		return GL_DEPTH_COMPONENT;

	case EPixel_Stecil:
		return GL_STENCIL;

	case EPixel_Short_RGBA:
		return GL_RGBA;
	case EPixel_Short_RGB:
		return GL_RGB;
	case EPixel_Short_GR:
		return GL_LUMINANCE_ALPHA;

	case EPixel_DXT1:
		return GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
	case EPixel_DXT3:
		return GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
	case EPixel_DXT5:
		return GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;

	default:
		return GL_NONE;
	}
}
GLenum GLTextureUtil::getGLDataType(EPixelFormat f){
	switch (f)
	{
	case EPixel_LUMINANCE8:
	case EPixel_Alpha8:
	case EPixel_Alpha8Luminance8:
	case EPixel_R8G8B8:
	case EPixel_B8G8R8:
	case EPixel_R8G8B8A8:
	case EPixel_X8R8G8B8:
	case EPixel_B8G8R8A8:
	case EPixel_X8B8G8R8:
		return GL_UNSIGNED_BYTE;

	case EPixel_R5G6B5:
		return GL_UNSIGNED_SHORT_5_6_5;

	case EPixel_Float16_R:
	case EPixel_Float16_RGB:
	case EPixel_Float16_RGBA:
	case EPixel_Float16_GR:
		return GL_HALF_FLOAT_ARB;

	case EPixel_Float32_R:
	case EPixel_Float32_RGB:
	case EPixel_Float32_RGBA:
	case EPixel_Float32_GR:
		return GL_FLOAT;

	case EPixel_Depth:
		return GL_DEPTH_COMPONENT;
	case EPixel_Stecil:
		return GL_STENCIL;

	case EPixel_LUMINANCE16:
	case EPixel_Short_RGBA:
	case EPixel_Short_RGB:
	case EPixel_Short_GR:
		return GL_UNSIGNED_SHORT;

	default:
		return GL_NONE;
	}
}

GLenum GLTextureUtil::getGLInternalFormat(EPixelFormat f){
	switch (f)
	{
	case EPixel_LUMINANCE8:
		return GL_LUMINANCE8;
	case EPixel_LUMINANCE16:
		return GL_LUMINANCE16;
	case EPixel_Alpha8:
		return GL_ALPHA8;
	case EPixel_Alpha4Luminance4:
		return GL_LUMINANCE4_ALPHA4;
	case EPixel_Alpha8Luminance8:
		return GL_LUMINANCE8_ALPHA8;

	case EPixel_R5G6B5:
		return GL_RGB5;

	case EPixel_B8G8R8:
	case EPixel_X8B8G8R8:
	case EPixel_R8G8B8:
	case EPixel_X8R8G8B8:
		return GL_RGB8;
	case EPixel_B8G8R8A8:
	case EPixel_R8G8B8A8:
		return GL_RGBA8;

	case EPixel_Float16_R:
		return GL_LUMINANCE16F_ARB;
	case EPixel_Float16_RGB:
		return GL_RGB16F_ARB;
	case EPixel_Float16_RGBA:
		return GL_RGBA16F_ARB;
	case EPixel_Float16_GR:
		return GL_LUMINANCE_ALPHA16F_ARB;

	case EPixel_Float32_R:
		return GL_LUMINANCE32F_ARB;
	case EPixel_Float32_RGB:
		return GL_RGB32F_ARB;
	case EPixel_Float32_RGBA:
		return GL_RGBA32F_ARB;
	case EPixel_Float32_GR:
		return GL_LUMINANCE_ALPHA32F_ARB;

	case EPixel_Depth:
		return GL_DEPTH_COMPONENT24_ARB;

	case EPixel_Short_RGBA:
		return GL_RGBA16;
	case EPixel_Short_RGB:
		return GL_RGB16;
	case EPixel_Short_GR:
		return GL_LUMINANCE16_ALPHA16;

	case EPixel_DXT1:
		return GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
	case EPixel_DXT3:
		return GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
	case EPixel_DXT5:
		return GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;

	case EPixel_Stecil:
	default:
		return GL_NONE;
	}
}
GLenum GLTextureUtil::getClosestFormat(EPixelFormat f){
	GLenum ff=getGLInternalFormat(f);
	if(ff==GL_NONE)
		return GL_RGBA8;

	return ff;
}

EPixelFormat GLTextureUtil::getClosestPixelFormat(GLenum f){
	switch (f)
	{
	case GL_LUMINANCE8:
		return EPixel_LUMINANCE8;
	case GL_LUMINANCE16:
		return EPixel_LUMINANCE16;
	case GL_ALPHA8:
		return EPixel_Alpha8;
	case GL_LUMINANCE4_ALPHA4:
		return EPixel_Alpha4Luminance4;
	case GL_LUMINANCE8_ALPHA8:
		return EPixel_Alpha8Luminance8;

	case GL_BGR:
		return EPixel_B8G8R8;
	case GL_BGRA:
		return EPixel_B8G8R8A8;

	case GL_RGB5:
		return EPixel_R5G6B5;
	case GL_RGB8:
		return EPixel_R8G8B8;
	case GL_RGBA8:
		return EPixel_R8G8B8A8;

	case GL_LUMINANCE16F_ARB:
		return EPixel_Float16_R;
	case GL_RGB16F_ARB:
		return EPixel_Float16_RGB;
	case GL_RGBA16F_ARB:
		return EPixel_Float16_RGBA;
	case GL_LUMINANCE_ALPHA16F_ARB:
		return EPixel_Float16_GR;

	case GL_LUMINANCE32F_ARB:
		return EPixel_Float32_R;
	case GL_RGB32F_ARB:
		return EPixel_Float32_RGB;
	case GL_RGBA32F_ARB:
		return EPixel_Float32_RGBA;
	case GL_LUMINANCE_ALPHA32F_ARB:
		return EPixel_Float32_GR;

	case GL_DEPTH:
		return EPixel_Depth;
	case GL_STENCIL:
		return EPixel_Stecil;

	case GL_RGBA16:
		return EPixel_Short_RGBA;
	case GL_RGB16:
		return EPixel_Short_RGB;
	case GL_LUMINANCE16_ALPHA16:
		return EPixel_Short_GR;

	case GL_COMPRESSED_RGBA_S3TC_DXT1_EXT:
		return EPixel_DXT1;
	case GL_COMPRESSED_RGBA_S3TC_DXT3_EXT:
		return EPixel_DXT3;
	case GL_COMPRESSED_RGBA_S3TC_DXT5_EXT:
		return EPixel_DXT5;

	default:
		return EPixel_R8G8B8A8;
	}
}

GLenum GLTextureUtil::getTextureTarget(ETextureType t)
{
	switch (t)
	{
	case ETT_1DTex:
		return GL_TEXTURE_1D;
	case ETT_2DTex:
		return GL_TEXTURE_2D;
	case ETT_3DTex:
		return GL_TEXTURE_3D;
	case ETT_CubeTex:
		return GL_TEXTURE_CUBE_MAP;
	}
	return 0;
}

int GLTextureUtil::getMaxMipmaps(int width,int height,int depth)
{
	int cnt=0;
	do 
	{
		if(width>1)width=width>>1;
		if(height>1)height=height>>1;
		if(depth>1)depth=depth>>1;
		++cnt;
		//printf("%d,%d,%d\n",width,height,depth);
	} while (!(width<=1 && height<=1 && depth<=1));

	return cnt;
}

}
}



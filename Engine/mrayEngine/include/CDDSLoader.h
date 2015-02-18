

/********************************************************************
	created:	2009/01/16
	created:	16:1:2009   10:18
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\CDDSLoader.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	CDDSLoader
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___CDDSLoader___
#define ___CDDSLoader___



#include "mTypes.h"
#include "ITexLoader.h"

namespace mray{
namespace loaders{

class MRAY_DLL CDDSLoader:public ITexLoader
{
	struct __declspec(align(1)) sDDPixelFormat 
	{
		ulong dwSize;                         // always 32
		ulong dwFlags;                        // indicator for valid fields
		ulong dwFourCC;                       // "DXT1".."DXT5"
		ulong dwRGBBitCount;                  // 16,24 or 32 bits
		ulong dwRBitMask;                     // mask for red channel
		ulong dwGBitMask;                     // mask for green channel
		ulong dwBBitMask;                     // mask for blue channel
		ulong dwABitMask;                     // mask for alpha channel
	};
	struct __declspec(align(1)) sDDSurfaceDesc
	{
		ulong  dwSize;                               // always 124...
		ulong  dwFlags;                              // texture type
		ulong  dwHeight;                             // height
		ulong  dwWidth;                              // width
		ulong  dwPitchOrLinearSize;                  // bytes per scanline
		ulong  dwDepth;                              // depth for volume textures
		ulong  dwMipMapCount;                        // number of mipmap items
		ulong  dwReserved1[11];                      // ...
		sDDPixelFormat ddpfPixelFormat;
		ulong  dwCaps1;
		ulong  dwCaps2;
		ulong  dwReserved2[3];
	};
	struct __declspec(align(1)) sDDSHeader
	{
		ulong  FourCC;                              // 'magic' FourCC 
		sDDSurfaceDesc SurfaceFormat;                       // surface format
	};


	video::ETextureType getTextureFormat(OS::IStream* Stream, bool CubeMapCrossFormat);

public:
	CDDSLoader();
	virtual bool load(OS::IStream* file,video::ImageInfo*tex,video::ETextureType target);
	virtual bool load(const  core::string&filename,video::ImageInfo*tex,video::ETextureType target);
	virtual bool canLoad(const  core::string&filename);
	virtual bool canLoad(OS::IStream* file);


	virtual const core::string& getExt();

	virtual const core::string& getDescription();

	virtual bool support(video::ETextureType type);
};

}
}


#endif //___CDDSLoader___

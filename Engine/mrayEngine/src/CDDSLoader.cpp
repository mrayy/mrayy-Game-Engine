#include "stdafx.h"

#include "CDDSLoader.h"


#include "CBmpLoader.h"
#include "IStream.h"
#include "IFileSystem.h"
#include "ILogManager.h"
#include "ColorConverter.h"
#include "TextureResourceManager.h"
#include "common.h"
#include "TraceManager.h"
#include "ITexture.h"
#include "StringUtil.h"

namespace mray{
namespace loaders{

	enum{
	// Some constants in DDS file
	DDSD_CAPS          = 0x00000001,
	DDSD_HEIGHT        = 0x00000002,
	DDSD_WIDTH         = 0x00000004,
	DDSD_PITCH         = 0x00000008,
	DDSD_PIXELFORMAT   = 0x00001000,
	DDSD_MIPMAPCOUNT   = 0x00020000,
	DDSD_LINEARSIZE    = 0x00080000,
	DDSD_DEPTH         = 0x00800000,
	// The dwFlags member of the modified DDSURFACEDESC2 structure 
	// can be set to one or more of the following values.
	DDPF_ALPHAPIXELS   = 0x00000001,
	DDPF_FOURCC        = 0x00000004,
	DDPF_RGB           = 0x00000040,
	DDPF_RGBA          = 0x00000041,
	DDPF_LUMINANCE		= 0x00020000,
	// The dwCaps1 member of the DDSCAPS2 structure
	// can be set to one or more of the following values.
	DDSCAPS_COMPLEX    = 0x00000008,
	DDSCAPS_TEXTURE    = 0x00001000,
	DDSCAPS_MIPMAP     = 0x00400000,
	// The dwCaps2 member of the DDSCAPS2 structure 
	// can be set to one or more of the following values.
	DDSCAPS2_CUBEMAP            = 0x00000200,
	DDSCAPS2_CUBEMAP_POSITIVEX  = 0x00000600,
	DDSCAPS2_CUBEMAP_NEGATIVEX  = 0x00000a00,
	DDSCAPS2_CUBEMAP_POSITIVEY  = 0x00001200,
	DDSCAPS2_CUBEMAP_NEGATIVEY  = 0x00002200,
	DDSCAPS2_CUBEMAP_POSITIVEZ  = 0x00004200,
	DDSCAPS2_CUBEMAP_NEGATIVEZ  = 0x00008200,
	DDSCAPS2_VOLUME             = 0x00200000
};

enum
{
	DDS_FORMAT_UNKNOWN = 0,
	DDS_FORMAT_DXT1,
	DDS_FORMAT_DXT2,
	DDS_FORMAT_DXT3,
	DDS_FORMAT_DXT4,
	DDS_FORMAT_DXT5,
	DDS_FORMAT_RGB,
	DDS_FORMAT_RGBA
};

#define makeFourCC(c1, c2, c3, c4) \
	((ulong) c1 | ((ulong) c2<<8) | ((ulong) c3<<16) | ((ulong) c4<<24))

const ulong Magic		= makeFourCC ('D','D','S',' ');
const ulong FOURCC_DXT1 = makeFourCC('D', 'X', 'T', '1');
const ulong FOURCC_DXT3 = makeFourCC('D', 'X', 'T', '3');
const ulong FOURCC_DXT5 = makeFourCC('D', 'X', 'T', '5');

CDDSLoader::CDDSLoader()
{
}

video::ETextureType CDDSLoader::getTextureFormat(OS::IStream* stream, bool CubeMapCrossFormat)
{
	sDDSHeader FileHeader ;
	stream->read(&FileHeader,sizeof(FileHeader));

	FATAL_ERROR((FileHeader.SurfaceFormat.dwCaps1 & DDSCAPS_TEXTURE) == 0,mT("No DDSCAPS_TEXTURE flag"));

	bool Is3DTexture =   ( FileHeader.SurfaceFormat.dwCaps2 & DDSCAPS2_VOLUME  ) != 0;
	bool IsCubeTexture = ( FileHeader.SurfaceFormat.dwCaps2 & DDSCAPS2_CUBEMAP ) != 0;

	if ( CubeMapCrossFormat )
	{
		gLogManager.log(mT("2D DDS textures are not supported, so its unable to load one in a cube map cross format"),ELL_ERROR);
	}
	else
	{
		if ( Is3DTexture   ) return video::ETT_3DTex;
		if ( IsCubeTexture ) return video::ETT_CubeTex;
	}

	gLogManager.log(mT("Not a volume texture or cubemap"),ELL_ERROR);

	return video::ETT_2DTex;
}

bool CDDSLoader::load(OS::IStream* file,video::ImageInfo*tex,video::ETextureType target){
	traceFunction(eEngine);

//	sDDSHeader FileHeader;
	sDDSurfaceDesc SurfaceFormat;

	ulong fourCC;
	file->read( &fourCC, sizeof( fourCC) );

	if(fourCC!=makeFourCC('D','D','S',' ')){
		gLogManager.log(mT("Not Valid DDS File! "),file->getStreamName(),ELL_WARNING);
		return 0;
	}
	file->read( &SurfaceFormat, sizeof( SurfaceFormat) );
	bool Is3DTexture =   ( SurfaceFormat.dwCaps2 & DDSCAPS2_VOLUME  ) != 0;
	bool IsCubeTexture = ( SurfaceFormat.dwCaps2 & DDSCAPS2_CUBEMAP ) != 0;

	video::EPixelFormat format;
	int BPP = SurfaceFormat.ddpfPixelFormat.dwRGBBitCount;
	if (SurfaceFormat.ddpfPixelFormat.dwFlags & DDPF_FOURCC)
	{
		switch(SurfaceFormat.ddpfPixelFormat.dwFourCC){
			case FOURCC_DXT1:
				format=video::EPixel_DXT1;
				break;
			case FOURCC_DXT3:
				format=video::EPixel_DXT3;
				break;
			case FOURCC_DXT5:
				format=video::EPixel_DXT5;
				break;
			default:
				gLogManager.log(mT("Unsupported Compressed DDS Files , only support (DXT1,DXT3,DXT5)."),file->getStreamName(),ELL_WARNING);
				return 0;
		}
	}else {
		bool alpha= SurfaceFormat.ddpfPixelFormat.dwFlags & DDPF_FOURCC;
		switch(BPP){
		case 8:
			format=video::EPixel_LUMINANCE8;
			break;
		case 24:
			format=video::EPixel_R8G8B8;
			break;
		case 32:
			format=video::EPixel_R8G8B8A8;
			break;
		default:
			gLogManager.log(mT("Not supported pixel Format"),ELL_WARNING);
			return false;
		}
	}


	uchar* RawBGRBitmap = NULL;

	switch (target)
	{
	case video::ETT_3DTex:
		{
			if(!Is3DTexture){
				gLogManager.log(mT("trying loading non 3D texture as 3D texture"),ELL_WARNING);
				return false;
			}
			tex->createData(math::vector3di(SurfaceFormat.dwWidth,SurfaceFormat.dwHeight,SurfaceFormat.dwDepth),format);
			file->read((uchar*)tex->imageData,tex->imageDataSize);
		}break;
	case video::ETT_CubeTex:
		{
			if(!IsCubeTexture){
				gLogManager.log(mT("trying loading non Cube texture as Cube texture"),ELL_WARNING);
				return false;
			}
			FATAL_ERROR( (BPP != 24) && (BPP != 32),
				mT("Only 24-bit and 32-bit cubemaps are supported"));

			ulong Caps = SurfaceFormat.dwCaps2;

			FATAL_ERROR( (Caps & DDSCAPS2_CUBEMAP_POSITIVEX) == 0, mT("DDSCAPS2_CUBEMAP_POSITIVEX missing") );
			FATAL_ERROR( (Caps & DDSCAPS2_CUBEMAP_POSITIVEY) == 0, mT("DDSCAPS2_CUBEMAP_POSITIVEY missing") );
			FATAL_ERROR( (Caps & DDSCAPS2_CUBEMAP_POSITIVEZ) == 0, mT("DDSCAPS2_CUBEMAP_POSITIVEZ missing") );
			FATAL_ERROR( (Caps & DDSCAPS2_CUBEMAP_NEGATIVEX) == 0, mT("DDSCAPS2_CUBEMAP_NEGATIVEX missing") );
			FATAL_ERROR( (Caps & DDSCAPS2_CUBEMAP_NEGATIVEY) == 0, mT("DDSCAPS2_CUBEMAP_NEGATIVEY missing") );
			FATAL_ERROR( (Caps & DDSCAPS2_CUBEMAP_NEGATIVEZ) == 0, mT("DDSCAPS2_CUBEMAP_NEGATIVEZ missing") );

			int FDepth = 6;
			tex->createData(math::vector3di(SurfaceFormat.dwWidth,SurfaceFormat.dwHeight,6),format);

			for(int i=0;i<6;++i){
				file->read((uchar*)tex->getSurface(i),tex->format);
				//texCube->unlock((video::ECubeFace)i);
			}
		}break;
	default://load as 2d texture
		{
		}
	}

		free( RawBGRBitmap );


	return 1;
}

bool CDDSLoader::load(const  core::string&filename,video::ImageInfo*tex,video::ETextureType target)
{
	traceFunction(eEngine);
	OS::IStreamPtr fp=gFileSystem.openFile(filename,OS::BIN_READ);
	if(!fp){
		gLogManager.log(mT("Couldn't open Bmp file"),filename,ELL_WARNING);
		return 0;
	}
	return load(fp,tex,target);

}

bool CDDSLoader::canLoad(const  core::string&filename)
{
	core::string str=core::StringUtil::ToLower(filename);
	return strSearch(str.c_str(),mT(".dds"))!=0;
}


bool CDDSLoader::canLoad(OS::IStream* file)
{
	ulong fourCC;
	file->seek(0,OS::ESeek_Set);
	file->read(&fourCC,sizeof(fourCC));
	if(fourCC==542327876) //'magic' number
		return true;
	return false;
}



const core::string g_sDDSExt=mT("dds");
const core::string g_sDDSDesc=mT("DDS Image Loader");

const core::string& CDDSLoader::getExt(){
	return g_sDDSExt;
}

const core::string& CDDSLoader::getDescription(){
	return g_sDDSDesc;
}

bool CDDSLoader::support(video::ETextureType type){
	return true;
}


}
}

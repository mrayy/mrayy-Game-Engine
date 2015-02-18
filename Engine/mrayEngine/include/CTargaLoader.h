

#ifndef ___TARGA___
#define ___TARGA___

#include "mTypes.h"
#include "ITexture.h"
#include "ITexLoader.h"

namespace mray{
namespace loaders{
// byte-align structures
#ifdef _MSC_VER
#	pragma pack( push, packing )
#	pragma pack( 1 )
#	define PACK_STRUCT
#elif defined( __GNUC__ )
#	define PACK_STRUCT	__attribute__((packed))
#else
#	define PACK_STRUCT
#endif

enum TGA_TYPES
{
	TGA_NODATA=0,
	TGA_INDEXED = 1,
	TGA_RGB = 2,
	TGA_GRAYSCALE = 3,
	TGA_INDEXED_RLE = 9,
	TGA_RGB_RLE = 10,
	TGA_GRAYSCALE_RLE = 11

};

struct tgaHeader
{
	uchar idLength;
	uchar colorMapType;
	uchar imageTypeCode;
	uchar colorMapSpec[5];
	ushort xOrigin;
	ushort yOrigin;
	ushort width;
	ushort height;
	uchar pDepth;
	uchar imageDesc;
}PACK_STRUCT;
struct tgaFooter
{
	uint ExtensionOffset;
	uint DeveloperOffset;
	char  Signature[18];
} PACK_STRUCT;


#ifdef _MSC_VER
#	pragma pack( pop, packing )
#	undef PACK_STRUCT
#elif defined( __GNUC__ )
#	undef PACK_STRUCT	__attribute__((packed))
#else
#	undef PACK_STRUCT
#endif

class MRAY_DLL CTargaLoader:public ITexLoader
{
public:

	CTargaLoader();
	virtual bool load(OS::IStream* file,video::ImageInfo* image,video::ETextureType target);
	virtual bool load(const  core::string&filename,video::ImageInfo* image,video::ETextureType target);
	virtual bool canLoad(const core::string&filename);
	virtual bool canLoad(OS::IStream* file);

	virtual const core::string& getExt();

	virtual const core::string& getDescription();

	virtual bool support(video::ETextureType type);
};

}
}

#endif

#ifndef ___CBMPLOADER___
#define ___CBMPLOADER___


#include "mTypes.h"
#include "ITexLoader.h"

namespace mray{
namespace loaders{
#ifdef _MSC_VER
#	pragma pack( push, packing )
#	pragma pack( 1 )
#	define PACK_STRUCT
#elif defined( __GNUC__ )
#	define PACK_STRUCT	__attribute__((packed))
#else
#	error compiler not supported
#endif

struct SBMPHeader
{
	ushort	Id;					//	BM - Windows 3.1x, 95, NT, 98, 2000, ME, XP
										//	BA - OS/2 Bitmap Array
										//	CI - OS/2 Color Icon
										//	CP - OS/2 Color Pointer
										//	IC - OS/2 Icon
										//	PT - OS/2 Pointer
	uint	FileSize;
	uint	Reserved;
	uint	BitmapDataOffset;
	uint	BitmapHeaderSize;	// should be 28h for windows bitmaps or
										// 0Ch for OS/2 1.x or F0h for OS/2 2.x
	uint Width;
	uint Height;
	ushort Planes;
	ushort BPP;					// 1: Monochrome bitmap
										// 4: 16 color bitmap
										// 8: 256 color bitmap
										// 16: 16bit (high color) bitmap
										// 24: 24bit (true color) bitmap
										// 32: 32bit (true color) bitmap

	uint  Compression;			// 0: none (Also identified by BI_RGB)
										// 1: RLE 8-bit / pixel (Also identified by BI_RLE4)
										// 2: RLE 4-bit / pixel (Also identified by BI_RLE8)
										// 3: Bitfields  (Also identified by BI_BITFIELDS)

	uint  BitmapDataSize;		// Size of the bitmap data in bytes. This number must be rounded to the next 4 byte boundary.
	uint  PixelPerMeterX;
	uint  PixelPerMeterY;
	uint  Colors;
	uint  ImportantColors;
}PACK_STRUCT ;

#ifdef _MSC_VER
#	pragma pack( pop, packing )
#endif

#undef PACK_STRUCT

class MRAY_DLL CBmpLoader:public ITexLoader
{
	int*palData;//will contain palette data
	//uchar* bmpData;

	void decompress8BitRLE(uchar*& bmpData, int size, int width, int height, int pitch);
	void decompress4BitRLE(uchar*& bmpData, int size, int width, int height, int pitch);
public:
	CBmpLoader();
	virtual bool load(OS::IStream* file,video::ImageInfo* image,video::ETextureType target);
	virtual bool load(const  core::string&filename,video::ImageInfo* image,video::ETextureType target);
	virtual bool canLoad(const  core::string&filename);
	virtual bool canLoad(OS::IStream* file);


	virtual const core::string& getExt();

	virtual const core::string& getDescription();

	virtual bool support(video::ETextureType type);
};


};//loaders
};//mray


#endif











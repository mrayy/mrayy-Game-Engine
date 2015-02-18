



#ifndef ___CPCXLoader___
#define ___CPCXLoader___

#include "mTypes.h"
#include "ITexture.h"
#include "ITexLoader.h"

namespace mray{
namespace loaders{


// byte-align structures
#if defined(_MSC_VER) ||  defined(__BORLANDC__) || defined (__BCPLUSPLUS__) 
#	pragma pack( push, packing )
#	pragma pack( 1 )
#	define PACK_STRUCT
#endif

struct SPCX_Header{
	
	byte manufacturer;
	byte version;
	byte encoding;
	byte bitsperpixel;
	short xmin;
	short ymin;
	short xmax;
	short ymax;
	ushort horizdpi;
	ushort vertdpi;
	byte palette[48];
	byte reserved;
	byte colorplanes;
	ushort bytesperline;
	ushort palettetype;
	ushort hscrsize;
	ushort vscrsize;
	byte filler[54];
}PACK_STRUCT;

// Default alignment
#if defined(_MSC_VER) ||  defined(__BORLANDC__) || defined (__BCPLUSPLUS__) 
#	pragma pack( pop, packing )
#endif

#undef PACK_STRUCT

class MRAY_DLL CPCXLoader:public ITexLoader
{

	bool read_scanline(byte *planes[], uint num_planes, uint bytesPerLine);
	bool load8bitPCX(video::ImageInfo* image,int bytesPerLine);
	bool load24bitPCX(video::ImageInfo* image,int bytesPerLine);

	OS::IStream* file;
public:

	CPCXLoader();
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

#ifndef ___CPNGLoader___
#define ___CPNGLoader___

#include "mTypes.h"
#include "ITexLoader.h"

/*
#ifdef __cplusplus
extern "C" {
#endif

////* XXX This is from Win32's <windef.h> 
#ifndef APIENTRY
	#if (_MSC_VER >= 800) || defined(_STDCALL_SUPPORTED)
		#define APIENTRY    __stdcall
	#else
		#define APIENTRY
	#endif
#endif


	
typedef struct {
	unsigned int Width;
	unsigned int Height;
	unsigned int Depth;
	unsigned int Alpha;
} pngInfo;

typedef struct {
	unsigned int Width;
	unsigned int Height;
	unsigned int Depth;
	unsigned int Alpha;

	unsigned int Components;
	unsigned char *Data;
	unsigned char *Palette;
} pngRawInfo;




#ifdef __cplusplus
}
#endif

extern int APIENTRY pngLoadRaw(const char *filename, pngRawInfo *rawinfo);
extern int APIENTRY pngLoadRawF(FILE *file, pngRawInfo *rawinfo);

extern int APIENTRY pngLoad(const char *filename, int mipmap, int trans, pngInfo *info);
extern int APIENTRY pngLoadF(FILE *file, int mipmap, int trans, pngInfo *info);

extern unsigned int APIENTRY pngBind(const char *filename, int mipmap, int trans, pngInfo *info, int wrapst, int minfilter, int magfilter);
extern unsigned int APIENTRY pngBindF(FILE *file, int mipmap, int trans, pngInfo *info, int wrapst, int minfilter, int magfilter);

extern void APIENTRY pngSetStencil(unsigned char red, unsigned char green, unsigned char blue);
extern void APIENTRY pngSetAlphaCallback(unsigned char (*callback)(unsigned char red, unsigned char green, unsigned char blue));
extern void APIENTRY pngSetViewingGamma(double viewingGamma);
extern void APIENTRY pngSetStandardOrientation(int standardorientation);
*/
namespace mray{
namespace loaders{

class MRAY_DLL CPNGLoader:public ITexLoader
{
	uint Width;
	uint Height;
	int BitDepth;
	int ColorType;
	uchar**rowData;
public:

	CPNGLoader();
	virtual bool load(OS::IStream* file,video::ImageInfo* image,video::ETextureType target);
	virtual bool load(const  core::string&filename,video::ImageInfo* image,video::ETextureType target);
	virtual bool canLoad(const core::string&filename);
	virtual bool canLoad(OS::IStream* file);


	virtual const core::string& getExt();

	virtual const core::string& getDescription();

	virtual bool support(video::ETextureType type);

private:
	

};

}
}

#endif




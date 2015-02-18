
#ifndef ___CJPGLOADER___
#define ___CJPGLOADER___

#include <stdio.h> // required for jpeglib.h

#include "CompileConfig.h"
#include "mTypes.h"
#include "ITexLoader.h"


namespace mray{
namespace loaders{

class MRAY_DLL CJPGLoader:public ITexLoader
{
public:

	CJPGLoader();
	virtual~CJPGLoader();
	virtual bool load(OS::IStream* file,video::ImageInfo* image,video::ETextureType target);
	virtual bool load(const  core::string&filename,video::ImageInfo* image,video::ETextureType target);
	virtual bool canLoad(const  core::string&filename);
	virtual bool canLoad(OS::IStream* file);

	virtual const core::string& getExt();

	virtual const core::string& getDescription();

	virtual bool support(video::ETextureType type);
private:

	math::Point2di size;
	int depth;
	
	byte *data;
	

};

}
}

#endif
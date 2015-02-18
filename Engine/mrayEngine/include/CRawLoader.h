

#ifndef ___CRAW___
#define ___CRAW___

#include "mTypes.h"
#include "ITexLoader.h"

namespace mray{
namespace loaders{

class MRAY_DLL CRawLoader:public ITexLoader
{
public:

	CRawLoader();
	virtual bool load(OS::IStream* file,video::ImageInfo* image,video::ETextureType target);
	virtual bool load(const  core::string&filename,video::ImageInfo* image,video::ETextureType target);
	virtual bool canLoad(const core::string &name);
	virtual bool canLoad(OS::IStream* file);

	virtual const core::string& getExt();

	virtual const core::string& getDescription();
	virtual bool support(video::ETextureType type);
};
}
}


#endif


#ifndef ___ITexLoader___
#define ___ITexLoader___


#include "IFileLoader.h"
#include "videoCommon.h"


namespace mray{
namespace video{
	class ImageInfo;
}
namespace loaders{

class ITexLoader:public IFileLoader
{
protected:
public:
	ITexLoader(){};
	virtual~ITexLoader(){};
	virtual bool load(OS::IStream *file,video::ImageInfo* image,video::ETextureType target)=0;
	virtual bool load(const  core::string&name,video::ImageInfo* image,video::ETextureType target)=0;
	virtual bool canLoad(const core::string& fileName)=0;
	virtual bool canLoad(OS::IStream*file)=0;

	virtual bool support(video::ETextureType type)=0;

	virtual const core::string& getExt()=0;
	virtual const core::string& getDescription()=0;
};
};//loaders
};//mray

#endif







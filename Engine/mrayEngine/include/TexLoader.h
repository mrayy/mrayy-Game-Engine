

#ifndef ___TEXLOADER___
#define ___TEXLOADER___


#include "IStream.h"

#include "GCPtr.h"
#include "ImageInfo.h"
#include "ITexLoader.h"

namespace mray{
namespace loaders{

class MRAY_DLL TexLoader
{
public:
	typedef std::map<core::string,ITexLoader*> LoadersMap;
	LoadersMap m_loaders;
	
public:
	TexLoader();
	virtual ~TexLoader();
	void addLoader(ITexLoader*loader);
	void removeLoader(const core::string& ext);
	ITexLoader* getLoader(const core::string&ext);

	bool load(const  core::string&name,video::ImageInfo* tex,video::ETextureType target);
	bool load(OS::IStream* file,video::ImageInfo* tex,video::ETextureType target);
};

};//loaders
};//mray

#endif
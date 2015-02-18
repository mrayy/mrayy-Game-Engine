

/********************************************************************
	created:	2014/04/10
	created:	10:4:2014   2:33
	filename: 	C:\Development\mrayEngine\Engine\mrayEngine\include\InternetCacheManager.h
	file path:	C:\Development\mrayEngine\Engine\mrayEngine\include
	file base:	InternetCacheManager
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __InternetCacheManager__
#define __InternetCacheManager__


#include "ISingleton.h"
#include "IStream.h"

namespace mray
{
namespace network
{

class MRAY_DLL InternetCacheManager:public ISingleton<InternetCacheManager>
{
protected:

	struct CachedItem
	{
		uint size;
		core::string path;
		core::string url;
	};
	core::string m_cachePath;

	core::string _getPath(const core::string& url);

	typedef std::map<uint, CachedItem> CacheMap;
	CacheMap m_cachedItems;
public:
	InternetCacheManager();
	virtual~InternetCacheManager();

	void SetCachePath(const core::string& path);

	OS::IStreamPtr AddCachedItem(OS::IStream* stream);
	OS::IStreamPtr LoadCachedItem(const core::string& url);
	OS::IStreamPtr GetOrCreateItem(const core::string& url);

	void ClearCache();
	void SaveCache();
};

}
}


#endif

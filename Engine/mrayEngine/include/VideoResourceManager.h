/********************************************************************
	created:	2013/02/10
	created:	10:2:2013   0:21
	filename: 	C:\Development\mrayEngine\Engine\mrayEngine\include\VideoResourceManager.h
	file path:	C:\Development\mrayEngine\Engine\mrayEngine\include
	file base:	VideoResourceManager
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __VideoResourceManager__
#define __VideoResourceManager__

#include "IResourceManager.h"
#include "ISingleton.h"
#include "IVideoSource.h"


namespace mray
{
	class IVideoResourceLoader
	{
	public:
		virtual video::IVideoSource* load(OS::IStream *file)=0;
		virtual video::IVideoSource* load(const  core::string&name)=0;
		virtual bool canLoad(OS::IStream*file)=0;
		virtual bool canLoad(const  core::string&name)=0;

		virtual core::string getExt()=0;
		virtual core::string getDescription()=0;
	};

class MRAY_DLL VideoResourceManager:public IResourceManager,public ISingleton<VideoResourceManager>
{
protected:
	typedef std::list<IVideoResourceLoader*> LoaderList;
	LoaderList m_loaders;

protected:
	virtual IResourcePtr createInternal(const core::string& name);

	virtual IResourcePtr loadResourceFromFile(OS::IStream* file);

	virtual core::string getDefaultGroup();
public:
	VideoResourceManager();
	virtual~VideoResourceManager();

	video::IVideoSourcePtr LoadVideo(const core::string& path);

	void AddVideoLoader(IVideoResourceLoader* loader);
	void RemoveVideoLoader(IVideoResourceLoader* loader);
	bool IsLoaderSupported(const core::string& ext);
};

#define gVideoResourceManager mray::VideoResourceManager::getInstance()


}


#endif
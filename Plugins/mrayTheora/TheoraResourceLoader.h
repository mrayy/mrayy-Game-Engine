
/********************************************************************
	created:	2013/02/10
	created:	10:2:2013   15:50
	filename: 	C:\Development\mrayEngine\Plugins\mrayTheora\TheoraResourceLoader.h
	file path:	C:\Development\mrayEngine\Plugins\mrayTheora
	file base:	TheoraResourceLoader
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __TheoraResourceLoader__
#define __TheoraResourceLoader__

#include "VideoResourceManager.h"


namespace mray
{
namespace video
{

class TheoraResourceLoader:public IVideoResourceLoader
{
protected:
public:
	TheoraResourceLoader();
	virtual~TheoraResourceLoader();

	video::IVideoSource* load(OS::IStream *file);
	video::IVideoSource* load(const  core::string&name);
	bool canLoad(OS::IStream*file);
	bool canLoad(const  core::string&name);

	core::string getDescription()
	{
		return mT("Theora Video Loader");
	}
};

class OGGTheoraResourceLoader:public TheoraResourceLoader
{
public:
	core::string getExt()
	{
		return mT("OGG");
	}
};

class OGVTheoraResourceLoader:public TheoraResourceLoader
{
public:
	core::string getExt()
	{
		return mT("OGV");
	}
};

}
}


#endif

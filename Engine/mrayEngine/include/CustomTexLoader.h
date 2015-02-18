


/********************************************************************
	created:	2011/05/02
	created:	2:5:2011   17:38
	filename: 	d:\Development\mrayEngine\Engine\mrayEngine\include\CustomTexLoader.h
	file path:	d:\Development\mrayEngine\Engine\mrayEngine\include
	file base:	CustomTexLoader
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __CustomTexLoader__
#define __CustomTexLoader__

#include "mTypes.h"
#include "ITexLoader.h"

namespace mray
{
namespace loaders
{

class MRAY_DLL CustomTexLoader:public ITexLoader
{
private:
protected:
	static const core::string s_TexEXTType;
	static const core::string s_TexDesc;
public:
	CustomTexLoader(){}
	virtual~CustomTexLoader(){}

	virtual bool load(OS::IStream* file,video::ImageInfo* image,video::ETextureType target);
	virtual bool load(const  core::string&filename,video::ImageInfo* image,video::ETextureType target);
	virtual bool canLoad(const  core::string&filename);
	virtual bool canLoad(OS::IStream* file);


	virtual const core::string& getExt();

	virtual const core::string& getDescription();

	virtual bool support(video::ETextureType type);
};


}
}

#endif

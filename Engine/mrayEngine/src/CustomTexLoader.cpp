#include "stdafx.h"


#include "CustomTexLoader.h"
#include "ImageInfo.h"
#include "IFileSystem.h"
#include "IStream.h"
#include "ILogManager.h"

#include "StringUtil.h"
namespace mray
{
namespace loaders
{


	const core::string CustomTexLoader::s_TexEXTType=mT("mtex");
	const core::string CustomTexLoader::s_TexDesc=mT("");

bool CustomTexLoader::load(OS::IStream* file,video::ImageInfo* tex,video::ETextureType target)
{
	if(!tex)
		return false;

	video::ETextureType ttype;

	file->read(&ttype,sizeof(ttype));
	if(ttype!=target)
		return false;
	
	video::EPixelFormat fmt;
	math::vector3di size;
	file->read(&size,sizeof(size));
	file->read(&fmt,sizeof(fmt));

	tex->createData(size,fmt);
	file->read(tex->imageData,tex->imageDataSize);

	return true;
}
bool CustomTexLoader::load(const  core::string&filename,video::ImageInfo* image,video::ETextureType target)
{
	OS::IStreamPtr fp=gFileSystem.openFile(filename,OS::BIN_READ);
	if(!fp){
		gLogManager.log(mT("Couldn't open Texture file"),filename,ELL_WARNING);
		return 0;
	}
	return load(fp,image,target);

}
bool CustomTexLoader::canLoad(const  core::string&filename)
{
	core::string str=core::StringUtil::ToLower(filename);
	return strSearch(str.c_str(),mT(".mtex"))!=0;
}
bool CustomTexLoader::canLoad(OS::IStream* file)
{
	return false;
}


const core::string& CustomTexLoader::getExt()
{
	return s_TexEXTType;
}

const core::string& CustomTexLoader::getDescription()
{
	return s_TexDesc;
}

bool CustomTexLoader::support(video::ETextureType type)
{
	return true;
}


}
}

#include "stdafx.h"


#include "CRawLoader.h"
#include "IStream.h"
#include "IFileSystem.h"
#include "ILogManager.h"
#include "TextureResourceManager.h"
#include "common.h"
#include "TraceManager.h"
#include "StringUtil.h"

namespace mray{
namespace loaders{

CRawLoader::CRawLoader()
{
#ifdef ___DEBUG___
	setDebugName("CRawLoader");
#endif
}


bool CRawLoader::load(OS::IStream* file,video::ImageInfo* tex,video::ETextureType target)
{
	traceFunction(eEngine);

	if(!tex || target!=video::ETT_2DTex)
		return false;

	int texDim=sqrt((float)file->length()); 
	int texSize=texDim*texDim;

//	tex->setResourceName(file->getStreamName());
	tex->createData(math::vector3di(texDim , texDim,1),video::EPixel_LUMINANCE8);

	//load texture
	file->read(tex->imageData,texSize);

	//set Texture
//	tex2D->unlock();
	return tex;
}

bool CRawLoader::load(const  core::string&fileName,video::ImageInfo* tex,video::ETextureType target)
{
	traceFunction(eEngine);
	OS::IStreamPtr fp=gFileSystem.openFile(fileName,OS::BIN_READ);
	if(!fp){
		gLogManager.log(mT("Couldn't open RAW file"),fileName,ELL_WARNING);
		return 0;
	}
	return load(fp,tex,target);
}


bool CRawLoader::canLoad(const core::string &name)
{
	core::string str=core::StringUtil::ToLower(name);
	return strSearch(str.c_str(),mT(".raw"))!=0;
}

bool CRawLoader::canLoad(OS::IStream* file){
	return 0;
}

const core::string g_sRAWExt=mT("raw");
const core::string g_sRAWDesc=mT("RAW Image Loader");

const core::string& CRawLoader::getExt(){
	return g_sRAWExt;
}

const core::string& CRawLoader::getDescription(){
	return g_sRAWDesc;
}

bool CRawLoader::support(video::ETextureType type){
	return type==video::ETT_2DTex;
}


}
}

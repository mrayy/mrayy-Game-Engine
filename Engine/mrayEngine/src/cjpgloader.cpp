#include "stdafx.h"
#include "CJPGLoader.h"
#include "ColorConverter.h"
#include "IFileSystem.h"
#include <IJL\IJL.h>
#include "ILogManager.h"
#include "textureResourceManager.h"
#include "common.h"
#include "TraceManager.h"

#include "mraySystem.h"
#include <stdio.h>


namespace mray{
namespace loaders{


CJPGLoader::CJPGLoader():data(0)
{
#ifdef ___DEBUG___
	setDebugName("CJPGLoader");
#endif
}

CJPGLoader::~CJPGLoader(){
}

bool CJPGLoader::load(OS::IStream* file,video::ImageInfo* image,video::ETextureType target){

	traceFunction(eEngine);

	if(!image || target!=video::ETT_2DTex)
		return false;


	file->seek(0,OS::ESeek_Set);

	JPEG_CORE_PROPERTIES img;
	mraySystem::memSet(&img,0,sizeof(img));
	if(ijlInit(&img)!=IJL_OK){
		gLogManager.log(mT("Unable to initialize Intel JPEG Library "),file->getStreamName(),ELL_WARNING);
		return 0;
	}

	int size=file->length();

	data=new byte[size];

	file->read(data,size);
	
	img.JPGBytes=data;
	img.JPGSizeBytes=size;

	if(ijlRead( &img, IJL_JBUFF_READPARAMS) != IJL_OK){
		gLogManager.log(mT("Unable to read JPEG file "),file->getStreamName(),ELL_WARNING);
		delete [] data;
		return 0;
	}

//	image->setResourceName(file->getStreamName());

	image->createData(math::vector3di(img.JPGWidth , img.JPGHeight,1),video::EPixel_R8G8B8);

	img.DIBBytes=(uchar*)image->imageData;
	img.DIBColor=IJL_BGR;

	img.DIBWidth    = img.JPGWidth;
	img.DIBHeight   = img.JPGHeight;
	img.DIBChannels = 3;


	if(ijlRead( &img, IJL_JBUFF_READWHOLEIMAGE) != IJL_OK){
		gLogManager.log(mT("Unable to read whole JPEG image "),file->getStreamName(),ELL_WARNING);
		delete [] data;
		ijlFree( &img );
		return 0;
	}

	const int JPEG_Bits = img.DIBChannels * 8;
	video::ColorConverter::convert24BitTo24Bit(image,math::vector2di(image->Size.x,image->Size.y),0,1,1);


	ijlFree( &img );
	delete [] data;
  

	//image->unlock(true);


	return true;

}


bool CJPGLoader::load(const  core::string&filename,video::ImageInfo* image,video::ETextureType target)
{
	traceFunction(eEngine);
	OS::IStreamPtr fp=gFileSystem.openFile(filename,OS::BIN_READ);
	if(!fp){
		gLogManager.log(mT("JPG file note found "),filename,ELL_WARNING);
		return 0;
	}

	return load(fp,image,target);

}
bool CJPGLoader::canLoad(const  core::string&name)
{
	core::string str(name);
	str.make_lower();
	return strSearch(str.c_str(),mT(".jpg"))!=0;
}

bool CJPGLoader::canLoad(OS::IStream* file)
{
	int type=0;
	file->seek(6,OS::ESeek_Set);
	file->read(&type,sizeof(int));
	return (type == 0x4a464946 || type == 0x4649464a);
}


const core::string g_sJPGExt=mT("jpg");
const core::string g_sJPGDesc=mT("JPG Image Loader");

const core::string& CJPGLoader::getExt(){
	return g_sJPGExt;
}

const core::string& CJPGLoader::getDescription(){
	return g_sJPGDesc;
}



bool CJPGLoader::support(video::ETextureType type){
	return type==video::ETT_2DTex;
}


}
}

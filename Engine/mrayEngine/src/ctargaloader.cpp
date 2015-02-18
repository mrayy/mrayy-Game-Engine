#include "stdafx.h"


#include "CTargaLoader.h"
#include "IStream.h"
#include "ILogManager.h"
#include "IFileSystem.h"
#include "TextureResourceManager.h"
#include "common.h"
#include "TraceManager.h"
#include "mraySystem.h"
#include "ColorConverter.h"
#include "StringUtil.h"

namespace mray{
namespace loaders{

CTargaLoader::CTargaLoader()
{
#ifdef ___DEBUG___
	setDebugName("CTargaLoader");
#endif
}
bool CTargaLoader::load(OS::IStream* file,video::ImageInfo* image,video::ETextureType target){
	traceFunction(eEngine);

	if(!image || target!=video::ETT_2DTex)
		return false;

	file->seek(0,OS::ESeek_Set);

	tgaHeader header;
	file->read(&header,sizeof(header));
	if((header.imageTypeCode!=TGA_RGB || header.imageTypeCode!=TGA_GRAYSCALE ||
	   header.imageTypeCode!=TGA_RGB_RLE || header.imageTypeCode!=TGA_GRAYSCALE_RLE)
	   && header.colorMapType!=0)
	{
		gLogManager.log(mT("Unkown Targa image type"),file->getStreamName(),ELL_WARNING);
		return 0;
	}

	int colorMode=header.pDepth/8;

	if(colorMode==2)
	{
		gLogManager.log(mT("unsupported targa depth color"),file->getStreamName(),ELL_WARNING);
		return 0;
	}

	video::EPixelFormat format;
	
	switch(header.imageTypeCode)
	{
	case TGA_RGB:
	case TGA_RGB_RLE:
		if(colorMode==3)
		{
			format=video::EPixel_R8G8B8;
		}else
		{
			format=video::EPixel_R8G8B8A8;
		}
		break;
	case TGA_GRAYSCALE:
	case TGA_GRAYSCALE_RLE:
		format=video::EPixel_LUMINANCE8;
		break;
	}

//	image->setResourceName(file->getStreamName());

	image->createData(math::vector3di(header.width , header.height,1),format);

	uchar* data=(uchar*)image->imageData;

	if(header.idLength>0)
		file->seek(header.idLength,OS::ESeek_Cur);

	if(header.imageTypeCode==TGA_RGB || header.imageTypeCode==TGA_GRAYSCALE)
	{
		file->read(data,image->imageDataSize);
		if(image->format==video::EPixel_R8G8B8)
			video::ColorConverter::convert24BitTo24Bit(image,math::vector2d(image->Size.x,image->Size.y),0,0,true);
		else  if(image->format==video::EPixel_R8G8B8A8)
			video::ColorConverter::convert32BitTo32Bit(image,math::vector2d(image->Size.x,image->Size.y),0,0,true);
		//image->SwapRedBlue(header.pDepth);
	}else
	{
		uchar id;
		uchar length;
		uchar color[]={0,0,0,0};
		uint i=0;
		uint j=0;
		while(i<image->imageDataSize)
		{
			id=(uchar)file->readByte();
			if(id>128)
			{
				length=(uchar)(id-127);
				color[2]=(uchar)file->readByte();//B
				color[1]=(uchar)file->readByte();//G
				color[0]=(uchar)file->readByte();//R
				if(colorMode==4)
					color[3]=(uchar)file->readByte();//A
				while(length>0)
				{
					data[i++]=color[0];
					data[i++]=color[1];
					data[i++]=color[2];
					if(colorMode==4)
						data[i++]=color[3];
			
					length--;
				}
			}else
			{
				length=(uchar)(id+1);
				while(length>0)
				{
					color[2]=(uchar)file->readByte();//B
					color[1]=(uchar)file->readByte();//G
					color[0]=(uchar)file->readByte();//R
					if(colorMode==4)
						color[3]=(uchar)file->readByte();//A

					data[i++]=color[0];
					data[i++]=color[1];
					data[i++]=color[2];
					if(colorMode==4)
						data[i++]=color[3];

					length--;
				}
			}
		}
	}

 	//image->FlipVertical();
//	image->unlock();
	return true;
}

bool CTargaLoader::load(const  core::string&fileName,video::ImageInfo* image,video::ETextureType target)
{
	traceFunction(eEngine);
	OS::IStreamPtr fp=gFileSystem.openFile(fileName,OS::BIN_READ);
	if(!fp){
		gLogManager.log(mT("Couldn't open Targa file"),fileName,ELL_WARNING);
		return 0;
	}
	return load(fp,image,target);

}


bool CTargaLoader::canLoad(const core::string&name)
{
	core::string str=core::StringUtil::ToLower(name);
	return strSearch(str.c_str(),mT(".tga"))!=0;
}
bool CTargaLoader::canLoad(OS::IStream* file)
{
	tgaFooter footer;
	mraySystem::memSet(&footer, 0, sizeof(tgaFooter));
	file->seek(sizeof(tgaFooter),OS::ESeek_End);
	file->read(&footer, sizeof(tgaFooter));
	// Cann't handle old targa versions
	return (!strcmp(footer.Signature,"TRUEVISION-XFILE.")); 
}


const core::string g_sTGAExt=mT("tga");
const core::string g_sTGADesc=mT("Targa Image Loader");


const core::string& CTargaLoader::getExt(){
	return g_sTGAExt;
}

const core::string& CTargaLoader::getDescription(){
	return g_sTGADesc;
}
bool CTargaLoader::support(video::ETextureType type){
	return type==video::ETT_2DTex;
}




}
}

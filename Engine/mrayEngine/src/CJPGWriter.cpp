#include "stdafx.h"


#include "CJPGWriter.h"
#include "IStream.h"
#include "IFileSystem.h"
#include "ColorConverter.h"
#include <IJL\IJL.h>
#include "ILogManager.h"
#include "TextureResourceManager.h"
#include <stdio.h>
#include "StringConverter.h"
#include "mraySystem.h"



namespace mray{
namespace loaders{


	const core::string CJPGWriter::m_typeExt=mT("jpg");
	const core::string CJPGWriter::m_typeDesc=mT("JPG Image Writer");

CJPGWriter::CJPGWriter()
{
#ifdef ___DEBUG___
	setDebugName("CJPGWriter");
#endif
}

bool CJPGWriter::write(video::ImageInfo*tex,OS::IStream* file,video::ETextureType target)
{
	if(target!=video::ETT_2DTex){
		gLogManager.log(mT("JPG support only 2D Textures"),ELL_WARNING);
		return 0;
	}	
/*
	OS::IStream *file=gFileSystem.openFile(filename,OS::BIN_WRITE);
	if(!file){
		gLogManager.log(mT("CJPGWriter::write()-File couldn't be created",filename,ELL_WARNING);
		return 0;
	}*/

	core::stringc fnameStr;

	JPEG_CORE_PROPERTIES img;
	mraySystem::memSet(&img,0,sizeof(img));
	IJLERR err;
	if((err=ijlInit(&img))!=IJL_OK){
		gLogManager.log(core::StringConverter::toString(ijlErrorStr(err)),ELL_WARNING);
		return 0;
	}
	GCPtr<video::ImageInfo> exportImg = new video::ImageInfo();
	exportImg->createData(tex->Size,tex->format);

	math::vector2di sz(tex->Size.x,tex->Size.y);

	switch(tex->format){
		case video::EPixel_R8G8B8:
			video::ColorConverter::convert24BitTo24Bit((uchar*)tex->imageData,(uchar*)exportImg->imageData,sz,0,1,0);
			break;
		case video::EPixel_R8G8B8A8:
			video::ColorConverter::convert32BitTo24Bit((uchar*)tex->imageData,(uchar*)exportImg->imageData,sz,0,1,0);
			break;
		case video::EPixel_LUMINANCE8:
			video::ColorConverter::convert8BitTo24BitAndResize((uchar*)tex->imageData,(uchar*)exportImg->imageData,sz,sz);
			break;
		default:
			gLogManager.log(mT("Unkown image type"),ELL_WARNING);
			ijlFree( &img );
			return 0;
	}

	//core:: string_to_char(filename,fnameStr);
	uchar*data=new uchar[tex->Size.x*tex->Size.y*3];

	//img.JPGFile	= fnameStr.c_str();
	img.DIBWidth	= tex->Size.x;
	img.DIBHeight	= tex->Size.y;
	img.JPGWidth	= tex->Size.x;
	img.JPGHeight	= tex->Size.y;
	img.jquality	= 75;
	img.DIBBytes	= (uchar*)exportImg->imageData;
	img.JPGBytes	= data;
/*	img.DIBChannels = 3;
	img.JPGChannels	=3;
	img.DIBPadBytes = tex->getPitch();
	img.DIBColor	= IJL_RGB;
	img.JPGColor	= IJL_RGB;
	*/
	//if((err=ijlWrite( &img, IJL_JFILE_WRITEWHOLEIMAGE)) != IJL_OK){
	if((err=ijlWrite( &img, IJL_JBUFF_WRITEWHOLEIMAGE)) != IJL_OK){
		gLogManager.log(core::StringConverter::toString(ijlErrorStr(err)),ELL_WARNING);
		ijlFree( &img );
		delete [] data;
		return 0;
	}
	file->write(img.JPGBytes,img.JPGSizeBytes);

	delete [] data;

	ijlFree( &img );
	return 1;
}

bool CJPGWriter::canWrite(const  core::string&filename)
{
	core::string fname=filename;
	fname.make_lower();
	if(strSearch(fname.c_str(),mT(".jpg"))!=0)return 1;
	return 0;
}


bool CJPGWriter::support(video::ETextureType type)
{
	return (type==video::ETT_2DTex);
}


}
}








#include "stdafx.h"



#include "CBmpWriter.h"
#include "CBmpLoader.h"
#include "IStream.h"
#include "IFileSystem.h"
#include "ColorConverter.h"
#include "ILogManager.h"
#include "PixelUtil.h"
#include "StringUtil.h"

namespace mray{
namespace loaders{


	const core::string CBmpWriter::m_typeExt=mT("bmp");
	const core::string CBmpWriter::m_typeDesc=mT("BMP Image Writer");

CBmpWriter::CBmpWriter()
{
#ifdef ___DEBUG___
	setDebugName("CBmpWriter");
#endif
}

bool CBmpWriter::write(video::ImageInfo*tex,OS::IStream* file,video::ETextureType target)
{
	if(target!=video::ETT_2DTex){
		gLogManager.log(mT("BMP support only 2D Textures"),ELL_WARNING);
		return 0;
	}
	SBMPHeader header;
	header.Id=0x4d42;
	header.Reserved=0;
	header.BitmapDataOffset=sizeof(header);
	header.BitmapHeaderSize=0x28;
	header.Width=tex->Size.x;
	header.Height=tex->Size.y;
	header.Planes=1;
	header.BPP=video::PixelUtil::getPixelDescription(tex->format).componentsCount*8;
	header.Compression=0;
	header.PixelPerMeterX=0;
	header.PixelPerMeterY=0;
	header.Colors=0;
	header.ImportantColors=0;

//	header.BitmapDataSize=(header.Width*header.Height)*tex->colorDepth/8;
//	header.BitmapDataSize=(header.BitmapDataSize+(tex->colorDepth/8)) & 
//		~(tex->colorDepth/8);
	header.BitmapDataSize=tex->imageDataSize;

	header.FileSize=header.BitmapDataOffset + header.BitmapDataSize;

	if(file->write(&header,sizeof(header))!=sizeof(header))return 0;

	uchar* buffer=(uchar*)tex->imageData;
	if(!buffer)return 0;
/*	uchar* imageOut=new uchar[tex->imageDataSize];
	//if(tex->colorDepth==8)
		mraySystem::memCopy(imageOut,buffer,tex->imageDataSize);
	if(tex->colorDepth==24)
		ColorConverter::convert24BitTo24Bit(buffer,imageOut,tex->imageSize,0,0,1);
	if(tex->colorDepth==32)
		ColorConverter::convert32BitTo32Bit(buffer,imageOut,tex->imageSize,0,0,1);
*/
	if(file->write(buffer,tex->imageDataSize)!=tex->imageDataSize)
	{
	//	delete [] imageOut;
		return 0;
	}

//	delete [] imageOut;
	gLogManager.log(mT("Bmp image file Write done :"),core::string(file->getStreamName()),ELL_SUCCESS);
	return 1;

} 

bool CBmpWriter::canWrite(const  core::string&filename)
{
	core::string fname=core::StringUtil::ToLower(filename);
	if(strSearch(fname.c_str(),mT(".bmp"))!=0)return 1;
	return 0;
}
const core::string& CBmpWriter::getExt(){
	return m_typeExt;
}

const core::string& CBmpWriter::getDescription(){
	return m_typeDesc;
}




bool CBmpWriter::support(video::ETextureType type)
{
	return (type==video::ETT_2DTex);
}



}
}

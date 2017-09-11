#include "stdafx.h"

#include "FreeImageLoader.h"
#include <LogManager.h>
#include <StringConverter.h>
#include <TraceManager.h>
#include <IStream.h>
#include <IFileSystem.h>
#include <ITexture.h>
#include <TextureResourceManager.h>
#include <PixelUtil.h>
#include "FreeImageCodec.h"

#include "StringUtil.h"

namespace mray{
namespace loaders{



void FreeImageLoader::initialize()
{
	for(int i=0;i<FreeImage_GetFIFCount();++i){
		FREE_IMAGE_FORMAT fft=(FREE_IMAGE_FORMAT)i;
		if(fft==FIF_PGMRAW || fft==FIF_PPMRAW || fft==FIF_PBMRAW)
			continue;
		core::string ext=core::StringConverter::toString(FreeImage_GetFIFExtensionList(fft));

		std::vector<core::string> stringVec;

		stringVec=core::StringUtil::Split(ext,mT(","));
		for(int j=0;j<stringVec.size();j++){
			FreeImageLoader* loader=new FreeImageLoader(stringVec[j],(FREE_IMAGE_FORMAT)i);
			gTextureResourceManager.addTextureLoader(loader);
		}
	}
}

void FreeImageLoader::shutdown(){
}

FreeImageLoader::FreeImageLoader()
{
	m_FTType=FIF_UNKNOWN;
	m_isDummy=true;
	FreeImageCodec::IncreaseRef();
	if(FreeImageCodec::GetRefCount()==1){
		initialize();
	}
	m_extDesc=mT("");
}

FreeImageLoader::FreeImageLoader(const core::string& ext,FREE_IMAGE_FORMAT ftType):
	m_ext(ext),m_FTType(ftType)
{
	m_isDummy=false;

	FreeImageCodec::IncreaseRef();
	if(FreeImageCodec::GetRefCount()==1){
		initialize();
	}

	m_extDesc= mT("FreeImage Loader '")+m_ext+mT("'");
}
FreeImageLoader::~FreeImageLoader(){

	FreeImageCodec::DecreaseRef();
	if(FreeImageCodec::GetRefCount()==0){
		shutdown();
	}
}

bool FreeImageLoader::load(OS::IStream* file,video::ImageInfo* t2d,video::ETextureType target){
	if(m_isDummy)
		return false;
	if(!t2d)return false;
	//image format
	//pointer to the image, once loaded
	FIBITMAP *dib(0);
	//pointer to the image data
	BYTE* bits(0);
	//image width and height
	unsigned int width(0), height(0);

	byte* data = new byte[file->length()];
	//file->read(data,file->length());
	DWORD sz=0;

	FIMEMORY* mem = FreeImage_OpenMemory((BYTE*)data, static_cast<DWORD>(file->length()));
	//FreeImage_AcquireMemory(mem, &data, &sz);
	file->read(data, file->length());

	//check that the plugin has reading capabilities and load the file
	if(FreeImage_FIFSupportsReading(m_FTType))
		dib = FreeImage_LoadFromMemory(m_FTType, mem);
	FreeImage_CloseMemory(mem);
	delete[] data;

	//if the image failed to load, return failure
	if(!dib)
		return false;

	//retrieve the image data
	bits = FreeImage_GetBits(dib);
	//get the image width and height
	width = FreeImage_GetWidth(dib);
	height = FreeImage_GetHeight(dib);
	//if this somehow one of these failed (they shouldn't), return failure
	if((bits == 0) || (width == 0) || (height == 0))
		return false;
	FREE_IMAGE_TYPE imageType = FreeImage_GetImageType(dib);
	FREE_IMAGE_COLOR_TYPE colourType = FreeImage_GetColorType(dib);
	unsigned bpp = FreeImage_GetBPP(dib);

	video::EPixelFormat imageFormat;

	switch(imageType)
	{
	case FIT_BITMAP:
		// Standard image type
		// Perform any colour conversions for greyscale
		if (colourType == FIC_MINISWHITE || colourType == FIC_MINISBLACK)
		{
			FIBITMAP* newBitmap = FreeImage_ConvertToGreyscale(dib);
			// free old bitmap and replace
			FreeImage_Unload(dib);
			dib = newBitmap;
			// get new formats
			bpp = FreeImage_GetBPP(dib);
			colourType = FreeImage_GetColorType(dib);
		}
		// Perform any colour conversions for RGB
		else if (bpp < 8 || colourType == FIC_PALETTE || colourType == FIC_CMYK)
		{
			FIBITMAP* newBitmap = FreeImage_ConvertTo24Bits(dib);
			// free old bitmap and replace
			FreeImage_Unload(dib);
			dib = newBitmap;
			// get new formats
			bpp = FreeImage_GetBPP(dib);
			colourType = FreeImage_GetColorType(dib);
		}



		// by this stage, 8-bit is greyscale, 16/24/32 bit are RGB[A]
		switch(bpp)
		{
		case 8:
			imageFormat = video::EPixel_LUMINANCE8;
			break;
		case 16:
			{
			FIBITMAP* newBitmap = FreeImage_ConvertTo24Bits(dib);
			// free old bitmap and replace
			FreeImage_Unload(dib);
			dib = newBitmap;
			// get new formats
			bpp = FreeImage_GetBPP(dib);
			colourType = FreeImage_GetColorType(dib);
			imageFormat = video::EPixel_B8G8R8;
			}
			break;
		case 24:
			imageFormat = video::EPixel_B8G8R8;
			break;
		case 32:
			imageFormat = video::EPixel_B8G8R8A8;
			break;


		};
		break;
	case FIT_RGBF:
		imageFormat = video::EPixel_Float16_RGB;
		break;
	case FIT_RGBAF:
		imageFormat = video::EPixel_Float16_RGBA;
		break;


	case FIT_FLOAT:
	case FIT_UINT16:
	case FIT_INT16:
	case FIT_RGB16:
	case FIT_RGBA16:
	case FIT_UNKNOWN:
	case FIT_COMPLEX:
	case FIT_UINT32:
	case FIT_INT32:
	case FIT_DOUBLE:
	default:
		gLogManager.log(mT("Loading unsupported image: ")+core::string(file->getStreamName()),ELL_ERROR);

		break;
	};

	unsigned char* srcData = FreeImage_GetBits(dib);
	unsigned srcPitch = FreeImage_GetPitch(dib);
/*
	// Final data - invert image and trim pitch at the same time
	size_t dstPitch = imgData->width * PixelUtil::getNumElemBytes(imgData->format);
	imgData->size = dstPitch * imgData->height;
	// Bind output buffer
	output.bind(OGRE_NEW MemoryDataStream(imgData->size));

	uchar* pSrc;
	uchar* pDst = output->getPtr();
	for (size_t y = 0; y < imgData->height; ++y)
	{
		pSrc = srcData + (imgData->height - y - 1) * srcPitch;
		IOSSystem::memCopy(pDst, pSrc, dstPitch);
		pDst += dstPitch;
		
	}*/

	t2d->createData(math::vector3di(width,height,1),imageFormat);

	size_t dstPitch = width * video::PixelUtil::getPixelDescription(imageFormat).elemSizeB;
	uchar* pSrc;
	uchar* pDst = t2d->imageData;
	for (size_t y = 0; y < height; ++y)
	{
		pSrc = srcData + (height - y - 1) * srcPitch;
		IOSSystem::memCopy(pDst, pSrc, dstPitch);
		pDst += dstPitch;
	}


	FreeImage_Unload(dib);
//	FreeImage_CloseMemory(fiMem);

	//return success
	return true;

}
bool FreeImageLoader::load(const  core::string&filename,video::ImageInfo* tex,video::ETextureType target){
	if(m_isDummy)
		return false;
	traceFunction(eEngine);
	OS::IStreamPtr fp=gFileSystem.openFile(filename,OS::BIN_READ);
	if(!fp){
		gLogManager.log(mT("Couldn't open Bmp file"),filename,ELL_WARNING);
		return 0;
	}
	return load(fp, tex,target);
}
bool FreeImageLoader::canLoad(const core::string& fileName){
	if(m_isDummy)
		return false;

	core::string str=core::StringUtil::ToLower(fileName);
	return strSearch(str.c_str(),m_ext.c_str())!=0;
	/*

	FREE_IMAGE_FORMAT fif=FreeImage_GetFileType(fileName.c_str(),0);
	if(fif==FIF_UNKNOWN)
		fif = FreeImage_GetFIFFromFilename(fileName.c_str());
	//if still unkown, return failure
	if(fif == FIF_UNKNOWN)
		return false;

	return true;*/
}
bool FreeImageLoader::canLoad(OS::IStream* file){
	if(m_isDummy)
		return false;
	if(file->isStream())
		return false;
	byte* data = new byte[file->length()];
	FIMEMORY* fiMem = FreeImage_OpenMemory((BYTE*)data, static_cast<DWORD>(file->length()));
	file->read(data, file->length());

	FREE_IMAGE_FORMAT fif = FreeImage_GetFileTypeFromMemory(fiMem, (int)file->length());
	FreeImage_CloseMemory(fiMem);
	delete[] data;
	//image format
//	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	//check the file signature and deduce its format
//	fif = FreeImage_GetFileTypeFromMemory((FIMEMORY*)data,file->length());
	if(fif == FIF_UNKNOWN)
		return false;
	return fif==m_FTType;
}

bool FreeImageLoader::support(video::ETextureType type){
	return type==video::ETT_2DTex;
}
const core::string& FreeImageLoader::getDescription(){
	return m_extDesc;
}

}
}

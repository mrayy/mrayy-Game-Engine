#include "stdafx.h"

#include "CBmpLoader.h"
#include "IStream.h"
#include "IFileSystem.h"
#include "ILogManager.h"
#include "ColorConverter.h"
#include "TextureResourceManager.h"
#include "common.h"
#include "TraceManager.h"
#include "StringUtil.h"

namespace mray{
namespace loaders{

CBmpLoader::CBmpLoader()
{
}

void CBmpLoader::decompress8BitRLE(uchar*& bmpData, int size, int width, int height, int pitch)
{
	uchar* p = bmpData;
	uchar* newBmp = new uchar[(width+pitch)*height];
	uchar* d = newBmp;
	uchar* destEnd = newBmp + (width+pitch)*height;
	int line = 0;

	while (bmpData - p < size && d < destEnd)
	{
		if (*p == 0)
		{
			++p;

			switch(*p)
			{
			case 0: // end of line
				++p;
				++line;
				d = newBmp + (line*(width+pitch));
				break;
			case 1: // end of bmp
				delete [] bmpData;
				bmpData = newBmp;
				return;
			case 2:
				++p; 
				d +=(uchar)*p;  // delta
				++p;
				d += ((uchar)*p)*(width+pitch);
				++p;
				break;
			default:
				{
					// absolute mode
					int count = (uchar)*p; ++p;
					int readAdditional = ((2-(count%2))%2);
					int i;

					for (i=0; i<count; ++i)
					{
						*d = *p;
						++p;
						++d;
					}

					for (i=0; i<readAdditional; ++i)
						++p;
				}
			}
		}
		else
		{
			int count = (uchar)*p; ++p;
			int color = (uchar)*p; ++p;
			for (int i=0; i<count; ++i)
			{
				*d = color;
				++d;
			}
		}
	}

	delete [] bmpData;
	bmpData = newBmp;
}


void CBmpLoader::decompress4BitRLE(uchar*& bmpData, int size, int width, int height, int pitch)
{
	int lineWidth = (width+1)/2+pitch;
	uchar* p = bmpData;
	uchar* newBmp = new uchar[lineWidth*height];
	uchar* d = newBmp;
	uchar* destEnd = newBmp + lineWidth*height;
	int line = 0;
	int shift = 4;

	while (bmpData - p < size && d < destEnd)
	{
		if (*p == 0)
		{
			++p;

			switch(*p)
			{
			case 0: // end of line
				++p;
				++line;
				d = newBmp + (line*lineWidth);
				shift = 4;
				break;
			case 1: // end of bmp
				delete [] bmpData;
				bmpData = newBmp;
				return;
			case 2:
				{
					++p;
					int x = (uchar)*p;
					++p;
					int y = (uchar)*p;
					++p;
					d += x/2 + y*lineWidth;
					shift = x%2==0 ? 4 : 0;
				}
				break;
			default:
				{
					// absolute mode
					int count = (uchar)*p;
					++p;
					int readAdditional = ((2-((count)%2))%2);
					int readShift = 4;
					int i;

					for (i=0; i<count; ++i)
					{
						int color = (((uchar)*p) >> readShift) & 0x0f;
						readShift -= 4;
						if (readShift < 0)
						{
							++*p;
							readShift = 4;
						}

						uchar mask = 0x0f << shift;
						*d = (*d & (~mask)) | ((color << shift) & mask);

						shift -= 4;
						if (shift < 0)
						{
							shift = 4;
							++d;
						}

					}

					for (i=0; i<readAdditional; ++i)
						++p;
				}
			}
		}
		else
		{
			int count = (uchar)*p;
			++p;
			int color1 = (uchar)*p;
			color1 = color1 & 0x0f;
			int color2 = (uchar)*p;
			color2 = (color2 >> 4) & 0x0f;
			++p;

			for (int i=0; i<count; ++i)
			{
				uchar mask = 0x0f << shift;
				uchar toSet = (shift==0 ? color1 : color2) << shift;
				*d = (*d & (~mask)) | (toSet & mask);

				shift -= 4;
				if (shift < 0)
				{
					shift = 4;
					++d;
				}
			}
		}
	}

	delete [] bmpData;
	bmpData = newBmp;
}
bool CBmpLoader::load(OS::IStream* file,video::ImageInfo* image,video::ETextureType target){
	traceFunction(eEngine);
	if(!image || target!=video::ETT_2DTex)
		return false;

	file->seek(0,OS::ESeek_Set);
	palData=0;
	SBMPHeader  header;
	file->read(&header,sizeof(header));

	int pitch=0;

	if(header.Id!=0x4d42)return 0;
	if(header.Compression>2)return 0;

	header.BitmapDataSize+=(4-(header.BitmapDataSize%4))%4;

	video::EPixelFormat format;

	switch(header.BPP)
	{
	case 1:
	case 2:
	case 4:
	case 8:
	case 16:
		gLogManager.log(mT("unsupported image file"),file->getStreamName(),ELL_WARNING);
		return 0;
		break;
	case 24:
// 		tmpData=new uchar[header.BitmapDataSize];
// 		video::ColorConverter::convert24BitTo24Bit(bmpData,tmpData,math::Point2di(header.Width,header.Height),0,0,1);
// 		tex2d->setData(tmpData,math::Point2di(header.Width,header.Height),video::EPixel_R8G8B8);
		format=video::EPixel_R8G8B8;
		break;
	case 32:
		format=video::EPixel_R8G8B8A8;
	}

	image->createData(math::vector3d(header.Width,header.Height,1),format);

	int pos=file->getPos();
	int palSize=(header.BitmapDataOffset-pos)/4;
	if(palSize)
	{
		palData=new int[palSize];
		file->read(palData,palSize*sizeof(int));
	}

	if(!header.BitmapDataSize)
	{
		header.BitmapDataSize=file->length()-header.BitmapDataOffset;
	}
	file->seek(header.BitmapDataOffset,OS::ESeek_Set);
	float t=header.Width*(header.BPP/8.0f);
	int widthInBytes=(int) t;
	t-=widthInBytes;
	if(t!=0)widthInBytes++;	

	int lineData=widthInBytes+(4-(widthInBytes%4))%4;
	pitch=lineData-widthInBytes;


	//bmpData=new uchar[header.BitmapDataSize];

	file->read(image->imageData,image->imageDataSize);

	switch(header.Compression)
	{
	case 1:
		decompress8BitRLE(image->imageData, image->imageDataSize, header.Width, header.Height, pitch);
		break;
	case 2:
		decompress4BitRLE(image->imageData, image->imageDataSize, header.Width, header.Height, pitch);
		break;
	};
	
	//tex->setResourceName(file->getStreamName());

	//video::ITexturePtr  tex=gTextureResourceManager.createTexture2D(file->getStreamName(),math::Point2di(1),video::EPixel_LUMINANCE8);
	//video::ITexturePtr  tex2d=(video::ITexturePtr )tex;
	
	//uchar*tmpData=0;
	switch(header.BPP)
	{
	case 24:
		
		video::ColorConverter::convert24BitTo24Bit(image,math::Point2di(header.Width,header.Height),0,0,1);
//		tex2d->setData(tmpData,math::Point2di(header.Width,header.Height),video::EPixel_R8G8B8);
		break;
	case 32:
	//	tmpData=new uchar[header.BitmapDataSize];
		video::ColorConverter::convert32BitTo32Bit(image,math::Point2di(header.Width,header.Height),0,0,1);
//		tex2d->setData(tmpData,math::Point2di(header.Width,header.Height),video::EPixel_R8G8B8A8);
		break;
	};
	if(palData)
	{
		delete[]palData;
		palData=0;
	}/*
	delete [] bmpData;
	if(tmpData)
		delete [] tmpData;*/

	//tex2d->unlock();
	return true;
}
bool CBmpLoader::load(const  core::string&filename,video::ImageInfo* image,video::ETextureType target)
{
	traceFunction(eEngine);
	OS::IStreamPtr fp=gFileSystem.openFile(filename,OS::BIN_READ);
	if(!fp){
		gLogManager.log(mT("Couldn't open Bmp file"),filename,ELL_WARNING);
		return 0;
	}
	return load(fp, image,target);


}

bool CBmpLoader::canLoad(const   core::string&filename)
{
	core::string str=core::StringUtil::ToLower(filename);
	return strSearch(str.c_str(),mT(".bmp"))!=0;
}


bool CBmpLoader::canLoad(OS::IStream* file)
{
	ushort type=0;
	file->seek(0,OS::ESeek_Set);
	file->read(&type,sizeof(ushort));
	return (type==0x4d42);
}




const core::string g_sBMPExt=mT("bmp");
const core::string g_sBMPDesc=mT("Bitmap Image Loader");

const core::string& CBmpLoader::getExt(){
	return g_sBMPExt;
}

const core::string& CBmpLoader::getDescription(){
	return g_sBMPDesc;
}
bool CBmpLoader::support(video::ETextureType type){
	return type==video::ETT_2DTex;
}





}
}

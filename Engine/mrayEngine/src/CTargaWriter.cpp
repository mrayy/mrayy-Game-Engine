#include "stdafx.h"



#include "CTargaWriter.h"
#include "CTargaLoader.h"
#include "IStream.h"
#include "IFileSystem.h"
#include "ColorConverter.h"
#include "ITexture.h"

#include "ILogManager.h"
#include "mraySystem.h"
#include "PixelUtil.h"
#include "StringUtil.h"

namespace mray{
namespace loaders{


	const core::string CTargaWriter::m_typeExt=mT("tga");
	const core::string CTargaWriter::m_typeDesc=mT("Targa Image Writer");

CTargaWriter::CTargaWriter()
{
#ifdef ___DEBUG___
	setDebugName("CTargaWriter");
#endif
}

void CTargaWriter::writeCompressedRow(OS::IStream* file,video::ImageInfo*tex)
{
	int incr=video::PixelUtil::getPixelDescription(tex->format).componentsCount;
	int counter=1;
	int pixelStart=0;
	int packetType=0;
	bool readyToWrite=0;
	video::SColor currPixel,nextPixel;
	uchar*buffer,*base=(uchar*)tex->imageData;
	int width=tex->Size.x;
	base+=incr*width*(tex->Size.y);
	buffer=base;
	for(int row=0;row<tex->Size.y;row++){
//		buffer=(uchar*)base;
		buffer-=incr*width;
		packetType=0;
		pixelStart=0;
		counter=1;
		currPixel.R=buffer[0];
		if(incr>1){
			currPixel.G=buffer[1];
			currPixel.B=buffer[2];
			if(incr==4)
				currPixel.A=buffer[3];
		}
		while(1)
		{
			if(pixelStart+counter>=width)
				readyToWrite=1;
			else{
				nextPixel.R=buffer[incr*(pixelStart+counter)+0];
				nextPixel.G=buffer[incr*(pixelStart+counter)+1];
				nextPixel.B=buffer[incr*(pixelStart+counter)+2];
				if(video::PixelUtil::getPixelDescription(tex->format).componentsCount==4)
					currPixel.A=buffer[incr*(pixelStart+counter)+3];
			}
			if(!readyToWrite)
			{
				if(currPixel==nextPixel)
				{
					if(packetType==0){
						counter++;
						if(counter>=128 || (pixelStart+counter)>=width)
							readyToWrite=1;
					}
					else
					{
						counter--;
						readyToWrite=1;
					}
				}else
				{
					if(packetType==1 || counter<=1){
						packetType=1;
						currPixel=nextPixel;
						counter++;
						if(counter>=128 || (pixelStart+counter)>=width)
							readyToWrite=1;
					}
					else
					{
						readyToWrite=1;
					}
				}
			}
			if(readyToWrite)
			{
				if(pixelStart + counter>width)
				{
					counter=width-pixelStart;
					if(packetType==0)
						file->writeByte((counter-1)|0x80);
					file->writeByte(currPixel.R);
					file->writeByte(currPixel.G);
					file->writeByte(currPixel.B);
					if(video::PixelUtil::getPixelDescription(tex->format).componentsCount==4)
						file->writeByte(currPixel.A);

					currPixel=nextPixel;
				}
				else
				{
					file->writeByte(counter-1);
					for(int i=0;i<counter;++i){
						file->writeByte(buffer[incr*(pixelStart+i)+0]);
						file->writeByte(buffer[incr*(pixelStart+i)+1]);
						file->writeByte(buffer[incr*(pixelStart+i)+2]);
						if(video::PixelUtil::getPixelDescription(tex->format).componentsCount==4)
							file->writeByte(buffer[incr*(pixelStart+i)+3]);
					}
				}
				if((pixelStart=pixelStart+counter)>=width)
					break;
				readyToWrite=0;
				packetType=0;
				counter=1;
			}
		}
	}
}

bool CTargaWriter::write(video::ImageInfo*tex,OS::IStream* file,video::ETextureType target)
{
	if(target!=video::ETT_2DTex){
		gLogManager.log(mT("Targa support only 2D Textures"),ELL_WARNING);
		return 0;
	}
	if(video::PixelUtil::getPixelDescription(tex->format).componentsCount==1)
	{
		gLogManager.log(mT("cann't write TGA image 8 bit"),ELL_WARNING);
		return 0;
	}
	tgaHeader header;
	mraySystem::memSet(&header,0,sizeof(header));
	header.imageTypeCode=TGA_RGB_RLE;
	header.width=tex->Size.x;
	header.height=tex->Size.y;
	header.imageDesc=1<<5;
	header.pDepth=video::PixelUtil::getPixelDescription(tex->format).componentsCount*8;


	if(file->write(&header,sizeof(header))!=sizeof(header))return 0;

	writeCompressedRow(file,tex);

	
	gLogManager.log(mT("Tga image file Write done : ")+core::string(file->getStreamName()),ELL_SUCCESS);
	return 1;
}

bool CTargaWriter::canWrite(const  core::string&filename)
{
	core::string fname=core::StringUtil::ToLower(filename);
	if(strSearch(fname.c_str(),mT(".tga"))!=0)return 1;
	return 0;
}


const core::string& CTargaWriter::getExt(){
	return m_typeExt;
}

const core::string& CTargaWriter::getDescription(){
	return m_typeDesc;
}


bool CTargaWriter::support(video::ETextureType type)
{
	return (type==video::ETT_2DTex);
}



}
}

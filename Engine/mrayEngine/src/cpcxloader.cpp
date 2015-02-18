#include "stdafx.h"

#include "CPCXLoader.h"
#include "IFileSystem.h"
#include "ILogManager.h"
#include "TextureResourceManager.h"
#include "common.h"
#include "TraceManager.h"
#include "IStream.h"
#include "StringUtil.h"


namespace mray{
namespace loaders{

CPCXLoader::CPCXLoader()
{
#ifdef ___DEBUG___
	setDebugName("CPCXLoader");
#endif
}
bool CPCXLoader::read_scanline(byte *planes[], uint num_planes, uint bytesPerLine){
	
	uint i, j;
	uint p, count;
	byte byte1;

	for(p = 0; p < num_planes; p++) {
		for(i = 0; i < bytesPerLine;) {
			if(file->read(&byte1,1) != 1)
				return 0;

			if(byte1 >> 6 == 0x3) {
				count = byte1 & ~(0x3 << 6);
				if(count == 0)
					return 0;
				if(file->read(&byte1,1) != 1)
					return 0;
			} else {
				count = 1;
			}

			for(j = 0; j < count; j++)
				planes[p][i++] = byte1;

			if(i >= bytesPerLine) {
				p++;
				if(p >= num_planes)
					return 1;
				i = 0;
			}
		}
	}

	return 1;
}

bool CPCXLoader::load8bitPCX(video::ImageInfo* image,int bytesPerLine){
	traceFunction(eEngine);
	int i, j;
	int max;
	byte *data, *p_data;
	byte *line, *planes[1];
	uint current_line = 0;
	byte byte1;
	byte palette[256][3];

	p_data = (byte *)malloc(sizeof(byte1) * image->Size.x * image->Size.x);
	if(!p_data)
		return 0;

	line = (byte *)malloc(sizeof(byte1) * bytesPerLine);
	if(!line) {
		free(p_data);
		return 0;
	}
	planes[0] = line;

	while(current_line < (uint)image->Size.y) {
		if(read_scanline(planes, 1, bytesPerLine) == 0) {
			free(p_data);
			free(line);
			return 0;
		}

		for(i = 0; i < image->Size.x; i++)
			p_data[image->Size.x * current_line + i] = planes[0][i];

		current_line++;
	}

	/* read palette */
	file->seek(-769, OS::ESeek_End);
	file->read(&byte1, 1);
	if(byte1 != 12) {
		gLogManager.log(mT("Invalid PCX file - the palette data has an illegal header"),file->getStreamName(),ELL_WARNING);
		free(p_data);
		free(line);
		return 0;
	}
	
	file->read((void*)palette, 256*3);

	data = (uchar *)malloc(sizeof(uchar) * image->Size.x * image->Size.y * 3);
	if(!data) {
		free(p_data);
		free(line);
		return 0;
	}

	max = image->Size.x * image->Size.y;

	//image->Size = max*3;
	j = 0;
	for(i = 0; i < max; i++) {
		data[j++] = palette[(p_data[i])][0];
		data[j++] = palette[(p_data[i])][1];
		data[j++] = palette[(p_data[i])][2];
	}

	free(p_data);
	free(line);
	image->imageData=data;;
	return 1;
}
bool CPCXLoader::load24bitPCX(video::ImageInfo* image,int bytesPerLine){
	traceFunction(eEngine);
	int i;
	byte *data;
	byte *line, *planes[3];
	uint current_line = 0;

	data = (byte *)malloc(sizeof(byte) * image->Size.x * image->Size.y * 3);
	if(!data)
		return 0;

	line = (byte *)malloc(sizeof(byte) * bytesPerLine * 3);
	if(!line) {
		free(data);
		return NULL;
	}
	planes[0] = line;
	planes[1] = planes[0] + bytesPerLine;
	planes[2] = planes[1] + bytesPerLine;

	while(current_line < (unsigned int)image->Size.y) {
		if(read_scanline( planes, 3, bytesPerLine) == 0) {
			free(data);
			free(line);
			return NULL;
		}

		for(i = 0; i < image->Size.x; i++) {
			data[image->Size.x * current_line * 3 + i * 3 + 0] = planes[0][i];
			data[image->Size.x * current_line * 3 + i * 3 + 1] = planes[1][i];
			data[image->Size.x * current_line * 3 + i * 3 + 2] = planes[2][i];
		}

		current_line++;
	}

	free(line);
	image->setData(data,image->Size,video::EPixel_R8G8B8);

	return 1;
}

bool CPCXLoader::load(OS::IStream* file,video::ImageInfo* image,video::ETextureType target)
{
	traceFunction(eEngine);
	if(!image || target!=video::ETT_2DTex)
		return false;

	file->seek(0,OS::ESeek_Set);

	SPCX_Header header;

	file->read(&header,sizeof(header));
	if (header.manufacturer != 0x0a && header.encoding != 0x01){
		gLogManager.log(mT("Invalid PCX file"),file->getStreamName(),ELL_WARNING);
		return 0;
	}

	if(header.bitsperpixel!=8 || header.bitsperpixel!=24 )
	{
		gLogManager.log(mT("Unsupported bitsPerPixel in PCX file"),file->getStreamName(),ELL_WARNING);
		return 0;
	}
	this->file=file;

//	image->setResourceName(file->getStreamName());
	image->Size.x=header.xmax-header.xmin + 1 ;
	image->Size.y=header.ymax-header.ymin + 1 ;
	image->Size.z=1 ;
	image->format=video::EPixel_R8G8B8;

	switch(header.colorplanes){
		case 1:
			if(load8bitPCX(image,header.bytesperline)==0){
				return 0;
			}
			break;
		case 3:
			if(load24bitPCX(image,header.bytesperline)==0){
				return 0;
			}
			break;
	}
//	tex2D->unlock();
	return true;
/*
	//retrieve header, first 4 bytes, first 2 should be 0x0A0C
	ushort header[2];
	file->read(header, 2*sizeof(ushort));

	if(header[0]!=0x050A)
	{
		gLogManager.log(mT("Invalid PCX file",file->getStreamName());
		return 0;
	}

	video::ImageInfo* image=new video::ITexture();
	//set bpp and format
	image->colorDepth=24;
	image->imageDataFormat=video::EPixel_R8G8B8;
	image->imageDataFormat=video::EPixel_DATA_UNSIGNED_BYTE;

	//retrieve minimum x value
	int xMin=file->getC();		//loword
	xMin |= file->getC() << 8;	//hiword

	//retrieve minimum y value
	int yMin=file->getC();		//loword
	yMin |= file->getC() << 8;	//hiword

	//retrieve maximum x value
	int xMax=file->getC();		//loword
	xMax |= file->getC() << 8;	//hiword

	//retrieve maximum y value
	int yMax=file->getC();		//loword
	yMax |= file->getC() << 8;	//hiword

	//calculate width and height
	image->Size.x = xMax-xMin+1;
	image->Size.y = yMax-yMin+1;

	//allocate memory for pixel data (paletted)
	
	uchar * pixelData=new uchar[image->Size.x*image->Size.y];
	if(!pixelData)
	{
		gLogManager.log(mT("Unable to create PCX data",file->getStreamName());

		image->subRef();
		return 0;
	}

	//set file pointer to beginning of image data
	file->seek(128, SEEK_SET);

	//decode and store the pixel data
	uint index=0;

	while(index<image->Size.x*image->Size.y)
	{
		int c = file->getC();

		if(c>0xBF)
		{
			int numRepeat = 0x3F & c;
			c=file->getC();

			for(int i=0; i<numRepeat; i++)
				pixelData[index++] = c;
		}
		else
			pixelData[index++] = c;

	}

	//allocate memory for the image palette
	uchar * paletteData = new uchar[768];

	//the palette is the last 769 bytes of the file
	file->seek(769, SEEK_SET);

	//retrieve first character, should be equal to 12
	int c=file->getC();
	if(c!=12)
	{
		gLogManager.log(mT("Invalid PCX file - the palette data has an illegal header",file->getStreamName());
		image->subRef();
		delete [] paletteData;
		return false;
	}

	//read and store the palette
	file->read(paletteData, 768);
	
	//allocate memory for the "unpaletted" data
	image->imageDataSize=image->Size.x*image->Size.y*3;
	uchar*data = new uchar[image->imageDataSize];
	image->imageData=data;
	if(!data)
	{
		gLogManager.log(mT("PCX Loader - Unable to allocate memory for the expanded data",file->getStreamName());
		image->subRef();
		delete []paletteData;
		return 0;
	}

	//calculate the "unpaletted" data - "flipping" the texture top-bottom
	for(unsigned int j=0; j<image->Size.y; j++)
	{
		for(unsigned int i=0; i<image->Size.x; i++)
		{
			data[3*(j*image->Size.x+i)]		= (unsigned char) paletteData[3*pixelData[(image->Size.y-1-j)*image->Size.x+i]];
			data[3*(j*image->Size.x+i)+1]	= (unsigned char) paletteData[3*pixelData[(image->Size.y-1-j)*image->Size.x+i]+1];
			data[3*(j*image->Size.x+i)+2]	= (unsigned char) paletteData[3*pixelData[(image->Size.y-1-j)*image->Size.x+i]+2];
		}
	}

	gLogManager.log(mT("PCX image file loaded ",file->getStreamName());
	return tex;
	*/

}

bool CPCXLoader::load(const  core::string&fileName,video::ImageInfo* image,video::ETextureType target)
{
	traceFunction(eEngine);
	OS::IStreamPtr fp=gFileSystem.openFile(fileName,OS::BIN_READ);
	if(!fp){
		gLogManager.log(mT("Couldn't open PCX file"),fileName,ELL_WARNING);
		return 0;
	}
	return load(fp,image,target);
}


bool CPCXLoader::canLoad(const  core::string&name)
{
	core::string str=core::StringUtil::ToLower(name);
	return strSearch(str.c_str(),mT(".pcx"))!=0;
}

bool CPCXLoader::canLoad(OS::IStream* file){

	file->seek(0,OS::ESeek_Set);
	ushort header[2];
	file->read(header,2*sizeof(ushort));
	if(header[0]!=0x050A)
		return false;
	return true;
}


const core::string g_sPCXExt=mT("pcx");
const core::string g_sPCXDesc=mT("PCX Image Loader");

const core::string& CPCXLoader::getExt(){
	return g_sPCXExt;
}

const core::string& CPCXLoader::getDescription(){
	return g_sPCXDesc;
}

bool CPCXLoader::support(video::ETextureType type){
	return type==video::ETT_2DTex;
}


}
}






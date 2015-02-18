#include "stdafx.h"

#include "PixelFormat.h"
#include "CMemory.h"

namespace mray{
namespace video{

PixelFormat*UnkownPixelFormat::duplicate(){
	return new UnkownPixelFormat();
}
int UnkownPixelFormat::convertToColor(byte*data,ulong&color){
	color=0;
	return 0;
}
int GrayPixelFormat::getDepth(){
	return 8;
}
int GrayPixelFormat::getComponentsCount(){
	return 1;
}
ulong GrayPixelFormat::getColorsScope(){
	return 256; //2^8 color
}

bool GrayPixelFormat::hasR(){return true;}
int GrayPixelFormat::getROffset(){return 0;}

bool GrayPixelFormat::hasG(){return true;}
int GrayPixelFormat::getGOffset(){return 0;}

bool GrayPixelFormat::hasB(){return true;}
int GrayPixelFormat::getBOffset(){return 0;}

bool GrayPixelFormat::hasA(){return true;}
int GrayPixelFormat::getAOffset(){return 0;}

PixelFormat*GrayPixelFormat::duplicate(){
	return new GrayPixelFormat();
}

int GrayPixelFormat::convertToColor(byte*data,ulong&color){
	GrayColorDesciptor d;
	int b=d.parse(data);
	color=d.toLong();
	return b;
}

int RGBPixelFormat::getDepth(){
	return 24;
}
int RGBPixelFormat::getComponentsCount(){
	return 3;
}
ulong RGBPixelFormat::getColorsScope(){
	return 16777216; //2^24 color
}

bool RGBPixelFormat::hasR(){return true;}
int RGBPixelFormat::getROffset(){return 0;}

bool RGBPixelFormat::hasG(){return true;}
int RGBPixelFormat::getGOffset(){return 1;}

bool RGBPixelFormat::hasB(){return true;}
int RGBPixelFormat::getBOffset(){return 2;}

PixelFormat*RGBPixelFormat::duplicate(){
	return new RGBPixelFormat();
}
int RGBPixelFormat::convertToColor(byte*data,ulong&color){
	RGBColorDesciptor d;
	int b=d.parse(data);
	color=d.toLong();
	return b;
}

int BGRPixelFormat::getDepth(){
	return 24;
}
int BGRPixelFormat::getComponentsCount(){
	return 3;
}
ulong BGRPixelFormat::getColorsScope(){
	return 16777216; //2^24 color
}

bool BGRPixelFormat::hasR(){return true;}
int BGRPixelFormat::getROffset(){return 2;}

bool BGRPixelFormat::hasG(){return true;}
int BGRPixelFormat::getGOffset(){return 1;}

bool BGRPixelFormat::hasB(){return true;}
int BGRPixelFormat::getBOffset(){return 0;}

PixelFormat*BGRPixelFormat::duplicate(){
	return new BGRPixelFormat();
}
int BGRPixelFormat::convertToColor(byte*data,ulong&color){
	BGRColorDesciptor d;
	int b=d.parse(data);
	color=d.toLong();
	return b;
}

int RGBAPixelFormat::getDepth(){
	return 32;
}
int RGBAPixelFormat::getComponentsCount(){
	return 4;
}
ulong RGBAPixelFormat::getColorsScope(){
	return 4294967295; //2^32 color
}

bool RGBAPixelFormat::hasR(){return true;}
int RGBAPixelFormat::getROffset(){return 0;}

bool RGBAPixelFormat::hasG(){return true;}
int RGBAPixelFormat::getGOffset(){return 1;}

bool RGBAPixelFormat::hasB(){return true;}
int RGBAPixelFormat::getBOffset(){return 2;}

bool RGBAPixelFormat::hasA(){return true;}
int RGBAPixelFormat::getAOffset(){return 3;}

PixelFormat*RGBAPixelFormat::duplicate(){
	return new RGBAPixelFormat();
}
int RGBAPixelFormat::convertToColor(byte*data,ulong&color){
	RGBAColorDesciptor d;
	int b=d.parse(data);
	color=d.toLong();
	return b;
}

}
}


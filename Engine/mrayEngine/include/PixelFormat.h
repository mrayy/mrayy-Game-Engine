/********************************************************************
	created:	2008/11/28
	created:	28:11:2008   14:28
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\PixelFormat.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	PixelFormat
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	defines the pixel formats
*********************************************************************/

#ifndef ___PixelFormat___
#define ___PixelFormat___

#include "mTypes.h"
#include "compileConfig.h"


namespace mray{
namespace video{

class MRAY_DLL IColorDesciptor{
public:
	virtual ulong toLong()=0;
	virtual int parse(byte*data)=0;

	virtual int toLongDirect(byte*data,ulong&color)=0;
};
class MRAY_DLL GrayColorDesciptor{
public:
	byte c;
	virtual ulong toLong(){
		return (ulong)c;
	}
	virtual int parse(byte*data){
		c=data[0];
		return 1;
	}
	virtual int toLongDirect(byte*data,ulong&color){
		color=data[0];
		return 1;
	}
};
class MRAY_DLL RGBColorDesciptor{
public:
	byte r,g,b;
	virtual ulong toLong(){
		return (ulong)(r<<16 | g<<8 | b);
	}
	virtual int parse(byte*data){
		r=data[0];
		g=data[1];
		b=data[2];
		return 3;
	}
	virtual int toLongDirect(byte*data,ulong&color){
		color=(ulong)(data[0]<<16 | data[1]<<8 | data[2]);
		return 3;
	}
};
class MRAY_DLL BGRColorDesciptor{
public:
	byte b,r,g;
	virtual ulong toLong(){
		return (ulong)(b<<16 | g<<8 | r);
	}
	virtual int parse(byte*data){
		b=data[0];
		g=data[1];
		r=data[2];
		return 3;
	}
	virtual int toLongDirect(byte*data,ulong&color){
		color=(ulong)(data[0]<<16 | data[1]<<8 | data[2]);
			return 3;
	}
};
class MRAY_DLL RGBAColorDesciptor{
public:
	byte r,g,b,a;
	virtual ulong toLong(){
		return (ulong)(a<<24 | r<<16 | g<<8 | b);
	}
	virtual int parse(byte*data){
		r=data[0];
		g=data[1];
		b=data[2];
		a=data[3];
		return 4;
	}
	virtual int toLongDirect(byte*data,ulong&color){
		color=(ulong)(data[3]<<16 | data[0]<<16 | data[1]<<8 | data[2]);
		return 4;
	}
};

class MRAY_DLL PixelFormat
{
public:
	virtual int getDepth()=0;
	virtual int getComponentsCount()=0;
	virtual ulong getColorsScope()=0;

	virtual bool hasR(){return false;}
	virtual int getROffset(){return 0;}

	virtual bool hasG(){return false;}
	virtual int getGOffset(){return 0;}

	virtual bool hasB(){return false;}
	virtual int getBOffset(){return 0;}

	virtual bool hasA(){return false;}
	virtual int getAOffset(){return 0;}

	virtual PixelFormat*duplicate()=0;

	//convert next bytes to the corresponding color,and returns number of bytes been read from the data
	virtual int convertToColor(byte*data,ulong&color)=0;
};
class UnkownPixelFormat:public PixelFormat
{
public:
	virtual int getDepth(){return 0;}
	virtual int getComponentsCount(){return 0;}
	virtual ulong getColorsScope(){return 0;}

	virtual bool hasR(){return false;}
	virtual int getROffset(){return 0;}

	virtual bool hasG(){return false;}
	virtual int getGOffset(){return 0;}

	virtual bool hasB(){return false;}
	virtual int getBOffset(){return 0;}

	virtual bool hasA(){return false;}
	virtual int getAOffset(){return 0;}

	virtual PixelFormat*duplicate();

	virtual int convertToColor(byte*data,ulong&color);
};


class MRAY_DLL GrayPixelFormat:public PixelFormat
{
public:
	int getDepth();
	int getComponentsCount();
	ulong getColorsScope();

	bool hasR();
	int getROffset();

	bool hasG();
	int getGOffset();

	bool hasB();
	int getBOffset();

	bool hasA();
	int getAOffset();

	PixelFormat*duplicate();

	virtual int convertToColor(byte*data,ulong&color);
};

class MRAY_DLL RGBPixelFormat:public PixelFormat
{
public:
	int getDepth();
	int getComponentsCount();
	ulong getColorsScope();

	bool hasR();
	int getROffset();

	bool hasG();
	int getGOffset();

	bool hasB();
	int getBOffset();

	PixelFormat*duplicate();

	virtual int convertToColor(byte*data,ulong&color);
};

class MRAY_DLL BGRPixelFormat:public PixelFormat
{
public:
	int getDepth();
	int getComponentsCount();
	ulong getColorsScope();

	bool hasR();
	int getROffset();

	bool hasG();
	int getGOffset();

	bool hasB();
	int getBOffset();

	PixelFormat*duplicate();

	virtual int convertToColor(byte*data,ulong&color);
};

class MRAY_DLL RGBAPixelFormat:public PixelFormat
{
public:
	int getDepth();
	int getComponentsCount();
	ulong getColorsScope();

	bool hasR();
	int getROffset();

	bool hasG();
	int getGOffset();

	bool hasB();
	int getBOffset();

	bool hasA();
	int getAOffset();

	PixelFormat*duplicate();

	virtual int convertToColor(byte*data,ulong&color);
};
}
}


#endif //___PixelFormat___
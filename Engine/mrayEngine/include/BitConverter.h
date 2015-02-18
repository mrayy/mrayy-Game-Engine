

/********************************************************************
	created:	2012/06/30
	created:	30:6:2012   15:37
	filename: 	d:\Development\mrayEngine\Engine\mrayEngine\BitConverter.h
	file path:	d:\Development\mrayEngine\Engine\mrayEngine
	file base:	BitConverter
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __BitConverter__
#define __BitConverter__

#include "compileConfig.h"
#include "mTypes.h"

namespace mray
{
namespace core
{

class MRAY_DLL BitConverter
{
private:
protected:
public:
	BitConverter(){}
	virtual~BitConverter(){}

	template<class T>
	static int GetBytesT(const T& v,byte* buff,int maxLen)
	{
		int len=sizeof(T);
		if(maxLen<len)
			return -1;
		byte* ptr=(byte*)&v;
		for(int i=0;i<len;++i)
		{
			buff[i]=ptr[i];
		}
		return len;
	}
	template<class T>
	static T GetValueT(const byte* buff)
	{
		T v;
		int len=sizeof(T);
		byte* ptr=(byte*)&v;
		for(int i=0;i<len;++i)
		{
			ptr[i]=buff[i];
		}
		return v;
	}

	static int GetBytes(bool val,byte* buff,int maxLen);
	static int GetBytes(char val,byte* buff,int maxLen);
	static int GetBytes(short val,byte* buff,int maxLen);
	static int GetBytes(ushort val,byte* buff,int maxLen);
	static int GetBytes(int val,byte* buff,int maxLen);
	static int GetBytes(uint val,byte* buff,int maxLen);
	static int GetBytes(long val,byte* buff,int maxLen);
	static int GetBytes(ulong val,byte* buff,int maxLen);
	static int GetBytes(float val,byte* buff,int maxLen);
	static int GetBytes(double val,byte* buff,int maxLen);

	static bool ToBool(const byte* value);
	static char ToChar(const byte* value);
	static short ToInt16(const byte* value);
	static ushort ToUInt16(const byte* value);
	static int ToInt32(const byte* value);
	static uint ToUInt32(const byte* value);
	static long ToInt64(const byte* value);
	static ulong ToUInt64(const byte* value);
	static float ToFloat(const byte* value);
	static double ToDouble(const byte* value);
};


}
}

#endif

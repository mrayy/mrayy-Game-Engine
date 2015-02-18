

/********************************************************************
	created:	2012/07/15
	created:	15:7:2012   13:02
	filename: 	d:\Development\mrayEngine\Engine\mrayEngine\include\IStringEncoding.h
	file path:	d:\Development\mrayEngine\Engine\mrayEngine\include
	file base:	IStringEncoding
	file ext:	h
	author:		MHD YAMEN SARAIJI
	
	purpose:	
*********************************************************************/
#ifndef ___IStringEncoding___
#define ___IStringEncoding___

#include "mstring.h"
#include "mtypes.h"

namespace mray
{
namespace core
{


class IStringEncoding
{
protected:

public:
	IStringEncoding(){}
	virtual~IStringEncoding(){}

	virtual int GetBytesCount(const core::string& str)=0;
	virtual int GetBytesCount(const mchar* str,int count)=0;

	virtual int GetBytes(const core::string& str,byte* buff,int maxLen)=0;
	virtual int GetBytes(const mchar* str,int count,byte* buff,int maxLen)=0;

	virtual int GetCharsCount(const byte* buff,int len)=0;
	virtual core::string GetString(const byte* buff,int len)=0;
	virtual int GetChars(const byte* buff,int len,mchar* str,int maxLen)=0;
	
};

}
}

#endif

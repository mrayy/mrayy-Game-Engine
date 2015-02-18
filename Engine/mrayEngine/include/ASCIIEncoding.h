

/********************************************************************
	created:	2012/07/15
	created:	15:7:2012   12:57
	filename: 	d:\Development\mrayEngine\Engine\mrayEngine\include\ASCIIEncoding.h
	file path:	d:\Development\mrayEngine\Engine\mrayEngine\include
	file base:	ASCIIEncoding
	file ext:	h
	author:		MHD YAMEN SARAIJI
	
	purpose:	
*********************************************************************/
#ifndef ___ASCIIEncoding___
#define ___ASCIIEncoding___

#include "IStringEncoding.h"


namespace mray
{
namespace core
{

class MRAY_DLL ASCIIEncoding:public IStringEncoding
{
protected:

public:
	ASCIIEncoding(){}
	virtual~ASCIIEncoding(){}

	int GetBytesCount(const core::string& str);
	int GetBytesCount(const mchar* str,int count);

	int GetBytes(const core::string& str,byte* buff,int maxLen);
	int GetBytes(const mchar* str,int count,byte* buff,int maxLen);

	int GetCharsCount(const byte* buff,int len);
	core::string GetString(const byte* buff,int len);
	int GetChars(const byte* buff,int len,mchar* str,int maxLen);
};

}
}

#endif





/********************************************************************
	created:	2012/07/15
	created:	15:7:2012   13:20
	filename: 	d:\Development\mrayEngine\Engine\mrayEngine\include\UnicodeEncoding.h
	file path:	d:\Development\mrayEngine\Engine\mrayEngine\include
	file base:	UnicodeEncoding
	file ext:	h
	author:		MHD YAMEN SARAIJI
	
	purpose:	
*********************************************************************/
#ifndef ___UnicodeEncoding___
#define ___UnicodeEncoding___

#include "IStringEncoding.h"


namespace mray
{
namespace core
{

class MRAY_DLL UnicodeEncoding:public IStringEncoding
{
protected:

public:
	UnicodeEncoding(){}
	virtual~UnicodeEncoding(){}


	int GetBytesCount(const core::string& str);
	int GetBytesCount(const mchar* str,int count);

	int GetBytes(const core::string& str,byte* buff,int maxLen);
	int GetBytes(const mchar* str,int count,byte* buff,int maxLen);

	int GetCharsCount(const byte* buff,int len);
	core::string GetString(const byte* buff,int len);
	int GetChars(const byte* buff,int len,mchar* str,int maxLen);

	int GetBytesCountW(const core::stringw& str);
	int GetBytesCountW(const wchar_t* str,int count);
	int GetBytesW(const core::stringw& str,byte* buff,int maxLen);
	int GetBytesW(const wchar_t* str,int count,byte* buff,int maxLen);
	core::stringw GetStringW(const byte* buff,int len);
	int GetCharsW(const byte* buff,int len,wchar_t* str,int maxLen);
};

}
}

#endif



#include "stdafx.h"
#include "UnicodeEncoding.h"



namespace mray
{
namespace core
{


int UnicodeEncoding::GetBytesCount(const core::string& str)
{
	return str.length()*2;
}
int UnicodeEncoding::GetBytesCount(const mchar* str,int count)
{
	return count*2;
}

int UnicodeEncoding::GetBytes(const core::string& str,byte* buff,int maxLen)
{
	return GetBytes(&str[0],str.length(),buff,maxLen);
}
int UnicodeEncoding::GetBytes(const mchar* ptr,int count,byte* buff,int maxLen)
{
	int len=math::Min(maxLen,count);
	const byte* dptr=(const byte*)ptr;
	for(int i=0;i<len;++i)
	{
#ifdef UNICODE
		*buff=*dptr;
		*(buff+1)=*(dptr+1);
#else
		*buff=*dptr;
		*(buff+1)=0;
#endif
		buff+=2;
		++ptr;
	}
	return len;
}

int UnicodeEncoding::GetCharsCount(const byte* buff,int len)
{
	return len/2;
}
core::string UnicodeEncoding::GetString(const byte* buff,int len)
{
	core::string ret;
	//ret.reallocate(len);
	mchar v=0;
	byte* ptr=(byte*)&v;
	for(int i=0;i<len;++i)
	{
#ifdef UNICODE
		ptr[0]=*buff;
		ptr[1]=*(buff+1);
#else
		ptr[0]=*buff;
#endif
		ret.append(1,v);
		buff+=2;
	}
	return ret;
}
int UnicodeEncoding::GetChars(const byte* buff,int len,mchar* str,int maxLen)
{
	int l=math::Min(2*len,maxLen);
	byte* ptr=( byte*)str;
	for(int i=0;i<l;++i)
	{
		*ptr=*buff;
	}
	return l/2;
}

int UnicodeEncoding::GetBytesCountW(const core::stringw& str)
{
	return str.length()*2;
}
int UnicodeEncoding::GetBytesCountW(const wchar_t* str,int count)
{
	return count*2;
}

int UnicodeEncoding::GetBytesW(const core::stringw& str,byte* buff,int maxLen)
{
	return GetBytesW(&str[0],str.length(),buff,maxLen);
}
int UnicodeEncoding::GetBytesW(const wchar_t* ptr,int count,byte* buff,int maxLen)
{
	int len=math::Min(maxLen,count);
	const byte* dptr=(const byte*)ptr;
	for(int i=0;i<len;++i)
	{
		*buff=*dptr;
		*(buff+1)=*(dptr+1);
		buff+=2;
		dptr+=2;
	}
	return len;
}

core::stringw UnicodeEncoding::GetStringW(const byte* buff,int len)
{
	core::stringw ret;
	//ret.reallocate(len);
	wchar_t v=0;
	byte* ptr=(byte*)&v;
	for(int i=0;i<len;i+=2)
	{
		ptr[0]=*buff;
		ptr[1]=*(buff+1);
		ret.append(1,v);
		buff+=2;
	}
	return ret;
}
int UnicodeEncoding::GetCharsW(const byte* buff,int len,wchar_t* str,int maxLen)
{
	int l=math::Min(2*len,maxLen);
	byte* ptr=( byte*)str;
	for(int i=0;i<l;++i)
	{
		*ptr=*buff;
	}
	return l/2;
}

}
}
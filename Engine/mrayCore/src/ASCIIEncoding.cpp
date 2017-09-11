
#include "stdafx.h"
#include "ASCIIEncoding.h"



namespace mray
{
namespace core
{

int ASCIIEncoding::GetBytesCount(const core::string& str)
{
	return str.length();
}
int ASCIIEncoding::GetBytesCount(const mchar* str,int count)
{
	return count;
}

int ASCIIEncoding::GetBytes(const core::string& str,byte* buff,int maxLen)
{
	return GetBytes(&str[0],str.length(),buff,maxLen);
}
int ASCIIEncoding::GetBytes(const mchar* ptr,int count,byte* buff,int maxLen)
{
	int len=math::Min(maxLen,count);
	for(int i=0;i<len;++i)
	{
#ifdef UNICODE
		*buff=63;//		'?'
#else
		*buff=*ptr;
#endif
		++buff;
		++ptr;
	}
	return len;
}

int ASCIIEncoding::GetCharsCount(const byte* buff,int len)
{
	return len;
}
core::string ASCIIEncoding::GetString(const byte* buff,int len)
{
	core::string ret;
	//ret.reallocate(len);
	for(int i=0;i<len;++i)
	{
		ret.append(1,(mchar)buff[i]);
	}
	return ret;
}
int ASCIIEncoding::GetChars(const byte* buff,int len,mchar* str,int maxLen)
{
	int l=math::Min(len,maxLen);
	for(int i=0;i<l;++i)
	{
		str[i]=(mchar)buff[i];
	}
	return l;
}

}
}

#include "stdafx.h"
#include "mString.h"

// #define WIN32_LEAN_AND_MEAN
// #include <windows.h>

namespace mray{
namespace core{


int  wchar_to_char(char*a,const  wchar_t *b){
	std::basic_string<char>  s;
	s.find("asd");
	if(s!="x")
	{
	}
	const core::string str;
	if(str!="")
	{
	}
	str.equalsn("as",2);
	int len=wcslen(b);
	return wcstombs(a,b,len);
	//return WideCharToMultiByte(CP_ACP, 0,b,-1,a,len*sizeof(wchar_t)+1,0,0);

	int i=0;
	int k=0;
	for(;i<len;++i){
		if(b[i]<256)
			a[k++]=(int)b[i];
	}
	a[k]=0;
	return k;
}

int  wchar_to_char(const  wchar_t *in,stringc&str){

	int len=wcslen(in);
	str.resize(len);
	return wcstombs(&str[0],in,len);
	//return WideCharToMultiByte(CP_ACP, 0,in,-1,&str[0],len*sizeof(wchar_t)+1,0,0);

}

int char_to_wchar( wchar_t*a,const char*b){

	int len=strlen(b);
	return mbstowcs(a,b,len);
	//return MultiByteToWideChar(CP_ACP, 0,b,-1,a,len+1);

	//	int len=strLength(b);
	int k=0;
	int i=0;
	for(;i<len;++i)
		a[i]=b[i];
	a[i]=0;
	return i;
}

int char_to_wchar(const char *in,stringw&str){

	int len=strlen(in);
	str.resize(len);
	return mbstowcs(&str[0],in,len);
	//return MultiByteToWideChar(CP_ACP, 0,in,-1,&str[0],len+1);

}

int char_to_string(const char *in,string&str){
#ifdef UNICODE
	return char_to_wchar(in,str);
#else
	str=in;
	return str.length();
#endif

}

string char_to_string(const char *in)
{
	string res;
	char_to_string(in,res);
	return res;
}

int wchar_to_string(const wchar_t *in,string&str){
#ifdef UNICODE
	str=in;
	return str.length();
#else
	return wchar_to_char(in,str);
#endif
}

string wchar_to_string(const wchar_t *in)
{
	string res;
	wchar_to_string(in,res);
	return res;
}

int string_to_char(const string &in,stringc&str){
#ifdef UNICODE
	return wchar_to_char(in.c_str(),str);
#else
	str=in;
	return str.length();
#endif
}
stringc  string_to_char(const string &in)
{
	stringc res;
	string_to_char(in,res);
	return res;
}

int string_to_wchar(const string &in,stringw&str){
#ifdef UNICODE
	str=in;
	return str.length();
#else
	return char_to_wchar(in.c_str(),str);
#endif
}
stringw  string_to_wchar(const string &in)
{
	stringw res;
	string_to_wchar(in,res);
	return res;
}

}
}








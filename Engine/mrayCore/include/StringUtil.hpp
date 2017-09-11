#include "stdafx.h"

#include "StringUtil.h"
#include <string>
#include <utf8/utf8.h>

namespace mray
{
namespace core
{

#define _lowerChar(C)(((C)>='A' && (C)<='Z')?((C)-'A'+'a'):(C))
	template <typename T>
void StringUtilT<T>::SplitPathFileName(const core::tstring<T>&str,core::tstring<T>&path,core::tstring<T>&file)
{
	core::tstring<T> fullPath(str);
	NormalizePathSlashes(&fullPath[0]);
	int i=fullPath.findlast('\\');
	if(i!=-1)
	{
		path=fullPath.substr(0,i+1);
		file=fullPath.substr(i+1,fullPath.length()-i-1);
	}else
	{
		path=fullPath;
		file="";
	}
}
template <typename T>
void StringUtilT<T>::SplitPathExt(const core::tstring<T>&str, core::tstring<T>&path, core::tstring<T>&ext)
{
	core::tstring<T> fullPath(str);
	NormalizePathSlashes(&fullPath[0]);
	int i=fullPath.findlast('.');
	if(i!=-1)
	{
		path=fullPath.substr(0,i);
		ext=fullPath.substr(i+1,fullPath.length()-i-1);
	}else
	{
		path=fullPath;
		ext="";
	}
}

template <typename T>
void StringUtilT<T>::NormalizePathSlashes(T*str){
	for(T*p=str;*p;p++){
		if(*p=='/')
			*p='\\';
	}
}

template <typename T>
core::tstring<T> StringUtilT<T>::FindAndReplace(const core::tstring<T>& a, const core::tstring<T>& findStr, const core::tstring<T>& replaceStr)
{
	core::tstring<T> ret=a;
	size_t pos=0;
	while ((pos=ret.find(findStr, pos)) != core::tstring<T>::npos)
	{
		ret.replace(pos, findStr.length(), replaceStr);
		pos += replaceStr.length();
	}
	return ret;
}


template <typename T>
bool StringUtilT<T>::BeginsWith(const core::tstring<T>& a, const core::tstring<T>& b, bool caseSensitive)
{
	const T* aptr = a.c_str();
	const T* bptr = b.c_str();
	while ( *bptr != 0)
	{
		if (caseSensitive)
		{
			if (*aptr != *bptr)break;
		}
		else if (_lowerChar(*aptr) != _lowerChar(*bptr))
			break;
		++aptr; ++bptr;
	}
	return *bptr == 0;
}

template <typename T>
core::tstring<T> StringUtilT<T>::ToLower(const core::tstring<T>& o)
{
	static const T diff = 'a' - 'A';
	core::tstring<T> ret;
	if(o.length()==0)
		return ret;
	const T* ptr=o.c_str();
	ret.resize(o.length());//+1);
	for (int i=0; i<o.length(); ++i)
	{
		if (ptr[i]>='A' && ptr[i]<='Z')
			ret[i] = ptr[i]+diff;
		else
			ret[i]=ptr[i];
	}
//	ret[o.length()]=0;
	return ret;
}



//! Makes the tstring upper case.
template <typename T>
core::tstring<T> StringUtilT<T>::ToUpper(const core::tstring<T>& o)
{
	static const T diff = 'A' - 'a';
	core::tstring<T> ret;
	if(o.length()==0)
		return ret;
	const T* ptr=o.c_str();
	ret.resize(o.length());//+1);
	for (int i=0; i<o.length(); ++i)
	{
		if (ptr[i]>='a' && ptr[i]<='z')
			ret[i] = ptr[i]+diff;
		else
			ret[i]=ptr[i];
	}
//	ret[o.length()]=0;
	return ret;
}
//make "path" relative to "root"
//example:	root=c:\game\bin
//			path=c:\game\data
//result:	..\data
template <typename T>
core::tstring<T> StringUtilT<T>::MakePathRelative(const core::tstring<T>&path, const core::tstring<T>&root)
{
	int matched=0;
	//	path.make_lower();
	//	app.make_lower();
	core::tstring<T> cpy(path);
	core::tstring<T> cpyRoot(root);
	NormalizePathSlashes(&cpy[0]);
	NormalizePathSlashes(&cpyRoot[0]);

	for(int i=0;i<cpy.length() && i<cpyRoot.length();++i)
	{
		if(tolower(cpy[i])!=tolower(cpyRoot[i]))
			break;
		else if(cpy[i]=='\\')
			matched=i;
	}
	core::tstring<T> minPath;
	int appFolders=1;
	for(int i=matched+1;i<cpyRoot.length();++i)
	{
		if(cpyRoot[i]=='\\'){
			minPath+=mT("..\\");
			++appFolders;
		}
	}
	minPath+=path.substr(matched+1,path.length()-matched);

	return minPath;
}

template <typename T>
core::tstring<T> StringUtilT<T>::Trim(const core::tstring<T>& str, const core::tstring<T>& chars)
{
	core::tstring<T> ret;
	bool found=false;
	for(int i=0;i<str.length();++i)
	{
		if(chars.find(str[i])==-1 || found)
		{
			found=true;
			ret.append(1,str[i]);
		}
	}
	for(int i=ret.length()-1;i>=0;--i)
	{
		if(chars.find(ret[i])==-1)
		{
			ret = ret.substr(0, i+1);
			break;
		}
	}
	return ret.c_str();
}
template <typename T>
std::vector<core::tstring<T>> StringUtilT<T>::Split(const core::tstring<T>& str, const core::tstring<T>& delimStr, uint maxSplits)
{
	std::vector<core::tstring<T>> ret;
	ret.reserve(maxSplits ? maxSplits:10);

	core::tstring<T> temp;
	const T*p=str.c_str();
	int cnt=0;
	for(;*p!=0;p++){
		if(delimStr.find(*p)!=-1 ){
			if(temp!=_T("\0")){
				ret.push_back(temp);
				temp=_T("\0");
				cnt++;
				if(maxSplits!=0 && cnt>=maxSplits)
					break;
			}
		}else
			temp+=*p;
	}
	if(maxSplits==0 || cnt<maxSplits){
		if(temp!=_T("\0")){
			ret.push_back(temp);
		}
	}
	return ret;
}

template <typename T>
void StringUtilT<T>::Utf8ToUtf16(const core::string& _utf8, core::stringw& utf16)
{
	utf16 = L"";
	utf8::utf8to16(_utf8.begin(), _utf8.end(), std::back_inserter(utf16));
}
template <typename T>
void StringUtilT<T>::Utf16ToUtf8(const core::stringw& utf16, core::string& _utf8)
{
	_utf8 = "";
	utf8::utf16to8(utf16.begin(), utf16.end(), std::back_inserter(_utf8));

}
}
}
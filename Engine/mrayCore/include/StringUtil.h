

/********************************************************************
	created:	2011/02/23
	created:	23:2:2011   15:17
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\StringUtil.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	StringUtil
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef StringUtil_h__
#define StringUtil_h__

#include "mstring.h"

namespace mray
{
namespace core
{

	template<typename T>
class  StringUtilT
{
private:
protected:
public:

	static void SplitPathFileName(const core::tstring<T>&str,core::tstring<T>&path,core::tstring<T>&file);
	static void SplitPathExt(const core::tstring<T>&str,core::tstring<T>&path,core::tstring<T>&ext);



	//! Makes the tstring lower case.
	static core::tstring<T> ToLower(const core::tstring<T>& o);

	//! Makes the tstring upper case.
	static core::tstring<T> ToUpper(const core::tstring<T>& o);

	static bool BeginsWith(const core::tstring<T>& a, const core::tstring<T>& b,bool caseSensitive=false);

	static core::tstring<T> FindAndReplace(const core::tstring<T>& a, const core::tstring<T>& findStr, const core::tstring<T>& replaceStr);

	//make "path" relative to "root"
	//example:	root=c:\game\bin
	//			path=c:\game\data
	//result:	..\data
	static core::tstring<T> MakePathRelative(const core::tstring<T>&path,const core::tstring<T>&root);

	//convert '/' chars into '\\'
	static void NormalizePathSlashes(T*str);

	static core::tstring<T> Trim(const core::tstring<T>& str,const core::tstring<T>& chars=mT(" \t"));

	static std::vector<core::tstring<T>> Split(const core::tstring<T>& str,const core::tstring<T>& delimStr,uint maxSplits=0);

	static void Utf8ToUtf16(const core::string& utf8, core::stringw& utf16);
	static void Utf16ToUtf8(const core::stringw& utf16, core::string& utf8);
};


typedef StringUtilT<char> StringUtilA;
typedef StringUtilT<wchar_t> StringUtilW;
typedef StringUtilT<mchar> StringUtil;



}
}

#include "StringUtil.hpp"

#endif // StringUtil_h__

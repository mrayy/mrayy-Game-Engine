

#include "stdafx.h"
#include "DataTypes.h"
#include "utf8/utf8.h"

#include <windows.h>

namespace mray
{
namespace ted
{

core::stringw ConvertToStringW(const char* multibyte, int cnt)
{
	wchar_t buffer[1024];
	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED,multibyte, cnt + 1, buffer, 1024);
	return buffer;

}


core::stringw ConvertToUtf16(const core::string& utf8)
{
	core::stringw ret;
	utf8::utf8to16(utf8.begin(), utf8.end(), std::back_inserter(ret));
	return ret;
}
core::string ConvertToUtf8(const core::stringw& utf16)
{
	core::string ret;
	utf8::utf16to8(utf16.begin(), utf16.end(), std::back_inserter(ret));
	return ret;
}

}
}

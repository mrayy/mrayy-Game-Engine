

/********************************************************************
	created:	2014/04/09
	created:	9:4:2014   21:46
	filename: 	C:\Development\mrayEngine\Projects\TEDxTokyo\TwitterVisualizer\DataTypes.h
	file path:	C:\Development\mrayEngine\Projects\TEDxTokyo\TwitterVisualizer
	file base:	DataTypes
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __DataTypes__
#define __DataTypes__




namespace mray
{
namespace ted
{

	typedef unsigned long long IDType;

	core::stringw ConvertToStringW(const char* multibyte, int cnt);
	core::stringw ConvertToUtf16(const core::string& utf8);
	core::string ConvertToUtf8(const core::stringw& utf16);

}
}

#endif

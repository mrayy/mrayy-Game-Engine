

/********************************************************************
	created:	2012/07/15
	created:	15:7:2012   13:01
	filename: 	d:\Development\mrayEngine\Engine\mrayEngine\include\Encoding.h
	file path:	d:\Development\mrayEngine\Engine\mrayEngine\include
	file base:	Encoding
	file ext:	h
	author:		MHD YAMEN SARAIJI
	
	purpose:	
*********************************************************************/
#ifndef ___Encoding___
#define ___Encoding___

#include "ASCIIEncoding.h"
#include "UnicodeEncoding.h"

namespace mray
{
namespace core
{

class MRAY_DLL StringEncoding
{
protected:

	static IStringEncoding* m_activeEncoder;
public:
	
	static ASCIIEncoding ASCII;
	static UnicodeEncoding Unicode;
	static IStringEncoding* GetActiveEncoder();
};

}
}

#endif


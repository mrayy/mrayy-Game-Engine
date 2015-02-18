

#include "stdafx.h"
#include "StringEncoding.h"

namespace mray
{
namespace core
{


	ASCIIEncoding StringEncoding::ASCII;
	UnicodeEncoding StringEncoding::Unicode;


#ifdef UNICODE
	IStringEncoding* StringEncoding::m_activeEncoder=&Unicode;
#else
	IStringEncoding* StringEncoding::m_activeEncoder=&ASCII;
#endif

	IStringEncoding* StringEncoding::GetActiveEncoder()
	{
		return m_activeEncoder;
	}

}
}


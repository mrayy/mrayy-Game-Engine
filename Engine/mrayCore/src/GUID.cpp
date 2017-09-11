#include "stdafx.h"


#include "GUID.h"
#include "CRC.h"

namespace mray
{

GUID::GUID()
{
	m_crc=0;
}
GUID::~GUID()
{
}
GUID::GUID(const GUID&o):
m_crc(o.m_crc) 
//#ifdef _DEBUG 
,m_string(o.m_string) 
//#endif
{
}
GUID::GUID(const core::stringc& str)
{
	*this=str;
}

GUID& GUID::operator=(const core::stringc&str)
{
//#ifdef _DEBUG
	m_string=str;
//#endif
	m_crc=OS::CRC::calcCRC(str.c_str(),str.length());
	return *this;
}




}



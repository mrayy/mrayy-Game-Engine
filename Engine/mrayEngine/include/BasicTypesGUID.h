


/********************************************************************
	created:	2011/12/20
	created:	20:12:2011   14:18
	filename: 	d:\Development\mrayEngine\Engine\mrayEngine\include\BasicTypesGUID.h
	file path:	d:\Development\mrayEngine\Engine\mrayEngine\include
	file base:	BasicTypesGUID
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __BasicTypesGUID__
#define __BasicTypesGUID__

#include "GUID.h"


namespace mray
{

#define typeof(type) mray::GUID(#type)
class BasicTypeInfo
{
protected:
	mray::GUID m_guid;
	bool m_isNumeric;
public:
	BasicTypeInfo():m_isNumeric(false)
	{}
	BasicTypeInfo(const mray::GUID& type,bool numeric):m_guid(type),m_isNumeric(numeric)
	{
	}
	const mray::GUID& GetGUID()const
	{
		return m_guid;
	}
	bool IsNumeric()const
	{
		return m_isNumeric;
	}
};
class MRAY_DLL BasicTypesGUID
{
protected:
	typedef std::map<uint,BasicTypeInfo> TypesMap;
	static TypesMap m_types;
protected:
	BasicTypesGUID();

	static BasicTypesGUID m_instance;
public:

	static void AddTypeInfo(const BasicTypeInfo& ifo);
	static const BasicTypeInfo* GetTypeInfo(uint type);
};

}

#endif

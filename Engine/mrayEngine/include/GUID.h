

/********************************************************************
	created:	2010/12/09
	created:	9:12:2010   15:01
	filename: 	i:\Programing\GameEngine\mrayEngine\testFrameWork\ScreenSpaceEff\GUID.h
	file path:	i:\Programing\GameEngine\mrayEngine\testFrameWork\ScreenSpaceEff
	file base:	GUID
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef GUID_h__
#define GUID_h__

#include "compileConfig.h"
#include <mString.h>
#include "mTypes.h"

namespace mray
{

class MRAY_DLL GUID
{
private:
protected:
//#ifdef _DEBUG
	core::stringc m_string;
//#endif
	uint m_crc;
public:
	GUID();
	GUID(const GUID&o);
	GUID(const core::stringc& str);
	virtual ~GUID();

	const core::stringc& GetString()const{return m_string;}

	GUID& operator=(const core::stringc&str);

	inline bool operator==(const GUID& o)const
	{
		return m_crc==o.m_crc;
	}
	inline uint ID()const{return m_crc;}
};

}

#endif // GUID_h__


/********************************************************************
	created:	2009/03/09
	created:	9:3:2009   15:33
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\CatagoryManager.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	CatagoryManager
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___CatagoryManager___
#define ___CatagoryManager___

#include "mString.h"


namespace mray{

class MRAY_DLL CatagoryManager
{
protected:
	core::string m_catagory;

	struct SBitStruct{
		SBitStruct(){}
		SBitStruct(const core::string&id,ulong bits){
			m_ID=id;
			m_bits=bits;
		}
		core::string m_ID;
		ulong m_bits;
	};
	typedef std::map<core::string,SBitStruct> BitMap;
	BitMap m_bitsMap;
	uint m_lastBit;

public:
	CatagoryManager(const core::string&name);
	virtual~CatagoryManager();

	bool addBit(const core::string&name);
	void setCombineBits(const core::string&name,ulong bits);

	ulong getBit(const core::string&name);

	void setCatagoryName(const core::string&name);
	const core::string& getCatagoryName();
};

}


#endif //___CatagoryManager___

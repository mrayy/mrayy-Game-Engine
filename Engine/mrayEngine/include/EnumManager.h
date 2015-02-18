
/********************************************************************
	created:	2009/04/11
	created:	11:4:2009   20:48
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\EnumManager.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	EnumManager
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___EnumManager___
#define ___EnumManager___

#include "ISingleton.h"
#include "string.h"



namespace mray{

typedef std::map<core::string,int> EnumNameValMap;
typedef std::map<int,core::string> EnumValNameMap;

class MRAY_DLL EnumInfo
{
public:
	core::string name;
	EnumNameValMap NameVal;
	EnumValNameMap ValName;

	bool ContainsValue(const core::string& v)const;
	bool ContainsValue(int v)const;

	const core::string& ToString(int v)const;
	int Parse(const core::string& v)const;
};
typedef std::map<core::string,EnumInfo*> EnumInfoMap;

class MRAY_DLL EnumManager:public ISingleton<EnumManager>
{
private:
	EnumInfoMap m_enumInfo;
	EnumInfo* m_currInfo;

	const core::string m_dummyString;
public:
	EnumManager();
	virtual~EnumManager();

	void startEnum(const core::string&name);
	void addValue(const core::string&name,int val);
	void endEnum();

	void removeEnum(const core::string&name);

	void clear();

	int getValue(const core::string&e,const core::string&v);
	const core::string& getName(const core::string&e,int v);
	const EnumInfo* getEnumInfo(const core::string&e);
};

}

#define StartRegisterEnum(e) mray::EnumManager::getInstance().startEnum(mT(#e))
#define RegisterEnumValue(v)mray::EnumManager::getInstance().addValue(mT(#v),(int)v)
#define RegisterEnumValueA(alias,v)mray::EnumManager::getInstance().addValue(alias,(int)v)
#define EndRegisterEnum() mray::EnumManager::getInstance().endEnum()


#endif //___EnumManager___

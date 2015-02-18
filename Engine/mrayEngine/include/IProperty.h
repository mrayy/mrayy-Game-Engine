#ifndef ___IProperty___
#define ___IProperty___

//#include "IAttribute.h"
#include "compileConfig.h"

#include "mString.h"
#include "StringConverter.h"
#include "macros.h"
//#include "EGenericTypes.h"
#include "common.h"
#include "GUID.h"



namespace mray{

	enum EPropertieAccess
	{
		EPA_READ,
		EPA_WRITE,
		EPA_RW
	};

	enum EPropertyFlags
	{
		EPF_Read		= BIT(0),
		EPF_Write		= BIT(1),
		EPF_Serialize	= BIT(2),
		EPF_Modify		= BIT(3)
	};

	class CPropertieSet;
	class IPropertyVistor;
	class EnumInfo;

	enum EPropertyBasicType
	{
		EPBT_Basic,
		EPBT_Struct,
		EPBT_Enum,
		EPBT_List
	};
	
class MRAY_DLL IProperty
{
protected:
	core::string m_name;
	core::string m_info;
	const EnumInfo* m_enum;

	EPropertyBasicType m_type;

	uint m_flags;

	mray::GUID m_propertyType;

	std::vector<core::string> m_itemList;
	EPropertieAccess m_access;
public:
	IProperty(const core::string&name,EPropertyBasicType type,const GUID& propType,const  core::string&info=mT(""));

	inline void setFlag(EPropertyFlags f,bool set)
	{
		if(set)
			m_flags |= (uint)f ;
		else
			m_flags &= ~(uint)f ;
	}
	inline bool getFlag(EPropertyFlags f)const{return m_flags & (uint)f;}

	const GUID& getPropertyType()const;

	const core::string&getName()const;
	const  core::string&getInfo()const;
	void setInfo(const  core::string&info);

	EPropertieAccess getAccessMode()const;

	virtual core::string toString(CPropertieSet*object)const=0;
	virtual bool parse(CPropertieSet*reciver,const core::string&str)=0;

	virtual bool isDefault(CPropertieSet*reciver)const=0;
	virtual EPropertyBasicType getType()const{return m_type;}

	virtual void OnVisit(IPropertyVistor* visitor);

	void SetItemList(const std::vector<core::string>& items);
	const std::vector<core::string>& GetSupportedItems()const;

	void SetEnum(const  EnumInfo* e);
	const EnumInfo*GetEnum()const;

	virtual bool CheckIsValidValue(const core::string& v);
};


}


#endif
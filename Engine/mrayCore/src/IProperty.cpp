#include "stdafx.h"

#include "IProperty.h"
#include "IPropertyVistor.h"
#include "EnumManager.h"


namespace mray{

IProperty::IProperty(const core::string&name,EPropertyBasicType type,const GUID& propType,const  core::string&info):
m_name(name),m_info(info),m_type(type),m_access(EPA_RW),m_enum(0),m_propertyType(propType)
{
	m_flags=0;
	setFlag(EPF_Serialize,true);
	setFlag(EPF_Read,true);
	setFlag(EPF_Modify,false);
}

const GUID& IProperty::getPropertyType()const
{
	return m_propertyType;
}
const core::string& IProperty::getName()const
{
	return m_name;
}
const  core::string& IProperty::getInfo()const
{
	return m_info;
}
void IProperty::setInfo(const  core::string&info)
{
	m_info=info;
}

EPropertieAccess IProperty::getAccessMode()const
{
	return m_access;
}


void IProperty::OnVisit(IPropertyVistor* visitor)
{
	visitor->Visit(this);
}

void IProperty::SetItemList(const std::vector<core::string>& items)
{
	m_itemList=items;
}
const std::vector<core::string>& IProperty::GetSupportedItems()const
{
	return m_itemList;
}

void IProperty::SetEnum(const  EnumInfo* e)
{
	m_enum=e;
}
const  EnumInfo* IProperty::GetEnum()const
{
	return m_enum;
}
bool IProperty::CheckIsValidValue(const core::string& v)
{
	switch(m_type)
	{
	case EPBT_List:
		for(int i=0;i<m_itemList.size();++i)
		{
			if(m_itemList[i]==v)
				return true;
		}
		return false;
	case EPBT_Enum:
		if(!m_enum)
			return false;
		return m_enum->ContainsValue(v);
	}
	return true;
}


}



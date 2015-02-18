#include "stdafx.h"

#include "IAttribute.h"


namespace mray{


IAttribute::IAttribute(const core::string&type,const core::string&name,const  core::string&info){
	setName(name);
	m_info=info;
	m_typeStr=type;
	m_type=EGenericTypesUtil::convertTypeStr(type);
}

void IAttribute::setName(const core::string&name){
	m_name=name;
}
const core::string&IAttribute::getName(){
	return m_name;
}

core::string& IAttribute::getNameStr(){
	return m_name;
}

EGenericType IAttribute::getType(){
	return m_type;
}
void IAttribute::setInfo(const  core::string&info){
	m_info=info;
}
const  core::string&IAttribute::getInfo(){
	return m_info;
}

const core::string& IAttribute::getTypeStr(){
	return EGenericTypesUtil::convertType(m_type);
}




}




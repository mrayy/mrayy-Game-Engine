#include "stdafx.h"
#include "Param.h"
#include <string.h>

namespace mray{

Param::Param()
:m_type(EPT_NULL)
{
}

Param::Param(const Param&o)
:m_type(EPT_NULL)
{
}
Param::~Param(){
	clearData();
}
void Param::clearData(){
}

void Param::copy(const Param&o){
	clearData();
	m_type=o.m_type;
	switch (o.m_type)
	{
	case EPT_INT:
		i=o.i;
		break;
	case EPT_FLOAT:
		f=o.f;
		break;
	case EPT_BOOL:
		b=o.b;
		break;
	case EPT_NULL:
		break;
	case EPT_STRING:
		m_str=o.m_str;
		break;
	}
}
Param& Param::operator =(const Param&o){
	copy(o);
	return *this;
}
bool Param::operator ==(const Param&o)const{
	if(m_type==o.m_type ){

		switch (o.m_type)
		{
		case EPT_INT:
			return i==o.i;
		case EPT_FLOAT:
			return f==o.f;
		case EPT_BOOL:
			return b==o.b;
		case EPT_STRING:
			return m_str==o.m_str;
		case EPT_NULL:
			return true;
		default:
			return true;
		}
	}
	return false;
}

void Param::setType(char t){
	switch (t)
	{
	case 'i':setInt(0);break;
	case 'f':setFloat(0);break;
	case 'b':setBool(0);break;
	case 'v':setVoid(0);break;
	case 's':
		setStr(mT(""));
		break;
	case 'n':setNull();break;
	}
}
bool Param::isValid(char t){
	switch (t)
	{
	case 'i':
	case 'f':
	case 'b':
	case 'v':
	case 's':
	case 'n':
		return true;
	}
	return false;
}
char Param::getTypeChar()const{
	switch (m_type)
	{
	case EPT_INT:return 'i';
	case EPT_FLOAT:return 'f';
	case EPT_BOOL:return 'b';
	case EPT_VOID:return 'v';;
	case EPT_STRING:return 's';
	case EPT_NULL:return 'n';
	}
	return '?';
}


Param::EParamType Param::getType()const{
	return m_type;
}

void Param::setNull(){
	m_type=EPT_NULL;
	v=0;
}

void Param::setInt(int o){
	m_type=EPT_INT;
	i=o;
}
void Param::setFloat(float o){
	m_type=EPT_FLOAT;
	f=o;
}
void Param::setBool(bool o){
	m_type=EPT_BOOL;
	b=o;
}
void Param::setVoid(void* o){
	m_type=EPT_VOID;
	v=o;
}
void Param::setStr(const core::string& o){
	m_type=EPT_STRING;
	m_str=(o);
}

int Param::getInt()const{
	return i;
}
float Param::getFloat()const{
	return f;
}
bool Param::getBool()const{
	return b;
}
void* Param::getVoid()const{
	return v;
}
const core::string& Param::getStr()const{
	return m_str;
}


}


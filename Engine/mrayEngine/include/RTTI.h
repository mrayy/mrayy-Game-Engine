

/********************************************************************
	created:	2009/01/15
	created:	15:1:2009   19:23
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\RTTI.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	RTTI
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	Runtime type information
*********************************************************************/

#ifndef ___RTTI___
#define ___RTTI___

#include "mString.h"
#include "StringConverter.h"
#include "GUID.h"

namespace mray{

class RTTI
{
	core::string m_name;
	RTTI *m_parent;
	mray::GUID m_guid;

public:
	RTTI(const char*name,RTTI* parent):m_parent(parent)
	{
		m_name=core::StringConverter::toString(name);

 		m_guid=m_name;
	};

	const mray::GUID& GetGUID()const{return m_guid;}

	const core::string &getTypeName()const{return m_name;}

	bool isKindOf(const RTTI*other)const {
		if(this==other)return true;
		RTTI*p=m_parent;
		while(p){
			if(p==other)
				return true;
			p=p->m_parent;
		}
		return false;
	}
	bool isKindOf(const core::string &other)const {
		if(m_name==other)return true;
		RTTI*p=m_parent;
		while(p){
			if(p->m_name==other)
				return true;
			p=p->m_parent;
		}
		return false;
	}

	RTTI*getParent(){
		return m_parent;
	}

};

#define DECLARE_RTTI protected:\
	static RTTI ms_RTTI_def;\
	public:\
		static const core::string &getClassType(){return ms_RTTI_def.getTypeName();}\
		virtual const core::string &getObjectType()const{return ms_RTTI_def.getTypeName();}\
		static bool isKindOf(const RTTI*other) {return ms_RTTI_def.isKindOf(other);}\
		static const RTTI*getClassRTTI(){return &ms_RTTI_def;}\
		virtual const RTTI*getObjectRTTI()const{return &ms_RTTI_def;}\
		template <class T>\
			T* tryCast() {\
				if(ms_RTTI_def.isKindOf(T::getRTTI()))\
					return (T*)this;\
				else return false;\
			}\


#define IMPLEMENT_RTTI(type,parent) RTTI type::ms_RTTI_def(#type,&parent::ms_RTTI_def);
#define IMPLEMENT_ROOT_RTTI(type) RTTI type::ms_RTTI_def(#type,0);

}


#endif //___RTTI___

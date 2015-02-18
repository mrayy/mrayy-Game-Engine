

/********************************************************************
	created:	2009/02/27
	created:	27:2:2009   23:22
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\SOptionElement.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	SOptionElement
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___SOptionElement___
#define ___SOptionElement___

#include "mString.h"
#include <set>

namespace mray{

class MRAY_DLL SOptionElement{
public:
	SOptionElement(const core::string&name,const core::string&value)
	{
		this->name=name;
		this->value=value;
	}
	SOptionElement(){}

	bool operator ==(const SOptionElement&o)const
	{
		return name==o.name;
	}
	bool operator <(const SOptionElement&o)const
	{
		return name<o.name;
	}
	bool operator ==(const mchar*name)const
	{
		return name==name;
	}

	void setValue(const core::string&value)
	{
		this->value=value;
	}
	void setValue(int v)
	{
		ValueSet::iterator it=valueSet.begin();
		std::advance(it,v);
		if(it==valueSet.end())return;
		value=*it;
	}
	int getValueIndex()const
	{
		int i=0;
		ValueSet::iterator it=valueSet.begin();
		for (;it!=valueSet.end();++it,++i)
		{
			if(*it==value)
				return i;
		}
		return -1;
	}
	const core::string& getValue()const
	{
		return value;
	}

	core::string name;
	core::string value;
	typedef std::set<core::string> ValueSet;
	ValueSet valueSet;
	//	std::vector<core::string> valueSet;
};

}


#endif //___SOptionElement___

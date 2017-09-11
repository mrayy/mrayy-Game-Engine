

#ifndef __VALUEGROUP__
#define __VALUEGROUP__

#include "mstring.h"
#include "IValue.h"


namespace mray
{

class MRAY_CORE_DLL  ValueGroup
{
protected:
	typedef std::map<core::string, ValueGroup*> SubGroupMap;
	typedef std::map<core::string, IValue*> ValueMap;

	core::string m_name;
	SubGroupMap m_subGroups;
	ValueMap m_values;

public:
	ValueGroup(const core::string&name);
	virtual ~ValueGroup();

	const core::string& GetName(){ return m_name; }

	IValue* AddValue(IValue* v);
	IValue* GetValue(const core::string& name);

	ValueGroup* AddValueGroup(ValueGroup* grp);
	ValueGroup* GetValueGroup(const core::string& name);

	void Clear();


	virtual void loadXMLSettings(xml::XMLElement* elem);
	virtual xml::XMLElement*  exportXMLSettings(xml::XMLElement* elem);
};

}


#endif
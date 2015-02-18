

#ifndef ___CSettingsFile___
#define ___CSettingsFile___

#include "mstring.h"
#include "compileConfig.h"



namespace mray{
namespace OS{
	class IStream;
}
namespace script{

	
struct SConfigTag{
	~SConfigTag(){
		m_attrSet.clear();
	}
	bool operator<(const SConfigTag&other)const {
		return tagName<other.tagName;
	}


	typedef std::map<core::string,core::string> Attributes;
	typedef Attributes::iterator AttributesIT;
	
	void addAttr(const core::string&name,const core::string&value){/*
		m_attrSet.insert(Attributes::value_type(std::make_pair(name,
			CAttribute<core::string>(name,"string",mT(""),value))));*/
		AttributesIT it=m_attrSet.find(name);
		if(it!=m_attrSet.end()){
			it->second=(value);
		}else{
			m_attrSet.insert(Attributes::value_type(name,value));
		}
	}

	const core::string& getAttr(const core::string& name){
		AttributesIT it=m_attrSet.find(core::string(name));
		if(it==m_attrSet.end())
			return core::string::Empty;
		return it->second;
	}
	bool hasAttr(const core::string& name)
	{
		return m_attrSet.find(core::string(name))!=m_attrSet.end();
	}

	AttributesIT getAttrEnd(){
		return m_attrSet.end();
	}


	
	core::string tagName;
	Attributes m_attrSet;
};

class MRAY_DLL CSettingsFile
{
public:
protected:
	typedef std::map<core::string,SConfigTag*> TagMap;
	TagMap m_Tags;
public:
	CSettingsFile(OS::IStream* stream);
	CSettingsFile();

	virtual ~CSettingsFile();

	bool loadSettings(OS::IStream* stream);
	void writeSettings(OS::IStream* file);

	SConfigTag*getTag(const core::string&tag);
	const core::string& getPropertie(const core::string&tag,const core::string&name);
	bool hasPropertie(const core::string&tag,const core::string&name);

	void clear();
	
	SConfigTag* addTag(const core::string&tag);
	void setPropertie(const core::string&tag,const core::string&name,const  core::string&value);

};

}
}

#endif 







#ifndef ___XMLElement___
#define ___XMLElement___

#include "mString.h"
#include "GCPtr.h"
#include "IXMLNode.h"


namespace mray{
namespace xml{

struct XMLAttribute{
public:
	XMLAttribute(){
	}
	XMLAttribute(const core::string&n,const core::string&v){
		name=n;
		value=v;
	}
	core::string name;
	core::string value;
};

class XMLElement;
class XMLTextNode;

typedef std::list<IXMLNode*> xmlSubElementsMap;
typedef xmlSubElementsMap::iterator xmlSubElementsMapIT;

typedef std::map<core::string,XMLAttribute> xmlAttributesMap;
typedef xmlAttributesMap::iterator xmlAttributesMapIT;

class MRAY_DLL XMLElement:public IXMLNode
{

protected:
	xmlSubElementsMap m_elements;

	xmlAttributesMap m_Attributes;

	core::string m_name;
	XMLElement* m_next;
	XMLElement* m_prev;
	XMLElement* m_lastElem;
public:
	XMLElement(const core::string&name="");
	virtual ~XMLElement();

	void setName(const core::string&n);
	const core::string& getName()const;

	 ENodeType GetType()const;

	int getSubElementsCount();
	void addSubElement(IXMLNode*elem);
	void removeSubElement(const core::string&name);
	XMLElement*getSubElement(const core::string&name);

	XMLElement* nextSiblingElement(const core::string&name);

	void Clear();
	
	int getAttributesCount();
    void addAttribute(const core::string&name,const core::string&value);
	void removeAttribute(const core::string&name);

	XMLAttribute*getAttribute(int index);
	XMLAttribute*getAttribute(const core::string&name);

	xmlAttributesMapIT getAttributesBegin();
	xmlAttributesMapIT getAttributesEnd();

	xmlSubElementsMapIT getElementsBegin();
	xmlSubElementsMapIT getElementsEnd();

	const  core::string& getValueString(const core::string&name);
	int getValueInt(const core::string&name);
	float getValueFloat(const core::string&name);
	bool getValueBool(const core::string&name);

	xml::XMLTextNode* GetTextNode();
};

}
}

#endif


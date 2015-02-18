


#ifndef ___XMLWriter___
#define ___XMLWriter___


#include "mstring.h"

namespace mray{
namespace xml{

	class IXMLNode;
	class XMLElement;
	class XMLComment;
	class XMLTextNode;

class MRAY_DLL XMLWriter
{
	struct STagAttr
	{
		STagAttr()
		{
			node=0;
		}
		STagAttr(IXMLNode*n)
		{
			node=n;
			hasSubAttr=0;
			hasSubTags=0;
		}
		bool hasSubAttr;
		bool hasSubTags;
		IXMLNode* node;
	};

	core::string m_tabs;
	int m_needToClose;
	int m_depth;

	bool m_isLastAttribute;
	bool m_notClosed;

	core::string m_xmlstream;

	std::list<STagAttr> m_tags;

	void generateTabs();

	void WriteNode(XMLElement* elem);
	void WriteNode(XMLComment* elem);
	void WriteNode(XMLTextNode* elem);

	XMLWriter& openTag(const core::string&name);
	XMLWriter& closeTag();
	void OnEnterElement(IXMLNode* n);
	void OnExitElement();
public:
	
	XMLWriter();
	virtual~XMLWriter();

	XMLWriter& addAttribute(const core::string&name,const core::string&value);

	XMLWriter& addElement(xml::IXMLNode*elem);

	core::string flush();

};


}
}


#endif


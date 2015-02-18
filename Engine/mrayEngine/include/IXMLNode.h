


/********************************************************************
	created:	2011/09/19
	created:	19:9:2011   16:19
	filename: 	d:\Development\mrayEngine\Engine\mrayEngine\include\IXMLNode.h
	file path:	d:\Development\mrayEngine\Engine\mrayEngine\include
	file base:	IXMLNode
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __IXMLNode__
#define __IXMLNode__


namespace mray
{
namespace xml
{

	enum ENodeType
	{
		ENT_Element,
		ENT_Text,
		ENT_Comment,
		ENT_Other
	};

	class XMLElement;

class IXMLNode
{
private:
protected:
	XMLElement* m_parent;
	friend class XMLElement;
public:
	IXMLNode():m_parent(0)
	{}
	virtual~IXMLNode(){}

	XMLElement*getParent(){return m_parent;}

	virtual ENodeType GetType()const=0;
};


}
}

#endif

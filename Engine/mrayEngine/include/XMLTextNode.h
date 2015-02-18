


/********************************************************************
	created:	2011/09/19
	created:	19:9:2011   16:20
	filename: 	d:\Development\mrayEngine\Engine\mrayEngine\include\XMLTextNode.h
	file path:	d:\Development\mrayEngine\Engine\mrayEngine\include
	file base:	XMLTextNode
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __XMLTextNode__
#define __XMLTextNode__

#include "IXMLNode.h"

namespace mray
{
namespace xml
{

class XMLTextNode:public IXMLNode
{
private:
protected:
	core::string m_value;
public:
	XMLTextNode(const core::string& val):m_value(val)
	{}
	virtual~XMLTextNode(){}

	virtual ENodeType GetType()const{return ENT_Text;}

	void SetValue(const core::string& val){m_value=val;}
	const core::string& GetValue()const{return m_value;}
};

}
}

#endif

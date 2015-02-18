#include "stdafx.h"

#include "XMLWriter.h"

#include "XMLElement.h"
#include "XMLComment.h"
#include "XMLTextNode.h"
#include "StringUtil.h"


namespace mray{
namespace xml{


XMLWriter::XMLWriter()
{
	m_needToClose=false;
	m_depth=0;
	m_isLastAttribute=false;
	m_notClosed=false;

}
XMLWriter::~XMLWriter()
{
}

void XMLWriter::generateTabs()
{
	m_tabs=mT("");
	for(int i=0;i<m_depth;++i)
	{
		m_tabs+=mT("\t");
	}
}

XMLWriter& XMLWriter::openTag(const core::string&name)
{
	core::string str=mT("");

	str+=m_tabs;
	str+=mT("<")+name;
	m_needToClose++;
	if(m_depth>1)
		m_isLastAttribute=1;
	else
		m_isLastAttribute=0;

	m_depth++;
	generateTabs();
//	m_tags.push_back(name);

	m_xmlstream+=(str);
	return *this;
}

XMLWriter& XMLWriter::closeTag()
{
	std::list<STagAttr>::iterator last=m_tags.end();
	--last;

	bool hasSubTags=(*last).hasSubTags;

	core::string str=mT("");
	m_depth--;
	generateTabs();
	//str+=m_tabs;
	/*
	if(!(*last).hasSubTags)
		str+=mT("/>");
	else
		str+=mT("</")+(*last).name+mT(">");*/
	str+=mT("\n");

	m_needToClose--;
	
	m_tags.erase(last);
	last=m_tags.end(); -- last;
	m_xmlstream+=(str);
	m_notClosed=false;
	return *this;
}
void XMLWriter::OnEnterElement(IXMLNode* n)
{
	std::list<STagAttr>::iterator last=m_tags.end();
	--last;
	if(last!=m_tags.end())
	{
		(*last).hasSubTags=1;
		if(m_notClosed)
		{
			m_xmlstream+=(mT(">"));
		}
	}
	
	m_tags.push_back(n);

	m_notClosed=true;
}
void XMLWriter::OnExitElement()
{
}

XMLWriter& XMLWriter::addAttribute(const core::string&name,const core::string&value){
	if(m_tags.size()>0)
	{
		std::list<STagAttr>::iterator last=m_tags.end();--last;
		if(last!=m_tags.end())
			(*last).hasSubAttr=1;
	}
	core::string str=mT(" ");


	//str+=mT("\n")+m_tabs;
	core::string v;
	v = core::StringUtil::FindAndReplace(value, "&", "&amp;");
	v = core::StringUtil::FindAndReplace(v, "\"", "&quot;");
	v = core::StringUtil::FindAndReplace(v, "'", "&apos;");
	v = core::StringUtil::FindAndReplace(v, "<", "&lt;");
	v = core::StringUtil::FindAndReplace(v, ">", "&gt;");
	str+=name+mT("=\"")+v +mT("\"");
	m_isLastAttribute=1;
	
	m_xmlstream+=(str);
	return *this;
}

XMLWriter& XMLWriter::addElement(xml::IXMLNode*elem)
{

	switch(elem->GetType())
	{
	case ENT_Comment:
		WriteNode(dynamic_cast<XMLComment*>(elem));
		break;
	case ENT_Element:
		WriteNode(dynamic_cast<XMLElement*>(elem));
		break;
	case ENT_Text:
		WriteNode(dynamic_cast<XMLTextNode*>(elem));
		break;
	}
	return *this;
}


void XMLWriter::WriteNode(XMLElement* elem)
{
	//openTag(elem->getName());
	if(elem->getParent()!=0)
		m_xmlstream+=mT("\n");

	m_xmlstream+= m_tabs+ mT("<")+elem->getName();
	m_depth++;
	generateTabs();
	{
		xmlAttributesMapIT it= elem->getAttributesBegin();
		xmlAttributesMapIT end= elem->getAttributesEnd();
		for(;it!=end;++it){
			addAttribute(it->second.name,it->second.value);
		}
	}
	if(elem->getSubElementsCount()>0)
	{
		m_xmlstream+=mT(">");
		xmlSubElementsMapIT it= elem->getElementsBegin();
		xmlSubElementsMapIT end= elem->getElementsEnd();
		IXMLNode*n=0;
		for(;it!=end;++it){
			n=*it;
			addElement(n);
		}

		m_depth--;
		generateTabs();
		if(n->GetType()==ENT_Element)
			m_xmlstream+=mT("\n")+m_tabs;
		m_xmlstream+=mT("</")+elem->getName()+mT(">");
	}
	else
	{
		m_xmlstream+=mT("/>");

		m_depth--;
		generateTabs();
	}

	//closeTag();
}
void XMLWriter::WriteNode(XMLComment* elem)
{
	core::string str=mT("<!--");
	str+=elem->GetValue();
	str+=mT("-->");
	m_xmlstream+=(str);
}
void XMLWriter::WriteNode(XMLTextNode* elem)
{
	core::string v;
	v = core::StringUtil::FindAndReplace(elem->GetValue(), "&", "&amp;");
	v = core::StringUtil::FindAndReplace(v, "\"", "&quot;");
	v = core::StringUtil::FindAndReplace(v, "'", "&apos;");
	v = core::StringUtil::FindAndReplace(v, "<", "&lt;");
	v = core::StringUtil::FindAndReplace(v, ">", "&gt;");
	m_xmlstream+=(v);
}

core::string XMLWriter::flush()
{
	core::string ret=m_xmlstream;
	m_xmlstream="";
	return ret;
}

}
}



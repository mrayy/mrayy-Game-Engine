#include "stdafx.h"

#include "XML_Font.h"
#include "GUILoggerSystem.h"
#include "TraceManager.h"
#include "StringConverter.h"
#include "FontResourceManager.h"
#include "common.h"
#include "XMLTree.h"

namespace mray{
namespace GUI{


IFont* XML_Font::loadXML(OS::IStream* stream){
	xml::XMLTree xmlTree;
	if(!xmlTree.load(stream)){
		gGUILoggerSystem.log(mT("Couldn't parse XML Font File :\"")+core::string(stream->getStreamName())+mT("\""),ELL_WARNING);
		return false;
	}

	xml::XMLElement*elem= xmlTree.getSubElement(mT("Fonts"));
	if (!elem)
		return 0;
	elem = elem->getSubElement("Font");
	IFont* f = 0;
	while (elem)
	{
		f=readFontElement(elem);
		gFontResourceManager.addFont(f);
		elem = elem->nextSiblingElement("Font");
	}
	return f;
}

IFont* XML_Font::readFontElement(xml::XMLElement*attrs){
	traceFunction(eGUI);

	IFont* ret=0;

	xml::XMLAttribute*type=attrs->getAttribute(mT("type"));
	if(!type){
		gGUILoggerSystem.log(mT("Error While Parsing Font XML File, No type for the Font"),ELL_ERROR);
	}
	IFontXMLParser*p= gFontResourceManager.getXMLParser(type->value);
	if(p){
		ret=p->parseAttributes(attrs);
	}else
	{
		//.. no supported font!
		gGUILoggerSystem.log(mT("Error While Parsing Font XML File, No support for font type: ")+type->value,ELL_ERROR);
	}
	return ret;

}
	/*
void XML_Font::onStart(){
	m_font=0;
}
void XML_Font::startElementHandler(xml::XMLElement*attrs){
	if(attrs->getName()==mT("Font"))
		readFontElement(attrs);
}
void XML_Font::endElementHandler(const core::string&elementName){
}
void XML_Font::texDataHandler(const core::string&text){
}
void XML_Font::onDone(bool success){
}
*/

}
}

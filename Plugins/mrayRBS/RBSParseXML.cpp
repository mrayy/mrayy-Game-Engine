
#include "stdafx.h"
#include "RBSParseXML.h"
#include "RuleBasedSystem.h"
#include "RBSManager.h"


#include <IStream.h>
#include <XMLTree.h>
#include <ILogManager.h>

namespace mray{
namespace AI{

RBSParseXML::RBSParseXML()
{
}
RBSParseXML::~RBSParseXML()
{
}

RuleBasedSystem* RBSParseXML::ParseXML(OS::IStream*stream)
{
	xml::XMLTree xmlTree;
	if(!xmlTree.load(stream)){
		gLogManager.log(core::string(mT("Couldn't parse XML RBS File :\""))+stream->getStreamName()+mT("\""),ELL_WARNING);
		return 0;
	}

	xml::XMLElement*e=xmlTree.getSubElement(mT("RBS"));

	if(!e){
		gLogManager.log(core::string(mT("cann't find RBS tag :\""))+stream->getStreamName()+mT("\""),ELL_WARNING);
		return 0;
	}

	return ParseRBS(e);
}
RuleBasedSystem* RBSParseXML::ParseRBS(xml::XMLElement*elem)
{
	RuleBasedSystem* rbs=new RuleBasedSystem(mT(""));
	ParseRBS(elem,rbs);
	return rbs;
}
bool RBSParseXML::ParseRBS(xml::XMLElement*elem,RuleBasedSystem*rbs)
{

	core::stringc attrName;
	core::stringc elemName;

	xml::XMLAttribute*attr;
	bool isGlobal=false;

	attr=elem->getAttribute(mT("name"));
	if(attr){
		rbs->SetName(attr->value);
	}
	attr=elem->getAttribute(mT("isglobal"));
	if(attr){
		isGlobal= core::StringConverter::toBool(attr->value);
	}


	if(isGlobal){
		RBSManager::getInstance().AddRBS(rbs);
	}

	xml::XMLElement* e;

	e=elem->getSubElement(mT("attachedrbs"));
	if(e){
		ParseAttachedRBSs(e,rbs);
	}
	e=elem->getSubElement(mT("symbols"));
	if(!e){
		gLogManager.log(mT("Couldn't find Symbols tag."),ELL_WARNING);
	}else
	{
		ParseSymbols(e,rbs);
	}
	e=elem->getSubElement(mT("rules"));
	if(!e){
		gLogManager.log(mT("Couldn't find Rules tag."),ELL_WARNING);
	}else
	{
		ParseRules(e,rbs);
	}
	return true;
}


void RBSParseXML::ParseAttachedRBSs(xml::XMLElement*elem,RuleBasedSystem*rbs)
{
	xml::XMLAttribute*attr;
	core::stringc elemName;

	xml::xmlSubElementsMapIT eit=elem->getElementsBegin();
	xml::xmlSubElementsMapIT eEnd=elem->getElementsEnd();
	for(;eit!=eEnd;++eit)
	{
		if((*eit)->GetType()!=xml::ENT_Element)continue;
		xml::XMLElement* e=dynamic_cast<xml::XMLElement*>(*eit);
		core::string_to_char(e->getName(),elemName);
		elemName.make_lower();

		if(elemName=="rbs"){
			attr=e->getAttribute(mT("name"));
			if(attr){
				RuleBasedSystem* att= RBSManager::getInstance().GetRBS(attr->value);
				if(att){
					rbs->AttachRBS(att);
				}
			}
		}
	}
}

void RBSParseXML::ParseSymbols(xml::XMLElement*elem,RuleBasedSystem*rbs)
{
	core::stringc elemName;

	xml::xmlSubElementsMapIT eit=elem->getElementsBegin();
	xml::xmlSubElementsMapIT eEnd=elem->getElementsEnd();
	for(;eit!=eEnd;++eit)
	{
		if((*eit)->GetType()!=xml::ENT_Element)continue;
		xml::XMLElement* e=dynamic_cast<xml::XMLElement*>(*eit);
		core::string_to_char(e->getName(),elemName);
		elemName.make_lower();

		if(elemName=="symbol"){
			RBSSymbol s(mT(""),false,ESV_Unset);
			if(ParseSymbol(e,s))
				rbs->AddSymbol(s);
		}
	}
}
bool RBSParseXML::ParseSymbol(xml::XMLElement*elem,RBSSymbol&symbol)
{
	xml::XMLAttribute*attr;

	attr=elem->getAttribute(mT("name"));
	if(!attr){
		return false;
	}
	symbol.name=attr->value;
	attr=elem->getAttribute(mT("value"));
	if(attr){
		symbol.value=core::StringConverter::toBool(attr->value);
	}
	attr=elem->getAttribute(mT("default"));
	if(attr){
		symbol.defaultValue=(core::StringConverter::toBool(attr->value)==true)?ESV_True:ESV_False;
	}

	return true;
}


void RBSParseXML::ParseRules(xml::XMLElement*elem,RuleBasedSystem*rbs)
{
	core::stringc elemName;

	xml::xmlSubElementsMapIT eit=elem->getElementsBegin();
	xml::xmlSubElementsMapIT eEnd=elem->getElementsEnd();
	for(;eit!=eEnd;++eit)
	{
		if((*eit)->GetType()!=xml::ENT_Element)continue;
		xml::XMLElement* e=dynamic_cast<xml::XMLElement*>(*eit);
		core::string_to_char(e->getName(),elemName);
		elemName.make_lower();

		if(elemName=="rule"){
			ParseRule(e,rbs);
		}
	}
}
void RBSParseXML::ParseRule(xml::XMLElement*elem,RuleBasedSystem*rbs)
{
	xml::XMLElement*e;
	std::vector<RBSSymbol> cond;
	std::vector<RBSSymbol> act;

	e=elem->getSubElement(mT("conditions"));
	if(!e){
		return;
	}
	ParseSymbolsToArray(e,cond);
	e=elem->getSubElement(mT("actions"));
	if(!e){
		return;
	}
	ParseSymbolsToArray(e,act);
	rbs->AddRule(cond,act);
}

void RBSParseXML::ParseSymbolsToArray(xml::XMLElement*elem,std::vector<RBSSymbol>&arr)
{
	core::stringc elemName;
	xml::xmlSubElementsMapIT eit=elem->getElementsBegin();
	xml::xmlSubElementsMapIT eEnd=elem->getElementsEnd();
	for(;eit!=eEnd;++eit)
	{
		if((*eit)->GetType()!=xml::ENT_Element)continue;
		xml::XMLElement* e=dynamic_cast<xml::XMLElement*>(*eit);
		core::string_to_char(e->getName(),elemName);
		elemName.make_lower();

		if(elemName=="symbol"){
			RBSSymbol s(mT(""),true,ESV_Unset);
			if(ParseSymbol(e,s))
				arr.push_back(s);

		}
	}
}

}
}

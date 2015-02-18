#include "stdafx.h"

#include "FuzzyXMLParser.h"
#include "DefuzzyMaxAv.h"
#include "DefuzzyCentroid.h"
#include "FuzzyLeftShoulderSet.h"
#include "FuzzyRightShoulderSet.h"
#include "FuzzySingletonSet.h"
#include "FuzzyTrapezoidalSet.h"
#include "FuzzyTriangleSet.h"

#include "FuzzySetProxy.h"

#include "FuzzyAndOpt.h"
#include "FuzzyNotOpt.h"
#include "FuzzyOrOpt.h"

#include "FuzzyFairlyHedge.h"
#include "FuzzyVeryHedge.h"

#include <IStream.h>
#include <XMLTree.h>
#include <ILogManager.h>

namespace mray{
namespace AI{

FuzzyXMLParser::FuzzyXMLParser()
{
}
FuzzyXMLParser::~FuzzyXMLParser()
{
}

FuzzyModule* FuzzyXMLParser::ParseXML(OS::IStream*stream)
{
	xml::XMLTree xmlTree;
	if(!xmlTree.load(stream)){
		gLogManager.log(core::string(mT("Couldn't parse XML Fuzzy File :\""))+stream->getStreamName()+mT("\""),ELL_WARNING);
		return 0;
	}

	xml::XMLElement*e=xmlTree.getSubElement(mT("FuzzyModule"));

	if(!e){
		gLogManager.log(core::string(mT("cann't find FuzzyModule tag :\""))+stream->getStreamName()+mT("\""),ELL_WARNING);
		return 0;
	}

	FuzzyModule* module=new FuzzyModule(0);

	ParseModuleElement(e,module);
	return module;
}

bool FuzzyXMLParser::ParseModuleElement(xml::XMLElement*element,FuzzyModule*module)
{

	core::stringc attrName;
	core::stringc elemName;

	/*
	for(;it!=end;++it){
		xml::XMLAttribute&attr= it->second;
		core::string_to_char(attr.name,attrName);
		attrName.make_lower();
	}
	*/
	xml::XMLElement* e;
	IDefuzzifyOperation*def=0;

	e=element->getSubElement(mT("defuzzier"));
	if(!e){
		gLogManager.log(mT("Couldn't find Defuzzy tag."),ELL_WARNING);
		def=new DefuzzyMaxAv();
	}else
	{
		def=ParseDefuzzyElement(e);
		if(!def){
			gLogManager.log(mT("Couldn't parse Defuzzy tag."),ELL_WARNING);
			def=new DefuzzyMaxAv();
		}
	}
	//module=new FuzzyModule(def);
	module->SetDefuzzier(def);

	e=element->getSubElement(mT("variables"));
	if(!e){
		gLogManager.log(mT("Couldn't find Variables tag."),ELL_WARNING);
	}else{
		ParseVariables(e,module);
	}

	e=element->getSubElement(mT("rules"));
	if(!e){
		gLogManager.log(mT("Couldn't find Rules tag."),ELL_WARNING);
	}else{
		ParseRules(e,module);
	}
	return true;
}


IDefuzzifyOperation* FuzzyXMLParser::ParseDefuzzyElement(xml::XMLElement*element)
{
	IDefuzzifyOperation*def=0;
	xml::XMLAttribute*attr;
	attr=element->getAttribute(mT("type"));
	if(!attr){
		return 0;
	}
	if(attr->value.equals_ignore_case(mT("centroid"))){
		attr=element->getAttribute(mT("samples"));
		int samples=16;
		if(attr){
			samples=core::StringConverter::toInt(attr->value);
		}
		def=new DefuzzyCentroid(samples);
	}else if(attr->value.equals_ignore_case(mT("maxav"))){
		def=new DefuzzyMaxAv();
	}

	return def;
}

void FuzzyXMLParser::ParseVariables(xml::XMLElement*element,FuzzyModule*module)
{
	core::stringc elemName;

	xml::xmlSubElementsMapIT eit=element->getElementsBegin();
	xml::xmlSubElementsMapIT eEnd=element->getElementsEnd();
	for(;eit!=eEnd;++eit)
	{
		if((*eit)->GetType()!=xml::ENT_Element)continue;
		xml::XMLElement* e=dynamic_cast<xml::XMLElement*>(*eit);
		core::string_to_char(e->getName(),elemName);
		elemName.make_lower();

		if(elemName=="variable"){
			FuzzyVariable* v=ParseVariable(e);
			if(v){
				module->AddVariable(v);
			}
		}

	}
}
void FuzzyXMLParser::ParseRules(xml::XMLElement*element,FuzzyModule*module)
{
	core::stringc elemName;

	xml::xmlSubElementsMapIT eit=element->getElementsBegin();
	xml::xmlSubElementsMapIT eEnd=element->getElementsEnd();
	for(;eit!=eEnd;++eit)
	{
		if((*eit)->GetType()!=xml::ENT_Element)continue;
		xml::XMLElement* e=dynamic_cast<xml::XMLElement*>(*eit);
		core::string_to_char(e->getName(),elemName);
		elemName.make_lower();

		if(elemName=="rule"){
			FuzzyRule* r=ParseRule(e,module);
			if(r){
				module->AddRule(r);
			}
		}

	}
}


FuzzyVariable* FuzzyXMLParser::ParseVariable(xml::XMLElement*element)
{
	FuzzyVariable*var=0;
	core::stringc elemName;
	core::string setName;
	xml::XMLAttribute*attr;
	attr=element->getAttribute(mT("name"));
	if(!attr){
		return 0;
	}
	var=new FuzzyVariable(attr->value);

	xml::xmlSubElementsMapIT eit=element->getElementsBegin();
	xml::xmlSubElementsMapIT eEnd=element->getElementsEnd();
	for(;eit!=eEnd;++eit)
	{
		if((*eit)->GetType()!=xml::ENT_Element)continue;
		xml::XMLElement* e=dynamic_cast<xml::XMLElement*>(*eit);
		core::string_to_char(e->getName(),elemName);
		elemName.make_lower();

		if(elemName=="set"){
			attr=e->getAttribute(mT("name"));
			if(attr)
				setName=attr->value;
			FuzzySet* r=ParseSet(setName,e);
			if(r){
				var->AddSet(setName,r);
			}
		}

	}

	return var;
}
FuzzySet* FuzzyXMLParser::ParseSet(const core::string&setName,xml::XMLElement*element)
{
	FuzzySet*set=0;
	core::stringc elemName;
	xml::XMLAttribute*attr;
	attr=element->getAttribute(mT("type"));
	if(!attr)
		return 0;
	if(attr->value.equals_ignore_case(mT("LeftShoulder")))
	{
		float min,peak,max;
		attr=element->getAttribute(mT("min"));
		if(!attr)
			return 0;
		min=core::StringConverter::toFloat(attr->value);
		attr=element->getAttribute(mT("peak"));
		if(!attr)
			return 0;
		peak=core::StringConverter::toFloat(attr->value);
		attr=element->getAttribute(mT("max"));
		if(!attr)
			return 0;
		max=core::StringConverter::toFloat(attr->value);
		set=new FuzzyLeftShoulderSet(setName,min,peak,max);
	}else if(attr->value.equals_ignore_case(mT("RightShoulder")))
	{
		float min,peak,max;
		attr=element->getAttribute(mT("min"));
		if(!attr)
			return 0;
		min=core::StringConverter::toFloat(attr->value);
		attr=element->getAttribute(mT("peak"));
		if(!attr)
			return 0;
		peak=core::StringConverter::toFloat(attr->value);
		attr=element->getAttribute(mT("max"));
		if(!attr)
			return 0;
		max=core::StringConverter::toFloat(attr->value);
		set=new FuzzyRightShoulderSet(setName,min,peak,max);
	}else if(attr->value.equals_ignore_case(mT("Singleton")))
	{
		float min,max;
		attr=element->getAttribute(mT("min"));
		if(!attr)
			return 0;
		min=core::StringConverter::toFloat(attr->value);
		attr=element->getAttribute(mT("max"));
		if(!attr)
			return 0;
		max=core::StringConverter::toFloat(attr->value);
		set=new FuzzySingletonSet(setName,min,max);
	}else if(attr->value.equals_ignore_case(mT("Trapezoidal")))
	{
		float min,lpeak,rpeak,max;
		attr=element->getAttribute(mT("min"));
		if(!attr)
			return 0;
		min=core::StringConverter::toFloat(attr->value);
		attr=element->getAttribute(mT("leftpeak"));
		if(!attr)
			return 0;
		lpeak=core::StringConverter::toFloat(attr->value);
		attr=element->getAttribute(mT("rightpeak"));
		if(!attr)
			return 0;
		rpeak=core::StringConverter::toFloat(attr->value);
		attr=element->getAttribute(mT("max"));
		if(!attr)
			return 0;
		max=core::StringConverter::toFloat(attr->value);
		set=new FuzzyTrapezoidalSet(setName,min,lpeak,rpeak,max);
	}else if(attr->value.equals_ignore_case(mT("Triangle")))
	{
		float min,peak,max;
		attr=element->getAttribute(mT("min"));
		if(!attr)
			return 0;
		min=core::StringConverter::toFloat(attr->value);
		attr=element->getAttribute(mT("peak"));
		if(!attr)
			return 0;
		peak=core::StringConverter::toFloat(attr->value);
		attr=element->getAttribute(mT("max"));
		if(!attr)
			return 0;
		max=core::StringConverter::toFloat(attr->value);
		set=new FuzzyTriangleSet(setName,min,peak,max);
	}
	return set;

}

FuzzyRule* FuzzyXMLParser::ParseRule(xml::XMLElement*element,FuzzyModule*module)
{

	FuzzyRule*rule=0;
	core::stringc elemName;
	core::string setName;
	xml::XMLElement*eAnt,*eCon;
	xml::XMLElement*eTerm;
	FuzzyTerm* antecedent=0;
	FuzzyTerm* consequence=0;
	xml::xmlSubElementsMapIT eit;
	xml::xmlSubElementsMapIT eEnd;

	eAnt=element->getSubElement(mT("antecedent"));
	eCon=element->getSubElement(mT("consequence"));
	if(!eAnt || !eCon)
		return 0;

	eTerm=eAnt->getSubElement(mT("Term"));
	if(!eTerm)
		return 0;
	FuzzyTerm*tAnt= ParseTerm(eTerm,module);
	if(!tAnt)
		return 0;

	eTerm=eCon->getSubElement(mT("Term"));
	if(!eTerm)
		return 0;
	FuzzyTerm*tCon= ParseTerm(eTerm,module);
	if(!tCon){
		delete tAnt;
		return 0;
	}

	rule=new FuzzyRule(tAnt,tCon);
	return rule;
}


FuzzyTerm* FuzzyXMLParser::ParseTerm(xml::XMLElement*element,FuzzyModule*module)
{
	FuzzyTerm*term=0;
	core::stringc elemName;
	xml::XMLElement*e;
	xml::XMLAttribute*attr;
	attr=element->getAttribute(mT("type"));
	if(!attr){
		return 0;
	}
	if(attr->value.equals_ignore_case((mT("and"))))
	{
		FuzzyAndOpt*andOp=new FuzzyAndOpt();
		term=andOp;
		xml::xmlSubElementsMapIT eit=element->getElementsBegin();
		xml::xmlSubElementsMapIT eEnd=element->getElementsEnd();
		for(;eit!=eEnd;++eit)
		{
			if((*eit)->GetType()!=xml::ENT_Element)continue;
			e=dynamic_cast<xml::XMLElement*>(*eit);

			if(e->getName().equals_ignore_case(mT("term"))){
				FuzzyTerm*t=ParseTerm(e,module);
				if(t){
					andOp->AddTerm(t);
				}
			}
		}
	}else if(attr->value.equals_ignore_case(mT("or")))
	{
		FuzzyOrOpt*andOp=new FuzzyOrOpt();
		term=andOp;
		xml::xmlSubElementsMapIT eit=element->getElementsBegin();
		xml::xmlSubElementsMapIT eEnd=element->getElementsEnd();
		for(;eit!=eEnd;++eit)
		{
			if((*eit)->GetType()!=xml::ENT_Element)continue;
			 e=dynamic_cast<xml::XMLElement*>(*eit);

			if(e->getName().equals_ignore_case(mT("term"))){
				FuzzyTerm*t=ParseTerm(e,module);
				if(t){
					andOp->AddTerm(t);
				}
			}
		}
	}else if(attr->value.equals_ignore_case(mT("not")))
	{
		FuzzyTerm*t;
		e=element->getSubElement(mT("Term"));
		if(e){
			t=ParseTerm(e,module);
			if(t)
				term=new FuzzyNotOpt(t);
		}
	}else if(attr->value.equals_ignore_case(mT("very")))
	{
		FuzzyTerm*t;
		e=element->getSubElement(mT("Term"));
		if(e){
			t=ParseTerm(e,module);
			if(t)
				term=new FuzzyVeryHedge(t);
		}
	}else if(attr->value.equals_ignore_case(mT("fairly")))
	{
		FuzzyTerm*t;
		e=element->getSubElement(mT("Term"));
		if(e){
			t=ParseTerm(e,module);
			if(t)
				term=new FuzzyFairlyHedge(t);
		}
	}else if(attr->value.equals_ignore_case(mT("set")))
	{
		FuzzyVariable*v=0;
		FuzzySet*s=0;
		attr=element->getAttribute(mT("variable"));
		if(attr){
			v=module->GetVariable(attr->value);
		}
		if(!v)
			return 0;
		attr=element->getAttribute(mT("set"));
		if(attr){
			s=v->GetSet(attr->value);
		}
		if(!s)
			return 0;
		term=new FuzzySetProxy(v->GetName(),s);
	}
	return term;
}

}
}
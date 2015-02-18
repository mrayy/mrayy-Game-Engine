
#include "stdafx.h"
#include "SegmentDesc.h"





namespace mray{

SegmentDesc::SegmentDesc()
{

}
SegmentDesc::~SegmentDesc()
{
	Clear();
}

void SegmentDesc::Serialize(xml::XMLElement*elem)
{

	if(!elem)
		return;
	xml::xmlAttributesMapIT it= elem->getAttributesBegin();
	xml::xmlAttributesMapIT end= elem->getAttributesEnd();

	core::stringc attrName;
	core::stringc elemName;

	for(;it!=end;++it){
		xml::XMLAttribute&attr= it->second;
		core::string_to_char(attr.name,attrName);
		attrName.make_lower();		
		if(attrName=="name"){
			name=attr.value;
		}else if(attrName=="trans"){
			trans=core::StringConverter::toVector3d(attr.value);
		}else if(attrName=="rot"){
			rot=core::StringConverter::toVector3d(attr.value);
		}else if(attrName=="model"){
			model=attr.value;
		}
	}

	children.clear();

	xml::xmlSubElementsMapIT eit=elem->getElementsBegin();
	xml::xmlSubElementsMapIT eEnd=elem->getElementsEnd();
	for(;eit!=eEnd;e++it)
	{
		xml::XMLElement* e=*eit;
		core::string_to_char(e->getName(),elemName);
		elemName.make_lower();

		if(elemName=="joint"){
			jointDesc.Serialize(e);
		}else if (elemName=="inertia")
		{
			inertiaDesc.Serialize(e);
		}else if (elemName=="shape")
		{
			shapeDesc.Serialize(e);
		}
		//parse children
		else if(elemName=="segment"){
			SegmentDesc *desc=new SegmentDesc();
			desc->Serialize(e);
			children.push_back(desc);
		} 
	}
}


void SegmentDesc::parseChildren(xml::XMLElement*elem){
	core::stringc elemName;

	xml::xmlSubElementsMapIT it=elem->getElementsBegin();
	xml::xmlSubElementsMapIT end=elem->getElementsEnd();
	for(;it!=end;++it){
		xml::XMLElement* e=*it;
		core::string_to_char(e->getName(),elemName);
		elemName.make_lower();
		if(elemName=="segment"){
			SegmentDesc* desc=new SegmentDesc();
			desc->Serialize(e);
			children.push_back(desc);
		}
	}
}
void SegmentDesc::Clear(){
	SegmentList::iterator it=children.begin();
	for(;it!=children.end();++it){
		delete *it;
	}
	children.clear();
}



}
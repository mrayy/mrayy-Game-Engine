#include "stdafx.h"



#include "FontAttributes.h"
#include "StringConverter.h"


namespace mray{
namespace GUI{


FontAttributes::FontAttributes(){

	fontName=mT("Default");
	fontSize=18;
	fontColor.Set(0.8,0.8,0.8,1);
	shadowColor.Set(0,0,0,1);
	hasShadow=true;
	shadowOffset=1;
	wrap=false;
	RightToLeft=false;
	fontAligment=EFA_MiddleCenter;
	spacing=0;
	lineSpacing = 0;
}
FontAttributes::~FontAttributes(){
}


void FontAttributes::loadXMLSettings(xml::XMLElement* elem){
	xml::xmlAttributesMapIT ait=elem->getAttributesBegin();
	xml::xmlAttributesMapIT aend=elem->getAttributesEnd();
	for (;ait!=aend;++ait){
		xml::XMLAttribute&a=ait->second;
		if(a.name.equals_ignore_case(mT("Font"))){
			fontName=a.value;
		}else if(a.name.equals_ignore_case(mT("Alignment"))){
			fontAligment = (EFontAligment)core::StringConverter::toEnum(mT("EFontAligment"), a.value);
		}else if(a.name.equals_ignore_case(mT("size"))){
			fontSize=core::StringConverter::toFloat(a.value);
		}else if(a.name.equals_ignore_case(mT("color"))){
			fontColor=core::StringConverter::toColor(a.value);
		}else if(a.name.equals_ignore_case(mT("shadowColor"))){
			shadowColor=core::StringConverter::toColor(a.value);
		}else if(a.name.equals_ignore_case(mT("hasShadow"))){
			hasShadow=core::StringConverter::toBool(a.value);
		}else if(a.name.equals_ignore_case(mT("shadowOffset"))){
			shadowOffset=core::StringConverter::toVector2d(a.value);
		}else if(a.name.equals_ignore_case(mT("spacing"))){
			spacing=core::StringConverter::toInt(a.value);
		}else if(a.name.equals_ignore_case(mT("wrap"))){
			wrap=core::StringConverter::toBool(a.value);
		}else if(a.name.equals_ignore_case(mT("RTL"))){
			RightToLeft=core::StringConverter::toBool(a.value);
		}
		else if (a.name.equals_ignore_case(mT("lineSpacing"))){
			lineSpacing = core::StringConverter::toInt(a.value);
		}
	}
}
xml::XMLElement*  FontAttributes::exportXMLSettings(xml::XMLElement* elem){

	xml::XMLElement* e=new xml::XMLElement(mT("Font"));
	e->addAttribute(mT("name"),fontName);
	e->addAttribute(mT("size"),core::StringConverter::toString(fontSize));
	e->addAttribute(mT("color"),core::StringConverter::toString(fontColor));
	e->addAttribute(mT("shadowColor"),core::StringConverter::toString(shadowColor));
	e->addAttribute(mT("hasShadow"),core::StringConverter::toString(hasShadow));
	e->addAttribute(mT("shadowOffset"),core::StringConverter::toString(shadowOffset));
	e->addAttribute(mT("wrap"),core::StringConverter::toString(wrap));
	e->addAttribute(mT("RTL"),core::StringConverter::toString(RightToLeft));
	e->addAttribute(mT("Alignment"),core::StringConverter::toString((int)fontAligment,core::string(mT("EFontAligment"))));
	e->addAttribute(mT("spacing"),core::StringConverter::toString(spacing));
	e->addAttribute(mT("lineSpacing"), core::StringConverter::toString(lineSpacing));
	elem->addSubElement(e);
	return e;
}

}
}
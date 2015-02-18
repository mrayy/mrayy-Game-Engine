#include "stdafx.h"

#include "XML_GUITheme.h"
#include "GUILoggerSystem.h"
#include "TraceManager.h"
#include "StringConverter.h"
#include "TextureResourceManager.h"
#include "common.h"
#include "XMLTree.h"
#include "GUIProfile.h"

namespace mray{
namespace GUI{

const core::string XML_GUITheme::ThemeElement=mT("GUITheme");
const core::string XML_GUITheme::ProfileElement=mT("Profile");
const core::string XML_GUITheme::RegionElement=mT("Region");
	 
IGUITheme* XML_GUITheme::loadXML(OS::IStream* stream){
	xml::XMLTree xmlTree;
	if(!stream)
		return 0;
	if(!xmlTree.load(stream)){
		gGUILoggerSystem.log(mT("Couldn't parse XML Theme File :\"")+core::string(stream->getStreamName())+mT("\""),ELL_WARNING);
		return false;
	}

	xml::XMLElement*elem=xmlTree.getSubElement(ThemeElement);

	if(elem)
		return readThemeElement(elem);
	return 0;
}

/*
void XML_GUITheme::startElementHandler(xml::XMLElement*attrs){
	if(attrs->getName().equals_ignore_case(ThemeElement))
		readThemeElement(attrs);
	else if(attrs->getName().equals_ignore_case(ProfileElement))
		readProfileElement(attrs);
	else if(attrs->getName().equals_ignore_case(RegionElement))
		readRegionElement(attrs);
}
void XML_GUITheme::endElementHandler(const core::string&elementName){
}
void XML_GUITheme::texDataHandler(const core::string&text){
}
void XML_GUITheme::onDone(bool success){
}
*/
IGUITheme* XML_GUITheme::readThemeElement(xml::XMLElement*attrs){
	traceFunction(eGUI);

	xml::XMLAttribute*a=attrs->getAttribute(mT("Name"));
	if(!a){
		gGUILoggerSystem.log(mT("Error While Parsing Theme XML File, No Name for the theme"),ELL_ERROR);
	}
	IGUITheme* theme=new GUITheme(a->value);
//	m_theme->setTexture(gTextureResourceManager.loadTexture2D(skin->value));

	xml::XMLElement*elem;
	GUIProfile* p;

	xml::xmlSubElementsMapIT it=attrs->getElementsBegin();
	xml::xmlSubElementsMapIT end= attrs->getElementsEnd();
	for(;it!=end;++it)
	{

		if((*it)->GetType()!=xml::ENT_Element)continue;
		elem=dynamic_cast<xml::XMLElement*>(*it);
		if(elem->getName().equals_ignore_case(ProfileElement)){
			p=readProfileElement(elem);
			if(p)
				theme->addProfile(p);
		}
	}

	return theme;
	
}
GUIProfile* XML_GUITheme::readProfileElement(xml::XMLElement*attrs){
	traceFunction(eGUI);

	xml::XMLAttribute*a=attrs->getAttribute(mT("Name"));
	if(!a){
		gGUILoggerSystem.log(mT("Error While Parsing GUIProfile XML File, No Name for the Profile"),ELL_ERROR);
	}
	GUIProfile* profile=new GUIProfile(a->value);

	xml::XMLAttribute*iset=attrs->getAttribute(mT("ImageSet"));
	if(!iset){
		gGUILoggerSystem.log(mT("Error While Parsing GUIProfile XML File, No ImageSet for the Profile"),ELL_WARNING);
	}else
	{
		profile->SetImageSetName(iset->value);
	}

	xml::xmlSubElementsMapIT it=attrs->getElementsBegin();
	xml::xmlSubElementsMapIT end= attrs->getElementsEnd();
	for(;it!=end;++it){
		if((*it)->GetType()!=xml::ENT_Element)continue;
		xml::XMLElement*elem=dynamic_cast<xml::XMLElement*>(*it);
		if(elem->getName().equals_ignore_case(RegionElement))
			readRegionElement(elem,profile);
		else if(elem->getName().equals_ignore_case(mT("RegionDiv")))
			readRegionDivElement(elem,profile);
	}
	return profile;
	
}
void XML_GUITheme::readRegionElement(xml::XMLElement*attrs,GUIProfile* p){
	traceFunction(eGUI);
	
	GUIProfile::GUIRegion r;
	xml::XMLAttribute*a;/*
	a=attrs->getAttribute(mT("Name"));
	if(a){
		r.name=a->value;
	}*/
	a=attrs->getAttribute(mT("ID"));
	if(a){
		r.id=core::StringConverter::toInt(a->value);
	}
	a=attrs->getAttribute(mT("Image"));
	if(a){
		r.targetImage=a->value;
		video::ImageRegion* ir= p->GetImageSet()->GetRegion(r.targetImage);
		if(ir)
		{
			r.texCoords=ir->GetTextureRect();
			r.sourceRect=ir->GetSrcRect();
		}

	}
	p->addRegion(r);
	
}


void XML_GUITheme::readRegionDivElement(xml::XMLElement*attrs,GUIProfile* p)
{
	GUIProfile::GUIRegion r;
	xml::XMLAttribute*a;
	int rows=0;
	int cols=0;
	a=attrs->getAttribute(mT("Rows"));
	if(a){
		rows=core::StringConverter::toInt(a->value);
	}
	a=attrs->getAttribute(mT("Cols"));
	if(a){
		cols=core::StringConverter::toInt(a->value);
	}

	core::string imageName;

	a=attrs->getAttribute(mT("Image"));
	if(a){
		imageName=a->value;
	}

	video::ImageRegion* ir= p->GetImageSet()->GetRegion(imageName);

	if(!rows || !cols || !ir)
		return;

	const math::rectf& tr= ir->GetTextureRect();
	const math::rectf& sr=ir->GetSrcRect();
	
	math::vector2d trSz=tr.getSize()/math::vector2d(cols,rows);
	math::vector2d srSz=sr.getSize()/math::vector2d(cols,rows);

	int lastID=p->getRegionCount();
	for(int i=0;i<rows;++i)
	{
		for(int j=0;j<cols;++j)
		{
			r.id=lastID++;
			r.targetImage=imageName;
			r.texCoords.ULPoint.x =tr.ULPoint.x+j*trSz.x;
			r.texCoords.ULPoint.y =tr.ULPoint.y+i*trSz.y;

			r.texCoords.BRPoint.x =tr.ULPoint.x+(j+1)*trSz.x;
			r.texCoords.BRPoint.y =tr.ULPoint.y+(i+1)*trSz.y;


			r.sourceRect.ULPoint.x =sr.ULPoint.x+j*srSz.x;
			r.sourceRect.ULPoint.y =sr.ULPoint.y+i*srSz.y;

			r.sourceRect.BRPoint.x =sr.ULPoint.x+(j+1)*srSz.x;
			r.sourceRect.BRPoint.y =sr.ULPoint.y+(i+1)*srSz.y;
			p->addRegion(r);

		}
	}

}

}
}

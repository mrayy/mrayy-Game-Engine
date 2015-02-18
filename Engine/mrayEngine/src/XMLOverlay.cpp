#include "stdafx.h"

#include "XMLOverlay.h"
#include "XMLTree.h"
#include "GUILoggerSystem.h"
#include "GUIOverlayManager.h"
#include "GUIOverlay.h"

namespace mray{
namespace GUI{

XMLOverlay::XMLOverlay(){
}
XMLOverlay::~XMLOverlay(){
}


GUIOverlay* XMLOverlay::loadOverlay(xml::XMLElement*attrs){

	GUIOverlay* o=new GUIOverlay();
	{
		xml::xmlAttributesMapIT it= attrs->getAttributesBegin();
		xml::xmlAttributesMapIT end= attrs->getAttributesEnd();
		for (;it!=end;++it)
		{
			xml::XMLAttribute&elem=it->second;
			if(elem.name.equals_ignore_case(mT("name"))){
				o->SetName(elem.value);
			}
		}
	}
	{
		xml::xmlSubElementsMapIT it= attrs->getElementsBegin();
		xml::xmlSubElementsMapIT end= attrs->getElementsEnd();
		for (;it!=end;++it)
		{
			if((*it)->GetType()!=xml::ENT_Element)continue;
			xml::XMLElement*e=dynamic_cast<xml::XMLElement*>(*it);
			o->AddElement(e);
		}
	}
	return o;
}

GUIOverlay* XMLOverlay::loadXML(OS::IStream* stream){
	xml::XMLTree xmlTree;
	if(!stream)
		return false;
	if(!xmlTree.load(stream)){
		gGUILoggerSystem.log(mT("Couldn't parse XML Overlay File :\"")+core::string(stream->getStreamName())+mT("\""),ELL_WARNING);
		return false;
	}
	m_stream=stream;
	
	GUIOverlay*o=0;
	xml::XMLElement*elem= xmlTree.getSubElement(mT("Overlay"));
	for (;elem;elem=elem->nextSiblingElement(mT("Overlay")))
	{
		o=loadOverlay(elem);
		if(o){
			GUIOverlayManager::getInstance().AddOverlay(o);
		}
	}
	return o;
}

}
}




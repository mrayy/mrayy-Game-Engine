

#include "stdafx.h"
#include "XMLSkeletonParser.h"
#include "SegmentDesc.h"


namespace mray{

XMLSkeletonParser::XMLSkeletonParser(){
}
XMLSkeletonParser::~XMLSkeletonParser(){
}

bool XMLSkeletonParser::LoadFile(const core::string &path,SegmentDesc&desc){
	xml::XMLTree xmlTree;
	GCPtr<OS::IStream> stream=gFileSystem.openFile(path,OS::TXT_READ);
	if(!xmlTree.load(stream)){
		gLogManager.log(mT("Couldn't parse XML Skeleton File :\"")+path+mT("\""),ELL_WARNING);
		return false;
	}

	xml::XMLElement*e=xmlTree.getSubElement(mT("Segment"));

	if(!e){
		gLogManager.log(mT("cann't find Segment tag :\"")+path+mT("\""),ELL_WARNING);
		return false;
	}
	desc.Serialize(e);

	return true;
}

}


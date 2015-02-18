

#include "stdafx.h"
#include "XMLPhysicalModelParser.h"
#include "SegmentDesc.h"


namespace mray{
namespace  PhysicsBasedCharacter
{

	XMLPhysicalModelParser::XMLPhysicalModelParser(){
	}
	XMLPhysicalModelParser::~XMLPhysicalModelParser(){
	}

	bool XMLPhysicalModelParser::LoadPhysicalModelFile(const core::string &path,SegmentDesc&desc){
		xml::XMLTree xmlTree;
		GCPtr<OS::IStream> stream=gFileSystem.openFile(path,OS::TXT_READ);
		if(!xmlTree.load(stream)){
			gLogManager.log(mT("Couldn't parse XML Font File :\"")+path+mT("\""),ELL_WARNING);
			return false;
		}

		xml::XMLElement*e=xmlTree.getSubElement(mT("Segment"));

		if(!e){
			gLogManager.log(mT("cann't find Segment tag :\"")+path+mT("\""),ELL_WARNING);
			return false;
		}
		desc.Serialize(e);

		stream->close();
		return true;
	}

	bool XMLPhysicalModelParser::LoadCollisionModelFile( const core::string &path,SegmentDesc&desc )
	{
		xml::XMLTree xmlTree;
		GCPtr<OS::IStream> stream=gFileSystem.openFile(path,OS::TXT_READ);
		if(!xmlTree.load(stream)){
			gLogManager.log(mT("Couldn't parse XML Font File :\"")+path+mT("\""),ELL_WARNING);
			return false;
		}

		xml::XMLElement*e=xmlTree.getSubElement(mT("Objects"));

		if(!e){
			gLogManager.log(mT("cann't find Objects tag :\"")+path+mT("\""),ELL_WARNING);
			return false;
		}

		SegmentObjectMap map = desc.getAllChildren();

		core::stringc elemName;

		xml::xmlSubElementsMapIT eit=e->getElementsBegin();
		xml::xmlSubElementsMapIT eEnd=e->getElementsEnd();
		for(;eit!=eEnd;eit++)
		{
			if((*eit)->GetType()!=xml::ENT_Element)
				continue;
			xml::XMLElement* e2=dynamic_cast<xml::XMLElement*>(*eit);
			core::string_to_char(e2->getName(),elemName);
			elemName.make_lower();

			if(elemName=="object")
			{
				//map->find(e2->getAttribute("Parent")->value).
				SegmentObjectMap::iterator myIt = map.find(e2->getAttribute(mT("Parent"))->value);
				if (myIt!=map.end())
				{
					SegmentDesc* desc = myIt->second;
					if( desc->shapeDesc.type==mT("objects"))
					{
						ObjectDesc* object = new ObjectDesc();
						object->Serialize(e2);
						desc->shapeDesc.objects.push_back(object);
					}
				}

			}
		}



		stream->close();
		return true;
	}
}
}


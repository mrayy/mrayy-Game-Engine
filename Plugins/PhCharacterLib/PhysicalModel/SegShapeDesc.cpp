#include "StdAfx.h"
#include "SegShapeDesc.h"

namespace mray
{
	namespace PhysicsBasedCharacter
	{
		void SegShapeDesc::Serialize(xml::XMLElement*elem)
		{
			if(!elem)
				return;
			xml::xmlAttributesMapIT it= elem->getAttributesBegin();
			xml::xmlAttributesMapIT end= elem->getAttributesEnd();

			core::stringc attrName;
			core::stringc elemName;

			for(;it!=end;++it)
			{
				xml::XMLAttribute&attr= it->second;
				core::string_to_char(attr.name,attrName);
				attrName.make_lower();
	
				if(attrName == "type"){
					type = attr.value;
					type.make_lower();
				}else if(attrName=="model"){
					model = attr.value;
					model.make_lower();
				}else if(attrName=="attach"){
					attach = attr.value;
					attach.make_lower();
				}else if(attrName=="length"){
					length = 0.01 * core::StringConverter::toFloat(attr.value);
				}else if(attrName=="radius"){
					radius = 0.01 * core::StringConverter::toFloat(attr.value);
				}else if(attrName=="extent"){
					extent = core::StringConverter::toVector3d(attr.value);
					extent.x = extent.x * 0.01;
					extent.y = extent.y * 0.01;
					extent.z = extent.z * 0.01;
				}else if(attrName=="trans"){
					trans = core::StringConverter::toVector3d(attr.value);
					trans.x = trans.x * 0.01;
					trans.y = trans.y * 0.01;
					trans.z = trans.z * 0.01;
				}	
			}

			xml::xmlSubElementsMapIT eit=elem->getElementsBegin();
			xml::xmlSubElementsMapIT eEnd=elem->getElementsEnd();
			ObjectDesc* objDesc =0;
			for(;eit!=eEnd;++eit)
			{
				if((*eit)->GetType()!=xml::ENT_Element)
					continue;
				xml::XMLElement* e=dynamic_cast<xml::XMLElement*>(*eit);
				core::string_to_char(e->getName(),elemName);
				elemName.make_lower();

				if(elemName=="object")
				{
					objDesc = new ObjectDesc();
					objDesc->Serialize(e);
					objects.push_back(objDesc);
				}					
			}

		}
		SegShapeDesc::~SegShapeDesc()
		{
			for(std::list<ObjectDesc*>::iterator it=objects.begin();it!=objects.end();++it)
			{
				delete *it;
			}
			objects.clear();
		}
	}

}
#include "stdafx.h"
#include "ObjectDesc.h"

namespace mray
{
	namespace PhysicsBasedCharacter
	{
		ObjectDesc::ObjectDesc()
		{
			width = 0;
			height = 0;
			depth = 0;
		}
		ObjectDesc::~ObjectDesc()
		{
		}

		void ObjectDesc::Serialize( xml::XMLElement*elem )
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

				if(attrName == "type")
				{
					type = attr.value;
					type.make_lower();
				}
				else if(attrName=="height")
				{
					height = /*0.01 **/ core::StringConverter::toFloat(attr.value);
				}
				else if(attrName=="radius")
				{
					radius = /*0.01 **/ core::StringConverter::toFloat(attr.value);
				}
				else if(attrName=="width")
				{
					width = /*0.01 **/ core::StringConverter::toFloat(attr.value);
				}
				else if(attrName=="depth")
				{
					depth = /*0.01 **/ core::StringConverter::toFloat(attr.value);
				}
				else if(attrName=="pos")
				{
					translation = core::StringConverter::toVector3d(attr.value);
					/*
					translation.x = translation.x * 0.01;
					translation.y = translation.y * 0.01;
					translation.z = translation.z * 0.01;*/
					
				}
				else if(attrName=="rot")
				{
					rotation = core::StringConverter::toVector3d(attr.value);
				}
			}
		}
	}
}
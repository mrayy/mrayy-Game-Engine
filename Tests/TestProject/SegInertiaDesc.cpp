#include "StdAfx.h"
#include "SegInertiaDesc.h"

namespace mray
{
	void SegInertiaDesc::Serialize(xml::XMLElement*elem)
	{
		if(!elem)
			return;
		xml::xmlAttributesMapIT it= elem->getAttributesBegin();
		xml::xmlAttributesMapIT end= elem->getAttributesEnd();

		core::stringc name;

		for(;it!=end;++it){
			xml::XMLAttribute&attr= it->second;
			core::string_to_char(attr.name,name);
			name.make_lower();

			if(name=="density"){
				density=core::StringConverter::toFloat(attr.value);
			}else if(name=="mass"){
				mass=core::StringConverter::toFloat(attr.value);
			}else if(name=="moments"){
				moments=core::StringConverter::toVector3d(attr.value);
			}else if(name=="trans"){
				trans=core::StringConverter::toVector3d(attr.value);
			}else if(name=="rot"){
				rot=core::StringConverter::toVector3d(attr.value);
			}

		}
	}

}
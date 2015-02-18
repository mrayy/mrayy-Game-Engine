#include "StdAfx.h"
#include "SegShapeDesc.h"

namespace mray
{
	void SegShapeDesc::Serialize(xml::XMLElement*elem)
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

			if(name=="length"){
				length=core::StringConverter::toFloat(attr.value);
			}else if(name=="radius"){
				radius=core::StringConverter::toFloat(attr.value);
			}

	}
}

}
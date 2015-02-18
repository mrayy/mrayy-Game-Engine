#include "StdAfx.h"
#include "SegInertiaDesc.h"

namespace mray
{
	namespace PhysicsBasedCharacter
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
					density = core::StringConverter::toFloat(attr.value);
				}else if(name=="mass"){
					mass = core::StringConverter::toFloat(attr.value);
				}else if(name=="moments"){
					moments = core::StringConverter::toVector3d(attr.value);
					moments.x = moments.x * 0.0001;
					moments.y = moments.y * 0.0001;
					moments.z = moments.z * 0.0001;
				}else if(name=="trans"){
					trans = core::StringConverter::toVector3d(attr.value);
					trans.x = trans.x * 0.01;
					trans.y = trans.y * 0.01;
					trans.z = trans.z * 0.01;
				}else if(name=="rot"){
					rot=core::StringConverter::toVector3d(attr.value);
				}else if(name=="shape"){
					shape=core::StringConverter::toBool(attr.value);
				}
	
			}
		}
	}

}
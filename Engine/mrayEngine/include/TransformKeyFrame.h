

#ifndef ___TransformKeyFrame___
#define ___TransformKeyFrame___

#include "IKeyFrame.h"


#include "compileConfig.h"

namespace mray{
namespace animation{
	

class MRAY_DLL TransformKeyFrame:public IKeyFrame
{
	math::vector3d m_pos;
	math::vector3d m_scale;
	math::quaternion m_orintation;
public:
	TransformKeyFrame();
	TransformKeyFrame(float time,const math::vector3d& pos,const math::vector3d& scale,const math::quaternion& orintation);

	const math::vector3d& getPos()const;
	void setPos(const math::vector3d& v);
	
	const math::vector3d& getScale()const;
	void setScale(const math::vector3d& v);
	
	const math::quaternion& getOrintation()const;
	void setOrintation(const math::quaternion& v);

	
	virtual IKeyFrame*duplicate()const;

	virtual xml::XMLElement* loadXMLSettings(xml::XMLElement* elem){
		xml::XMLElement* e=IKeyFrame::loadXMLSettings(elem);
		if(!e)
			return 0;

		xml::XMLAttribute*attr=e->getAttribute(mT("Pos"));
		if(attr)
			core::StringConverter::parse(attr->value,m_pos); 
		attr=e->getAttribute(mT("Scale"));
		if(attr)
			core::StringConverter::parse(attr->value,m_scale); 
		attr=e->getAttribute(mT("Orintation"));
		if(attr)
			core::StringConverter::parse(attr->value,m_orintation); 
		return e;
	}
	virtual xml::XMLElement* exportXMLSettings(xml::XMLElement* elem){
		xml::XMLElement* e=IKeyFrame::exportXMLSettings(elem);
		e->addAttribute(mT("Pos"),core::StringConverter::toString(m_pos));
		e->addAttribute(mT("Scale"),core::StringConverter::toString(m_scale));
		e->addAttribute(mT("Orintation"),core::StringConverter::toString(m_orintation));
		return e;
	}
};

}
}




#endif


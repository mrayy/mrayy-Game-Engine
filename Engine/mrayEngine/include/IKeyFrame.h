

#ifndef ___IKeyFrame___
#define ___IKeyFrame___


#include "XMLElement.h"
#include "StringConverter.h"

namespace mray{
namespace animation{

class IKeyFrame
{
	float m_time;
public:
	IKeyFrame():m_time(0)
	{}
	IKeyFrame(float t):m_time(t)
	{}
	virtual~IKeyFrame()
	{}

	float getTime()const{
		return m_time;
	}
	void setTime(float t){
		m_time=t;
	}

	virtual IKeyFrame*duplicate()const=0;

	virtual xml::XMLElement* loadXMLSettings(xml::XMLElement* elem){
		//xml::XMLElement*elem=e->getSubElement(mT("KeyFrame"));
		//if(!elem)return 0;

		xml::XMLAttribute*attr=elem->getAttribute(mT("Time"));
		if(attr)
			m_time=core::StringConverter::toFloat(attr->value);
		return elem;
	}
	virtual xml::XMLElement*  exportXMLSettings(xml::XMLElement* elem){
		xml::XMLElement* e=new xml::XMLElement(mT("KeyFrame"));
		e->addAttribute(mT("Time"),core::StringConverter::toString(m_time));
		elem->addSubElement(e);
		return e;
	}
};

}
}



#endif




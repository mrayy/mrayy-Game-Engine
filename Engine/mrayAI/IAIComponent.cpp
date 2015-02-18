#include "stdafx.h"


#include "IAIComponent.h"


namespace mray
{
namespace AI
{
	
	//////////////////////////////////////////////////////////////////////////

	IAIComponent::PropertyTypeType IAIComponent::PropertyTypeType::instance;

	IAIComponent::PropertyTypeType::PropertyTypeType():IProperty(mT("Type"),EPBT_Basic,GUID("string"),mT("Agent's Name")),m_default(mT(""))
	{
		m_access=EPA_READ;
	}

	core::string IAIComponent::PropertyTypeType::GetValue(CPropertieSet*object)const
	{
		IAIComponent* o=dynamic_cast<IAIComponent*>(object);
		if(!o)return m_default;
		return o->getObjectType();
	}
	bool IAIComponent::PropertyTypeType::SetValue(CPropertieSet*object,const core::string& v)
	{
		return false;
	}
	core::string IAIComponent::PropertyTypeType::toString(CPropertieSet*object)const
	{
		return GetValue(object);
	}
	bool IAIComponent::PropertyTypeType::parse(CPropertieSet*reciver,const core::string&str)
	{
		return false;
	}
	bool IAIComponent::PropertyTypeType::isDefault(CPropertieSet*reciver)const
	{
		if(GetValue(reciver)==GetDefaultValue())
			return true;
		return false;
	}

IAIComponent::IAIComponent()
:m_componentTriggerPeriod(0),m_componentUpdateTime(0),m_enableVisualize(true)
{
	CPropertieDictionary* dic=0;
	if(CreateDictionary(&dic))
		dic->addPropertie(&PropertyTypeType::instance);
}
IAIComponent::~IAIComponent()
{
}

void IAIComponent::EnableVisualize(bool e)
{
	m_enableVisualize=e;
}
bool IAIComponent::IsEnableVisualize()
{
	return m_enableVisualize;
}

void IAIComponent::UpdateComponent(float dt)
{
	m_componentUpdateTime+=dt;
	if(m_componentUpdateTime>=m_componentTriggerPeriod){
		m_componentUpdateTime-=m_componentTriggerPeriod;
		Update(dt);
	}
}
void IAIComponent::VisualizeComponent(IRenderDevice*dev)
{
	if(m_enableVisualize)
		Visualize(dev);
}

xml::XMLElement* IAIComponent::loadXMLSettings(xml::XMLElement* elem)
{
	IObject::loadFromXML(elem);
	xml::XMLAttribute*attr= elem->getAttribute(mT("UpdateTrigger"));
	if(attr){
		m_componentTriggerPeriod=core::StringConverter::toFloat(attr->value);
	}
	attr= elem->getAttribute(mT("EnableVisualize"));
	if(attr){
		m_enableVisualize=core::StringConverter::toBool(attr->value);
	}

	return elem;
	
}

}
}

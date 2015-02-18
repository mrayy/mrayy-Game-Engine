

/********************************************************************
	created:	2010/07/01
	created:	1:7:2010   14:41
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\PropertyValue.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	PropertyValue
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef PropertyValue_h__
#define PropertyValue_h__

#include "IValue.h"
#include "CPropertie.h"

namespace mray
{

template <typename Container,class T>
class PropertyValue:public IValue
{
private:
protected:
	rwProperty<Container,T>* m_property;
	EValueType m_type;
public:
	PropertyValue(rwProperty<Container,T>*p,EValueType type):IValue(p->getName()),m_property(p),m_type(type)
	{
	}
	virtual ~PropertyValue()
	{
	}

	virtual EValueType getType(){
		return m_type;
	}

	virtual const mchar* toString()const
	{
		return m_property->toString().c_str();
	}
	virtual void parse(const core::string&v)
	{
		return m_property->parse(v);
	}
	virtual IValue* duplicate()
	{
		return new PropertyValue<Container,T>(m_property,m_type);
	}

	virtual void loadXMLSettings(xml::XMLElement* elem)
	{
	}
	virtual xml::XMLElement*  exportXMLSettings(xml::XMLElement* elem)
	{
		return 0;
	}

};

}
#endif // PropertyValue_h__

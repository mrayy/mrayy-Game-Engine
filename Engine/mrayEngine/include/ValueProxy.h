

/********************************************************************
	created:	2010/07/01
	created:	1:7:2010   14:35
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\ValueProxy.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine
	file base:	ValueProxy
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ValueProxy_h__
#define ValueProxy_h__

#include "IValue.h"

namespace mray
{

class ValueProxy:public IValue
{
private:
protected:
	IValue* m_value;
public:
	ValueProxy(IValue*v):IValue(v->getName()),m_value(v)
	{}
	virtual~ValueProxy(){}

	virtual EValueType getType(){return m_value->getType();}

	virtual const mchar* toString()const{return m_value->toString();}
	virtual void parse(const core::string&v){m_value->parse(v);}
	virtual IValue* duplicate()
	{
		return m_value->duplicate();
	}

	virtual void loadXMLSettings(xml::XMLElement* elem)
	{
		m_value->loadXMLSettings(elem);
	}
	virtual xml::XMLElement*  exportXMLSettings(xml::XMLElement* elem)
	{
		return m_value->exportXMLSettings(elem);
	}

};

}
#endif // ValueProxy_h__

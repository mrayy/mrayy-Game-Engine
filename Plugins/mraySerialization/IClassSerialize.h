


/********************************************************************
	created:	2011/09/19
	created:	19:9:2011   15:43
	filename: 	d:\Development\mrayEngine\Engine\mraySerialization\IClassSerialize.h
	file path:	d:\Development\mrayEngine\Engine\mraySerialization
	file base:	IClassSerialize
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __IClassSerialize__
#define __IClassSerialize__


namespace mray
{
namespace serialize
{

class IClassSerialize
{
private:
protected:
	core::string m_className;
public:
	IClassSerialize(const core::string&className):m_className(className)
	{}
	virtual~IClassSerialize(){}

	const core::string& GetName()const{return m_className;}

	virtual void Serialize(void*obj,xml::XMLElement*elem)=0;
	virtual bool Deserialize(void*result,xml::XMLElement* elem)=0;
	

};


}
}

#endif

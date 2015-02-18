
/********************************************************************
	created:	2011/09/14
	created:	14:9:2011   15:02
	filename: 	d:\Development\mrayEngine\Engine\mrayShaderGenerator\SGIValue.h
	file path:	d:\Development\mrayEngine\Engine\mrayShaderGenerator
	file base:	SGIValue
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __SGIValue__
#define __SGIValue__

#include "SGCommon.h"

namespace mray
{
namespace shader
{

	class SGIVisitor;

class SGIValue
{
private:
protected:
	video::EUniformType m_type;
public:
	SGIValue(video::EUniformType type=video::EUT_Unkown):m_type(type)
	{}
	virtual~SGIValue(){}

	virtual bool IsConst()const=0;

	video::EUniformType GetType()const {return m_type;}

	virtual void Visit(SGIVisitor*visitor)=0;
	virtual bool Load(xml::XMLElement* elem)=0;
	virtual xml::XMLElement* Save(xml::XMLElement* elem)=0;

	static SGIValue* CreateValue(const core::string&type);
};

MakeSharedPtrType(SGIValue);

}
}

#endif
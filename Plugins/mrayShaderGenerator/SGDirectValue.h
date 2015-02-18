

/********************************************************************
	created:	2011/09/17
	created:	17:9:2011   17:00
	filename: 	d:\Development\mrayEngine\Engine\mrayShaderGenerator\SGDirectValue.h
	file path:	d:\Development\mrayEngine\Engine\mrayShaderGenerator
	file base:	SGDirectValue
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __SGDirectValue__
#define __SGDirectValue__

#include "SGIValue.h"

namespace mray
{
namespace shader
{

class SGDirectValue:public SGIValue
{
private:
protected:
	core::string m_value;
public:
	SGDirectValue(video::EUniformType type,core::string v);
	virtual~SGDirectValue();

	virtual bool IsConst()const{return true;}
	const core::string& GetValue()const{return m_value;}

	virtual void Visit(SGIVisitor*visitor);
	bool Load(xml::XMLElement* elem);
	xml::XMLElement* Save(xml::XMLElement* elem);
};


}
}

#endif

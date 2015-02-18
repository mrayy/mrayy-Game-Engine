
/********************************************************************
	created:	2011/09/14
	created:	14:9:2011   15:03
	filename: 	d:\Development\mrayEngine\Engine\mrayShaderGenerator\SGValue.h
	file path:	d:\Development\mrayEngine\Engine\mrayShaderGenerator
	file base:	SGValue
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __SGValue__
#define __SGValue__

#include "SGIValue.h"

#include <vector>

namespace mray
{
namespace shader
{

//Direct Value
class SGValue:public SGIValue
{
private:
protected:
	std::vector<SGIValuePtr> m_values;
public:
	SGValue(video::EUniformType type=video::EUT_Unkown);
	virtual~SGValue();

	//ushort GetIndex()const{return m_index;}

	bool IsConst()const{return true;}

	//const core::string& GetName()const{return m_name;}

	void SetValues(const std::vector<SGIValuePtr>&vals);
	const std::vector<SGIValuePtr>& GetValues()const;

	void Visit(SGIVisitor*visitor);
	virtual bool Load(xml::XMLElement* elem);
	virtual xml::XMLElement* Save(xml::XMLElement* elem);

};

MakeSharedPtrType(SGValue);

}
}

#endif
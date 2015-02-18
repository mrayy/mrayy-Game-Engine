
/********************************************************************
	created:	2011/09/14
	created:	14:9:2011   15:03
	filename: 	d:\Development\mrayEngine\Engine\mrayShaderGenerator\SGFunctionCall.h
	file path:	d:\Development\mrayEngine\Engine\mrayShaderGenerator
	file base:	SGFunctionCall
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __SGFunctionCall__
#define __SGFunctionCall__


#include "SGValueStatement.h"

namespace mray
{
namespace shader
{


class SGFunctionCall:public SGValueStatement
{
private:
protected:
	core::string m_functionCall;
public:
	SGFunctionCall(const core::string& function,video::EUniformType type=video::EUT_Unkown);
	~SGFunctionCall();

	const core::string& GetFunctionName(){return m_functionCall;}

	void Visit(SGIVisitor*visitor);

	bool Load(xml::XMLElement* elem);
	xml::XMLElement* Save(xml::XMLElement* elem);
};
MakeSharedPtrType(SGStatement);

}
}

#endif
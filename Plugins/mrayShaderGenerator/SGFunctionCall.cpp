
#include "stdafx.h"
#include "SGFunctionCall.h"
#include "SGIVisitor.h"


namespace mray
{
namespace shader
{



SGFunctionCall::SGFunctionCall(const core::string& function,video::EUniformType type)
:SGValueStatement(type), m_functionCall(function)
{

}
SGFunctionCall::~SGFunctionCall()
{
}

void SGFunctionCall::Visit(SGIVisitor*visitor)
{
	visitor->OnVisit(this);
}

bool SGFunctionCall::Load(xml::XMLElement* elem)
{
	xml::XMLAttribute*attr;
	SGValueStatement::Load(elem);

	attr=elem->getAttribute(mT("Name"));
	if(attr)
	{
		m_functionCall=attr->value;
	}
	return true;
}
xml::XMLElement* SGFunctionCall::Save(xml::XMLElement* elem)
{
	xml::XMLElement* e=new xml::XMLElement(mT("Call"));
	e->addAttribute(mT("Name"),m_functionCall);
	SGValueStatement::Save(e);
	if(elem)
		elem->addSubElement(e);
	return e;
}

}
}

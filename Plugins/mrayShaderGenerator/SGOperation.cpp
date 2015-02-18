
#include "stdafx.h"
#include "SGOperation.h"
#include "SGIVisitor.h"


namespace mray
{
namespace shader
{

	const core::string s_opTypeStr[]=
	{
		mT("Add"),
		mT("Sub"),
		mT("Mult"),
		mT("Div"),
		mT("Assign")
	};
SGOperation::SGOperation(EOperationType op,video::EUniformType resType):SGValueStatement(resType),m_opType(op)
{
}
//binary operation
SGOperation::SGOperation(EOperationType op,SGIValueCRef v1,SGIValueCRef v2)
:SGValueStatement(v1->GetType()),m_opType(op)
{
	AddInputValue(v1);
	AddInputValue(v2);
}
//assign operation
SGOperation::SGOperation(SGIValueCRef o,SGIValueCRef v)
:SGValueStatement(o->GetType()),m_opType(EOT_ASSIGN)
{
	AddInputValue(v);
	AddOutputValue(o);
}
SGOperation::~SGOperation()
{
}

void SGOperation::Visit(SGIVisitor*visitor)
{
	visitor->OnVisit(this);
}

bool SGOperation::Load(xml::XMLElement* elem)
{
	SGValueStatement::Load(elem);
	xml::XMLAttribute*attr=elem->getAttribute(mT("OpType"));
	if(attr)
	{
		for(int i=0;i<EOperationType::EOT_COUNT;++i)
		{
			if(attr->value.equals_ignore_case(s_opTypeStr[i]))
			{
				m_opType=(EOperationType)i;
				break;
			}
		}
	}
	return true;
}
xml::XMLElement* SGOperation::Save(xml::XMLElement* elem)
{
	xml::XMLElement* e=new xml::XMLElement(mT("Operation"));
	SGValueStatement::Save(e);
	e->addAttribute(mT("ValueType"),mT("Operation"));
	e->addAttribute(mT("OpType"),s_opTypeStr[m_opType]);
	elem->addSubElement(e);
	return e;
}

}
}

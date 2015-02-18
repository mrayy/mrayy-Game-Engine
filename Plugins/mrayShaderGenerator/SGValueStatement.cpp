
#include "stdafx.h"
#include "SGValueStatement.h"



namespace mray
{
namespace shader
{

SGValueStatement::SGValueStatement(video::EUniformType type):SGIValue(type)
{}
SGValueStatement::~SGValueStatement()
{}


void SGValueStatement::AddInputValue(SGIValueCRef val)
{
	m_inputValues.push_back(val);
}
bool SGValueStatement::AddOutputValue(SGVariablePtr val)
{
	if(val->IsConst())
	{
		core::string msg=mT("SGIStatement::AddOutputValue(): Cann't Add constant output value:\'")+val->GetName()+mT("\'");
		gVideoLoggerSystem.log(msg,ELL_WARNING);
		return false;
	}
	m_outputValues.push_back(val);
	return true;
}


const std::vector<SGIValuePtr>& SGValueStatement::GetInputValues()const
{
	return m_inputValues;
}
const std::vector<SGVariablePtr>& SGValueStatement::GetOutputValues()const
{
	return m_outputValues;
}

bool SGValueStatement::Load(xml::XMLElement* elem)
{
	xml::XMLElement* node;
	xml::XMLAttribute* attr;
	node=elem->getSubElement(mT("Input"));
	while(node)
	{
		bool loaded=false;
		attr=node->getAttribute(mT("ValueType"));
		if(attr)
		{
			SGIValue*val=SGIValue::CreateValue(attr->value);
			if(val)
			{
				loaded=true;
				val->Load(node);
				m_inputValues.push_back(val);
			}
		}
		if(!loaded)
		{
			//check if it is statement
			xml::xmlSubElementsMapIT it= node->getElementsBegin();
			for(;it!=node->getElementsEnd();++it)
			{
				xml::XMLElement* e=dynamic_cast<xml::XMLElement*>(*it);
				if(e)
				{
					SGIStatement* st=SGIStatement::CreateStatement(e->getName());
					if(st)
					{
						SGValueStatement*vst=dynamic_cast<SGValueStatement*>(st);
						if(!vst)
						{
							delete st;
							continue;
						}
						vst->Load(e);
						m_inputValues.push_back(vst);
						break;
					}
				}
			}
		}
		node=node->nextSiblingElement(mT("Input"));
	}
	node=elem->getSubElement(mT("Output"));
	while(node)
	{
		SGVariable*val=new SGVariable(mT(""),video::EUT_Unkown);
		if(val->Load(node))
		{
			m_outputValues.push_back(val);
		}else
			delete val;
		node=node->nextSiblingElement(mT("Output"));
	}
	return true;
}
xml::XMLElement* SGValueStatement::Save(xml::XMLElement* elem)
{
	for(int i=0;i<m_inputValues.size();++i)
	{
		xml::XMLElement* e=new xml::XMLElement(mT("Input"));
		m_inputValues[i]->Save(e);
		elem->addSubElement(e);
	}
	for(int i=0;i<m_outputValues.size();++i)
	{
		xml::XMLElement* e=new xml::XMLElement(mT("Output"));
		m_outputValues[i]->Save(e);
		elem->addSubElement(e);
	}
	return elem;
}


}
}


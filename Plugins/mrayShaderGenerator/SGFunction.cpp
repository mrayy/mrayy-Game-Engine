
#include "stdafx.h"
#include "SGFunction.h"
#include "SGIVisitor.h"
#include "SGFunctionPrototype.h"
#include "SGFunctionParameterProtoype.h"



namespace mray
{
namespace shader
{

SGFunction::SGFunction(const core::string& name,EFunctionType type,const core::string& desc)
:m_name(name),m_type(type),m_desc(desc)
{
}

SGFunction::~SGFunction()
{/*
	for(int i=0;i<m_inputVars.size();++i)
	{
		delete m_inputVars[i];
	}
	for(int i=0;i<m_outputVars.size();++i)
	{
		delete m_outputVars[i];
	}*/
	m_inputVars.clear();
	m_outputVars.clear();
}

void SGFunction::AddVariable(SGVariableCRef val)
{
	m_variables.push_back(val);
}

SGVariableCRef SGFunction::AddInputVariable(const core::string& name,video::EUniformType type,bool uniform,EUniformSemantic semantic,ushort index)
{
	if(GetInputVariableByName(name))
	{
		gVideoLoggerSystem.log(mT("SGFunction::AddInputVariable(): Variable with name:\'")+name+mT("\' already exists in function:\'")+m_name+mT("\'"),
			ELL_WARNING);
		return SGVariablePtr::Null;
	}
	SGVariablePtr p;

	p=new SGVariable(name,type,uniform?EVU_UNIFORM:EVU_INPUT,semantic,index);
	m_inputVars.push_back(p);
	return m_inputVars[m_inputVars.size()-1];
}

SGVariableCRef SGFunction::AddOutputVariable(const core::string& name,video::EUniformType type,EUniformSemantic semantic,ushort index)
{
	if(GetOutputVariableByName(name))
	{
		gVideoLoggerSystem.log(mT("SGFunction::AddOutputVariable(): Variable with name:\'")+name+mT("\' already exists in function:\'")+m_name+mT("\'"),
			ELL_WARNING);
		return SGVariablePtr::Null;
	}
	SGVariablePtr p;
	p=new SGVariable(name,type,EVU_OUTPUT,semantic,index);
	m_outputVars.push_back(p);
	return m_inputVars[m_inputVars.size()-1];
}

void SGFunction::AddStatement(SGIStatementCRef st)
{
	m_statements.push_back(st);
}

SGVariableCRef SGFunction::GetInputVariableByName(const core::string& name)const 
{
	for(int i=0;i<m_inputVars.size();++i)
	{
		if(m_inputVars[i]->GetName()==name)
			return m_inputVars[i];
	}
	return SGVariablePtr::Null;
}

SGVariableCRef SGFunction::GetOutputVariableByName(const core::string& name)const 
{
	for(int i=0;i<m_outputVars.size();++i)
	{
		if(m_outputVars[i]->GetName()==name)
			return m_outputVars[i];
	}
	return SGVariablePtr::Null;
}
SGVariableCRef SGFunction::GetInputVariableBySemantic(EUniformSemantic sem,ushort index)const 
{
	for(int i=0;i<m_inputVars.size();++i)
	{
		if(m_inputVars[i]->GetSemantic()==sem &&
			 m_inputVars[i]->GetIndex()==index)
			return m_inputVars[i];
	}
	return SGVariablePtr::Null;
}

SGVariableCRef SGFunction::GetOutputVariableBySemantic(EUniformSemantic sem,ushort index)const 
{
	for(int i=0;i<m_outputVars.size();++i)
	{
		if(m_outputVars[i]->GetSemantic()==sem&&
			m_outputVars[i]->GetIndex()==index)
			return m_outputVars[i];
	}
	return SGVariablePtr::Null;
}

SGFunctionPrototype* SGFunction::CreatePrototype(SGShaderLibrary* lib)
{
	SGFunctionPrototype::ParamsList inParams,outParams;

	SGFunctionPrototype* ret=new SGFunctionPrototype(lib);
	ret->SetName(m_name);
	ret->SetDescription(m_desc);

	if(!m_returnValue.isNull())
	{
		ret->SetReturnType(m_returnValue->GetType());
	}

	for(int i=0;i<m_inputVars.size();++i)
	{
		SGFunctionParameterProtoype* p=new SGFunctionParameterProtoype(EVU_INPUT);
		p->SetName(m_inputVars[i]->GetName());
		p->SetDescription(m_inputVars[i]->GetDescription());
		p->SetType(m_inputVars[i]->GetType());
		inParams.push_back(p);
	}
	for(int i=0;i<m_outputVars.size();++i)
	{
		SGFunctionParameterProtoype* p=new SGFunctionParameterProtoype(EVU_OUTPUT);
		p->SetName(m_outputVars[i]->GetName());
		p->SetDescription(m_outputVars[i]->GetDescription());
		p->SetType(m_outputVars[i]->GetType());
		outParams.push_back(p);
	}
	ret->SetInputParams(inParams);
	ret->SetOutputParams(outParams);
	return ret;
}

void SGFunction::Visit(SGIVisitor*visitor)
{
	visitor->OnVisit(this);
}
bool SGFunction::Load(xml::XMLElement* elem)
{
	xml::XMLAttribute*attr;
	xml::XMLElement* node;

	attr=elem->getAttribute(mT("Name"));
	if(attr)
	{
		m_name=attr->value;
	}

	if(GetDescription()!=mT(""))
	{
		xml::XMLElement* desc=new xml::XMLElement(mT("Description"));
		desc->addSubElement(new xml::XMLTextNode(GetDescription()));
		elem->addSubElement(desc);
	}
	node=elem->getSubElement(mT("Input"));
	while(node)
	{
		SGVariable*val=new SGVariable(mT(""),video::EUT_Unkown);
		if(val->Load(node))
		{
			m_inputVars.push_back(val);
		}else
			delete val;

		node=node->nextSiblingElement(mT("Input"));
	}
	node=elem->getSubElement(mT("Output"));
	while(node)
	{
		SGVariable*val=new SGVariable(mT(""),video::EUT_Unkown);
		if(val->Load(node))
		{
			m_outputVars.push_back(val);
		}else
			delete val;

		node=node->nextSiblingElement(mT("Output"));
	}
	node=elem->getSubElement(mT("ReturnValue"));
	if(node)
	{
		attr=node->getAttribute(mT("ValueType"));
		if(attr)
		{
			SGIValue*val=SGIValue::CreateValue(attr->value);
			if(val)
			{
				val->Load(node);
				m_returnValue=val;
			}
		}
	}

	xml::XMLElement*bodyNode=elem->getSubElement(mT("Body"));
	if(bodyNode)
	{
		node=bodyNode->getSubElement(mT("Variable"));
		while(node)
		{
			SGVariable* val=new SGVariable(mT(""),video::EUT_Unkown);
			if(val->Load(node))
			{
				m_variables.push_back(val);
			}else
				delete val;
			node=bodyNode->nextSiblingElement(mT("Variable"));
		}

		xml::XMLElement*sts=bodyNode->getSubElement(mT("Statements"));
		if(sts)
		{
			xml::xmlSubElementsMapIT it= sts->getElementsBegin();
			for(;it!=sts->getElementsEnd();++it)
			{
				xml::XMLElement* e=dynamic_cast<xml::XMLElement*>(*it);
				if(!e)
					continue;
				SGIStatement* st=SGIStatement::CreateStatement(e->getName());
				if(st)
				{
					if(st->Load(e))
					{
						m_statements.push_back(st);
					}else
						delete st;
				}
			}
		}
	}
	return true;
}

xml::XMLElement* SGFunction::Save(xml::XMLElement* parent)
{
	xml::XMLElement* elem=new xml::XMLElement(mT("Function"));

	elem->addAttribute(mT("Name"),m_name);

	if(GetDescription()!=mT(""))
	{
		xml::XMLElement* desc=new xml::XMLElement(mT("Description"));
		desc->addSubElement(new xml::XMLTextNode(GetDescription()));
		elem->addSubElement(desc);
	}
	if(!m_returnValue.isNull())
	{
		xml::XMLElement* retVal=new xml::XMLElement(mT("ReturnValue"));
		m_returnValue->Save(retVal);
		elem->addSubElement(retVal);
	}


	for(int i=0;i<m_inputVars.size();++i)
	{
		xml::XMLElement* var=new xml::XMLElement(mT("Input"));
		m_inputVars[i]->Save(var);
		elem->addSubElement(var);
	}
	for(int i=0;i<m_outputVars.size();++i)
	{
		xml::XMLElement* var=new xml::XMLElement(mT("Output"));
		m_outputVars[i]->Save(var);
		elem->addSubElement(var);
	}
	xml::XMLElement *bodyElem=new xml::XMLElement(mT("Body"));
	elem->addSubElement(bodyElem);
	for(int i=0;i<m_variables.size();++i)
	{
		xml::XMLElement* var=new xml::XMLElement(mT("Variable"));
		m_variables[i]->Save(var);
		bodyElem->addSubElement(var);
	}
	xml::XMLElement* sts=new xml::XMLElement(mT("Statements"));
	for(int i=0;i<m_statements.size();++i)
	{
		m_statements[i]->Save(sts);
	}
	bodyElem->addSubElement(sts);

	if(parent)
		parent->addSubElement(elem);
	return elem;

}

}
}
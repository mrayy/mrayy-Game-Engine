
#include "stdafx.h"
#include "SGProgram.h"
#include "SGIVisitor.h"

namespace mray
{
namespace shader
{
	const core::string SGProgram::s_ProgramTypeStr[]=
	{
		mT("Vertex"),
		mT("Fragment"),
		mT("Geometry")
	};


SGProgram::SGProgram(EProgramType type):m_type(type)
{
}
SGProgram::~SGProgram()
{/*
	for(int i=0;i<m_functions.size();++i)
	{
		delete m_functions[i];
	}
	for(int i=0;i<m_uniforms.size();++i)
	{
		delete m_uniforms[i];
	}*/
	m_uniforms.clear();
	m_functions.clear();
}

SGFunction* SGProgram::CreateFunction(const core::string& name,EFunctionType type)
{
	if(GetFunctionByName(name))
	{
		core::string msg=mT("SGProgram::CreateFunction(): Function with name:\'")+name+mT("\' already exists in ");
		switch(m_type)
		{
		case EPT_VERTEX:msg+=mT("Vertex");break;
		case EPT_FRAGMENT:msg+=mT("Fragment");break;
		case EPT_GEOMETRY:msg+=mT("Geometry");break;
		}
		msg+=mT(" Program");
		gVideoLoggerSystem.log(msg,ELL_WARNING);
		return 0;
	}
	SGFunctionPtr f=new SGFunction(name,type);
	m_functions.push_back(f);

	return f;
}

SGFunction* SGProgram::GetFunctionByName(const core::string& name)
{
	for(int i=0;i<m_functions.size();++i)
	{
		if(m_functions[i]->GetName()==name)
			return m_functions[i];
	}
	return 0;
}

bool SGProgram::AddUniform(SGVariableCRef u)
{
	if(GetUniformByName(u->GetName()))
	{
		core::string msg=mT("SGProgram::AddUniform(): Uniform with name:\'")+u->GetName()+mT("\' already exists in ");
		switch(m_type)
		{
		case EPT_VERTEX:msg+=mT("Vertex");break;
		case EPT_FRAGMENT:msg+=mT("Fragment");break;
		case EPT_GEOMETRY:msg+=mT("Geometry");break;
		}
		msg+=mT(" Program");
		gVideoLoggerSystem.log(msg,ELL_WARNING);
		return false;
	}

	m_uniforms.push_back(u);
	return true;

}

SGVariableCRef SGProgram::GetUniformByName(const core::string &name)
{
	for(int i=0;i<m_uniforms.size();++i)
	{
		if(m_uniforms[i]->GetName()==name)
			return m_uniforms[i];
	}
	return SGVariablePtr::Null;
}

void SGProgram::AddDependency(const core::string &name)
{
	for(int i=0;i<m_dependencies.size();++i)
	{
		if(m_dependencies[i]==name)
			return;
	}
	m_dependencies.push_back(name);
}

void SGProgram::SetEntryPoint(SGFunctionCRef f)
{
	m_entryPoint=f;
}

SGFunctionCRef SGProgram::GetEntryPoint()const
{
	return m_entryPoint;
}

void SGProgram::Visit(SGIVisitor*visitor)
{
	visitor->OnVisit(this);
	for(int i=0;i<m_functions.size();++i)
	{
		m_functions[i]->Visit(visitor);
	}
}

bool SGProgram::Load(xml::XMLElement* elem)
{
	xml::XMLAttribute* attr;
	xml::XMLElement* node;
	attr=elem->getAttribute(mT("Type"));
	if(attr)
	{
		for(int i=0;i<EPT_COUNT;++i)
		{
			if(attr->value.equals_ignore_case(s_ProgramTypeStr[i]))
			{
				m_type=(EProgramType)i;
				break;
			}
		}
	}

	node=elem->getSubElement(mT("Description"));
	if(node)
	{
		if(node->getElementsBegin()!=node->getElementsEnd())
		{
			xml::XMLTextNode* desc=dynamic_cast<xml::XMLTextNode*>(*node->getElementsBegin());
			if(desc)
				m_desc=desc->GetValue();
		}
	}



	node=elem->getSubElement(mT("Depend"));
	while(node)
	{
		attr=node->getAttribute(mT("Name"));
		if(attr)
			m_dependencies.push_back(attr->value);
		node=node->nextSiblingElement(mT("Depend"));
	}

	node=elem->getSubElement(mT("Uniform"));
	SGVariable* var=0;
	while(node)
	{
		if(!var)
			var=new SGVariable(mT(""),video::EUT_Unkown);
		
		if(var->Load(node))
		{
			m_uniforms.push_back(var);
			var=0;
		}
		node=node->nextSiblingElement(mT("Uniform"));
	}
	if(var)
		delete var;

	node=elem->getSubElement(mT("Function"));
	SGFunction* func=0;
	while(node)
	{
		if(!func)
			func=new SGFunction(mT(""));

		if(func->Load(node))
		{
			m_functions.push_back(func);
			func=0;
		}
		node=node->nextSiblingElement(mT("Function"));
	}
	if(func)
		delete func;
	attr=elem->getAttribute(mT("EntryPoint"));
	if(attr)
	{
		m_entryPoint=GetFunctionByName(attr->value);
	}

	return true;
}

xml::XMLElement* SGProgram::Save(xml::XMLElement* parent)
{
	xml::XMLElement* elem=new xml::XMLElement(mT("Program"));


	elem->addAttribute(mT("Type"),s_ProgramTypeStr[GetType()]);
	if(GetDescription()!=mT(""))
	{
		xml::XMLElement* desc=new xml::XMLElement(mT("Description"));
		desc->addSubElement(new xml::XMLTextNode(GetDescription()));
		elem->addSubElement(desc);
	}
	if(!GetEntryPoint().isNull())
		elem->addAttribute(mT("EntryPoint"),GetEntryPoint()->GetName());

	const std::vector<core::string>& dep=GetDependencies();
	for(int i=0;i<dep.size();++i)
	{
		xml::XMLElement* depE=new xml::XMLElement(mT("Depend"));
		depE->addAttribute(mT("Name"),dep[i]);
		elem->addSubElement(depE);
	}

	//write uniforms
	const std::vector<SGVariablePtr>& uniforms=GetUniforms();
	for(int i=0;i<uniforms.size();++i)
	{
		xml::XMLElement* unif=new xml::XMLElement(mT("Uniform"));
		uniforms[i]->Save(unif);
		elem->addSubElement(unif);
	}
	//visit functions
	for(int i=0;i<m_functions.size();++i)
	{
		m_functions[i]->Save(elem);
	}

	if(parent)
		parent->addSubElement(elem);
	return elem;

}



}
}
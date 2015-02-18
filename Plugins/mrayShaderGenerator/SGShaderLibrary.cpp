

#include "stdafx.h"
#include "SGShaderLibrary.h"
#include "SGFunctionPrototype.h"



namespace mray
{
namespace shader
{

SGShaderLibrary::SGShaderLibrary()
{
}
SGShaderLibrary::~SGShaderLibrary()
{
	ClearFunctions();
}

void SGShaderLibrary::AddFunction(SGFunctionPrototype*f)
{
	m_functions.push_back(f);
}


void SGShaderLibrary::ClearFunctions()
{
	FunctionsList::iterator it=m_functions.begin();
	for(;it!=m_functions.end();++it)
	{
		delete *it;
	}
	m_functions.clear();
}

void SGShaderLibrary::Load(xml::XMLElement* elem)
{
	xml::XMLAttribute *attr;
	xml::XMLElement* node;

	attr=elem->getAttribute(mT("Name"));
	if(attr)
		m_name=attr->value;
	else
		gLogManager.log(mT("SGShaderLibrary::Load() - Libray name is not provided!"),ELL_WARNING);

	node=elem->getSubElement(mT("Description"));
	if(node)
	{
		if(node->getElementsBegin()!=node->getElementsEnd())
		{
			xml::XMLTextNode* desc=dynamic_cast<xml::XMLTextNode*>(*node->getElementsBegin());
			if(desc)
				m_description=desc->GetValue();
		}
	}

	node=elem->getSubElement(mT("Function"));
	SGFunctionPrototype* func=0;
	while(node)
	{
		if(!func)
			func=new SGFunctionPrototype(this);
		if(func->Load(node))
		{
			m_functions.push_back(func);
			func=0;
		}
		node=node->nextSiblingElement(mT("Function"));
	}
	if(func)
		delete func;

}

xml::XMLElement* SGShaderLibrary::Save(xml::XMLElement* parent)
{
	xml::XMLElement* elem=new xml::XMLElement(mT("ShaderLib"));

	elem->addAttribute(mT("Name"),m_name);

	if(m_description!=mT(""))
	{
		xml::XMLElement* desc=new xml::XMLElement(mT("Description"));
		desc->addSubElement(new xml::XMLTextNode(m_description));
		elem->addSubElement(desc);
	}

	FunctionsList::iterator it=m_functions.begin();
	for(;it!=m_functions.end();++it)
	{
		(*it)->Save(elem);
	}
	if(parent)
		parent->addSubElement(elem);
	return elem;

}





}
}



#include "stdafx.h"
#include "SGFunctionPrototype.h"
#include "SGShaderLibrary.h"
#include "SGFunctionParameterProtoype.h"
#include "EnumManager.h"



namespace mray
{
namespace shader
{

SGFunctionPrototype::SGFunctionPrototype(SGShaderLibrary* lib):m_ownerLib(lib),m_retType(video::EUT_Unkown)
{

}
SGFunctionPrototype::~SGFunctionPrototype()
{

	ParamsList::iterator it;
	it=m_inputParams.begin();
	for(;it!=m_inputParams.end();++it)
	{
		delete *it;
	}
	it=m_outputParams.begin();
	for(;it!=m_outputParams.end();++it)
	{
		delete *it;
	}
}

bool SGFunctionPrototype::Load(xml::XMLElement* elem)
{
	xml::XMLAttribute *attr;
	xml::XMLElement* node;

	attr=elem->getAttribute(mT("Name"));
	if(attr)
		m_name=attr->value;
	else
	{
		gLogManager.log(mT("SGFunctionPrototype::Load() - Function name is not provided!"),ELL_WARNING);
		return false;
	}
	attr=elem->getAttribute(mT("RetType"));
	if(attr)
	{
		m_retType=(video::EUniformType)EnumManager::getInstance().getValue(mT("EUniformType"),attr->value);
	}
	else
	{
		m_retType=video::EUT_Unkown;
	}

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

	node=elem->getSubElement(mT("Input"));
	while(node)
	{
		SGFunctionParameterProtoype* var=new SGFunctionParameterProtoype(EVU_INPUT);
		var->Load(node);
		m_inputParams.push_back(var);
		node=node->nextSiblingElement(mT("Input"));
	}
	node=elem->getSubElement(mT("Output"));
	while(node)
	{
		SGFunctionParameterProtoype* var=new SGFunctionParameterProtoype(EVU_OUTPUT);
		var->Load(node);
		m_outputParams.push_back(var);
		node=node->nextSiblingElement(mT("Output"));
	}
	return true;
}

xml::XMLElement* SGFunctionPrototype::Save(xml::XMLElement* parent)
{
	xml::XMLElement* elem=new xml::XMLElement(mT("Function"));

	elem->addAttribute(mT("Name"),m_name);

	if(m_description!=mT(""))
	{
		xml::XMLElement* desc=new xml::XMLElement(mT("Description"));
		desc->addSubElement(new xml::XMLTextNode(m_description));
		elem->addSubElement(desc);
	}

	elem->addAttribute(mT("RetType"),EnumManager::getInstance().getName(mT("EUniformType"),m_retType));

	ParamsList::iterator it;
	it=m_inputParams.begin();
	for(;it!=m_inputParams.end();++it)
	{
		(*it)->Save(elem);
	}
	it=m_outputParams.begin();
	for(;it!=m_outputParams.end();++it)
	{
		(*it)->Save(elem);
	}
	if(parent)
		parent->addSubElement(elem);
	return elem;
}

}
}
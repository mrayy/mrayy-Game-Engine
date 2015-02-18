#include "stdafx.h"

#include "GPUShader.h"
#include "ShaderResourceManager.h"
#include "XMLTextNode.h"

namespace mray{
namespace video{

GPUShader::GPUShader()
{
}
GPUShader::~GPUShader()
{
	UnuseResources();
}

void GPUShader::SetGeometryShader(const IGPUShaderProgramPtr& s)
{
	if(m_geometryShader.pointer()){
		m_geometryShader->unuse();
		_RemoveDependedResource(m_geometryShader);
	}
	m_geometryShader=s;
	if(m_geometryShader.pointer())
	{
		_AddDependedResource(m_geometryShader);
	}
}
void GPUShader::SetVertexShader(const IGPUShaderProgramPtr& s)
{
	if(m_vertexShader.pointer()){
		m_vertexShader->unuse();
		_RemoveDependedResource(m_vertexShader);
	}
	m_vertexShader=s;
	if(m_vertexShader.pointer())
	{
		_AddDependedResource(m_vertexShader);
	}
}
void GPUShader::SetFragmentShader(const IGPUShaderProgramPtr& s)
{
	if(m_fragmentShader.pointer()){
		m_fragmentShader->unuse();
		_RemoveDependedResource(m_fragmentShader);
	}
	m_fragmentShader=s;
	if(m_fragmentShader.pointer())
	{
		_AddDependedResource(m_fragmentShader);
	}
}

const IGPUShaderProgramPtr& GPUShader::GetGeometryShader()
{
	return m_geometryShader;
}
const IGPUShaderProgramPtr& GPUShader::GetVertexShader()
{
	return m_vertexShader;
}
const IGPUShaderProgramPtr& GPUShader::GetFragmentShader()
{
	return m_fragmentShader;
}

void GPUShader::use()
{
	if(m_geometryShader.pointer())
		m_geometryShader->use();
	if(m_vertexShader.pointer())
		m_vertexShader->use();
	if(m_fragmentShader.pointer())
		m_fragmentShader->use();
}
void GPUShader::unuse()
{
	if(m_geometryShader.pointer())
		m_geometryShader->unuse();
	if(m_vertexShader.pointer())
		m_vertexShader->unuse();
	if(m_fragmentShader.pointer())
		m_fragmentShader->unuse();
}
xml::XMLElement* GPUShader::loadXMLSettings(xml::XMLElement* e)
{
	xml::XMLElement* elem=e->getSubElement(mT("Shader"));
	xml::XMLElement* subShader;
	if(!elem)
		return 0;
	xml::XMLAttribute* attr=elem->getAttribute(mT("Name"));
	if(attr)
		SetName(attr->value);

	//try to check for sub shaders before loading from attributes
	subShader=elem->getSubElement(mT("GeometryShader"));
	if(subShader)
	{
		SetGeometryShader(gShaderResourceManager.parseShaderXMLElement(subShader,false));
		
	}else
	{
		attr=elem->getAttribute(mT("GeometryShader"));
		if(attr)
		{
			SetGeometryShader(gShaderResourceManager.getResource(attr->value));
		}
	}
	subShader=elem->getSubElement(mT("VertexShader"));
	if(subShader)
	{
		SetVertexShader(gShaderResourceManager.parseShaderXMLElement(subShader,false));

	}else
	{
		attr=elem->getAttribute(mT("VertexShader"));
		if(attr)
		{
			SetVertexShader(gShaderResourceManager.getResource(attr->value));
		}
	}
	subShader=elem->getSubElement(mT("FragmentShader"));
	if(subShader)
	{
		SetFragmentShader(gShaderResourceManager.parseShaderXMLElement(subShader,false));

	}else
	{
		attr=elem->getAttribute(mT("FragmentShader"));
		if(attr)
		{
			SetFragmentShader(gShaderResourceManager.getResource(attr->value));
		}
	}

	return elem;
}
xml::XMLElement* GPUShader::exportXMLSettings(xml::XMLElement* elem)
{
	xml::XMLElement* node=0;

	node=new xml::XMLElement(mT("Shader"));

	node->addAttribute(mT("Name"),GetName());
	if(!m_geometryShader.isNull())
		node->addAttribute(mT("GeometryShader"),m_geometryShader->getName());
	if(!m_vertexShader.isNull())
		node->addAttribute(mT("VertexShader"),m_vertexShader->getName());
	if(!m_fragmentShader.isNull())
		node->addAttribute(mT("FragmentShader"),m_fragmentShader->getName());

	return node;

}

}
}

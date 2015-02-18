
#include "stdafx.h"
#include "SGXMLVisitor.h"

#include "StringConverter.h"

#include "SGProgram.h"
#include "SGFunction.h"
#include "SGFunctionCall.h"
#include "SGForStatement.h"
#include "SGOperation.h"
#include "SGValue.h"
#include "SGVariable.h"
#include "SGDirectValue.h"

#include "XMLElement.h"
#include "XMLTextNode.h"
#include "XMLComment.h"

namespace mray
{
namespace shader
{

	const core::string s_VariableUsage[]=
	{
		mT("Normal"),
		mT("Uniform"),
		mT("Input"),
		mT("Output")
	};
	const core::string s_VarSemanticStr[]=
	{
		mT("Unkown"),
		mT("Position"),
		mT("Normal"),
		mT("Texcoord"),
		mT("Color"),
		mT("Blendweight"),
		mT("Blendindices"),
		mT("Binormal"),
		mT("Tangent")
	};

	const core::string s_ValTypesStr[]=
	{
		mT("int"),
		mT("int2"),
		mT("int3"),
		mT("int4"),
		mT("float"),
		mT("float2"),
		mT("float3"),
		mT("float4"),
		mT("sampler1D"),
		mT("sampler2D"),
		mT("sampler3D"),
		mT("samplerCUBE"),
		mT("float2x2"),
		mT("float2x3"),
		mT("float2x4"),
		mT("float3x3"),
		mT("float3x4"),
		mT("float4x2"),
		mT("float4x3"),
		mT("float4x4")
	};

SGXMLVisitor::SGXMLVisitor()
{
	m_element=0;
}
SGXMLVisitor::~SGXMLVisitor()
{
}

xml::XMLElement* SGXMLVisitor::Traverse(SGProgram*program)
{
	m_element=0;
	m_current=0;
	program->Visit(this);
	return m_element;
}

void SGXMLVisitor::OnVisit(SGProgram*v)
{
	const std::vector<core::string>& dep=v->GetDependencies();
	const std::vector<SGVariablePtr>& uniforms=v->GetUniforms();

	m_element=new xml::XMLElement(mT("Program"));
	static const core::string ProgramType[]=
	{
		mT("Vertex"),mT("Fragment"),mT("Geometry")
	};
	m_element->addAttribute(mT("Type"),ProgramType[v->GetType()]);
	if(v->GetDescription()!=mT(""))
		m_element->addSubElement(new xml::XMLTextNode(v->GetDescription()));

	xml::XMLElement* e;
	if(dep.size()>0)
	{
		 e= new xml::XMLElement(mT("Dependents"));
		for(int i=0;i<dep.size();++i)
		{
			xml::XMLElement* ee= new xml::XMLElement(mT("Dependent"));
			ee->addSubElement(new xml::XMLTextNode(dep[i]));
			e->addSubElement(ee);
		}
		m_element->addSubElement(e);
	}

	if(uniforms.size()>0)
	{
		e= new xml::XMLElement(mT("Uniforms"));
		m_current=e;
		for(int i=0;i<uniforms.size();++i)
		{
			uniforms[i]->Visit(this);
		}
		m_element->addSubElement(e);
	}


	m_current=m_element;
}

void SGXMLVisitor::OnVisit(SGFunction*v)
{
	xml::XMLElement* tmp=m_current;
	const std::vector<SGVariablePtr>& inputs=v->GetInputVars();
	const std::vector<SGVariablePtr>& outputs=v->GetOutputVars();
	const std::vector<SGVariablePtr>& values=v->GetVariables();
	const std::vector<SGIStatementPtr>& statements=v->GetStatements();

	static const core::string Ftypes[]=
	{
		mT("Default"),
		mT("MainVertex"),
		mT("MainFragment")
	};
	xml::XMLElement*e=new xml::XMLElement(mT("Function"));
	m_current->addSubElement(e);

	e->addAttribute(mT("Name"),v->GetName());
	if(v->GetDescription()!=mT(""))
		e->addSubElement(new xml::XMLTextNode(v->GetDescription()));
	e->addAttribute(mT("Type"),Ftypes[v->GetType()]);

	if(v->GetReturnValue())
	{
		xml::XMLElement* e=new xml::XMLElement(mT("ReturnValue"));
		m_current->addSubElement(e);
		m_current=e;
		v->GetReturnValue()->Visit(this);
	}
	xml::XMLElement*ee;

	if(inputs.size()>0)
	{
		ee=new xml::XMLElement(mT("Inputs"));
		e->addSubElement(ee);

		m_current=ee;
		for(int i=0;i<inputs.size();++i)
		{
			inputs[i]->Visit(this);
		}
	}

	if(outputs.size()>0)
	{
		ee=new xml::XMLElement(mT("Outputs"));
		e->addSubElement(ee);
		m_current=ee;
		for(int i=0;i<outputs.size();++i)
		{
			outputs[i]->Visit(this);
		}
	}

	if(values.size()>0)
	{
		ee=new xml::XMLElement(mT("Variables"));
		e->addSubElement(ee);

		m_current=ee;
		for(int i=0;i<values.size();++i)
		{
			values[i]->Visit(this);
		}
	}
	if(statements.size()>0)
	{
		ee=new xml::XMLElement(mT("Statements"));
		e->addSubElement(ee);

		m_current=ee;
		for(int i=0;i<statements.size();++i)
		{
			statements[i]->Visit(this);
		}
	}
	m_current=tmp;
}

void SGXMLVisitor::OnVisit(SGFunctionCall*v)
{

	xml::XMLElement* tmp=m_current;
	const std::vector<SGIValuePtr>& inputs=v->GetInputValues();
	const std::vector<SGVariablePtr>& outputs=v->GetOutputValues();

	xml::XMLElement* e=new xml::XMLElement(mT("FunctionCall"));
	e->addAttribute(mT("Name"),v->GetFunctionName());
	m_current->addSubElement(e);
	
	xml::XMLElement* ee;

	if(inputs.size()>0)
	{
		ee=new xml::XMLElement(mT("Inputs"));
		e->addSubElement(ee);
		m_current=ee;
		for(int i=0;i<inputs.size();++i)
		{
			inputs[i]->Visit(this);
		}
	}
	if(outputs.size()>0)
	{
		ee=new xml::XMLElement(mT("Outputs"));
		e->addSubElement(ee);
		m_current=ee;
		for(int i=0;i<outputs.size();++i)
		{
			outputs[i]->Visit(this);
		}
	}

	m_current=tmp;
}
void SGXMLVisitor::OnVisit(SGOperation*v)
{
	static const core::string opType[]=
	{
		mT("Add"),
		mT("Sub"),
		mT("Mul"),
		mT("Div"),
		mT("Assign")
	};
	xml::XMLElement* tmp=m_current;
	const std::vector<SGIValuePtr>& inputs=v->GetInputValues();
	const std::vector<SGVariablePtr>& outputs=v->GetOutputValues();

	xml::XMLElement* e=new xml::XMLElement(mT("Operation"));
	e->addAttribute(mT("Type"),opType[v->GetOperation()]);
	m_current->addSubElement(e);

	xml::XMLElement* ee;

	if(inputs.size()>0)
	{
		ee=new xml::XMLElement(mT("Inputs"));
		e->addSubElement(ee);
		m_current=ee;
		for(int i=0;i<inputs.size();++i)
		{
			inputs[i]->Visit(this);
		}
	}
	if(outputs.size()>0)
	{
		ee=new xml::XMLElement(mT("Outputs"));
		e->addSubElement(ee);
		m_current=ee;
		for(int i=0;i<outputs.size();++i)
		{
			outputs[i]->Visit(this);
		}
	}

	m_current=tmp;
}
void SGXMLVisitor::OnVisit(SGForStatement*v)
{
}

void SGXMLVisitor::OnVisit(SGValue*v)
{
	xml::XMLElement* tmp=m_current;
	xml::XMLElement* e=new xml::XMLElement(mT("Value"));
	m_current->addSubElement(e);
	e->addAttribute(mT("Type"),s_ValTypesStr[v->GetType()]);
	m_current=e;

	const std::vector<SGIValuePtr>&vals=v->GetValues();
	for(int i=0;i<vals.size();++i)
	{
		vals[i]->Visit(this);
	}

	m_current=tmp;

}
void SGXMLVisitor::OnVisit(SGVariable*v)
{
	xml::XMLElement* e=new xml::XMLElement(mT("Variable"));
	e->addAttribute(mT("Type"),s_ValTypesStr[v->GetType()]);
	e->addAttribute(mT("Name"),v->GetName());
	e->addAttribute(mT("Const"),core::StringConverter::toString(v->IsConst()));
	e->addAttribute(mT("Index"),core::StringConverter::toString(v->GetIndex()));
	e->addAttribute(mT("Usage"),s_VariableUsage[v->GetUsage()]);
	e->addAttribute(mT("Semantic"),s_VarSemanticStr[v->GetSemantic()]);

	if(v->GetDescription()!=mT(""))
		e->addSubElement(new xml::XMLTextNode(v->GetDescription()));

	m_current->addSubElement(e);
}
void SGXMLVisitor::OnVisit(SGDirectValue*v)
{
	xml::XMLElement* e=new xml::XMLElement(mT("DirectValue"));
	e->addAttribute(mT("Type"),s_ValTypesStr[v->GetType()]);
	e->addAttribute(mT("Value"),v->GetValue());

	m_current->addSubElement(e);
}



}
}



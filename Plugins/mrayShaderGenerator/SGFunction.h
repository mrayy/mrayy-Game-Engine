

/********************************************************************
	created:	2011/09/14
	created:	14:9:2011   15:01
	filename: 	d:\Development\mrayEngine\Engine\mrayShaderGenerator\SGFunction.h
	file path:	d:\Development\mrayEngine\Engine\mrayShaderGenerator
	file base:	SGFunction
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __SGFunction__
#define __SGFunction__

#include "SGCommon.h"
#include "SGVariable.h"
#include "SGIStatement.h"

namespace mray
{
namespace shader
{

	class SGShaderLibrary;
	class SGFunctionPrototype;

class SGFunction
{
public:
private:
protected:

	core::string m_name;
	EFunctionType m_type;
	core::string m_desc;

	std::vector<SGVariablePtr> m_variables;
	std::vector<SGIStatementPtr> m_statements;

	SGIValuePtr m_returnValue;

	std::vector<SGVariablePtr> m_inputVars;
	std::vector<SGVariablePtr> m_outputVars;

public:
	SGFunction(const core::string& name,EFunctionType type=EFT_DEFAULT,const core::string& desc=mT(""));
	~SGFunction();

	const core::string& GetName()const{return m_name;}

	const core::string& GetDescription()const{return m_desc;}

	EFunctionType GetType()const{return m_type;}

	void AddVariable(SGVariableCRef val);

	SGVariableCRef AddInputVariable(const core::string& name,video::EUniformType type,bool uniform,EUniformSemantic semantic=EUS_UNKOWN,ushort index=0);

	SGVariableCRef AddOutputVariable(const core::string& name,video::EUniformType type,EUniformSemantic semantic=EUS_UNKOWN,ushort index=0);

	void AddStatement(SGIStatementCRef st);

	SGVariableCRef GetInputVariableByName(const core::string& name)const ;

	SGVariableCRef GetOutputVariableByName(const core::string& name)const ;

	SGVariableCRef GetInputVariableBySemantic(EUniformSemantic sem,ushort index)const ;

	SGVariableCRef GetOutputVariableBySemantic(EUniformSemantic sem,ushort index)const ;

	const std::vector<SGVariablePtr>& GetInputVars()const{return m_inputVars;}

	const std::vector<SGVariablePtr>& GetOutputVars()const{return m_outputVars;}

	void SetReturnValue(SGIValueCRef val){ m_returnValue=val;}

	SGIValueCRef GetReturnValue()const{return m_returnValue;}

	const std::vector<SGVariablePtr>& GetVariables()const{return m_variables;}

	const std::vector<SGIStatementPtr>& GetStatements()const{return m_statements;}

	SGFunctionPrototype* CreatePrototype(SGShaderLibrary* lib);

	void Visit(SGIVisitor*visitor);

	bool Load(xml::XMLElement* elem);
	xml::XMLElement* Save(xml::XMLElement* elem);
};

MakeSharedPtrType(SGFunction);

}
}

#endif
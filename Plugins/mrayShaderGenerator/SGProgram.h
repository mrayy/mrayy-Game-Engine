
/********************************************************************
	created:	2011/09/14
	created:	14:9:2011   15:03
	filename: 	d:\Development\mrayEngine\Engine\mrayShaderGenerator\SGProgram.h
	file path:	d:\Development\mrayEngine\Engine\mrayShaderGenerator
	file base:	SGProgram
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __SGProgram__
#define __SGProgram__

#include "SGFunction.h"

namespace mray
{
namespace shader
{


class SGProgram
{
private:
protected:
	static const core::string s_ProgramTypeStr[];

	std::vector<core::string> m_dependencies;
	std::vector<SGFunctionPtr> m_functions;

	std::vector<SGVariablePtr> m_uniforms;

	SGFunctionPtr m_entryPoint;
	EProgramType m_type;

	core::string m_desc;

public:
	SGProgram(EProgramType type);
	~SGProgram();

	EProgramType GetType()const{return m_type;}

	void SetDescription(const core::string& d){m_desc=d;}

	const core::string& GetDescription()const{return m_desc;}

	const std::vector<SGVariablePtr>& GetUniforms()const{return m_uniforms;}

	const std::vector<core::string>& GetDependencies()const{return m_dependencies;}

	SGFunction* CreateFunction(const core::string& name,EFunctionType type=EFT_DEFAULT);

	SGFunction* GetFunctionByName(const core::string& name);

	bool AddUniform(SGVariableCRef u);

	SGVariableCRef GetUniformByName(const core::string &name);
	
	void AddDependency(const core::string &name);

	void SetEntryPoint(SGFunctionCRef f);

	SGFunctionCRef GetEntryPoint()const;

	void Visit(SGIVisitor*visitor);

	bool Load(xml::XMLElement* elem);
	xml::XMLElement* Save(xml::XMLElement* elem);

};

}
}

#endif
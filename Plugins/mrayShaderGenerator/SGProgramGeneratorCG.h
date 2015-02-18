


/********************************************************************
	created:	2011/09/15
	created:	15:9:2011   11:36
	filename: 	d:\Development\mrayEngine\Engine\mrayShaderGenerator\SGProgramGeneratorCG.h
	file path:	d:\Development\mrayEngine\Engine\mrayShaderGenerator
	file base:	SGProgramGeneratorCG
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __SGProgramGeneratorCG__
#define __SGProgramGeneratorCG__

#include "SGIProgramGenerator.h"
#include "SGIStatement.h"
#include "SGIVisitor.h"

namespace mray
{
namespace shader
{

class SGProgramGeneratorCG:public SGIProgramGenerator,public SGIVisitor
{
private:
protected:

	static const core::string s_GPUTypesStr[];
	static const core::string s_GPUTypes_halfStr[];

	static const core::string s_GPUSemanticStr[];

	core::string m_shader;
	bool m_withComments;

	bool m_useHalfDataType;

	int m_tabsCount;
	void WriteTabs();

	void WriteValue(bool isConst,const core::string&name,video::EUniformType type,ushort index);

	void WriteStatements(const std::vector<SGIStatementPtr>& statements);
public:
	SGProgramGeneratorCG();
	virtual~SGProgramGeneratorCG();

	const core::string& GetType()const;

	const core::string& GenerateShader(SGProgram*program);

	void OnVisit(SGProgram*v);

	void OnVisit(SGFunction*v);

	void OnVisit(SGFunctionCall*v);
	void OnVisit(SGOperation*v);
	void OnVisit(SGForStatement*v);

	void OnVisit(SGValue*v);
	void OnVisit(SGVariable*v);
	void OnVisit(SGDirectValue*v);
};

class SGProgramGeneratorFactoryCG
{
public:
	static const core::string Type;
	virtual const core::string& GetType()const;
	virtual SGIProgramGenerator* CreateGenerator()const;
};

}
}

#endif

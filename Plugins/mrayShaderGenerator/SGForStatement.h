

/********************************************************************
	created:	2011/09/15
	created:	15:9:2011   16:28
	filename: 	d:\Development\mrayEngine\Engine\mrayShaderGenerator\SGForStatement.h
	file path:	d:\Development\mrayEngine\Engine\mrayShaderGenerator
	file base:	SGForStatement
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __SGForStatement__
#define __SGForStatement__

#include "SGIStatement.h"

namespace mray
{
namespace shader
{

class SGForStatement:public SGIStatement
{
private:
protected:
	int m_start;
	int m_end;
	int m_incr;
	core::string m_varName;

	std::vector<SGIStatementPtr> m_statements;

public:
	SGForStatement(const core::string &varName,int start,int end,int incr);
	virtual~SGForStatement();

	void AddStatement(SGIStatementCRef st);

	const std::vector<SGIStatementPtr>& GetStatements()const{return m_statements;}


	int GetStart()const{return m_start;}
	int GetEnd()const{return m_end;}
	int GetIncrement()const{return m_incr;}
	const core::string&GetVarName()const{return m_varName;}

	void Visit(SGIVisitor*visitor);
	bool Load(xml::XMLElement* elem);
	xml::XMLElement* Save(xml::XMLElement* elem);
};

}
}

#endif

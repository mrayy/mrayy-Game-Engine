
#include "stdafx.h"
#include "SGForStatement.h"
#include "SGIVisitor.h"


namespace mray
{
namespace shader
{

SGForStatement::SGForStatement(const core::string &varName,int start,int end,int incr)
:m_varName(varName),m_start(start),m_end(end),m_incr(incr)
{
}
SGForStatement::~SGForStatement()
{
}

void SGForStatement::AddStatement(SGIStatementCRef st)
{
	m_statements.push_back(st);
}

void SGForStatement::Visit(SGIVisitor*visitor)
{
	visitor->OnVisit(this);
}


}
}

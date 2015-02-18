
#include "stdafx.h"
#include "SGIStatement.h"
#include "SGFunctionCall.h"
#include "SGOperation.h"

namespace mray
{
namespace shader
{

SGIStatement::SGIStatement()
{
}
SGIStatement::~SGIStatement()
{
}

SGIStatement* SGIStatement::CreateStatement(const core::string& name)
{
	if(name.equals_ignore_case(mT("Call")))
		return new SGFunctionCall(mT(""));
	if(name.equals_ignore_case(mT("Operation")))
		return new SGOperation(EOT_ASSIGN,video::EUT_Unkown);
	return 0;
}

}
}
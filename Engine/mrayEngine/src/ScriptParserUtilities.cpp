#include "stdafx.h"

#include "ScriptParserUtilities.h"
#include "ILexical.h"


namespace mray
{
namespace script
{

ScriptParserUtilities::ScriptParserUtilities()
:m_tokenList(0)
{
}
ScriptParserUtilities::~ScriptParserUtilities()
{
}

script::ScriptToken* ScriptParserUtilities::nextToken()
{
	if(m_tokenList->isDone()){
		return 0;
	}
	script::ScriptToken* token=m_tokenList->getNextToken();
	if(!token || token->token==script::ERT_EOF)
		return 0;
	return token;
}
script::ScriptToken* ScriptParserUtilities::peekToken()
{
	if(m_tokenList->isDone()){
		return 0;
	}
	script::ScriptToken* token=m_tokenList->getToken(1);
	if(!token || token->token==script::ERT_EOF)
		return 0;
	return token;
}
bool ScriptParserUtilities::lookAhead(const core::string&str)
{
	script::ScriptToken* token=peekToken();
	if(token && token->lex.equals_ignore_case(str))
		return true;
	return false;
}

}
}


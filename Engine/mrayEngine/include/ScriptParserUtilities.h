


/********************************************************************
	created:	2011/09/20
	created:	20:9:2011   11:57
	filename: 	d:\Development\mrayEngine\Engine\mrayEngine\include\ScriptParserUtilities.h
	file path:	d:\Development\mrayEngine\Engine\mrayEngine\include
	file base:	ScriptParserUtilities
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __ScriptParserUtilities__
#define __ScriptParserUtilities__

#include "mstring.h"

namespace mray
{
namespace script
{
	class CTokenList;
	class ScriptToken;

class ScriptParserUtilities
{
private:
protected:
	CTokenList* m_tokenList;
public:
	ScriptParserUtilities();
	virtual~ScriptParserUtilities();

	void SetTokens(script::CTokenList* tokens){m_tokenList=tokens;}

	script::ScriptToken* nextToken();
	script::ScriptToken* peekToken();
	bool lookAhead(const core::string&str);
};

}
}

#endif

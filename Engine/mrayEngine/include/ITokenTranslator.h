
#ifndef ___ITokenTranslator___
#define ___ITokenTranslator___

#include "mString.h"

namespace mray{
namespace script{

class ILexical;

enum EReservedTokens
{
	ERT_Unkown,
	ERT_EOF,
	ERT_Count
};

enum ESpecialTokens
{
	EST_Ident,
	EST_Int,
	EST_Float,
	EST_String,
	EST_Other,
	EST_EOD_T		//special token for end of definition table
};
class ScriptToken
{
public:
	ScriptToken()
	{
		token=0;
		lex="";
		line=0;
		col=0;
		file=mT("");
	}
	int token;
	core::string lex;
	int line;
	int col;
	core::string file;
	ESpecialTokens type;
};
class ITokenTranslator
{
public:
	virtual~ITokenTranslator(){}
	virtual int translateToken(const core::string&str,ILexical*tokenzer,core::string &resToken)=0;
};

}
}

#endif
